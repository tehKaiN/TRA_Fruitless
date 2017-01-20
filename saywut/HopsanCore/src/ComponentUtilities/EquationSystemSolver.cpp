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
//! @file   EquationSystemSolver.h
//! @author Robert Braun <robert.braun@liu.se>
//! @date   2012-02-04
//!
//! @brief Contains an equation system solver utility
//!
//$Id: EquationSystemSolver.cpp 8580 2016-01-15 07:16:33Z robbr48 $

#include "ComponentUtilities/EquationSystemSolver.h"
#include "ComponentUtilities/ludcmp.h"
#include "CoreUtilities/HopsanCoreMessageHandler.h"
#include "Component.h"
#include "ComponentUtilities/matrix.h"

#include <cstring>
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <iostream>

using namespace hopsan;

//! @class hopsan::EquationSystemSolver
//! @ingroup ComponentUtilityClasses
//! @brief A numerical solver utility for equation systems using LU-decomposition
//!
//! Solves \f$J*b = x\f$ by first transforming
//! it to \f$L*U*b = x\f$ and subsequently solving
//! \f$U*b = y\f$ and \f$L*y = x\f$
//!

//! @brief Constructor for equation system solver utility
//! @param pParentComponent Pointer to parent component
//! @param n Number of states
EquationSystemSolver::EquationSystemSolver(Component *pParentComponent, int n)
{
    mpParentComponent = pParentComponent;

    // Weights for equations, used when running several iterations
    mSystemEquationWeight[0]=1;
    mSystemEquationWeight[1]=0.67;
    mSystemEquationWeight[2]=0.5;
    mSystemEquationWeight[3]=0.5;

    mnVars = n;
    mpOrder = new int[n];                   //Used to keep track of the order of the equations
    mpDeltaStateVar = new Vec(n);           //Difference between nwe state variables and the previous ones
    mSingular = false;                      //Tells whether or not the Jacobian is singular
}


//! @brief Constructor for equation system solver utility with additional arguments
//! @param pParentComponent Pointer to parent component
//! @param n Number of states
//! @param pJacobian Pointer to Jacobian matrix
//! @param pEquations Pointer to vector with equations (RHS)
//! @param pVariables Pointer to vector with state variables
EquationSystemSolver::EquationSystemSolver(Component *pParentComponent, int n, Matrix *pJacobian, Vec *pEquations, Vec *pVariables)
{
    mpParentComponent = pParentComponent;

    // Weights for equations, used when running several iterations
    mSystemEquationWeight[0]=1;
    mSystemEquationWeight[1]=0.67;
    mSystemEquationWeight[2]=0.5;
    mSystemEquationWeight[3]=0.5;

    mnVars = n;
    mpOrder = new int[n];                   //Used to keep track of the order of the equations
    mpDeltaStateVar = new Vec(n);           //Difference between nwe state variables and the previous ones
    mSingular = false;                      //Tells whether or not the Jacobian is singular

    mpJacobian = pJacobian;
    mpEquations = pEquations;
    mpVariables = pVariables;
}


//! @brief Solves a system of equations
//! @param jacobian Jacobian matrix
//! @param equations Vector of system equations
//! @param variables Vector of state variables
//! @param iteration How many times the solver has been executed before in the same time step
void EquationSystemSolver::solve(Matrix &jacobian, Vec &equations, Vec &variables, int iteration)
{
    //Stop simulation if LU decomposition failed due to singularity
    if(!ludcmp(jacobian, mpOrder) && mpParentComponent)
    {
        mpParentComponent->addErrorMessage("Unable to perform LU-decomposition: Jacobian matrix is probably singular.");
        mpParentComponent->stopSimulation();
    }

    //Solve system using L and U matrices
    solvlu(jacobian,equations,*mpDeltaStateVar,mpOrder);

    //Calculate new system variables
    for(int i=0; i<mnVars; ++i)
    {
        variables[i] = variables[i] - mSystemEquationWeight[iteration - 1] * (*mpDeltaStateVar)[i];
    }
}



