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
//! @file   HydraulicPressureSensor.hpp
//! @author Björn Eriksson <bjorn.eriksson@liu.se>
//! @date   2010-01-16
//!
//! @brief Contains a Hydraulic Pressure Sensor Component
//!
//$Id: HydraulicPressureSensor.hpp 8207 2015-07-16 18:49:37Z petno25 $

#ifndef HYDRAULICPRESSURESENSOR_HPP_INCLUDED
#define HYDRAULICPRESSURESENSOR_HPP_INCLUDED

#include <iostream>
#include "ComponentEssentials.h"

namespace hopsan {

    //!
    //! @brief
    //! @ingroup HydraulicComponents
    //!
    class HydraulicPressureSensor : public ComponentSignal
    {
    private:
        Port *mpP1;

        double *mpND_p, *mpOut;

    public:
        static Component *Creator()
        {
            return new HydraulicPressureSensor();
        }

        void configure()
        {
            mpP1 = addReadPort("P1", "NodeHydraulic", "", Port::NotRequired);
            addOutputVariable("out", "Pressure", "Pressure", &mpOut);
        }


        void initialize()
        {
            mpND_p = getSafeNodeDataPtr(mpP1, NodeHydraulic::Pressure);
            simulateOneTimestep(); //Set initial output node value
        }


        void simulateOneTimestep()
        {
            (*mpOut) = (*mpND_p);
        }
    };
}

#endif // HYDRAULICPRESSURESENSOR_HPP_INCLUDED
