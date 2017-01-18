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
//! @file   HydraulicPressureReducingValve.hpp
//! @author Robert Braun <robert.braun@liu.se>
//! @date   2011-01-26
//!
//! @brief Contains a hydraulic pressure relief valve with first order dynamics

//$Id: HydraulicPressureReducingValve.hpp 8139 2015-06-17 12:30:13Z petno25 $

#ifndef HYDRAULICPRESSUREREDUCINGVALVE_HPP_INCLUDED
#define HYDRAULICPRESSUREREDUCINGVALVE_HPP_INCLUDED

#include "ComponentEssentials.h"
#include "ComponentUtilities.h"

namespace hopsan {

    //!
    //! @brief A Hydraulic Pressure Releife Valve
    //! @ingroup HydraulicComponents
    //!
    class HydraulicPressureReducingValve : public ComponentQ
    {
    private:
        // Member variables
        TurbulentFlowFunction mTurb;
        ValveHysteresis mHyst;
        FirstOrderTransferFunction mFilterLP;
        double mPrevX0;

        // Port and node data pointers
        Port *mpP1, *mpP2;
        double *mpP1_p, *mpP1_q, *mpP1_c, *mpP1_Zc, *mpP2_p, *mpP2_q, *mpP2_c, *mpP2_Zc;
        double *mpPref, *mpPh, *mpXv;

        // Constants
        double mTao, mKcs, mKcf, Cs, Cf, mQnom, mPnom;

    public:
        static Component *Creator()
        {
            return new HydraulicPressureReducingValve();
        }

        void configure()
        {
            mpP1 = addPowerPort("P1", "NodeHydraulic");
            mpP2 = addPowerPort("P2", "NodeHydraulic");

            addInputVariable("p_ref", "Reference Opening Pressure", "Pa", 2000000.0, &mpPref);
            addInputVariable("p_h", "Hysteresis Width", "Pa", 500000.0, &mpPh);

            addOutputVariable("xv", "Equivalent spool position", "", &mpXv);

            addConstant("tao", "Time Constant of Spool", "s", 0.01, mTao);
            addConstant("k_cs", "Steady State Characteristic due to Spring", "(m^3/s)/Pa", 0.00000001, mKcs);
            addConstant("k_cf", "Steady State Characteristic due to Flow Forces", "(m^3/s)/Pa", 0.00000001, mKcf);
            addConstant("q_nom", "Flow with Fully Open Valve and pressure drop Pnom", "m^3/s", 0.001, mQnom);
        }


        void initialize()
        {
            mpP1_p = getSafeNodeDataPtr(mpP1, NodeHydraulic::Pressure);
            mpP1_q = getSafeNodeDataPtr(mpP1, NodeHydraulic::Flow);
            mpP1_c = getSafeNodeDataPtr(mpP1, NodeHydraulic::WaveVariable);
            mpP1_Zc = getSafeNodeDataPtr(mpP1, NodeHydraulic::CharImpedance);

            mpP2_p = getSafeNodeDataPtr(mpP2, NodeHydraulic::Pressure);
            mpP2_q = getSafeNodeDataPtr(mpP2, NodeHydraulic::Flow);
            mpP2_c = getSafeNodeDataPtr(mpP2, NodeHydraulic::WaveVariable);
            mpP2_Zc = getSafeNodeDataPtr(mpP2, NodeHydraulic::CharImpedance);

            mPnom = 7e6f;
            double x0max = mQnom / sqrt(mPnom);
            Cs = sqrt(mPnom) / mKcs;
            Cf = 1.0 / (mKcf*sqrt(mPnom));

            //x0 = 0.00001;
            mPrevX0 = 0.0;

            double wCutoff = 1.0 / mTao;
            double num[2] = {1.0, 0.0};
            double den[2] = {1.0, 1.0/wCutoff};
            mFilterLP.initialize(mTimestep, num, den, mPrevX0, mPrevX0, 0.0, x0max);
        }