//! @brief Solves a system of equations with just one iteration (slightly faster)
//! @param jacobian Jacobian matrix
//! @param equations Vector of system equations
//! @param variables Vector of state variables
void EquationSystemSolver::solve(Matrix &jacobian, Vec &equations, Vec &variables)
{
    //Stop simulation if LU decomposition failed due to singularity
    if(!ludcmp(jacobian, mpOrder) && mpParentComponent)
    {
        mpParentComponent->addErrorMessage("Unable to perform LU-decomposition: Jacobian matrix is probably singular.");
        mpParentComponent->stopSimulation();
    }

    //Solve system using L and U matrices
    solvlu(jacobian,equations,*mpDeltaStateVar,mpOrder);

    //Calculate new system variables
    for(int i=0; i<mnVars; ++i)
    {
        variables[i] = variables[i] - (*mpDeltaStateVar)[i];
    }
}




//! @brief Solves a system of equations. Requires pre-defined pointers to jacobian, equations and state variables.
void EquationSystemSolver::solve()
{
    //Stop simulation if LU decomposition failed due to singularity
    if(!ludcmp(*mpJacobian, mpOrder) && mpParentComponent)
    {
        mpParentComponent->addErrorMessage("Unable to perform LU-decomposition: Jacobian matrix is probably singular.");
        mpParentComponent->stopSimulation();
    }

    //Solve system using L and U matrices
    solvlu(*mpJacobian,*mpEquations,*mpDeltaStateVar,mpOrder);

    //Calculate new system variables
    for(int i=0; i<mnVars; ++i)
    {
        (*mpVariables)[i] = (*mpVariables)[i] - (*mpDeltaStateVar)[i];
    }
}



//! @brief Constructor for solver utility using numerical integration methods
//! @param pParentComponent Pointer to parent component
//! @param pStateVars Pointer to vector with state variables
//! @param tolerance Tolerance (for Newton-Rhapson with implicit methods)
//! @param maxIter Maximum number of iterations with Newton-Rhapson
NumericalIntegrationSolver::NumericalIntegrationSolver(Component *pParentComponent, std::vector<double> *pStateVars, double tolerance, size_t maxIter)
{
    mpParentComponent = pParentComponent;
    mTimeStep = mpParentComponent->getTimestep();
    mpStateVars = pStateVars;
    mnStateVars = pStateVars->size();
    mTolerance = tolerance;
    mMaxIter = maxIter;
}


//! @brief Returns a list of available integration methods
const std::vector<HString> NumericalIntegrationSolver::getAvailableSolverTypes()
{
    std::vector<HString> availableSolvers;
    availableSolvers.push_back(HString("Forward Euler"));
    availableSolvers.push_back(HString("Midpoint Method"));
    availableSolvers.push_back(HString("Runge-Kutta"));
    availableSolvers.push_back(HString("Dormand-Prince"));
    availableSolvers.push_back(HString("Backward Euler"));
    availableSolvers.push_back(HString("Trapezoid Rule"));
    return availableSolvers;
}


//! @brief Solves a system using numerical integration
//! @param solverType Integration method to use
void NumericalIntegrationSolver::solve(const int solverType)
{
    //DEBUG
    //solvevariableTimeStep();
    //return;
    //END DEBUG

    switch(solverType)
    {
    case 0:
        solveForwardEuler();
        break;
    case 1:
        solveMidpointMethod();
        break;
    case 2:
        solveRungeKutta();
        break;
    case 3:
        solveDormandPrince();
        break;
    case 4:
        solveBackwardEuler();
        break;
    case 5:
        solveTrapezoidRule();
        break;
    default:
        mpParentComponent->addErrorMessage("Unknown solver type!");
        mpParentComponent->stopSimulation();
    }
}


//! @brief Solves a system using forward Euler method
void NumericalIntegrationSolver::solveForwardEuler()
{
    for(int i=0; i<mnStateVars; ++i)
    {
        (*mpStateVars)[i] = (*mpStateVars)[i] + mTimeStep*mpParentComponent->getStateVariableDerivative(i);
    }
    mpParentComponent->reInitializeValuesFromNodes();
    mpParentComponent->solveSystem();
}


