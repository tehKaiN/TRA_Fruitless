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
//! @file   HydraulicPressureControlledPump.hpp
//! @author Robert Braun <robert.braun@liu.se>
//! @date   2010-12-20
//!
//! @brief Contains a pressure controlled pump with second order dynamics
//! Written by Petter Krus 9005617
//! Translated to HOPSAN NG by Robert Braun 101220
//!
//$Id: HydraulicPressureControlledPump.hpp 8139 2015-06-17 12:30:13Z petno25 $

#ifndef HYDRAULICPRESSURECONTROLLEDPUMP_HPP_INCLUDED
#define HYDRAULICPRESSURECONTROLLEDPUMP_HPP_INCLUDED

#include <iostream>
#include "ComponentEssentials.h"
#include "ComponentUtilities.h"
#include <math.h>

namespace hopsan {

    //!
    //! @brief
    //! @ingroup HydraulicComponents
    //!
    class HydraulicPressureControlledPump : public ComponentQ
    {
    private:
        double pnom, movementnom, qmin;
        double *mpPdif, *mpMovement, *mpQmax, *mpLp, *mpRp, *mpWp1, *mpClp, *mpTaov, *mpTp, *mpTm;
        double a1, a2, b1, b2, b3, y1, y2, u1, u2, ud, vd, yd;
        double gamma, qminl, qmaxl;
        double *mpND_p1, *mpND_q1, *mpND_c1, *mpND_Zc1, *mpND_p2, *mpND_q2, *mpND_c2, *mpND_Zc2, *mpND_p3, *mpND_q3, *mpND_c3, *mpEps, *mpA;
        Port *mpP1, *mpP2, *mpPREF, *mpOut, *mpOut2;

    public:
        static Component *Creator()
        {
            return new HydraulicPressureControlledPump();
        }

        void configure()
        {
            pnom = 7e6;
            movementnom = 125;

            mpP1 = addPowerPort("P1", "NodeHydraulic");
            mpP2 = addPowerPort("P2", "NodeHydraulic");
            mpPREF = addPowerPort("PREF", "NodeHydraulic", "", Port::NotRequired);

            addOutputVariable("eps", "NodeSignal", "", 1.0, &mpEps);
            addOutputVariable("a", "NodeSignal", "rad", 0, &mpA);
            addInputVariable("p_dif", "Reference pressure difference", "Pa", 1000000, &mpPdif);
            addInputVariable("omega_p", "Pump movement", "rad/s", 125, &mpMovement);
            addInputVariable("q_max", "Nominal maximal flow (at 125 rad/s)", "m^3/s", 0.00125, &mpQmax);
            addInputVariable("l_p", "Regulator inductance at nominal pressure", "", 70000000, &mpLp);
            addInputVariable("r_p", "Static characteristic at nominal pressure", "", 1000000000, &mpRp);
            addInputVariable("omega_p1", "Lead frequency of regulator", "rad/s", 200, &mpWp1);
            addInputVariable("C_lp", "Leakage coefficient of pump", "", 0.000000000001, &mpClp);
            addInputVariable("tao_v", "Time constant of control valve", "s", 0.001, &mpTaov);
            addInputVariable("t_p", "Time from min to full displacement", "s", 0.15, &mpTp);
            addInputVariable("t_m", "Time from full to min displacement", "s", 0.12, &mpTm);

            addConstant("q_min", "Nominal minimal flow (at 125 rad/s)", "m^3/s", 0, qmin);
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

            mpND_p3 = getSafeNodeDataPtr(mpPREF, NodeHydraulic::Pressure);
            mpND_q3 = getSafeNodeDataPtr(mpPREF, NodeHydraulic::Flow);
            mpND_c3 = getSafeNodeDataPtr(mpPREF, NodeHydraulic::WaveVariable);

            double p1 = (*mpND_p1);
            double c1 = (*mpND_c1);
            double p2 = (*mpND_p2);
            double q2 = (*mpND_q2);
            double c2 = (*mpND_c2);
            double c3 = (*mpND_c3);
            double Zc1 = (*mpND_Zc1);
            double Zc2 = (*mpND_Zc2);

            double pdif = (*mpPdif);
            double movement = (*mpMovement);
            double qmax = (*mpQmax);
            double lp = (*mpLp);
            double rp = (*mpRp);
            double wp1 = (*mpWp1);
            double Clp = (*mpClp);
            double taov = (*mpTaov);

            double y0, lpe/*, vmin, vmax*/;

            q2 = qmax * (*mpEps)*(movement / movementnom);

            gamma = 1 / (Clp * (Zc1 + Zc2) + 1);
            if (p2 < 1.0) { p2 = 1.0; }
            lpe = lp * sqrt(pnom / p2) * (movementnom / (movement+0.001));
            y0 = q2 * (lpe + rp * taov + Zc2 * gamma / wp1);

            (*mpND_p1) = p1;
            (*mpND_p2) = p2;

            qmaxl = qmax * (movement / movementnom);
            qminl = qmin * (movement / movementnom);

            //vmax = qmaxl * sqrt(fabs(p2 - 1e5) / (pnom * tp));
            //vmin = -qmaxl * sqrt(fabs(p2 - 1e5) / (pnom * tm));

            double c2e = (Clp * Zc1 + 1) * gamma * c2 + Clp * Zc2 * gamma * c1;
            double u = pdif - c2e + c3;
            yd = y0;
            ud = u;
            y1 = y0;
            u1 = u;
            vd = 0.0;

            (*mpA) = 0;
            (*mpND_q2) = q2;

        }