        void simulateOneTimestep()
        {
            double p1, q1, c1, Zc1, p2, q2, c2, Zc2;
            double b1, gamma, b2, xs, xh, xsh, wCutoff, pref, ph;
            bool cav = false;

            //Get variable values from nodes
            p1 = (*mpP1_p);
            q1 = (*mpP1_q);
            c1 = (*mpP1_c);
            Zc1 = (*mpP1_Zc);
            p2 = (*mpP2_p);
            q2 = (*mpP2_q);
            c2 = (*mpP2_c);
            Zc2 = (*mpP2_Zc);
            pref = (*mpPref);
            ph = (*mpPh);

            //PRV Equations

            //Help variable b1
            b1 = Cs + (p1-p2)*Cf;
            if ( (p1-p2)*Cf < 0.0 )
            {
                b1 = Cs;
            }

            //Help variable gamma
            if (p1>p2)
            {
                if ( (sqrt(p1-p2)*2.0 + (Zc1+Zc2)*mPrevX0) != 0.0 )
                {
                    gamma = sqrt(p1-p2)*2.0 / (sqrt(p1-p2)*2.0 + (Zc1+Zc2)*mPrevX0);
                }
                else
                {
                    gamma = 1.0;
                }
            }
            else
            {
                if ( (sqrt(p2-p1)*2.0 + (Zc1+Zc2)*mPrevX0) != 0.0 )
                {
                    gamma = sqrt(p2-p1)*2.0 / (sqrt(p2-p1)*2.0 + (Zc1+Zc2)*mPrevX0);
                }
                else
                {
                    gamma = 1.0;
                }
            }

            //Help variable b2
            if (p1 > p2)
            {
                b2 = gamma*(Zc1+Zc2)*sqrt(p1-p2);
            }
            else
            {
                b2 = gamma*(Zc1+Zc2)*sqrt(p2-p1);
            }
            if (b2 < 0.0)
            {
                b2 = 0.0;
            }

            // Calculation of spool position
            xs = (gamma*(-c2) + b2*mPrevX0/2.0 + pref) / (b1+b2);

            //Hysteresis
            xh = ph / (b1+b2);                                  //Hysteresis width [m]
            xsh = mHyst.getValue(xs, xh, mPrevX0);

            //Filter
            wCutoff = (1.0 + b2/b1) * 1.0/mTao;                //Cutoff frequency
            double num[2] = {1.0, 0.0};
            double den[2] = {1.0, 1.0/wCutoff};
            mFilterLP.setNumDen(num,den);
            double x0 = mFilterLP.update(xsh);

            //Turbulent flow equation
            mTurb.setFlowCoefficient(x0);
            q2 = mTurb.getFlow(c1,c2,Zc1,Zc2);
            q1 = -q2;
            p2 = c2+Zc2*q2;
            p1 = c1+Zc1*q1;

            // Cavitation
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
                xs = ((-c2) + b2*x0/2.0 + pref) / (b1+b2);
                xsh = mHyst.getValue(xs, xh, mPrevX0);
                x0 = mFilterLP.value();        //! @todo Make the filter actually redo last step if possible; this will create an undesired delay of one iteration

                mTurb.setFlowCoefficient(x0);
                q2 = mTurb.getFlow(c1,c2,Zc1,Zc2);
                q1 = -q2;
                p2 = c2+Zc2*q2;
                p1 = c1+Zc1*q1;

                if (p1 < 0.0) { p1 = 0.0; }
                if (p2 < 0.0) { p2 = 0.0; }
            }

            mPrevX0 = x0;

            //Write new variables to nodes
            (*mpP1_p) = p1;
            (*mpP1_q) = q1;
            (*mpP2_p) = p2;
            (*mpP2_q) = q2;

            (*mpXv) = x0;
        }
    };
}

#endif // HYDRAULICPRESSUREREDUCINGVALVE_HPP_INCLUDED
