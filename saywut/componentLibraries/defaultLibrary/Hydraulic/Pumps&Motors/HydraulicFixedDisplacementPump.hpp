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
//! @file   HydraulicFixedDisplacementPump.hpp
//! @author Robert Braun <robert.braun@liu.se>
//! @date   2010-01-07
//!
//! @brief Contains a hydraulic pump with fixed displacement component
//!
//$Id: HydraulicFixedDisplacementPump.hpp 8139 2015-06-17 12:30:13Z petno25 $

#ifndef HYDRAULICFIXEDDISPLACEMENTPUMP_HPP_INCLUDED
#define HYDRAULICFIXEDDISPLACEMENTPUMP_HPP_INCLUDED

#include <iostream>
#include "ComponentEssentials.h"

namespace hopsan {

    //!
    //! @brief
    //! @ingroup HydraulicComponents
    //!
    class HydraulicFixedDisplacementPump : public ComponentQ
    {
    private:
        double *mpND_p1, *mpND_q1, *mpND_c1, *mpND_Zc1, *mpND_p2, *mpND_q2, *mpND_c2, *mpND_Zc2, *mpND_a;
        double *mpN, *mpDp, *mpClp;
        Port *mpP1, *mpP2;

    public:
        static Component *Creator()
        {
            return new HydraulicFixedDisplacementPump();
        }

        void configure()
        {
            mpP1 = addPowerPort("P1", "NodeHydraulic");
            mpP2 = addPowerPort("P2", "NodeHydraulic");

            addOutputVariable("a", "Angle", "rad", 0.0, &mpND_a);
            addInputVariable("n_p", "Angular Velocity", "rad/s", 250.0, &mpN);
            addInputVariable("D_p", "Displacement", "m^3/rev", 0.00005, &mpDp);
            addInputVariable("C_lp", "Leakage Coefficient", "(m^3/s)/Pa", 0.0, &mpClp);
        }


        void initialize()
        {
            mpND_p1 = getSafeNodeDataPtr(mpP1, NodeHydraulic::Pressure);
            mpND_q1 = getSafeNodeDataPtr(mpP1, NodeHydraulic::Flow);
            mpND_c1 = getSafeNodeDataPtr(mpP1, NodeHydraulic::WaveVariable);
            mpND_Zc1 = getSafeNodeDataPtr(mpP1, NodeHydraulic::CharImpedance);

            mpND_p2 = getSafeNodeDataPtr(mpP2, NodeHydraulic::Pressure);
            mpND_q2 = getSafeNodeDataPtr(mpP2, NodeHydraulic::Flow);
            mpND_c2 = getSafeNodeDataPtr(mpP2, NodeHydraulic::WaveVariable);
            mpND_Zc2 = getSafeNodeDataPtr(mpP2, NodeHydraulic::CharImpedance);

            (*mpND_a) = 0;
        }


        void simulateOneTimestep()
        {
            //Declare local variables
            double p1, q1, c1, Zc1, p2, q2, c2, Zc2;
            bool cav = false;

            double n = (*mpN);
            double dp = (*mpDp);
            double Clp = (*mpClp);

            //Get variable values from nodes
            c1 = (*mpND_c1);
            Zc1 = (*mpND_Zc1);
            c2 = (*mpND_c2);
            Zc2 = (*mpND_Zc2);

            //Fixed Displacement Pump equations
            q2 = ( dp*n/(2.0*pi) + Clp*(c1-c2) ) / ( (Zc1+Zc2)*Clp+1 );
            q1 = -q2;
            p2 = c2 + Zc2*q2;
            p1 = c1 + Zc1*q1;

            /* Cavitation Check */
            if (p1 < 0.0)
            {
                c1 = 0.0;
                Zc1 = 0.0;
                cav = true;
            }
            if (p2 < 0.0)
            {
                c2 = 0.0;
                Zc2 = 0.0;
                cav = true;
            }
            if (cav)
            {
                q2 = ( dp*n/(2.0*pi) + Clp*(c1-c2) ) / ( (Zc1+Zc2)*Clp+1 );

                p1 = c1 + Zc1 * q1;
                p2 = c2 + Zc2 * q2;
                if (p1 <= 0.0)
                {
                    p1 = 0.0;
                    q2 = std::min(q2, 0.0);
                }
                if (p2 <= 0.0)
                {
                    p2 = 0.0;
                    q2 = std::max(q2, 0.0);
                }
                q1 = -q2;
            }

            //Write new values to nodes
            (*mpND_p1) = p1;
            (*mpND_q1) = q1;
            (*mpND_p2) = p2;
            (*mpND_q2) = q2;
            (*mpND_a) += n*mTimestep;
        }
    };
}

#endif // HYDRAULICFIXEDDISPLACEMENTPUMP_HPP_INCLUDED