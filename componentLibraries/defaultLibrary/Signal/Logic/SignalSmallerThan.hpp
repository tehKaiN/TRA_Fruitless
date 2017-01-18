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
//! @file   SignalSmallerThan.hpp
//! @author Robert Braun <robert.braun@liu.se>
//! @date   2010-10-18
//!
//! @brief Contains a Smaller Than Component
//!
//$Id: SignalSmallerThan.hpp 8139 2015-06-17 12:30:13Z petno25 $

#ifndef SIGNALSMALLERTHAN_HPP_INCLUDED
#define SIGNALSMALLERTHAN_HPP_INCLUDED

#include "ComponentEssentials.h"
#include "ComponentUtilities.h"

namespace hopsan {

    //!
    //! @brief
    //! @ingroup SignalComponents
    //!
    class SignalSmallerThan : public ComponentSignal
    {

    private:
        double *mpND_in, *mpND_out, *mpLimit;

    public:
        static Component *Creator()
        {
            return new SignalSmallerThan();
        }

        void configure()
        {
            addInputVariable("in", "", "", 0.0, &mpND_in);
            addInputVariable("x_limit", "Limit Value", "", 0.0, &mpLimit);
            addOutputVariable("out", "in<x_limit", "", &mpND_out);
        }


        void initialize()
        {
            simulateOneTimestep();
        }


        void simulateOneTimestep()
        {
            //Smaller than equations
            (*mpND_out) = boolToDouble( (*mpND_in) < (*mpLimit) );
        }
    };
}

#endif // SIGNALSMALLERTHAN_HPP_INCLUDED
