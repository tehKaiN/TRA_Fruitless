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
//! @file   LookupTable.h
//! @author Peter Nordin <peter.nordin@liu.se>
//! @date   2011-04-12
//!
//! @brief Contains the utility Lookuptable class
//!
//$Id: LookupTable.cc 8131 2015-06-17 12:02:37Z petno25 $

#include "ComponentUtilities/LookupTable.h"


//LookupTable1DNonTemplate::LookupTable1DNonTemplate()
//{
//    clear();
//}

//void LookupTable1DNonTemplate::clear()
//{
//    mIndexData.clear();
//    mValueData.clear();
//    mIndexIncreasingOrDecreasing = Unknown;
//    mFirstIndex = 0;
//    mFirstValue = 0;
//    mLastIndex = 1;
//    mLastValue = 1;
//}

//std::vector<double> &LookupTable1DNonTemplate::getIndexDataRef()
//{
//    return mIndexData;
//}

//std::vector<double> &LookupTable1DNonTemplate::getValueDataRef()
//{
//    return mValueData;
//}

//bool LookupTable1DNonTemplate::isDataOK()
//{
//    if ( (mIndexData.size() == mValueData.size()) && mIndexData.size() > 1)
//    {
//        mFirstIndex = mIndexData[0];
//        mFirstValue = mValueData[0];
//        mLastIndex = mIndexData[mIndexData.size()-1];
//        mLastValue = mIndexData[mValueData.size()-1];

//        if (mIndexIncreasingOrDecreasing == Unknown)
//        {
//            calcIncreasingOrDecreasing();
//        }

//        return (mIndexIncreasingOrDecreasing == StrictlyIncreasing);
//    }
//    else
//    {
//        mFirstIndex = 0;
//        mFirstValue = 0;
//        mLastIndex = 1;
//        mLastValue = 1;
//        return false;
//    }
//}

//LookupTable1DNonTemplate::IncreasingEnumT LookupTable1DNonTemplate::isIndexIncreasingOrDecresing() const
//{
//    return mIndexIncreasingOrDecreasing;
//}

////! @brief Check if data in columns are strictly increasing or decreasing
////! @todo maybe we should force the input data vector to be strictly increasing or decreasing
////! @todo maybe data should be automatically sorted when reading the file instead
//LookupTable1DNonTemplate::IncreasingEnumT LookupTable1DNonTemplate::calcIncreasingOrDecreasing()
//{
//    mIndexIncreasingOrDecreasing = NotStrictlyIncOrDec;

//    if(!mIndexData.empty())
//    {
//        bool increasing=true;
//        bool decreasing=true;
//        for(size_t row=1; row<mIndexData.size(); ++row)
//        {
//            if (mIndexData[row] > mIndexData[row-1])
//            {
//                increasing = increasing && true;
//                decreasing = false;
//            }

//            if (mIndexData[row] < mIndexData[row-1])
//            {
//                decreasing = decreasing && true;
//                increasing = false;
//            }
//        }

//        if(increasing)
//        {
//            mIndexIncreasingOrDecreasing = StrictlyIncreasing;
//        }

//        if(decreasing)
//        {
//            mIndexIncreasingOrDecreasing = StrictlyDecreasing;
//        }
//    }

//    return mIndexIncreasingOrDecreasing;
//}

////! @brief Sorts the data so that the index vector is strictly increasing
//void LookupTable1DNonTemplate::sortIncreasing()
//{
//    if (mIndexIncreasingOrDecreasing == Unknown)
//    {
//        calcIncreasingOrDecreasing();
//    }

//    // If row is strictly decreasing the swap row order, else run quicksort and hope for the best
//    if (mIndexIncreasingOrDecreasing == StrictlyDecreasing)
//    {
//        reverseRows();
//        isDataOK();
//    }
//    // Else if not already strictly increasing then sort it
//    else if (mIndexIncreasingOrDecreasing == NotStrictlyIncOrDec)
//    {
//        quickSort(mIndexData, 0, mIndexData.size()-1);
//        isDataOK();
//    }
//    // Else we are already OK
//}

//double LookupTable1DNonTemplate::interpolate(const double x) const
//{
//    // Handle outside index range
//    if( x<mFirstIndex )
//    {
//        return mFirstValue;
//    }
//    // Handle outside index range
//    else if( x>=mLastIndex )
//    {
//        return mLastValue;
//    }
//    // Handle within range
//    else
//    {
//        size_t row = intervalHalfSubDiv(x, 0, mIndexData.size()-1);
//        if(mIndexData[row+1] ==  mIndexData[row])       //Check for division by zero (this means that if several X values have the same value, we will always pick the first one since we cannot interpolate between them)
//        {
//            return mValueData[row];
//        }
//        else
//        {
//            return mValueData[row] + (x - mIndexData[row])*(mValueData[row+1] -  mValueData[row])/(mIndexData[row+1] -  mIndexData[row]);
//        }
//    }
//    return x; //!< @todo  Don't know if this is correct, return x if we were unsuccessful
//}