//! @brief Solves a system using midpoint method
void NumericalIntegrationSolver::solveMidpointMethod()
{
    std::vector<double> k1, k2;
    k1.resize(mnStateVars);
    k2.resize(mnStateVars);

    std::vector<double> orgStateVars;
    orgStateVars.resize(mnStateVars);

    orgStateVars= *mpStateVars;
    for(int i=0; i<mnStateVars; ++i)
    {
        k1[i] = mpParentComponent->getStateVariableDerivative(i);
    }
    for(int i=0; i<mnStateVars; ++i)
    {
        (*mpStateVars)[i] = (*mpStateVars)[i] + mTimeStep/2.0*k1[i];
    }
    mpParentComponent->reInitializeValuesFromNodes();
    mpParentComponent->solveSystem();
    for(int i=0; i<mnStateVars; ++i)
    {
        k2[i] = mpParentComponent->getStateVariableDerivative(i);
    }
    *mpStateVars = orgStateVars;
    mpParentComponent->reInitializeValuesFromNodes();
    for(int i=0; i<mnStateVars; ++i)
    {
        (*mpStateVars)[i] = (*mpStateVars)[i] + mTimeStep*k2[i];
    }
    mpParentComponent->solveSystem();
}


//! @brief Solves a system using implicit Euler
void NumericalIntegrationSolver::solveBackwardEuler()
{
    std::vector<double> yorg;
    yorg.resize(mnStateVars);
    yorg= *mpStateVars;

    std::vector<double> y0;
    y0.resize(mnStateVars);

    std::vector<double> y1;
    y1.resize(mnStateVars);

    for(int i=0; i<mnStateVars; ++i)
    {
       y0[i] = yorg[i] + mTimeStep*mpParentComponent->getStateVariableDerivative(i);
    }
    mpParentComponent->reInitializeValuesFromNodes();
    mpParentComponent->solveSystem();

    bool doBreak = false;
    size_t i;
    for(i=0; i<mMaxIter; ++i)
    {
        (*mpStateVars) = y0;
        for(int j=0; j<mnStateVars; ++j)
        {
            y1[j] = yorg[j] + mTimeStep*mpParentComponent->getStateVariableDerivative(j);
        }
        mpParentComponent->reInitializeValuesFromNodes();
        mpParentComponent->solveSystem();

        doBreak = true;
        for(int j=0; j<mnStateVars; ++j)
        {
            if(fabs( fabs(y1[j]-y0[j])/y0[j] ) > mTolerance)
            {
                doBreak = false;
            }
        }

        if(doBreak) break;

        y0 = y1;
    }
    if(!doBreak)
    {
        std::stringstream ss;
        ss << "Backward Euler solver failed to converge after " << i << " iterations.";
        mpParentComponent->addWarningMessage(ss.str().c_str());
    }

    (*mpStateVars) = y1;
}


//! @brief Solves a system using trapezoid rule of integration
void NumericalIntegrationSolver::solveTrapezoidRule()
{
    //Store original state variables = y(t)
    std::vector<double> yorg;
    yorg.resize(mnStateVars);
    yorg= *mpStateVars;

    //Store original state variable derivatives = f(t,y(t))
    std::vector<double> dorg;
    dorg.resize(mnStateVars);
    for(int i=0; i<mnStateVars; ++i)
    {
        dorg[i] = mpParentComponent->getStateVariableDerivative(i);
    }

    std::vector<double> y0;
    y0.resize(mnStateVars);

    std::vector<double> y1;
    y1.resize(mnStateVars);

    for(int i=0; i<mnStateVars; ++i)
    {
        y0[i] = yorg[i] + 0.5*mTimeStep*(dorg[i] + mpParentComponent->getStateVariableDerivative(i));
    }
    mpParentComponent->reInitializeValuesFromNodes();
    mpParentComponent->solveSystem();

    bool doBreak=true;
    size_t i;
    for(i=0; i<mMaxIter; ++i)
    {
        (*mpStateVars) = y0;
        for(int j=0; j<mnStateVars; ++j)
        {
            y1[j] = yorg[j] + 0.5*mTimeStep*(dorg[j] + mpParentComponent->getStateVariableDerivative(j));
        }
        mpParentComponent->reInitializeValuesFromNodes();
        mpParentComponent->solveSystem();

        doBreak = true;
        for(int j=0; j<mnStateVars; ++j)
        {
            if(fabs( fabs(y1[j]-y0[j])/y0[j] ) > mTolerance)
            {
                doBreak = false;
            }
        }

        if(doBreak) break;

        y0 = y1;
    }
    if(!doBreak)
    {
        std::stringstream ss;
        ss << "Trapezoid Rule solver failed to converge after " << i << " iterations.";
        mpParentComponent->addWarningMessage(ss.str().c_str());
    }

    (*mpStateVars) = y1;
}


