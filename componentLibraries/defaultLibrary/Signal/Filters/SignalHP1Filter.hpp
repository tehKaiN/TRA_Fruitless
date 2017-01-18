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
//! @file   SignalHP1Filter.hpp
//! @author Robert Braun <karl.pettersson@liu.se>
//! @date   2010-12-06
//!
//! @brief Contains a Signal First Order High Pass Filter Component using CoreUtilities
//!
//$Id: SignalHP1Filter.hpp 8540 2015-12-22 10:37:53Z petno25 $

#ifndef SIGNALHP1FILTER_HPP_INCLUDED
#define SIGNALHP1FILTER_HPP_INCLUDED

#include "ComponentEssentials.h"
#include "ComponentUtilities.h"

namespace hopsan {

    //!
    //! @brief
    //! @ingroup SignalComponents
    //!
    class SignalHP1Filter : public ComponentSignal
    {

    private:
        FirstOrderTransferFunction mTF;
        double mW, mMin, mMax;
        double *mpIn, *mpOut;

    public:
        static Component *Creator()
        {
            return new SignalHP1Filter();
        }

        void configure()
        {
            addInputVariable("in","","", 0.0, &mpIn);
            addOutputVariable("out", "","",0.0, &mpOut);

            addConstant("omega", "Break frequency", "Frequency", 1000.0, mW);
            addConstant("y_min", "Lower output limit", "", -1.5E+300, mMin);
            addConstant("y_max", "Upper output limit", "", 1.5E+300, mMax);
        }


        void initialize()
        {
            double num[2];
            double den[2];

            num[1] = 1.0/mW;
            num[0] = 0.0;
            den[1] = 1.0/mW;
            den[0] = 1.0;

            mTF.initialize(mTimestep, num, den, (*mpIn), (*mpOut), mMin, mMax);

            // Do not init output as the startvalue given in that port will initialize the filter output
        }


        void simulateOneTimestep()
        {
            (*mpOut) = mTF.update((*mpIn));
        }
    };
}

#endif // SIGNALHP1FILTER_HPP_INCLUDED


