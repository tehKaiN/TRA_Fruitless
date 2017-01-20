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
//! @file   SignalTimestep.hpp
//! @author Peter Nordin <peter.nordin@liu.se>
//! @date   2015-11-17
//$Id: SignalTimestep.hpp 8468 2015-11-17 16:20:51Z petno25 $


#ifndef SIGNALTIMESTEP_HPP
#define SIGNALTIMESTEP_HPP

#include "ComponentEssentials.h"

namespace hopsan {

//!
//! @brief
//! @ingroup SignalComponents
//!
class SignalTimestep : public ComponentSignal
{

private:
    double *mpTs;

public:
    static Component *Creator()
    {
        return new SignalTimestep();
    }

    void configure()
    {
        addOutputVariable("ts", "System simulation time step length", "Time", &mpTs);
    }


    void initialize()
    {
        simulateOneTimestep();
    }


    void simulateOneTimestep()
    {
        (*mpTs) = mTimestep;
    }
};
}

#endif // SIGNALTIMESTEP_HPP