//! @brief Solves a system using Runge-Kutta (RK4)
void NumericalIntegrationSolver::solveRungeKutta()
{
    std::vector<double> k1, k2, k3, k4;
    k1.resize(mnStateVars);
    k2.resize(mnStateVars);
    k3.resize(mnStateVars);
    k4.resize(mnStateVars);

    std::vector<double> orgStateVars;
    orgStateVars.resize(mnStateVars);

    orgStateVars= *mpStateVars;
    for(int i=0; i<mnStateVars; ++i)
    {
        k1[i] = mpParentComponent->getStateVariableDerivative(i);
    }
    for(int i=0; i<mnStateVars; ++i)
    {
        (*mpStateVars)[i] = (*mpStateVars)[i] + mTimeStep/2.0*k1[i];
    }
    mpParentComponent->reInitializeValuesFromNodes();
    mpParentComponent->solveSystem();
    for(int i=0; i<mnStateVars; ++i)
    {
        k2[i] = mpParentComponent->getStateVariableDerivative(i);
    }
    *mpStateVars = orgStateVars;
    mpParentComponent->reInitializeValuesFromNodes();
    for(int i=0; i<mnStateVars; ++i)
    {
        (*mpStateVars)[i] = (*mpStateVars)[i] + mTimeStep/2.0*k2[i];
    }
    mpParentComponent->solveSystem();
    for(int i=0; i<mnStateVars; ++i)
    {
        k3[i] = mpParentComponent->getStateVariableDerivative(i);
    }
    *mpStateVars = orgStateVars;
    mpParentComponent->reInitializeValuesFromNodes();
    for(int i=0; i<mnStateVars; ++i)
    {
        (*mpStateVars)[i] = (*mpStateVars)[i] + mTimeStep*k3[i];
    }
    mpParentComponent->solveSystem();
    for(int i=0; i<mnStateVars; ++i)
    {
        k4[i] = mpParentComponent->getStateVariableDerivative(i);
    }
    *mpStateVars = orgStateVars;
    mpParentComponent->reInitializeValuesFromNodes();
    for(int i=0; i<mnStateVars; ++i)
    {
        (*mpStateVars)[i] = (*mpStateVars)[i] + mTimeStep/6.0*(k1[i]+2.0*k2[i]+2.0*k3[i]+k4[i]);
    }
    mpParentComponent->solveSystem();
}


