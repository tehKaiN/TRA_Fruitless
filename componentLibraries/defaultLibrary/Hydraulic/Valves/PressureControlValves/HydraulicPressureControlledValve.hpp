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
//! @file   HydraulicPressureControlledValve.hpp
//! @author Robert Braun <robert.braun@liu.se>
//! @date   2010-01-13
//!
//! @brief Contains a hydraulic pressure controlled valve with first order dynamics
//!
//$Id: HydraulicPressureControlledValve.hpp 8139 2015-06-17 12:30:13Z petno25 $

#ifndef HYDRAULICPRESSURECONTROLLEDVALVE_HPP_INCLUDED
#define HYDRAULICPRESSURECONTROLLEDVALVE_HPP_INCLUDED

#include "ComponentEssentials.h"
#include "ComponentUtilities.h"

namespace hopsan {

    //!
    //! @brief
    //! @ingroup HydraulicComponents
    //!
    class HydraulicPressureControlledValve : public ComponentQ
    {
    private:
        // Member variables
        double mCs, mCf, mPrevX0;
        TurbulentFlowFunction mTurb;
        ValveHysteresis mHyst;
        FirstOrderTransferFunction mFilterLP;

        // Port and node data pointers
        Port *mpP1, *mpP2, *mpPOpen, *mpPClose;
        double *mpP1_p, *mpP1_q, *mpP1_c, *mpP1_Zc, *mpP2_p, *mpP2_q, *mpP2_c, *mpP2_Zc,
               *mpPOpen_p, *mpPOpen_c, *mpPClose_p, *mpPClose_c;
        double *mpPref, *mpPh;
        double *mpX0;

        // Constants
        double  mTao, mKcs, mKcf, mPnom, mQnom;

    public:
        static Component *Creator()
        {
            return new HydraulicPressureControlledValve();
        }

        void configure()
        {
            mpP1 = addPowerPort("P1", "NodeHydraulic");
            mpP2 = addPowerPort("P2", "NodeHydraulic");
            mpPOpen = addPowerPort("P_OPEN", "NodeHydraulic");
            mpPClose = addPowerPort("P_CLOSE", "NodeHydraulic");

            addInputVariable("p_ref", "Reference Opening Pressure", "Pa", 2000000.0, &mpPref);
            addInputVariable("p_h", "Hysteresis Width", "Pa", 500000, &mpPh);

            addOutputVariable("x0", "", "", 0, &mpX0);

            addConstant("tao", "Time Constant of Spool", "s", 0.01, mTao);
            addConstant("k_cs", "Steady State Characteristic due to Spring", "(m^3/s)/Pa", 0.00000001, mKcs);
            addConstant("k_cf", "Steady State Characteristic due to Flow Forces", "(m^3/s)/Pa", 0.00000001, mKcf);
            addConstant("q_nom", "Flow with Fully Open Valve and pressure drop p_nom", "m^3/s", 0.001, mQnom);
            addConstant("p_nom", "Nominal pressure drop", "Pa", 7e6, mPnom);
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

            mpPOpen_p = getSafeNodeDataPtr(mpPOpen, NodeHydraulic::Pressure);
            mpPOpen_c = getSafeNodeDataPtr(mpPOpen, NodeHydraulic::WaveVariable);

            mpPClose_p = getSafeNodeDataPtr(mpPClose, NodeHydraulic::Pressure);
            mpPClose_c = getSafeNodeDataPtr(mpPClose, NodeHydraulic::WaveVariable);

            mPrevX0 = 0.0;
            mCs = sqrt(mPnom)/mKcs;
            mCf = 1/(mKcf * sqrt(mPnom));
            double x0max = mQnom/sqrt(mPnom);

            double wCutoff = 1 / mTao;
            double num[2] = {1.0, 0.0};
            double den[2] = {1.0, 1.0/wCutoff};
            mFilterLP.initialize(mTimestep, num, den, mPrevX0, mPrevX0, 0, x0max);
        }


        void simulateOneTimestep()
        {
            //Declare local variables
            double p1, q1, c1, Zc1, p2, q2, c2, Zc2, p_open, c_open, p_close, c_close;
            double b1, xs, xh, xsh, pref, ph;
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
            p_open = (*mpPOpen_p);
            c_open = (*mpPOpen_c);
            p_close = (*mpPClose_p);
            c_close = (*mpPClose_c);
            pref = (*mpPref);
            ph = (*mpPh);

            /* Equations */

            b1 = mCs+mCf*(p1-p2);                               // Help Variable, equals sqrt(p1-p2)/Kctot
            xs = (p_open - pref - p_close) / b1;                // Spool position calculation
            xh = ph/b1;
            xsh = mHyst.getValue(xs, xh, mPrevX0);              // Hysteresis
            double x0 = mFilterLP.update(xsh);                  // Dynamics
            mTurb.setFlowCoefficient(x0);                       // Turbulent Flow Calculation
            q2 = mTurb.getFlow(c1, c2, Zc1, Zc2);
            q1 = -q2;

            p1 = c1 + Zc1 * q1;                                 // Pressure Calulation
            p2 = c2 + Zc2 * q2;
            p_open = c_open;
            p_close = c_close;

            p_open = std::max(0.0, p_open);
            p_close = std::max(0.0, p_close);

            if (p1 < 0.0)                                       // Check for cavitation
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
            if (cav)                                            //Cavitatiaon, redo calculations with new c and Zc
            {
                q2 = mTurb.getFlow(c1, c2, Zc1, Zc2);
                q1 = -q2;
                p1 = c1 + Zc1 * q1;
                p2 = c2 + Zc2 * q2;
                if (p1 < 0.0) { p1 = 0.0; }
                if (p2 < 0.0) { p2 = 0.0; }
            }

            mPrevX0 = x0;

            (*mpP1_p) = p1;                                    //Write new values to nodes
            (*mpP1_q) = q1;
            (*mpP2_p) = p2;
            (*mpP2_q) = q2;
            (*mpPOpen_p) = p_open;
            (*mpPClose_p) = p_close;

            (*mpX0) = x0;
        }
    };
}

#endif // HYDRAULICPRESSURECONTROLLEDVALVE_HPP_INCLUDED

