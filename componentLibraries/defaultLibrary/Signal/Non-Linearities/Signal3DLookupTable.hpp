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
//! @file   Signal3DLookUpTable.hpp
//! @author Peter Nordin <peter.nordin@liue.se>
//! @date   2014-06-23
//!
//! @brief Contains a three dimensional lookup table
//!
//$Id: Signal3DLookupTable.hpp 8688 2016-01-28 07:31:58Z petno25 $

#ifndef SIGNAL3DLOOKUPTABLE_HPP_INCLUDED
#define SIGNAL3DLOOKUPTABLE_HPP_INCLUDED

#include "ComponentEssentials.h"
#include "ComponentUtilities.h"
#include "HopsanTypes.h"

namespace hopsan {

    //!
    //! @brief The data csv file should be  on , separated form or ; separated form
    //!
    //! @ingroup SignalComponents
    //!
    class Signal3DLookupTable : public ComponentSignal
    {

    private:
        double *mpInRow, *mpInCol, *mpInPlane, *mpOut;

        bool mReloadCSV;
        HString mDataCurveFileName;
        CSVParserNG mDataFile;
        LookupTable3D mLookupTable;

    public:
        static Component *Creator()
        {
            return new Signal3DLookupTable();
        }

        void configure()
        {
            addInputVariable("row", "", "", 0.0, &mpInRow);
            addInputVariable("col", "", "", 0.0, &mpInCol);
            addInputVariable("plane", "", "", 0.0, &mpInPlane);
            addOutputVariable("out", "", "", &mpOut);

            addConstant("filename", "Data file (absolute or relative to model path)", "", "FilePath", mDataCurveFileName);
            addConstant("reload","Reload csv file in initialize", "", true, mReloadCSV);
        }


        void initialize()
        {
            if ( mLookupTable.isEmpty() || mReloadCSV )
            {
                bool isOK=false;
                mLookupTable.clear();

                isOK = mDataFile.openFile(findFilePath(mDataCurveFileName));
                if (isOK)
                {
                    mDataFile.indexFile();
                    isOK = true;
                }
                if(!isOK)
                {
                    addErrorMessage("Unable to initialize CSV file: "+mDataCurveFileName+", "+mDataFile.getErrorString());
                    stopSimulation();
                    mDataFile.closeFile();
                    return;
                }
                else
                {
                    // Make sure that selected data vector is in range
                    const size_t nDataCols = mDataFile.getNumDataCols();
                    if ( !mDataFile.allRowsHaveSameNumCols() || nDataCols != 4 )
                    {
                        addErrorMessage(HString("Wrong number of data columns: ")+to_hstring(nDataCols)+" != 4");
                        stopSimulation();
                        return;
                    }

                    std::vector<long int> rowscols;
                    isOK = mDataFile.copyRow(mDataFile.getNumDataRows()-1,rowscols);
                    if (!isOK)
                    {
                        addErrorMessage("Could not parse the number of rows and columns (last line) from CSV file: "+mDataCurveFileName);
                        stopSimulation();
                        return;
                    }

                    size_t nRows = rowscols[0];
                    size_t nCols = rowscols[1];
                    size_t nPlanes = rowscols[2];

                    // Copy row and column index vectors (ignoring the final row with nRows and nCols)
                    isOK = mDataFile.copyEveryNthFromColumn(0, nCols*nPlanes, mLookupTable.getIndexDataRef(0));
                    isOK = isOK && mDataFile.copyEveryNthFromColumnRange(1, 0, nCols*nPlanes, nPlanes, mLookupTable.getIndexDataRef(1));
                    isOK = isOK && mDataFile.copyRangeFromColumn(2, 0, nPlanes, mLookupTable.getIndexDataRef(2));
                    if (!isOK)
                    {
                        addErrorMessage("Could not parse one or all of the csv index columns");
                        stopSimulation();
                        mDataFile.closeFile();
                        return;
                    }

                    // Remove "extra element (num rows)" from row index column, cols and planes not needed since we did not fetch all values
                    if (mLookupTable.getDimSize(0) == nRows+1)
                    {
                        mLookupTable.getIndexDataRef(0).pop_back();
                    }

                    // Copy values
                    isOK = mDataFile.copyRangeFromColumn(3, 0, mDataFile.getNumDataRows()-1, mLookupTable.getValueDataRef());
                    if (!isOK)
                    {
                        addErrorMessage("Could not parse the csv value column");
                        stopSimulation();
                        mDataFile.closeFile();
                        return;
                    }

                    // Now the data is in the lookup table and we can throw away the csv data to conserve memory
                    mDataFile.closeFile();

                    // Make sure the correct number of rows and columns are available
                    if ( (nRows != mLookupTable.getDimSize(0)) ||
                         (nCols != mLookupTable.getDimSize(1)) ||
                         (nPlanes != mLookupTable.getDimSize(2)))
                    {
                        addErrorMessage(HString("The actual number of extracted rows: "+to_hstring(mLookupTable.getDimSize(0))+
                                                ", cols: "+to_hstring(mLookupTable.getDimSize(1))+
                                                ", planes: "+to_hstring(mLookupTable.getDimSize(2))+
                                                ", Does not match the specification (last line): "+
                                                to_hstring(nRows)+" "+to_hstring(nCols)+" "+to_hstring(nPlanes)));
                        stopSimulation();
                        return;
                    }

                    // Make sure strictly increasing (no sorting will be done if that is already the case)
                    mLookupTable.sortIncreasing();

                    // Check if data is OK before we continue
                    isOK = mLookupTable.isDataOK();
                    if(!isOK)
                    {
                        addErrorMessage("The LookupTable data is not OK after reading from file: "+mDataCurveFileName);
                        if (!mLookupTable.isDataSizeOK())
                        {
                            addErrorMessage("Something is wrong with the size of the index or data vectors");
                        }
                        if (!mLookupTable.allIndexStrictlyIncreasing())
                        {
                            addErrorMessage("Even after sorting, one or more index columns are still not strictly increasing");
                        }
                        stopSimulation();
                    }
                }
            }
            simulateOneTimestep();
        }


        void simulateOneTimestep()
        {
            (*mpOut) = mLookupTable.interpolate(*mpInRow, *mpInCol, *mpInPlane);
        }
    };
}

#endif // SIGNALLOOKUPTABLE2D_HPP_INCLUDED
