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
//! @file   HydraulicPumpPiston.hpp
//! @author Robert Braun <robert.braun@liu.se>
//! @date   2012-10-12
//!
//! @brief Contains a hydraulic piston with one chamber of C type
//!
//$Id: HydraulicPumpPiston.hpp 8139 2015-06-17 12:30:13Z petno25 $

#ifndef HYDRAULICPUMPPISTON_HPP__INCLUDED
#define HYDRAULICPUMPPISTON_HPP__INCLUDED

#include <sstream>

#include "ComponentEssentials.h"
#include "ComponentUtilities.h"

namespace hopsan {

//!
//! @brief
//! @ingroup HydraulicComponents
//!
class HydraulicPumpPiston : public ComponentC
{

    private:
        double wfak, alpha;
        double ci1, cl1;  //Members because old value need to be remembered (c1 and c2 are remembered through nodes)

        //Node data pointers
        double *mpA1, *mpSl, *mpV01, *mpBp, *mpBetae, *mpCLeak, *mpF0;
        double *mpP1_p, *mpP1_q, *mpP1_c, *mpP1_Zc;
        double *mpP3_f, *mpP3_x, *mpP3_v, *mpP3_c, *mpP3_Zx, *mpP3_me;

        //Ports
        Port *mpP1, *mpP3;

    public:
        static Component *Creator()
        {
            return new HydraulicPumpPiston();
        }

        void configure()
        {
            //Set member attributes
            wfak = 0.1;
            alpha = 0.1;

            //Add ports to the component
            mpP1 = addPowerPort("P1", "NodeHydraulic");
            mpP3 = addPowerPort("P3", "NodeMechanic");

            //Register changeable parameters to the HOPSAN++ core
            addInputVariable("A_1", "Piston Area", "m^2", 0.001, &mpA1);
            addInputVariable("s_l", "Stroke", "m", 1.0, &mpSl);
            addInputVariable("V_1", "Dead Volume in Chamber 1", "m^3", 0.0003, &mpV01);
            addInputVariable("B_p", "Viscous Friction", "Ns/m", 1000.0, &mpBp);
            addInputVariable("Beta_e", "Bulk Modulus", "Pa", 1000000000.0, &mpBetae);
            addInputVariable("c_leak", "Leakage Coefficient", "", 0.00000000001, &mpCLeak);
            addInputVariable("F_0", "Spring Force", "N", 1000, &mpF0);
        }


        void initialize()
        {
            mpP1_p = getSafeNodeDataPtr(mpP1, NodeHydraulic::Pressure);
            mpP1_q = getSafeNodeDataPtr(mpP1, NodeHydraulic::Flow);
            mpP1_c = getSafeNodeDataPtr(mpP1, NodeHydraulic::WaveVariable);
            mpP1_Zc = getSafeNodeDataPtr(mpP1, NodeHydraulic::CharImpedance);

            mpP3_f = getSafeNodeDataPtr(mpP3, NodeMechanic::Force);
            mpP3_x = getSafeNodeDataPtr(mpP3, NodeMechanic::Position);
            mpP3_v = getSafeNodeDataPtr(mpP3, NodeMechanic::Velocity);
            mpP3_c = getSafeNodeDataPtr(mpP3, NodeMechanic::WaveVariable);
            mpP3_Zx = getSafeNodeDataPtr(mpP3, NodeMechanic::CharImpedance);
            mpP3_me = getSafeNodeDataPtr(mpP3, NodeMechanic::EquivalentMass);

            //Declare local variables;
            double p1, x3, v3;
            double Zc1, c3, Zx3;
            double qi1, V1, qLeak, V1min;

            //Read variables from nodes
            p1 = (*mpP1_p);
            x3 = (*mpP3_x);
            v3 = (*mpP3_v);
            const double A1 = (*mpA1);
            const double V01 = (*mpV01);
            const double betae = (*mpBetae);
            const double bp = (*mpBp);
            const double cLeak = (*mpCLeak);
            const double F0 = (*mpF0);

            //Size of volumes
            V1 = V01+A1*(-x3);
            V1min = betae*mTimestep*mTimestep*A1*A1/(wfak*1.0);
            if(V1<V1min) V1 = V1min;

            Zc1 = 3.0 / 2.0 * betae/V1*mTimestep/(1.0-alpha);    //Number of ports in volume is 2 internal plus the external ones
            Zx3 = A1*A1*Zc1 + bp;

            //Internal flows
            qi1 = v3*A1;

            ci1 = p1 + Zc1*qi1;

            //Leakage flow
            qLeak = cLeak*(p1);

            cl1 = p1 + Zc1*(-qLeak);

            c3 = A1*ci1+F0;

            //Write to nodes
            (*mpP1_c) = p1 + Zc1*(*mpP1_q);
            (*mpP1_Zc) = Zc1;
            (*mpP3_c) = c3;
            (*mpP3_Zx) = Zx3;
        }

        void simulateOneTimestep()
        {
            //Declare local variables;
            double c3, Zx3;
            double V1, qLeak, qi1, p1mean, V1min;
            //Read variables from nodes
            double q1 = (*mpP1_q);
            double c1 = (*mpP1_c);
            double Zc1 = (*mpP1_Zc);
            double x3 = (*mpP3_x);
            double v3 = (*mpP3_v);
            double me = (*mpP3_me);

            const double A1 = (*mpA1);
            const double sl = (*mpSl);
            const double V01 = (*mpV01);
            const double betae = (*mpBetae);
            const double bp = (*mpBp);
            const double cLeak = (*mpCLeak);
            const double F0 = (*mpF0);


            //Leakage flow
            qLeak = cLeak*(cl1)/(1.0+cLeak*(Zc1));

            //Internal flows
            qi1 = v3*A1;

            //Size of volumes
            V1 = V01+A1*(sl/2-x3);
            V1min = betae*mTimestep*mTimestep*A1*A1/(wfak*me);
            if(V1<V1min) V1 = V1min;

            // Cylinder volumes are modelled the same way as the multiport volume:
            //   c1 = Wave variable for external port
            //   ci1 = Wave variable for internal (mechanical) port
            //   cl1 = Wave variable for leakage port

            //Volume 1
            Zc1 = 3.0 / 2.0 * betae/V1*mTimestep/(1.0-alpha);    //Number of ports in volume is 2 internal plus the external ones

            p1mean = (ci1 + Zc1*2.0*qi1) + (cl1 + Zc1*2.0*(-qLeak)) + (c1 + 2.0*Zc1*q1);
            p1mean = p1mean/3.0;
            ci1 = std::max(0.0, alpha * ci1 + (1.0 - alpha)*(p1mean*2.0 - ci1 - 2.0*Zc1*qi1));
            cl1 = std::max(0.0, alpha * cl1 + (1.0 - alpha)*(p1mean*2.0 - cl1 - 2.0*Zc1*(-qLeak)));

            //Internal mechanical port
            c3 = A1*ci1+F0;
            Zx3 = A1*A1*Zc1+ bp;

            //Write to nodes
            (*mpP1_c) = alpha * c1 + (1.0 - alpha)*(p1mean*2 - c1 - 2*Zc1*q1);
            (*mpP1_Zc) = Zc1;
            (*mpP3_c) = c3;
            (*mpP3_Zx) = Zx3;
        }
    };
}

#endif
