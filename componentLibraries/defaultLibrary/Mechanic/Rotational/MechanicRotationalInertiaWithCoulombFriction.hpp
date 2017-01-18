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
//! @file   MechanicRotationalInertiaWithCoulumbFriction.hpp
//! @author Robert Braun <robert.braun@liu.se>
//! @date   2011-09-01
//!
//! @brief Contains a rotational inertia with coulumb friction and damper
//!
//$Id: MechanicRotationalInertiaWithCoulombFriction.hpp 8139 2015-06-17 12:30:13Z petno25 $

#ifndef MECHANICROTATIONALINERTIAWITHCOULUMBFRICTION_HPP_INCLUDED
#define MECHANICROTATIONALINERTIAWITHCOULUMBFRICTION_HPP_INCLUDED

#include "ComponentEssentials.h"
#include "ComponentUtilities.h"

namespace hopsan {

    //!
    //! @brief
    //! @ingroup MechanicalComponents
    //!
    class MechanicRotationalInertiaWithCoulombFriction : public ComponentQ
    {

    private:
        double *mpJ, *mpB;
        double ts, tk;
        double *mpP1_T, *mpP1_a, *mpP1_w, *mpP1_c, *mpP1_Zx, *mpP2_T, *mpP2_a, *mpP2_w, *mpP2_c, *mpP2_Zx;  //Node data pointers
        DoubleIntegratorWithDampingAndCoulombFriction mIntegrator;
        Port *mpP1, *mpP2;                                                                                  //Ports

    public:
        static Component *Creator()
        {
            return new MechanicRotationalInertiaWithCoulombFriction();
        }

        void configure()
        {
            mpP1 = addPowerPort("P1", "NodeMechanicRotational");
            mpP2 = addPowerPort("P2", "NodeMechanicRotational");
            addInputVariable("J", "Inertia", "kgm^2", 1.0, &mpJ);
            addInputVariable("B", "Viscous Friction Coefficient", "Nms/rad", 10, &mpB);
            addConstant("t_s", "Static Friction Torque", "Nm", 50, ts);
            addConstant("t_k", "Kinetic Friction Torque", "Nm", 45, tk);
        }


        void initialize()
        {
            //Assign node data pointers
            mpP1_T = getSafeNodeDataPtr(mpP1, NodeMechanicRotational::Torque);
            mpP1_a = getSafeNodeDataPtr(mpP1, NodeMechanicRotational::Angle);
            mpP1_w = getSafeNodeDataPtr(mpP1, NodeMechanicRotational::AngularVelocity);
            mpP1_c = getSafeNodeDataPtr(mpP1, NodeMechanicRotational::WaveVariable);
            mpP1_Zx = getSafeNodeDataPtr(mpP1, NodeMechanicRotational::CharImpedance);

            mpP2_T = getSafeNodeDataPtr(mpP2, NodeMechanicRotational::Torque);
            mpP2_a = getSafeNodeDataPtr(mpP2, NodeMechanicRotational::Angle);
            mpP2_w = getSafeNodeDataPtr(mpP2, NodeMechanicRotational::AngularVelocity);
            mpP2_c = getSafeNodeDataPtr(mpP2, NodeMechanicRotational::WaveVariable);
            mpP2_Zx = getSafeNodeDataPtr(mpP2, NodeMechanicRotational::CharImpedance);

            // Print debug message if start angles or velocities doe not match
            if( !fuzzyEqual((*mpP1_a), -(*mpP2_a)) )
            {
                addWarningMessage("Start angles does not match in:  "+mpP1->getName()+"  and  "+mpP2->getName());
            }
            if( !fuzzyEqual((*mpP1_w), -(*mpP2_w)) )
            {
                addWarningMessage("Start velocities does not match in:  "+mpP1->getName()+"  and  "+mpP2->getName());
            }

            // Initialize
            mIntegrator.initialize(mTimestep, 0, ts/(*mpJ), tk/(*mpJ), (*mpP1_T)-(*mpP2_T), (*mpP2_a), (*mpP2_w));
        }


        void simulateOneTimestep()
        {
            double t1, a1, w1, c1, Zx1, t2, a2, w2, c2, Zx2, J, B;

            //Get variable values from nodes
            a1 = (*mpP1_a);
            c1 = (*mpP1_c);
            Zx1 = (*mpP1_Zx);
            a2 = (*mpP2_a);
            c2 = (*mpP2_c);
            Zx2 = (*mpP2_Zx);
            J = (*mpJ);
            B = (*mpB);

            mIntegrator.setDamping((B+Zx1+Zx2) / J * mTimestep);
            mIntegrator.integrateWithUndo((c1-c2)/J);
            w2 = mIntegrator.valueFirst();
            a2 = mIntegrator.valueSecond();

            w1 = -w2;
            a1 = -a2;
            t1 = c1 + Zx1*w1;
            t2 = c2 + Zx2*w2;

            //Write new values to nodes
            (*mpP1_T) = t1;
            (*mpP1_a) = a1;
            (*mpP1_w) = w1;
            (*mpP2_T) = t2;
            (*mpP2_a) = a2;
            (*mpP2_w) = w2;
        }
    };
}

#endif // MECHANICROTATIONALINERTIAWITHCOULUMBFRICTION_HPP_INCLUDED

