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

#ifndef OPTIMIZATIONTESTFUNCTION6D_HPP_INCLUDED
#define OPTIMIZATIONTESTFUNCTION6D_HPP_INCLUDED

#include "ComponentEssentials.h"
#include "ComponentUtilities/AuxiliarySimulationFunctions.h"
#include <math.h>

namespace hopsan {

class OptimizationTestFunction6D : public ComponentSignal
{
private:
    double *mpX1, *mpX2, *mpX3, *mpX4, *mpX5, *mpX6, *mpOut;
    int mFunc;

public:
    static Component *Creator()
    {
        return new OptimizationTestFunction6D();
    }

    void configure()
    {
        // Add ports to the component
        addInputVariable("x1", "", "", 0.0, &mpX1);
        addInputVariable("x2", "", "", 0.0, &mpX2);
        addInputVariable("x3", "", "", 0.0, &mpX3);
        addInputVariable("x4", "", "", 0.0, &mpX4);
        addInputVariable("x5", "", "", 0.0, &mpX5);
        addInputVariable("x6", "", "", 0.0, &mpX6);

        addOutputVariable("out", "", "", 0.0, &mpOut);

        std::vector<HString> functions;
        functions.push_back("Rosenbrock Function");
        functions.push_back("Sphere Function");
        functions.push_back("Styblinski-Tang Function");

        addConditionalConstant("function", "Test Function", functions, mFunc);
    }

    void initialize()
    {
        simulateOneTimestep();
    }

    void simulateOneTimestep()
    {
        double x1 = (*mpX1);
        double x2 = (*mpX2);
        double x3 = (*mpX3);
        double x4 = (*mpX4);
        double x5 = (*mpX5);
        double x6 = (*mpX6);

        switch(mFunc)
        {
        case 0:
            (*mpOut) = rosenbrock(x1,x2,x3,x4,x5,x6);
            break;
        case 1:
            (*mpOut) = sphere(x1,x2,x3,x4,x5,x6);
            break;
        case 2:
            (*mpOut) = styblinskiTang(x1,x2,x3,x4,x5,x6);
            break;
        default:
            (*mpOut) = 0;
            break;
        }
    }



    inline double rosenbrock(const double x1, const double x2, const double x3, const double x4, const double x5, const double x6) const
    {
        return (1.0-x1)*(1.0-x1) + 100.0*(x2-x1*x1)*(x2-x1*x1) +
               (1.0-x2)*(1.0-x2) + 100.0*(x3-x2*x2)*(x3-x2*x2) +
               (1.0-x3)*(1.0-x3) + 100.0*(x4-x3*x3)*(x4-x3*x3) +
               (1.0-x4)*(1.0-x4) + 100.0*(x5-x4*x4)*(x5-x4*x4) +
               (1.0-x5)*(1.0-x5) + 100.0*(x6-x5*x5)*(x6-x5*x5);
    }

    inline double sphere(const double x1, const double x2, const double x3, const double x4, const double x5, const double x6) const
    {
        return x1*x1+x2*x2+x3*x3+x4*x4+x5*x5+x6*x6;
    }

    inline double styblinskiTang(const double x1, const double x2, const double x3, const double x4, const double x5, const double x6) const
    {
        return (x1*x1*x1*x1-16.0*x1*x1+5.0*x1 + x2*x2*x2*x2-16.0*x2*x2+5.0*x2 +
                x3*x3*x3*x3-16.0*x3*x3+5.0*x3 + x4*x4*x4*x4-16.0*x4*x4+5.0*x4 +
                x5*x5*x5*x5-16.0*x5*x5+5.0*x5 + x6*x6*x6*x6-16.0*x6*x6+5.0*x6)/2.0;
    }



};

}
#endif //OPTIMIZATIONTESTFUNCTION6D_HPP_INCLUDED
