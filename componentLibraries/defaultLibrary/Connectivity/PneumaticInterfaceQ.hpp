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
//! @file   PneumaticInterfaceQ.hpp
//! @author Robert Braun <robert.braun@liu.se>
//! @date   2013-03-01
//!
//! @brief Contains a pneumatic interface component of Q-type
//!
//$Id: PneumaticInterfaceQ.hpp 8139 2015-06-17 12:30:13Z petno25 $

#ifndef PNEUMATICINTERFACEQ_HPP_INCLUDED
#define PNEUMATICINTERFACEQ_HPP_INCLUDED

#include "ComponentEssentials.h"

namespace hopsan {

    //!
    //! @brief
    //! @ingroup MechanicalComponents
    //!
    class PneumaticInterfaceQ : public ComponentQ
    {

    private:
        Port *mpP1;

    public:
        static Component *Creator()
        {
            return new PneumaticInterfaceQ();
        }

        void configure()
        {
            mpP1 = addPowerPort("P1", "NodePneumatic");
        }

        void initialize()
        {
            //Interfacing is handled through readnode/writenode from the RT wrapper file
        }

        void simulateOneTimestep()
        {
            //Interfacing is handled through readnode/writenode from the RT wrapper file
        }
    };
}

#endif // PNEUMATICINTERFACEQ_HPP_INCLUDED
