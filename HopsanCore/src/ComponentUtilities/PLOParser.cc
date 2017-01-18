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
//! @file   PLOParser.cc
//! @author Peter Nordin <peter.nordin@liu.se>
//! @date   2014-10-03
//!
//! @brief Contains the Core Utility PLOParser class
//!
//$Id: PLOParser.cc 8164 2015-06-30 10:40:15Z petno25 $

#include "ComponentUtilities/PLOParser.h"
#include "ComponentUtilities/num2string.hpp"

#include <fstream>
#include <cstring>

using namespace hopsan;

PLOParser::PLOParser()
{
    clearData();
}

void PLOParser::clearData()
{
    mData.clear();
    mDataNames.clear();
    mErrorString.clear();
    mPlotScales.clear();
    mPloVersion = 0;
    mNumDataCols = 0;
    mNumDataRows = 0;

}

bool PLOParser::isEmpty() const
{
    return mData.empty();
}

bool PLOParser::readFile(const HString &rFilepath)
{
    // Clear old data
    clearData();

    // Attempt to open file
    std::ifstream myfile(rFilepath.c_str());
    if (myfile.is_open())
    {
        std::string tmp;

        // Read version data and check if this seems to be a plo file
        myfile >> tmp;
        if ( tmp != "'VERSION'" )
        {
            mErrorString = rFilepath+" Does not seem to be a plo file, Aborting import!";
            myfile.close();
            return false;
        }

        // Read version number
        myfile >> mPloVersion;
        if (mPloVersion < 1 || mPloVersion > 2)
        {
            mErrorString = "Incorrect PLO version: " + to_hstring(mPloVersion);
            myfile.close();
            return false;
        }


        // Skip name line
        getline(myfile, tmp); // First remove previous newline
        getline(myfile, tmp);
        //! @todo maybe parse this for model info for plo v2 format

        // Read num data
        myfile >> mNumDataCols;
        myfile >> mNumDataRows;

        if ( mNumDataCols < 1 || mNumDataRows < 1 )
        {
            mErrorString = "No data rows or columns found";
            myfile.close();
            return false;
        }

        // Read column names

        mDataNames.reserve(mNumDataCols+1); //One extra in case plo v1 and Time in first column
        for (size_t i=0; i<mNumDataCols; ++i)
        {
            myfile >> tmp;
            HString name(tmp.c_str());

            //! @todo isn't there frequency as well
            if ( (i==0) && (mPloVersion == 1) && (name=="'Time',") )
            {
                ++mNumDataCols;
            }
            // Remove ', and ' from names
            name.replace("',", "");
            name.replace("'", "");

            // Remember name
            mDataNames.push_back(name);
        }

        // Read plotscales
        mPlotScales.resize(mNumDataCols);
        for (size_t i=0; i<mNumDataCols; ++i)
        {
            double scale;
            myfile >> scale;
            mPlotScales[i] = scale;
        }

        // Read data
        mData.resize(mNumDataCols*mNumDataRows);
        for (size_t i=0; i<mNumDataCols*mNumDataRows; ++i)
        {
            double val;
            myfile >> val;
            mData[i] = val;
        }

        if (mPloVersion == 1)
        {
            // Read DAT line (not used, ignored in HopsanNG)
            myfile >> tmp;

            // Read modelname
            myfile >> tmp;
            //! @todo should we save this?
        }

        // All good
        myfile.close();
        return true;
    }
    else
    {
        mErrorString = "Could not open file";
        myfile.close();
        return false;
    }
}

HString PLOParser::getErrorString() const
{
    return mErrorString;
}

size_t PLOParser::getNumDataRows() const
{
    return mNumDataRows;
}

size_t PLOParser::getNumDataCols() const
{
    return mNumDataCols;
}

size_t PLOParser::getPloFileVersion() const
{
    return mPloVersion;
}

std::vector<HString> PLOParser::getDataNames() const
{
    return mDataNames;
}

int PLOParser::getColIdxForDataName(const HString &rName) const
{
    for (size_t i=0; i<mDataNames.size(); ++i)
    {
        if (rName == mDataNames[i])
        {
            return int(i);
        }
    }
    return -1;
}

bool PLOParser::copyRow(const size_t rowIdx, std::vector<double> &rRow)
{
    if (rowIdx < mNumDataRows)
    {
        rRow.resize(mNumDataCols);
        for (size_t c=0; c<mNumDataCols; ++c)
        {
            rRow[c] = mData[rowIdx*mNumDataCols+c];
        }
        return true;
    }
    return false;
}

bool PLOParser::copyColumn(const size_t columnIdx, std::vector<double> &rColumn)
{
    if (columnIdx < mNumDataCols)
    {
        rColumn.resize(mNumDataRows);
        for (size_t r=0; r<mNumDataRows; ++r)
        {
            rColumn[r] = mData[r*mNumDataCols+columnIdx];
        }
        return true;
    }
    return false;
}

bool PLOParser::copyColumn(const HString &rDataName, std::vector<double> &rColumn)
{
    int cid = getColIdxForDataName(rDataName);
    if (cid >= 0)
    {
        return copyColumn(size_t(cid), rColumn);
    }
    return false;
}
