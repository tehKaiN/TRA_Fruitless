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

//!
//! @file   SignalCounter.hpp
//! @author Robert Braun <robert.braun@liu.se>
//! @date   2014-12-18
//!
//! @brief Contains a signal flank counter component
//!
//$Id: SignalCounter.hpp 8139 2015-06-17 12:30:13Z petno25 $
#ifndef SIGNALCOUNTER_HPP
#define SIGNALCOUNTER_HPP


#include "ComponentEssentials.h"

namespace hopsan {

class SignalCounter : public ComponentSignal
{
private:
    bool mR, mF;
    double *mpIn, *mpOut;
    double mNextT, mTs;

    bool mPrev;

public:
    static Component *Creator()
    {
        return new SignalCounter();
    }

    void configure()
    {
        addConstant("r", "Count rising flags", "", true, mR);
        addConstant("f", "Count falling flags", "", true, mF);
        addInputVariable("in", "", "", 0, &mpIn);
        addOutputVariable("out", "", "", &mpOut);
    }

    void initialize()
    {
        (*mpOut) = 0;
        mPrev = false;
    }

    void simulateOneTimestep()
    {
        bool in = doubleToBool(*mpIn);

        if(mR && in && !mPrev)
        {
            (*mpOut) += 1;

        }
        else if(mF && !in && mPrev)
        {
            (*mpOut) += 1;
        }

        mPrev = in;
    }
};
}

#endif // SIGNALCOUNTER_HPP
