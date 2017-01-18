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

#ifndef PNEUMATICMACHINE_HPP_INCLUDED
#define PNEUMATICMACHINE_HPP_INCLUDED

#include <iostream>
#include "ComponentEssentials.h"
#include "ComponentUtilities.h"
#include "math.h"

//!
//! @file PneumaticMachine.hpp
//! @author Petter Krus <petter.krus@liu.se>
//! @date Fri 23 Jan 2015 11:34:49
//! @brief This is a pump/motor with an inertia load
//! @ingroup PneumaticComponents
//!
//==This code has been autogenerated using Compgen==
//from 
/*{, C:, HopsanTrunk, componentLibraries, defaultLibrary, \
Pneumatic}/PneumaticMachine.nb*/

using namespace hopsan;

class PneumaticMachine : public ComponentQ
{
private:
     double Dm;
     double Cim;
     double Bm;
     double Jm;
     double R;
     double cv;
     Port *mpPp1;
     Port *mpPp2;
     Port *mpPmr1;
     double delayParts1[9];
     double delayParts2[9];
     double delayParts3[9];
     double delayParts4[9];
     double delayParts5[9];
     double delayParts6[9];
     double delayParts7[9];
     double delayParts8[9];
     Matrix jacobianMatrix;
     Vec systemEquations;
     Matrix delayedPart;
     int i;
     int iter;
     int mNoiter;
     double jsyseqnweight[4];
     int order[8];
     int mNstep;
     //Port Pp1 variable
     double pp1;
     double qmp1;
     double Tp1;
     double dEp1;
     double cp1;
     double Zcp1;
     //Port Pp2 variable
     double pp2;
     double qmp2;
     double Tp2;
     double dEp2;
     double cp2;
     double Zcp2;
     //Port Pmr1 variable
     double tormr1;
     double thetamr1;
     double wmr1;
     double cmr1;
     double Zcmr1;
     double eqInertiamr1;
//==This code has been autogenerated using Compgen==
     //inputVariables
     //outputVariables
     //LocalExpressions variables
     double cp;
     //Expressions variables
     //Port Pp1 pointer
     double *mpND_pp1;
     double *mpND_qmp1;
     double *mpND_Tp1;
     double *mpND_dEp1;
     double *mpND_cp1;
     double *mpND_Zcp1;
     //Port Pp2 pointer
     double *mpND_pp2;
     double *mpND_qmp2;
     double *mpND_Tp2;
     double *mpND_dEp2;
     double *mpND_cp2;
     double *mpND_Zcp2;
     //Port Pmr1 pointer
     double *mpND_tormr1;
     double *mpND_thetamr1;
     double *mpND_wmr1;
     double *mpND_cmr1;
     double *mpND_Zcmr1;
     double *mpND_eqInertiamr1;
     //Delay declarations
//==This code has been autogenerated using Compgen==
     //inputVariables pointers
     //inputParameters pointers
     double *mpDm;
     double *mpCim;
     double *mpBm;
     double *mpJm;
     double *mpR;
     double *mpcv;
     //outputVariables pointers
     Delay mDelayedPart10;
     Delay mDelayedPart11;
     Delay mDelayedPart12;
     Delay mDelayedPart20;
     Delay mDelayedPart21;
     Delay mDelayedPart30;
     Delay mDelayedPart40;
     Delay mDelayedPart50;
     EquationSystemSolver *mpSolver;

public:
     static Component *Creator()
     {
        return new PneumaticMachine();
     }

