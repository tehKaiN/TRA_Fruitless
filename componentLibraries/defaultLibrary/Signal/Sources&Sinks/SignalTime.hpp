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
//! @file   SignalSource.hpp
//! @author Björn Eriksson <bjorn.eriksson@liu.se>
//! @date   2010-10-03
//!
//! @brief Contains a Signal Time Component
//!
//$Id: SignalTime.hpp 8139 2015-06-17 12:30:13Z petno25 $

#ifndef SIGNALTIME_HPP_INCLUDED
#define SIGNALTIME_HPP_INCLUDED

#include "ComponentEssentials.h"

namespace hopsan {

    //!
    //! @brief
    //! @ingroup SignalComponents
    //!
    class SignalTime : public ComponentSignal
    {

    private:
        double *mpOut;

    public:
        static Component *Creator()
        {
            return new SignalTime();
        }

        void configure()
        {
            addOutputVariable("out", "Simulation time", "Time", &mpOut);
        }


        void initialize()
        {
            simulateOneTimestep();
        }


        void simulateOneTimestep()
        {
            (*mpOut) = mTime;
        }
    };
}

#endif // SIGNALTIME_HPP_INCLUDED