////! @brief Recursively subdivide into halves to find start row for interpolation
////! @note This function assumes that the the indexData is strictly increasing
//size_t LookupTable1DNonTemplate::intervalHalfSubDiv(const double x, const size_t i1, const size_t iend) const
//{
//    if (iend-i1 <= 1)
//    {
//        // When the two indexes are next to each other lets return the smallest one as the start row for interpolation
//        return i1;
//    }
//    else
//    {
//        //Calc split index
//        size_t splitIdx = i1 + (iend - i1)/2; //Allow truncation

//        if (x <= mIndexData[splitIdx])
//        {
//            // Use lower half
//            return intervalHalfSubDiv(x, i1, splitIdx);
//        }
//        else
//        {
//            // Use higher half
//            return intervalHalfSubDiv(x, splitIdx, iend);
//        }
//    }
//}

////! @brief Quicksort algorithm, based on WIKIPEDIA
//size_t LookupTable1DNonTemplate::quickSortPartition(const std::vector<double> &rIndexArray, const size_t left, const size_t right, const size_t pivotIndex)
//{
//    // left is the index of the leftmost element of the array
//    // right is the index of the rightmost element of the array (inclusive)
//    //   number of elements in subarray = right-left+1

//    double pivotValue = rIndexArray[pivotIndex];
//    swapRows(pivotIndex, right);  // Move pivot to end
//    size_t storeIndex = left;
//    for (size_t i=left; i<right; ++i)  // left ≤ i < right
//    {
//        if (rIndexArray[i] < pivotValue)
//        {
//            swapRows(i,storeIndex);
//            storeIndex = storeIndex + 1;
//        }
//    }
//    swapRows(storeIndex,right); // Move pivot to its final place
//    return storeIndex;
//}

////! @brief Quicksort algorithm, based on WIKIPEDIA
//void LookupTable1DNonTemplate::quickSort(const std::vector<double> &rIndexArray, const size_t left, const size_t right)
//{
//    // If the list has 2 or more items
//    if (left < right)
//    {
//        // See "Choice of pivot" section below for possible choices
//        //choose any 'pivotIndex' such that 'left' ≤ 'pivotIndex' ≤ 'right'
//        size_t pivotIndex = left+(right-left)/2;

//        // Get lists of bigger and smaller items and final position of pivot
//        size_t pivotNewIndex = quickSortPartition(rIndexArray, left, right, pivotIndex);

//        // Recursively sort elements smaller than the pivot
//        // but not if it happened to be zero (would lead to underflow in size_t)
//        if (pivotNewIndex>0)
//        {
//            quickSort(rIndexArray, left, pivotNewIndex-1);
//        }

//        // Recursively sort elements at least as big as the pivot
//        quickSort(rIndexArray, pivotNewIndex+1, right);
//    }
//}

////! @brief Swaps the data on two "rows"
////! @note Index values must be in range, no check is performed
//void LookupTable1DNonTemplate::swapRows(const size_t r1, const size_t r2)
//{
//    double tmp;

//    tmp = mIndexData[r1];
//    mIndexData[r1] = mIndexData[r2];
//    mIndexData[r2] = tmp;

//    tmp = mValueData[r1];
//    mValueData[r1] = mValueData[r2];
//    mValueData[r2] = tmp;
//}

////! @brief Reverses the "row" order
//void LookupTable1DNonTemplate::reverseRows()
//{
//    std::vector<double>::reverse_iterator rit;
//    std::vector<double> tempData;

//    tempData.reserve(mIndexData.size());
//    for (rit=mIndexData.rbegin(); rit!=mIndexData.rend(); ++rit)
//    {
//        tempData.push_back(*rit);
//    }
//    mIndexData.swap(tempData);

//    tempData.clear();
//    tempData.reserve(mValueData.size());
//    for (rit=mValueData.rbegin(); rit!=mValueData.rend(); ++rit)
//    {
//        tempData.push_back(*rit);
//    }
//    mValueData.swap(tempData);
//}


//double LookupTable2DNonTemplate::interpolate2D(const double x1, const double x2) const
//{

//        // y x -->
//        // |    c11             c12
//        // v
//        //       r1         *    r2
//        //
//        //      c21             c22

//        // First interpolate between c11 and c21 to find r1 (data is stored column wise)
//        //double r1 = interpolate(x1,inCol1)



//        // Now interpolate between c21 and c12 to find r2



//        // Now interpolate between r1 and r2  to find the desired value
//}
