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

#define S_FUNCTION_NAME model
#define S_FUNCTION_LEVEL 2

#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include "simstruc.h"
#include "HopsanCore/include/HopsanCore.h"
using namespace hopsan;


HopsanEssentials gHopsanCore;
ComponentSystem* pComponentSystem;
bool isOkToSimulate = false;

#define NUMPARAMS 0//0

double *pInputNode0;
double *pInputNode1;
double *pOutputNode0;
double *pOutputNode1;
double *pOutputNode2;

static void mdlInitializeSizes(SimStruct *S)
{
    // If NUMPARAMS != 0 then the mask setup will not work, so we can not use S-function parameters, for the parameters
    // instead we let the mask create local workspace variables corresponding to our system parameters
    ssSetNumSFcnParams(S, NUMPARAMS);
#if defined(MATLAB_MEX_FILE)
    if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S)) {
        //mdlCheckParameters(S);
        //if (ssGetErrorStatus(S) != NULL) {
        //    return;
        //}
    } else {
        return; /* Parameter mismatch will be reported by Simulink. */
    }
#endif

    //Define S-function input signals
    if (!ssSetNumInputPorts(S,2)) return;				//Number of input signals
    ssSetInputPortWidth(S, 0, DYNAMICALLY_SIZED);		//Input signal 0
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    ssSetInputPortWidth(S, 1, DYNAMICALLY_SIZED);		//Input signal 1
    ssSetInputPortDirectFeedThrough(S, 1, 1);

    //Define S-function output signals
    if (!ssSetNumOutputPorts(S,4)) return;				//Number of output signals
    ssSetOutputPortWidth(S, 0, DYNAMICALLY_SIZED);		//Output signal 0
    ssSetOutputPortWidth(S, 1, DYNAMICALLY_SIZED);		//Output signal 1
    ssSetOutputPortWidth(S, 2, DYNAMICALLY_SIZED);		//Output signal 2

    ssSetOutputPortWidth(S, 3, DYNAMICALLY_SIZED);		//Debug output signal
    ssSetNumSampleTimes(S, 1);
    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE);

    if(!gHopsanCore.hasComponent("HydraulicLaminarOrifice"))
    {
        ssSetErrorStatus(S, "Error: Component libraries failed to load.");
        return;
    }

//    std::vector<std::string> extLibs;
//    readExternalLibsFromTxtFile("externalLibs.txt",extLibs);
//    for (size_t i=0; i<extLibs.size(); ++i)
//    {
//        gHopsanCore.loadExternalComponentLib(extLibs[i].c_str());
//    }

    const char* hmfFilePath = "model.hmf";
    double startT, stopT;
    pComponentSystem = gHopsanCore.loadHMFModelFile(hmfFilePath, startT, stopT);
    if (pComponentSystem==0)
    {
        while(gHopsanCore.checkMessage())
        {
            HString msg, type, tag;
            gHopsanCore.getMessage(msg, type, tag);
            ssPrintf("%s\n",msg.c_str());
        }
        ssSetErrorStatus(S,"Error could not open model: model.hmf");
        return;
    }
    startT = ssGetTStart(S);
    stopT = ssGetTFinal(S);
    pComponentSystem->setDesiredTimestep(0.001);
    mexCallMATLAB(0, 0, 0, 0, "modelMaskSetup");                              //Run the port label script
}


static void mdlInitializeSampleTimes(SimStruct *S)
{
    //ssSetSampleTime(S, 0, 0.001);
    //ssSetOffsetTime(S, 0, 0.0);

    //Update tunable parameters
    const mxArray* in;
    mwSize parsize;
    void* pBuffer=0;
    HString valstr;



    isOkToSimulate = pComponentSystem->checkModelBeforeSimulation();
    if (isOkToSimulate)
    {
        pComponentSystem->setNumLogSamples(0);
        pComponentSystem->disableLog();
        pComponentSystem->initialize(0,1);
    }
    else
    {
        while(gHopsanCore.checkMessage())
        {
            HString msg, type, tag;
            gHopsanCore.getMessage(msg, type, tag);
            ssPrintf("%s\n",msg.c_str());
        }
        ssSetErrorStatus(S,"Error isSimulationOk() returned False! Most likely some components could not be loaded or some connections could not be established.");
        return;
    }

    pInputNode0 = pComponentSystem->getSubComponent("Ein")->getSafeNodeDataPtr("out", 0);
    pInputNode1 = pComponentSystem->getSubComponent("Fin")->getSafeNodeDataPtr("out", 0);
    pOutputNode0 = pComponentSystem->getSubComponent("Pout")->getSafeNodeDataPtr("in", 0);
    pOutputNode1 = pComponentSystem->getSubComponent("Tout")->getSafeNodeDataPtr("in", 0);
    pOutputNode2 = pComponentSystem->getSubComponent("Vout")->getSafeNodeDataPtr("in", 0);


    // Free parameter buffer memory
    if (pBuffer)
    {
        free(pBuffer);
    }
}


static void mdlOutputs(SimStruct *S, int_T tid)
{
    //S-function input signals
    InputRealPtrsType uPtrs1 = ssGetInputPortRealSignalPtrs(S,0);

    //S-function output signals
        real_T *y0 = ssGetOutputPortRealSignal(S,0);
    real_T *y1 = ssGetOutputPortRealSignal(S,1);
    real_T *y2 = ssGetOutputPortRealSignal(S,2);
    real_T *y3 = ssGetOutputPortRealSignal(S,3);

    int_T width1 = ssGetOutputPortWidth(S,0);

    //Input parameters
    double input0 = (*uPtrs1[0]);
    double input1 = (*uPtrs1[1]);

    //Equations
    double output0;
    double output1;
    double output2;
    double output3;

    output3 = 0;		//Error code 0: Nothing is wrong

        (*pInputNode0) = input0;
        (*pInputNode1) = input1;

    double time = ssGetT(S);
    pComponentSystem->simulate(time);

        output0 = (*pOutputNode0);
        output1 = (*pOutputNode1);
        output2 = (*pOutputNode2);


    //Output parameters
    *y0 = output0;
    *y1 = output1;
    *y2 = output2;
    *y3 = output3;

    while(gHopsanCore.checkMessage())
    {
        HString msg, type, tag;
        gHopsanCore.getMessage(msg, type, tag);
        if (type != "debug")
        {
            ssPrintf("%s\n",msg.c_str());
        }
    }
}

static void mdlTerminate(SimStruct *S)
{
    pComponentSystem->finalize();
}


 /* Simulink/Simulink Coder Interfaces */
 #ifdef MATLAB_MEX_FILE /* Is this file being compiled as a MEX-file? */
 #include "simulink.c" /* MEX-file interface mechanism */
 #else
 #include "cg_sfun.h" /* Code generation registration function */
 #endif