     void configure()
     {
//==This code has been autogenerated using Compgen==

        mNstep=9;
        jacobianMatrix.create(8,8);
        systemEquations.create(8);
        delayedPart.create(9,6);
        mNoiter=2;
        jsyseqnweight[0]=1;
        jsyseqnweight[1]=0.67;
        jsyseqnweight[2]=0.5;
        jsyseqnweight[3]=0.5;


        //Add ports to the component
        mpPp1=addPowerPort("Pp1","NodePneumatic");
        mpPp2=addPowerPort("Pp2","NodePneumatic");
        mpPmr1=addPowerPort("Pmr1","NodeMechanicRotational");
        //Add inputVariables to the component

        //Add inputParammeters to the component
            addInputVariable("Dm", "Displacement", "m3", \
0.000049999999999999996,&mpDm);
            addInputVariable("Cim", "Leak coeff.", "m3/(s Pa)", 0.,&mpCim);
            addInputVariable("Bm", "Visc. friction coeff.", "N/m/s", \
0.,&mpBm);
            addInputVariable("Jm", "Moment of inertia", "kg/m2", 0.1,&mpJm);
            addInputVariable("R", "Gas constant", "J/Kg K", 287.,&mpR);
            addInputVariable("cv", "heatcoeff", "J/Kg K", 718,&mpcv);
        //Add outputVariables to the component

//==This code has been autogenerated using Compgen==
        //Add constantParameters
        mpSolver = new EquationSystemSolver(this,8);
     }