//! @brief Solves a system using Dormand-Prince
void NumericalIntegrationSolver::solveDormandPrince()
{
    std::vector<double> k1, k2, k3, k4, k5, k6;
    k1.resize(mnStateVars);
    k2.resize(mnStateVars);
    k3.resize(mnStateVars);
    k4.resize(mnStateVars);
    k5.resize(mnStateVars);
    k6.resize(mnStateVars);

    std::vector<double> orgStateVars;
    orgStateVars.resize(mnStateVars);

    orgStateVars= *mpStateVars;
    for(int i=0; i<mnStateVars; ++i)
    {
        k1[i] = mpParentComponent->getStateVariableDerivative(i);
    }

    for(int i=0; i<mnStateVars; ++i)
    {
        (*mpStateVars)[i] = (*mpStateVars)[i] + mTimeStep/5.0*k1[i];
    }
    mpParentComponent->reInitializeValuesFromNodes();
    mpParentComponent->solveSystem();
    for(int i=0; i<mnStateVars; ++i)
    {
        k2[i] = mpParentComponent->getStateVariableDerivative(i);
    }

    *mpStateVars = orgStateVars;
    mpParentComponent->reInitializeValuesFromNodes();
    for(int i=0; i<mnStateVars; ++i)
    {
        (*mpStateVars)[i] = (*mpStateVars)[i] + mTimeStep/40.0*(3*k1[i] + 9*k2[i]);
    }
    mpParentComponent->solveSystem();
    for(int i=0; i<mnStateVars; ++i)
    {
        k3[i] = mpParentComponent->getStateVariableDerivative(i);
    }

    *mpStateVars = orgStateVars;
    mpParentComponent->reInitializeValuesFromNodes();
    for(int i=0; i<mnStateVars; ++i)
    {
        (*mpStateVars)[i] = (*mpStateVars)[i] + mTimeStep*(44.0/45.0*k1[i] - 56.0/15.0*k2[i] + 32.0/9.0*k3[i]);
    }
    mpParentComponent->solveSystem();
    for(int i=0; i<mnStateVars; ++i)
    {
        k4[i] = mpParentComponent->getStateVariableDerivative(i);
    }

    *mpStateVars = orgStateVars;
    mpParentComponent->reInitializeValuesFromNodes();
    for(int i=0; i<mnStateVars; ++i)
    {
        (*mpStateVars)[i] = (*mpStateVars)[i] + mTimeStep*(19372.0/6561.0*k1[i] - 25360.0/2187.0*k2[i] + 64448.0/6561.0*k3[i] - 212.0/729.0*k4[i]);
    }
    mpParentComponent->solveSystem();
    for(int i=0; i<mnStateVars; ++i)
    {
        k5[i] = mpParentComponent->getStateVariableDerivative(i);
    }

    *mpStateVars = orgStateVars;
    mpParentComponent->reInitializeValuesFromNodes();
    for(int i=0; i<mnStateVars; ++i)
    {

        (*mpStateVars)[i] = (*mpStateVars)[i] + mTimeStep*(9017.0/3168.0*k1[i] -  355.0/33.0*k2[i] + 46732.0/5247.0*k3[i] + 49.0/176.0*k4[i] - 5103.0/18656.0*k5[i]);
    }
    mpParentComponent->solveSystem();
    for(int i=0; i<mnStateVars; ++i)
    {
        k6[i] = mpParentComponent->getStateVariableDerivative(i);
    }

    *mpStateVars = orgStateVars;
    mpParentComponent->reInitializeValuesFromNodes();
    for(int i=0; i<mnStateVars; ++i)
    {
        (*mpStateVars)[i] = (*mpStateVars)[i] + mTimeStep*(35.0/384.0*k1[i] + 500.0/1113.0*k3[i] + 125.0/192.0*k4[i] - 2187.0/6784.0*k5[i] + 11.0/84.0*k6[i]);
    }
    mpParentComponent->solveSystem();
}


//! @brief Solves a system using trapezoid rule with variable step size (experimental, do not use)
void NumericalIntegrationSolver::solvevariableTimeStep()
{
    //Store original state variables = y(t)
    std::vector<double> yorg;
    yorg.resize(mnStateVars);
    yorg= *mpStateVars;

    solveBackwardEuler();

    std::vector<double> eulerStateVars = *mpStateVars;
    *mpStateVars = yorg;

    solveTrapezoidRule();

    double maxErr = 0;
    for(int i=0; i<mnStateVars; ++i)
    {
        maxErr = std::max(maxErr, fabs(eulerStateVars[i] - (*mpStateVars)[i]));
    }

    if(maxErr == 0)
        return;

    double h = mpParentComponent->getTimestep();
    double h_new = std::max(1e-5, std::min(1e-2, 0.9*h*1e-8/maxErr));
    mpParentComponent->setTimestep(h_new);

    if(h_new < h)
    {
        *mpStateVars = yorg;
        solveTrapezoidRule();
    }

    //DEBUG
    std::cout << "maxErr = " << maxErr << ", timestep = " << mpParentComponent->getTimestep() << "\n";
    //END DEBUG
}

