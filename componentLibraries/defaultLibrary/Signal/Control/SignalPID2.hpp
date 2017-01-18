/*-----------------------------------------------------------------------------
 This source file is a part of Hopsan

 Copyright (c) 2009 to present year, Hopsan Group

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

 For license details and information about the Hopsan Group see the files
 GPLv3 and HOPSANGROUP in the Hopsan source code root directory

 For author and contributor information see the AUTHORS file
-----------------------------------------------------------------------------*/

#ifndef SIGNALPID2_HPP_INCLUDED
#define SIGNALPID2_HPP_INCLUDED

#include "ComponentEssentials.h"
#include "ComponentUtilities.h"


//!
//! @file SignalPID2.hpp
//! @author Peter Nordin <peter.nordin@liu.se>
//! @brief PID controller with anti-windup
//! @ingroup SignalComponents
//!

namespace hopsan{

class SignalPID2 : public ComponentSignal
{
private:
    // States
    double mI;
    double mLastErr;
    bool mUseDeInput;

    // Constants
    double mK,mTi,mTd,mTt,mUmax,mUmin;

    // NodeData pointers
    double *mpErr, *mpDerr, *mpOut;


public:
    static Component *Creator()
    {
        return new SignalPID2();
    }

    void configure()
    {
        addInputVariable("e", "Control error", "", 0, &mpErr);
        addInputVariable("de", "Derivative signal input", "", 0, &mpDerr);
        addOutputVariable("u", "Control signal", "", &mpOut);

        addConstant("K", "Gain", "", 1, mK);
        addConstant("Ti", "Integral time", "s", 1, mTi);
        addConstant("Tt", "Anti-windup tracking constant", "s", 1, mTt);
        addConstant("Td", "Derivative time", "s", 0, mTd);
        addConstant("Umin", "", "", -1e100, mUmin);
        addConstant("Umax", "", "", 1e100, mUmax);
    }

    void initialize()
    {
        mI = 0.0;
        mLastErr = 0.0;

        // Decide if we should use de input for derivative signal
        mUseDeInput = (getPort("de")->isConnected());

        if (mTt < mTi)
        {
            addWarningMessage("Tt is lower then Ti this is not correct!");
        }
    }
    void simulateOneTimestep()
    {
        double dErr;

        // Read input control error
        const double err = (*mpErr);

        // Decide what derivative signal to use, de input or derivative in e input
        if (mUseDeInput)
        {
            dErr = (*mpDerr);
        }
        else
        {
            dErr = err-mLastErr;
            mLastErr = err;
        }

        //! @todo many of the constant based calculations can be pre calculated in initialize
        // Integrate
        mI = mI + mK * mTimestep/mTi * err;

        // Calculate control signal
        const double v = mK*err + mI + mK*mTd/mTimestep * dErr;

        // Limit to Umin Umax for anti-windup correction
        double u;
        if (v > mUmax)
        {
            u = mUmax;
        }
        else if (v < mUmin)
        {
            u = mUmin;
        }
        else
        {
            u = v;
        }

        // Anti-windup I correction
        mI = mI + mTimestep/mTt * (u-v);

        // Write output and remember error
        (*mpOut) = v;
    }
};
}
#endif // SIGNALPID2_HPP_INCLUDED