        void simulateOneTimestep()
        {
            //Get variable values from nodes
            double p1 = (*mpND_p1);
            double c1 = (*mpND_c1);
            double Zc1 = (*mpND_Zc1);
            double p2 = (*mpND_p2);
            double c2 = (*mpND_c2);
            double Zc2 = (*mpND_Zc2);
            double c3 = (*mpND_c3);

            double pdif = (*mpPdif);
            double movement = (*mpMovement);
            double qmax = (*mpQmax);
            double lp = (*mpLp);
            double rp = (*mpRp);
            double wp1 = (*mpWp1);
            double Clp = (*mpClp);
            double taov = (*mpTaov);
            double tp = (*mpTp);
            double tm = (*mpTm);

            double lpe, c1e, c2e, qp, ql, q1, q2, ymin, ymax, vmin, vmax;

            qmaxl = qmax * (movement / movementnom);
            qminl = qmin * (movement / movementnom);

            if (p2 < 1.0) { p2 = 1.0; }
            lpe = lp * sqrt(pnom / p2) * (movementnom / (movement+0.001));
            if (c3 < 0.0) { c3 = 0.0; }
            gamma = 1 / (Clp * (Zc1 + Zc2) + 1);

            c1e = (Clp * Zc2 + 1) * gamma * c1 + Clp * Zc1 * gamma * c2;
            c2e = (Clp * Zc1 + 1) * gamma * c2 + Clp * Zc2 * gamma * c1;

            double denom = lpe + rp * taov + Zc2 * gamma / wp1;
            double g1 = lpe * taov / denom;
            double g2 = (rp + Zc2 * gamma) / denom;
            double u = pdif - c2e + c3;
            ymax = qmaxl * denom;
            ymin = qminl * denom;
            vmax = qmaxl * denom * sqrt(fabs(p2 - 1e5) / (pnom * tp));
            vmin = -qmaxl * denom * sqrt(fabs(p2 - 1e5) / (pnom * tm));
            qp = fltppu(u, wp1, g1, g2, ymin, ymax, vmin, vmax) / denom;

            //Calculate pressures
            p1 = c1e - Zc1 * gamma * qp;
            p2 = c2e + Zc2 * gamma * qp;

            //Leakage flow
            ql = Clp * (p2 - p1);
            q2 = qp - ql;


            //Cavitation check
            if (p1 < 0.0)
            {
                p1 = 0.0;
                q2 = std::min(q2, 0.0);
            }
            if (p2 <= 0.0)
            {
                p2 = 0.0;
                q2 = std::max(q2, 0.0);
            }
            if (c3 < 0.0) { c3 = 0.0; }

            q1 = -q2;


            //Write new values to nodes
            (*mpND_p1) = p1;
            (*mpND_q1) = q1;
            (*mpND_p2) = p2;
            (*mpND_q2) = q2;
            (*mpND_p3) = c3;
            (*mpND_q3) = 0.0;
            (*mpEps) = q2/qmaxl;
            (*mpA) += movement*mTimestep;
        }


        //High pass filter times an integration, with separate minimum and maximum values for input and output variables. Converted from old Hopsan.

        double fltppu(double u, double w01, double c1, double c2, double ymin, double ymax, double vmin, double vmax)
        {

                /* Local variables */
            double v;
            double ret_val;
            double alf1, alf2, g, ws;

            y2 = y1;
            y1 = yd;
            u2 = u1;
            u1 = ud;

            /*  Calculation of constants */

            ws = 1 / mTimestep;
            alf2 = c1 * 4*ws*ws;
            alf1 = 2 / (w01 * mTimestep);
            g = c2 + alf2 + ws*2;
            a1 = (c2 - alf2)*2/g;
            a2 = (c2 + alf2 - ws*2)/g;
            b1 = (alf1 + 1)/g;
            b2 = 2 / g;
            b3 = (1 - alf1)/g;

            ret_val = -a1 * y1 - a2 * y2 + b1 * u + b2 * u1 + b3 * u2;
            v = (ret_val - y1) / mTimestep;
            if (v > vmax)
            {
                ret_val = y1 + vmax * mTimestep;
                v = vmax;
            }
            else if (v <= vmin)
            {
                ret_val = y1 + vmin * mTimestep;
                v = vmin;
            }
            if (ret_val > ymax)
            {
                ret_val = ymax;
                v = 0.0;
            }
            else if (ret_val < ymin)
            {
                ret_val = ymin;
                v = 0.0;
            }
            ud = u;
            yd = ret_val;
            vd = v;

            return ret_val;
        }
    };
}

#endif // HYDRAULICPRESSURECONTROLLEDPUMP_HPP_INCLUDED