    void initialize()
     {
        //Read port variable pointers from nodes
        //Port Pp1
        mpND_pp1=getSafeNodeDataPtr(mpPp1, NodePneumatic::Pressure);
        mpND_qmp1=getSafeNodeDataPtr(mpPp1, NodePneumatic::MassFlow);
        mpND_Tp1=getSafeNodeDataPtr(mpPp1, NodePneumatic::Temperature);
        mpND_dEp1=getSafeNodeDataPtr(mpPp1, NodePneumatic::EnergyFlow);
        mpND_cp1=getSafeNodeDataPtr(mpPp1, NodePneumatic::WaveVariable);
        mpND_Zcp1=getSafeNodeDataPtr(mpPp1, NodePneumatic::CharImpedance);
        //Port Pp2
        mpND_pp2=getSafeNodeDataPtr(mpPp2, NodePneumatic::Pressure);
        mpND_qmp2=getSafeNodeDataPtr(mpPp2, NodePneumatic::MassFlow);
        mpND_Tp2=getSafeNodeDataPtr(mpPp2, NodePneumatic::Temperature);
        mpND_dEp2=getSafeNodeDataPtr(mpPp2, NodePneumatic::EnergyFlow);
        mpND_cp2=getSafeNodeDataPtr(mpPp2, NodePneumatic::WaveVariable);
        mpND_Zcp2=getSafeNodeDataPtr(mpPp2, NodePneumatic::CharImpedance);
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

        //Read variables from nodes
        //Port Pp1
        pp1 = (*mpND_pp1);
        qmp1 = (*mpND_qmp1);
        Tp1 = (*mpND_Tp1);
        dEp1 = (*mpND_dEp1);
        cp1 = (*mpND_cp1);
        Zcp1 = (*mpND_Zcp1);
        //Port Pp2
        pp2 = (*mpND_pp2);
        qmp2 = (*mpND_qmp2);
        Tp2 = (*mpND_Tp2);
        dEp2 = (*mpND_dEp2);
        cp2 = (*mpND_cp2);
        Zcp2 = (*mpND_Zcp2);
        //Port Pmr1
        tormr1 = (*mpND_tormr1);
        thetamr1 = (*mpND_thetamr1);
        wmr1 = (*mpND_wmr1);
        cmr1 = (*mpND_cmr1);
        Zcmr1 = (*mpND_Zcmr1);
        eqInertiamr1 = (*mpND_eqInertiamr1);

        //Read inputVariables from nodes

        //Read inputParameters from nodes
        Dm = (*mpDm);
        Cim = (*mpCim);
        Bm = (*mpBm);
        Jm = (*mpJm);
        R = (*mpR);
        cv = (*mpcv);

        //Read outputVariables from nodes

//==This code has been autogenerated using Compgen==

        //LocalExpressions
        cp = cv + R;

        //Initialize delays
        delayParts1[1] = (-0.31831*Dm*Power(mTimestep,2)*pp1 + \
0.31831*Dm*Power(mTimestep,2)*pp2 - 8.*Jm*thetamr1 + \
2.*Power(mTimestep,2)*tormr1)/(4.*Jm + 2.*Bm*mTimestep);
        mDelayedPart11.initialize(mNstep,delayParts1[1]);
        delayParts1[2] = (-0.159155*Dm*Power(mTimestep,2)*pp1 + \
0.159155*Dm*Power(mTimestep,2)*pp2 + 4.*Jm*thetamr1 - \
2.*Bm*mTimestep*thetamr1 + Power(mTimestep,2)*tormr1)/(4.*Jm + \
2.*Bm*mTimestep);
        mDelayedPart12.initialize(mNstep,delayParts1[2]);
        delayParts2[1] = (-0.159155*Dm*mTimestep*pp1 + \
0.159155*Dm*mTimestep*pp2 + mTimestep*tormr1 - 2.*Jm*wmr1 + \
Bm*mTimestep*wmr1)/(2.*Jm + Bm*mTimestep);
        mDelayedPart21.initialize(mNstep,delayParts2[1]);

        delayedPart[1][1] = delayParts1[1];
        delayedPart[1][2] = mDelayedPart12.getIdx(1);
        delayedPart[2][1] = delayParts2[1];
        delayedPart[3][1] = delayParts3[1];
        delayedPart[4][1] = delayParts4[1];
        delayedPart[5][1] = delayParts5[1];
        delayedPart[6][1] = delayParts6[1];
        delayedPart[7][1] = delayParts7[1];
        delayedPart[8][1] = delayParts8[1];
     }
    void simulateOneTimestep()
     {
        Vec stateVar(8);
        Vec stateVark(8);
        Vec deltaStateVar(8);

        //Read variables from nodes
        //Port Pp1
        Tp1 = (*mpND_Tp1);
        cp1 = (*mpND_cp1);
        Zcp1 = (*mpND_Zcp1);
        //Port Pp2
        Tp2 = (*mpND_Tp2);
        cp2 = (*mpND_cp2);
        Zcp2 = (*mpND_Zcp2);
        //Port Pmr1
        cmr1 = (*mpND_cmr1);
        Zcmr1 = (*mpND_Zcmr1);

        //Read inputVariables from nodes

        //LocalExpressions
        cp = cv + R;

        //Initializing variable vector for Newton-Raphson
        stateVark[0] = thetamr1;
        stateVark[1] = wmr1;
        stateVark[2] = qmp2;
        stateVark[3] = dEp1;
        stateVark[4] = dEp2;
        stateVark[5] = pp1;
        stateVark[6] = pp2;
        stateVark[7] = tormr1;

        //Iterative solution using Newton-Rapshson
        for(iter=1;iter<=mNoiter;iter++)
        {
         //Machine
         //Differential-algebraic system of equation parts

          //Assemble differential-algebraic equations
          systemEquations[0] =thetamr1 + \
(Power(mTimestep,2)*(-0.159155*Dm*pp1 + 0.159155*Dm*pp2 + tormr1))/(4.*Jm + \
2.*Bm*mTimestep) + delayedPart[1][1] + delayedPart[1][2];
          systemEquations[1] =(mTimestep*(-0.159155*Dm*pp1 + 0.159155*Dm*pp2 \
+ tormr1))/(2.*Jm + Bm*mTimestep) + wmr1 + delayedPart[2][1];
          systemEquations[2] =qmp2 + (pp2*(Cim*(pp1*(-1. - 1.*Tp1) + pp2*(1. \
+ Tp1)) + Dm*(-0.15915494309189535 - \
0.15915494309189535*Tp1)*wmr1)*onNegative(qmp2) + pp1*Tp2*(Cim*(-1.*pp1 + \
pp2) - 0.15915494309189535*Dm*wmr1)*onPositive(qmp2))/(R*(1. + Tp1)*Tp2);
          systemEquations[3] =dEp1 + cp*qmp2*Tp1*onNegative(-qmp2) + \
(cp*qmp2*Tp2 + tormr1*wmr1)*onPositive(-qmp2);
          systemEquations[4] =dEp2 - cp*qmp2*Tp2*onNegative(qmp2) + \
(-(cp*qmp2*Tp1) + tormr1*wmr1)*onPositive(qmp2);
          systemEquations[5] =pp1 - lowLimit(cp1 + dEp1*Zcp1,0);
          systemEquations[6] =pp2 - lowLimit(cp2 + dEp2*Zcp2,0);
          systemEquations[7] =-cmr1 + tormr1 - wmr1*Zcmr1;

          //Jacobian matrix
          jacobianMatrix[0][0] = 1;
          jacobianMatrix[0][1] = 0;
          jacobianMatrix[0][2] = 0;
          jacobianMatrix[0][3] = 0;
          jacobianMatrix[0][4] = 0;
          jacobianMatrix[0][5] = (-0.159155*Dm*Power(mTimestep,2))/(4.*Jm + \
2.*Bm*mTimestep);
          jacobianMatrix[0][6] = (0.159155*Dm*Power(mTimestep,2))/(4.*Jm + \
2.*Bm*mTimestep);
          jacobianMatrix[0][7] = Power(mTimestep,2)/(4.*Jm + \
2.*Bm*mTimestep);
          jacobianMatrix[1][0] = 0;
          jacobianMatrix[1][1] = 1;
          jacobianMatrix[1][2] = 0;
          jacobianMatrix[1][3] = 0;
          jacobianMatrix[1][4] = 0;
          jacobianMatrix[1][5] = (-0.159155*Dm*mTimestep)/(2.*Jm + \
Bm*mTimestep);
          jacobianMatrix[1][6] = (0.159155*Dm*mTimestep)/(2.*Jm + \
Bm*mTimestep);
          jacobianMatrix[1][7] = mTimestep/(2.*Jm + Bm*mTimestep);
          jacobianMatrix[2][0] = 0.;
          jacobianMatrix[2][1] = (pp2*(0. + Dm*(-0.15915494309189535 - \
0.15915494309189535*Tp1))*onNegative(qmp2) + (0. - \
0.15915494309189535*Dm)*pp1*Tp2*onPositive(qmp2))/(R*(1. + Tp1)*Tp2);
          jacobianMatrix[2][2] = 1.;
          jacobianMatrix[2][3] = 0.;
          jacobianMatrix[2][4] = 0.;
          jacobianMatrix[2][5] = (pp2*(0. + Cim*(-1. - \
1.*Tp1))*onNegative(qmp2) - 1.*Cim*pp1*Tp2*onPositive(qmp2) + \
Tp2*(Cim*(-1.*pp1 + pp2) - \
0.15915494309189535*Dm*wmr1)*onPositive(qmp2))/(R*(1. + Tp1)*Tp2);
          jacobianMatrix[2][6] = (pp2*(0. + Cim*(1. + Tp1))*onNegative(qmp2) \
+ (Cim*(pp1*(-1. - 1.*Tp1) + pp2*(1. + Tp1)) + Dm*(-0.15915494309189535 - \
0.15915494309189535*Tp1)*wmr1)*onNegative(qmp2) + \
1.*Cim*pp1*Tp2*onPositive(qmp2))/(R*(1. + Tp1)*Tp2);
          jacobianMatrix[2][7] = 0.;
          jacobianMatrix[3][0] = 0;
          jacobianMatrix[3][1] = tormr1*onPositive(-qmp2);
          jacobianMatrix[3][2] = cp*Tp1*onNegative(-qmp2) + \
cp*Tp2*onPositive(-qmp2);
          jacobianMatrix[3][3] = 1;
          jacobianMatrix[3][4] = 0;
          jacobianMatrix[3][5] = 0;
          jacobianMatrix[3][6] = 0;
          jacobianMatrix[3][7] = wmr1*onPositive(-qmp2);
          jacobianMatrix[4][0] = 0;
          jacobianMatrix[4][1] = tormr1*onPositive(qmp2);
          jacobianMatrix[4][2] = -(cp*Tp2*onNegative(qmp2)) - \
cp*Tp1*onPositive(qmp2);
          jacobianMatrix[4][3] = 0;
          jacobianMatrix[4][4] = 1;
          jacobianMatrix[4][5] = 0;
          jacobianMatrix[4][6] = 0;
          jacobianMatrix[4][7] = wmr1*onPositive(qmp2);
          jacobianMatrix[5][0] = 0;
          jacobianMatrix[5][1] = 0;
          jacobianMatrix[5][2] = 0;
          jacobianMatrix[5][3] = -(Zcp1*dxLowLimit(cp1 + dEp1*Zcp1,0));
          jacobianMatrix[5][4] = 0;
          jacobianMatrix[5][5] = 1;
          jacobianMatrix[5][6] = 0;
          jacobianMatrix[5][7] = 0;
          jacobianMatrix[6][0] = 0;
          jacobianMatrix[6][1] = 0;
          jacobianMatrix[6][2] = 0;
          jacobianMatrix[6][3] = 0;
          jacobianMatrix[6][4] = -(Zcp2*dxLowLimit(cp2 + dEp2*Zcp2,0));
          jacobianMatrix[6][5] = 0;
          jacobianMatrix[6][6] = 1;
          jacobianMatrix[6][7] = 0;
          jacobianMatrix[7][0] = 0;
          jacobianMatrix[7][1] = -Zcmr1;
          jacobianMatrix[7][2] = 0;
          jacobianMatrix[7][3] = 0;
          jacobianMatrix[7][4] = 0;
          jacobianMatrix[7][5] = 0;
          jacobianMatrix[7][6] = 0;
          jacobianMatrix[7][7] = 1;
//==This code has been autogenerated using Compgen==

          //Solving equation using LU-faktorisation
          mpSolver->solve(jacobianMatrix, systemEquations, stateVark, iter);
          thetamr1=stateVark[0];
          wmr1=stateVark[1];
          qmp2=stateVark[2];
          dEp1=stateVark[3];
          dEp2=stateVark[4];
          pp1=stateVark[5];
          pp2=stateVark[6];
          tormr1=stateVark[7];
          //Expressions
          qmp1 = -qmp2;
        }

        //Calculate the delayed parts
        delayParts1[1] = (-0.31831*Dm*Power(mTimestep,2)*pp1 + \
0.31831*Dm*Power(mTimestep,2)*pp2 - 8.*Jm*thetamr1 + \
2.*Power(mTimestep,2)*tormr1)/(4.*Jm + 2.*Bm*mTimestep);
        delayParts1[2] = (-0.159155*Dm*Power(mTimestep,2)*pp1 + \
0.159155*Dm*Power(mTimestep,2)*pp2 + 4.*Jm*thetamr1 - \
2.*Bm*mTimestep*thetamr1 + Power(mTimestep,2)*tormr1)/(4.*Jm + \
2.*Bm*mTimestep);
        delayParts2[1] = (-0.159155*Dm*mTimestep*pp1 + \
0.159155*Dm*mTimestep*pp2 + mTimestep*tormr1 - 2.*Jm*wmr1 + \
Bm*mTimestep*wmr1)/(2.*Jm + Bm*mTimestep);

        delayedPart[1][1] = delayParts1[1];
        delayedPart[1][2] = mDelayedPart12.getIdx(0);
        delayedPart[2][1] = delayParts2[1];
        delayedPart[3][1] = delayParts3[1];
        delayedPart[4][1] = delayParts4[1];
        delayedPart[5][1] = delayParts5[1];
        delayedPart[6][1] = delayParts6[1];
        delayedPart[7][1] = delayParts7[1];
        delayedPart[8][1] = delayParts8[1];

        //Write new values to nodes
        //Port Pp1
        (*mpND_pp1)=pp1;
        (*mpND_qmp1)=qmp1;
        (*mpND_dEp1)=dEp1;
        //Port Pp2
        (*mpND_pp2)=pp2;
        (*mpND_qmp2)=qmp2;
        (*mpND_dEp2)=dEp2;
        //Port Pmr1
        (*mpND_tormr1)=tormr1;
        (*mpND_thetamr1)=thetamr1;
        (*mpND_wmr1)=wmr1;
        (*mpND_eqInertiamr1)=eqInertiamr1;
        //outputVariables

        //Update the delayed variabels
        mDelayedPart11.update(delayParts1[1]);
        mDelayedPart12.update(delayParts1[2]);
        mDelayedPart21.update(delayParts2[1]);

     }
    void deconfigure()
    {
        delete mpSolver;
    }
};
#endif // PNEUMATICMACHINE_HPP_INCLUDED
