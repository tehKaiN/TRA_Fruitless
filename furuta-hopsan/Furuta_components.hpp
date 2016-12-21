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

#ifndef FURUTA_COMPONENTS_HPP_INCLUDED
#define FURUTA_COMPONENTS_HPP_INCLUDED

#include "ComponentEssentials.h"
#include "ComponentUtilities.h"

namespace hopsan {

    class Furuta_components : public ComponentQ
    {
    private:
        //Input variable node data pointers
        double *mpU, *mpM, *mpIr, *mpIl, *mpR, *mpL, *mpMr, *mpMl, *mpK;
        //Output variable node data pointers
        double *mpQ1, *mpDotq1, *mpQ2, *mpDotq2, *mpX, *mpDotx;
				
				// State-space model
				Matrix A;
				Vec B;
				const double g = 9.81;
				
				void initializeMatrixA(
					Matrix &A, double M, double Ir, double Il, double r,
					double l, double mr, double ml, double k
				)
				{
					A.create(6,6);
					// Helper vars
					double detD, a1, a2, a3, a4, a5, a6;
					detD = M*(Il*l + ml*l*l)*(Ir*r + M*r*r + ml*l*l);
					a1 = (1/detD)*((-M*(M*r*l)+ml*l*r)*(-M*g*l-ml*g*l*0.5));
					a2 = (1/detD)*(M*(Ir*r + M*r*r + ml*l*l)*(-M*g*l-ml*g*l*0.5));
					a3 = (1/detD)*(M*l*l*(Il*l+l*l+ml*l*l)*M*g);
					a4 = (1/detD)*(M*(Il*l+l*l+ml*l*l)*2*M + 2*M*l*(-M*l*l));
					a5 = (1/detD)*(-M*M*r*(-M*l*l)-M*(M*r*l+ml*l*r)*(-2*M*l));
					a6 = (1/detD)*((Ir*r+M*r*r+ml*l*l)*2*l*M*g+(-2*M*M*r*r*l-2*M*r*r*ml*l)*M*g+(-M*M*r*r*l*l-2*M*r*l*ml*l*r-ml*ml*l*l*r*r)*k+(Ir*r+M*r*r+ml*l*l)*(Il*l+l*l+ml*l*l)*k);
					// 1st row
					A[0][0] = 0;
					A[0][1] = 1;
					A[0][2] = 0;
					A[0][3] = 0;
					A[0][4] = 0;
					A[0][5] = 0;
					// 2nd row
					A[1][0] = 0;
					A[1][1] = 0;
					A[1][2] = a1;
					A[1][3] = 0;
					A[1][4] = a4;
					A[1][5] = 0;
					// 3rd row
					A[2][0] = 0;
					A[2][1] = 0;
					A[2][2] = 0;
					A[2][3] = 1;
					A[2][4] = 0;
					A[2][5] = 0;
					// 4th row
					A[3][0] = 0;
					A[3][1] = 0;
					A[3][2] = a2;
					A[3][3] = 0;
					A[3][4] = a5;
					A[3][5] = 0;
					// 5th row
					A[4][0] = 0;
					A[4][1] = 0;
					A[4][2] = 0;
					A[4][3] = 0;
					A[4][4] = 0;
					A[4][5] = 1;
					// 6th row
					A[5][0] = 0;
					A[5][1] = 0;
					A[5][2] = a3;
					A[5][3] = 0;
					A[5][4] = a6;
					A[5][5] = 0;
				}

				void initializeVectorB(
					Vec &B, double M, double Ir, double Il, double r,
					double l, double mr, double ml, double k
				)
				{
					double detD = M*(Il*l + ml*l*l)*(Ir*r + M*r*r + ml*l*l);
					// Helper vars
					double b1 = (1/detD)*(M*(Il*l+l*l+ml*l*l));
					double b2 = (1/detD)*(-M*(M*r*l+ml*l*r));
					
					B.create(6);
					B[0] = 0;
					B[1] = b1;
					B[2] = 0;
					B[3] = b2;
					B[4] = 0;
					B[5] = 0;
				}
    public:
        static Component *Creator()
        {
            return new Furuta_components();
        }

        void configure()
        {
            //Register constant parameters
            //Register input variables
            addInputVariable("u", "Input", "", 0, &mpU);
            addInputVariable("M", "Mass on spring", "kg", 0, &mpM);
            addInputVariable("Ir", "1st arm inertia", "kg*m^2", 0, &mpIr);
            addInputVariable("Il", "2nd arm inertia", "kg*m^2", 0, &mpIl);
            addInputVariable("r", "1st arm length", "m", 0, &mpR);
            addInputVariable("l", "2nd arm length", "m", 0, &mpL);
            addInputVariable("mr", "1st arm mass", "kg", 0, &mpMr);
            addInputVariable("ml", "2nd arm mass", "kg", 0, &mpMl);
            addInputVariable("k", "spring's coefficient", "N/m", 0, &mpK);
            //Register output variables
            addOutputVariable("q1", "1st arm angle", "rad", 0, &mpQ1);
            addOutputVariable("dotq1", "1st arm angle speed", "rad/s", 0, &mpDotq1);
            addOutputVariable("q2", "2nd arm angle", "rad", 0, &mpQ2);
            addOutputVariable("dotq2", "2nd arm angle speed", "rad/s", 0, &mpDotq2);
            addOutputVariable("x", "spring's wydluzenie", "m", 0, &mpX);
            addOutputVariable("dotx", "spring's wydluzenie speed", "m/s", 0, &mpDotx);
            //Add power ports
            //Set default power port start values
        }


        void initialize()
        {

            //Declare local variables
            double u, M, Ir, Il, r, l, mr, ml, k;
            double q1, dotq1, q2, dotq2, x, dotx;

            //Read variable values from nodes
            u = (*mpU);
            M = (*mpM);
            Ir = (*mpIr);
            Il = (*mpIl);
            r = (*mpR);
            l = (*mpL);
            mr = (*mpMr);
            ml = (*mpMl);
            k = (*mpK);
						
						initializeMatrixA(A, M, Ir, Il, r, l, mr, ml, k);
						initializeVectorB(B, M, Ir, Il, r, l, mr, ml, k);
						
            //WRITE YOUR INITIALIZATION CODE HERE

            //Write new values to nodes
            (*mpQ1) = q1;
            (*mpDotq1) = dotq1;
            (*mpQ2) = q2;
            (*mpDotq2) = dotq2;
            (*mpX) = x;
            (*mpDotx) = dotx;
        }


        void simulateOneTimestep()
        {
            //Declare local variables
            double u, M, Ir, Il, r, l, mr, ml, k;
            double q1, dotq1, q2, dotq2, x, dotx;

            //Read variable values from nodes
            u = (*mpU);
            M = (*mpM);
            Ir = (*mpIr);
            Il = (*mpIl);
            r = (*mpR);
            l = (*mpL);
            mr = (*mpMr);
            ml = (*mpMl);
            k = (*mpK);

            //WRITE YOUR EQUATIONS HERE

            //Write new values to nodes
            (*mpQ1) = q1;
            (*mpDotq1) = dotq1;
            (*mpQ2) = q2;
            (*mpDotq2) = dotq2;
            (*mpX) = x;
            (*mpDotx) = dotx;
        }


        void finalize()
        {
            //WRITE YOUR FINALIZE CODE HERE (OPTIONAL)
        }


        void deconfigure()
        {
            //WRITE YOUR DECONFIGURATION CODE HERE (OPTIONAL)
        }
    };
}

#endif //FURUTA_COMPONENTS_HPP_INCLUDED


