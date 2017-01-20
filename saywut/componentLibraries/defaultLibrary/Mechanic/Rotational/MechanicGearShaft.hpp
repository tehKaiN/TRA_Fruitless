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

#ifndef MECHANICGEARSHAFT_HPP_INCLUDED
#define MECHANICGEARSHAFT_HPP_INCLUDED

#include <iostream>
#include "ComponentEssentials.h"
#include "ComponentUtilities.h"
#include "math.h"

//!
//! @file MechanicGearShaft.hpp
//! @author Petter Krus <petter.krus@liu.se>
//! @date Fri 19 Dec 2014 16:29:43
//! @brief Rotational shaft with torsional spring
//! @ingroup MechanicComponents
//!
//==This code has been autogenerated using Compgen==
//from 
/*{, C:, HopsanTrunk, componentLibraries, defaultLibrary, Mechanic, \
Rotational}/MechanicGearShaft.nb*/

using namespace hopsan;

class MechanicGearShaft : public ComponentC
{
private:
     double Ks;
     double alpha;
     Port *mpPmr1;
     Port *mpPmr2;
     int mNstep;
     //Port Pmr1 variable
     double tormr1;
     double thetamr1;
     double wmr1;
     double cmr1;
     double Zcmr1;
     double eqInertiamr1;
     //Port Pmr2 variable
     double tormr2;
     double thetamr2;
     double wmr2;
     double cmr2;
     double Zcmr2;
     double eqInertiamr2;
//==This code has been autogenerated using Compgen==
     //inputVariables
     double gearRatio;
     //outputVariables
     //InitialExpressions variables
     double Zthetarot1;
     double Zthetarot2;
     double cmr1f;
     double cmr2f;
     //LocalExpressions variables
     double cmr10;
     double cmr20;
     //Expressions variables
     //Port Pmr1 pointer
     double *mpND_tormr1;
     double *mpND_thetamr1;
     double *mpND_wmr1;
     double *mpND_cmr1;
     double *mpND_Zcmr1;
     double *mpND_eqInertiamr1;
     //Port Pmr2 pointer
     double *mpND_tormr2;
     double *mpND_thetamr2;
     double *mpND_wmr2;
     double *mpND_cmr2;
     double *mpND_Zcmr2;
     double *mpND_eqInertiamr2;
     //Delay declarations
//==This code has been autogenerated using Compgen==
     //inputVariables pointers
     double *mpgearRatio;
     //inputParameters pointers
     double *mpKs;
     double *mpalpha;
     //outputVariables pointers
     EquationSystemSolver *mpSolver;

public:
     static Component *Creator()
     {
        return new MechanicGearShaft();
     }

     void configure()
     {
//==This code has been autogenerated using Compgen==

        mNstep=9;

        //Add ports to the component
        mpPmr1=addPowerPort("Pmr1","NodeMechanicRotational");
        mpPmr2=addPowerPort("Pmr2","NodeMechanicRotational");
        //Add inputVariables to the component
            addInputVariable("gearRatio","gear ratio \
wmr1/wmr2","",1.,&mpgearRatio);

        //Add inputParammeters to the component
            addInputVariable("Ks", "Spring constant (when applied at 2)", \
"Nm/rad", 1000.,&mpKs);
            addInputVariable("alpha", "damping factor", "", 0.9,&mpalpha);
        //Add outputVariables to the component

//==This code has been autogenerated using Compgen==
        //Add constantParameters
            addConstant("alpha", "numerical damping", "", 0.3,alpha);
     }

