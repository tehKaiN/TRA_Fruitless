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
//! @file   SignalAbsoluteValue.hpp
//! @author Björn Eriksson <robert.braun@liu.se>
//! @date   2011-02-03
//!
//! @brief Contains a Signal Absolute Value Component
//!
//$Id: SignalAbsoluteValue.hpp 8139 2015-06-17 12:30:13Z petno25 $

#ifndef SIGNALABSOLUTEVALUE_HPP_INCLUDED
#define SIGNALABSOLUTEVALUE_HPP_INCLUDED

#include "ComponentEssentials.h"

namespace hopsan {

    //!
    //! @brief
    //! @ingroup SignalComponents
    //!
    class SignalAbsoluteValue : public ComponentSignal
    {

    private:
        double *mpIn, *mpOut;

    public:
        static Component *Creator()
        {
            return new SignalAbsoluteValue();
        }

        void configure()
        {
            addInputVariable("in", "", "", 0.0, &mpIn);
            addOutputVariable("out", "ABS of in", "", &mpOut);
        }


        void initialize()
        {
            simulateOneTimestep();
        }

        void simulateOneTimestep()
        {
            if(*mpIn > 0)
            {
                (*mpOut) = (*mpIn);
            }
            else
            {
                (*mpOut) = -(*mpIn);
            }
        }
    };
}

#endif // SIGNALABSOLUTEVALUE_HPP_INCLUDED
