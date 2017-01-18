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
//! @file   SignalSecondOrderFilter.hpp
//! @author Björn Eriksson <bjorn.eriksson@liu.se>
//! @date   2010-01-22
//!
//! @brief Contains a Signal Second Order Filter Component using CoreUtilities
//!
//$Id: SignalSecondOrderFilter.hpp 8540 2015-12-22 10:37:53Z petno25 $

#ifndef SIGNALSECONORDERFILTER_HPP_INCLUDED
#define SIGNALSECONORDERFILTER_HPP_INCLUDED

#include "ComponentEssentials.h"
#include "ComponentUtilities.h"

namespace hopsan {

    //!
    //! @brief
    //! @ingroup SignalComponents
    //!
    class SignalSecondOrderFilter : public ComponentSignal
    {

    private:
        SecondOrderTransferFunction mTF2;
        double mWnum, mDnum, mWden, mDden, mK;
        double mMin, mMax;
        double *mpIn, *mpOut;

    public:
        static Component *Creator()
        {
            return new SignalSecondOrderFilter();
        }

        void configure()
        {
            addInputVariable("in","","", 0.0, &mpIn);
            addOutputVariable("out", "","",0.0, &mpOut);

            addConstant("k", "Gain", "-", 1.0, mK);
            addConstant("omega_1", "Numerator break frequency", "Frequency", 1.0e10, mWnum);
            addConstant("delta_1", "Numerator damp coefficient", "", 1.0, mDnum);
            addConstant("omega_2", "Denominator break frequency", "Frequency", 1000, mWden);
            addConstant("delta_2", "Denominator damp coefficient", "", 1.0, mDden);
            addConstant("y_min", "Lower output limit", "", -1.5E+300, mMin);
            addConstant("y_max", "Upper output limit", "", 1.5E+300, mMax);
        }


        void initialize()
        {
            double num[3];
            double den[3];

            num[2] = mK/(mWnum*mWnum);
            num[1] = mK*2.0*mDnum/mWnum;
            num[0] = mK;
            den[2] = 1.0/pow(mWden, 2);
            den[1] = 2.0*mDden/mWden;
            den[0] = 1.0;

            mTF2.initialize(mTimestep, num, den, (*mpIn), (*mpOut), mMin, mMax);

            // Do not write initial value to out port, its startvalue is used to initialize the filter
        }


        void simulateOneTimestep()
        {
            (*mpOut) = mTF2.update(*mpIn);
        }
    };
}

#endif // SIGNALSECONORDERFILTER_HPP_INCLUDED
