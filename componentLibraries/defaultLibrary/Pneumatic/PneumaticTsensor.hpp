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

#ifndef PNEUMATICTSENSOR_HPP_INCLUDED
#define PNEUMATICTSENSOR_HPP_INCLUDED

#include <iostream>
#include "ComponentEssentials.h"
#include "ComponentUtilities.h"
#include "math.h"

//!
//! @file PneumaticTsensor.hpp
//! @author Petter Krus <petter.krus@liu.se>
//! @date Wed 13 Mar 2013 17:01:51
//! @brief Pneumatic tempreature sensor
//! @ingroup PneumaticComponents
//!
//$Id: PneumaticTsensor.hpp 8139 2015-06-17 12:30:13Z petno25 $

using namespace hopsan;

class PneumaticTsensor : public ComponentSignal
{
private:
     Port *mpPp1;
     double *mpPp1_T, *mpOut;

public:
     static Component *Creator()
     {
        return new PneumaticTsensor();
     }

     void configure()
     {
        mpPp1=addReadPort("Pp1","NodePneumatic", "", Port::NotRequired);
        addOutputVariable("out", "Temperature", "K", &mpOut);
     }

    void initialize()
     {
        mpPp1_T=getSafeNodeDataPtr(mpPp1, NodePneumatic::Temperature);
        simulateOneTimestep();
     }

    void simulateOneTimestep()
     {
        (*mpOut) = (*mpPp1_T);
     }
};
#endif // PNEUMATICTSENSOR_HPP_INCLUDED