    void initialize()
     {
        //Read port variable pointers from nodes
        //Port Pmr1
        mpND_tormr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::Torque);
        mpND_thetamr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::Angle);
        mpND_wmr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::AngularVelocity);
        mpND_cmr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::WaveVariable);
        mpND_Zcmr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::CharImpedance);
        mpND_eqInertiamr1=getSafeNodeDataPtr(mpPmr1, \
NodeMechanicRotational::EquivalentInertia);
        //Port Pmr2
        mpND_tormr2=getSafeNodeDataPtr(mpPmr2, \
NodeMechanicRotational::Torque);
        mpND_thetamr2=getSafeNodeDataPtr(mpPmr2, \
NodeMechanicRotational::Angle);
        mpND_wmr2=getSafeNodeDataPtr(mpPmr2, \
NodeMechanicRotational::AngularVelocity);
        mpND_cmr2=getSafeNodeDataPtr(mpPmr2, \
NodeMechanicRotational::WaveVariable);
        mpND_Zcmr2=getSafeNodeDataPtr(mpPmr2, \
NodeMechanicRotational::CharImpedance);
        mpND_eqInertiamr2=getSafeNodeDataPtr(mpPmr2, \
NodeMechanicRotational::EquivalentInertia);

        //Read variables from nodes
        //Port Pmr1
        tormr1 = (*mpND_tormr1);
        thetamr1 = (*mpND_thetamr1);
        wmr1 = (*mpND_wmr1);
        cmr1 = (*mpND_cmr1);
        Zcmr1 = (*mpND_Zcmr1);
        eqInertiamr1 = (*mpND_eqInertiamr1);
        //Port Pmr2
        tormr2 = (*mpND_tormr2);
        thetamr2 = (*mpND_thetamr2);
        wmr2 = (*mpND_wmr2);
        cmr2 = (*mpND_cmr2);
        Zcmr2 = (*mpND_Zcmr2);
        eqInertiamr2 = (*mpND_eqInertiamr2);

        //Read inputVariables from nodes
        gearRatio = (*mpgearRatio);

        //Read inputParameters from nodes
        Ks = (*mpKs);
        alpha = (*mpalpha);

        //Read outputVariables from nodes

//==This code has been autogenerated using Compgen==
        //InitialExpressions
        Zthetarot1 = (Ks*mTimestep)/((1 - alpha)*Power(gearRatio,2));
        Zthetarot2 = (Ks*mTimestep)/(1 - alpha);
        cmr1 = gearRatio*tormr1 - wmr1*Zthetarot1;
        cmr2 = tormr2 - wmr2*Zthetarot2;
        cmr1f = tormr1/gearRatio;
        cmr2f = tormr2;

        //LocalExpressions
        Zthetarot1 = (Ks*mTimestep)/((1 - alpha)*Power(gearRatio,2));
        Zthetarot2 = (Ks*mTimestep)/(1 - alpha);
        cmr10 = cmr2 + 2*wmr2*Zthetarot2;
        cmr20 = cmr1*gearRatio + (2*Ks*mTimestep*wmr1)/((1 - \
alpha)*gearRatio);

        //Initialize delays

     }
    void simulateOneTimestep()
     {
        //Read variables from nodes
        //Port Pmr1
        tormr1 = (*mpND_tormr1);
        thetamr1 = (*mpND_thetamr1);
        wmr1 = (*mpND_wmr1);
        eqInertiamr1 = (*mpND_eqInertiamr1);
        //Port Pmr2
        tormr2 = (*mpND_tormr2);
        thetamr2 = (*mpND_thetamr2);
        wmr2 = (*mpND_wmr2);
        eqInertiamr2 = (*mpND_eqInertiamr2);

        //Read inputVariables from nodes
        gearRatio = (*mpgearRatio);

        //LocalExpressions
        Zthetarot1 = (Ks*mTimestep)/((1 - alpha)*Power(gearRatio,2));
        Zthetarot2 = (Ks*mTimestep)/(1 - alpha);
        cmr10 = cmr2 + 2*wmr2*Zthetarot2;
        cmr20 = cmr1*gearRatio + (2*Ks*mTimestep*wmr1)/((1 - \
alpha)*gearRatio);

          //Expressions
          cmr1 = cmr1f/gearRatio;
          cmr2 = cmr2f;
          cmr1f = (1 - alpha)*cmr10 + alpha*cmr1f;
          cmr2f = (1 - alpha)*cmr20 + alpha*cmr2f;
          Zcmr1 = Zthetarot1;
          Zcmr2 = Zthetarot2;

        //Calculate the delayed parts


        //Write new values to nodes
        //Port Pmr1
        (*mpND_cmr1)=cmr1;
        (*mpND_Zcmr1)=Zcmr1;
        //Port Pmr2
        (*mpND_cmr2)=cmr2;
        (*mpND_Zcmr2)=Zcmr2;
        //outputVariables

        //Update the delayed variabels

     }
    void deconfigure()
    {
        delete mpSolver;
    }
};
#endif // MECHANICGEARSHAFT_HPP_INCLUDED
