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

//$Id: GeneratorHandler.h 8474 2015-11-18 08:48:01Z petno25 $

#ifndef GENERATORHANDLER_H
#define GENERATORHANDLER_H

#ifdef _WIN32
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0502
#include "Windows.h"
#endif
#include <vector>
#include "win32dll.h"
#include "HopsanTypes.h"

namespace hopsan {

class ComponentSystem;

class DLLIMPORTEXPORT GeneratorHandler
{
public:
    GeneratorHandler();
    ~GeneratorHandler();

    bool isLoadedSuccessfully();

    typedef void (*call_modelica_generator_t)(HString path, hopsan::HString gccPath, bool showDialog, int solver, bool compile, hopsan::HString coreIncludePath, hopsan::HString binPath);
    typedef void (*call_cpp_generator_t)(HString hppPath, hopsan::HString gccPath, bool compile, hopsan::HString coreIncludePath, hopsan::HString binPath);
    typedef void (*call_fmu_import_generator_t)(HString path, HString targetPath, HString coreIncludePath, HString binPath, HString gccPath, bool showDialog);
    typedef void (*call_fmu_export_generator_t)(HString path, hopsan::ComponentSystem *pSystem, HString coreIncludePath, HString binPath, HString gccPath, int version, bool x64, bool showDialog);
    typedef void (*call_simulink_export_generator_t)(HString path, HString modelName, hopsan::ComponentSystem *pSystem, bool disablePortLabels, HString coreIncludePath, HString binPath, bool showDialog);
    typedef void (*call_simulink_cosim_export_generator_t)(HString path, hopsan::ComponentSystem *pSystem, bool disablePortLabels, int compiler, HString coreIncludePath, HString binPath, bool showDialog);
    typedef void (*call_lvsit_export_generator_t)(HString path, hopsan::ComponentSystem *pSystem, HString coreIncludePath, HString binPath, bool showDialog);
    typedef void (*call_complib_compiler_t)(HString path, HString extraCFlags, HString extraLFlags, HString coreIncludePath, HString binPath, HString gccPath, bool showDialog);
    typedef void (*call_library_generator_t)(HString path, std::vector<HString> hppFiles, bool showDialog);

    call_modelica_generator_t callModelicaGenerator;
    call_cpp_generator_t callCppGenerator;
    call_fmu_import_generator_t callFmuImportGenerator;
    call_fmu_export_generator_t callFmuExportGenerator;
    call_simulink_export_generator_t callSimulinkExportGenerator;
    call_simulink_cosim_export_generator_t callSimulinkCoSimExportGenerator;
    call_lvsit_export_generator_t callLabViewSITGenerator;
    call_complib_compiler_t callComponentLibraryCompiler;
    call_library_generator_t callLibraryGenerator;

private:
    bool mLoadedSuccessfully;

#ifdef _WIN32
    HINSTANCE mpLib;
#else
    void* mpLib;
#endif
};

}

#endif // GENERATORHANDLER_H
