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


//$Id: HopsanTypes.h 8765 2016-02-11 12:23:57Z petno25 $

#ifndef HOPSANTYPES_H
#define HOPSANTYPES_H

#include "win32dll.h"
#include <cstddef>

namespace hopsan {

class DLLIMPORTEXPORT HString
{
private:
    char *mpDataBuffer;
    size_t mSize;

public:
    static const size_t npos;

    HString();
    ~HString();
    HString(const char* str);
    HString(const int value);
    HString(const HString &rOther);
    HString(const HString &rOther, size_t pos, size_t len=npos);
    void setString(const char* str);
    HString &append(const char* str);
    HString &append(const char chr);
    HString &append(const HString &str);
    HString &erase (size_t pos = 0, size_t len = npos);
    void clear();

    void replace(const size_t pos, const size_t len, const char* str);
    HString &replace(const char* oldstr, const char* newstr);
    HString &replace(const HString &rOldstr, const HString &rNewstr);

    const char *c_str() const;
    size_t size() const;
    bool empty() const;
    bool compare(const char* other) const;
    bool compare(const HString &rOther) const;
    bool isNummeric() const;
    bool isBool() const;
    double toDouble(bool *isOK) const;
    long int toLongInt(bool *isOK) const;

    HString substr(const size_t pos, const size_t len=npos) const;

    size_t find_first_of(const char c, size_t pos = 0) const;
    size_t rfind(const char c, size_t pos = npos) const;
    size_t find(const char c, size_t pos = 0) const;
    size_t find(const char *s, size_t pos = 0) const;
    size_t find(const HString &s, size_t pos = 0) const;
    bool containes(const HString &rString) const;
    bool containes(const char c) const;
    bool containes(const char *s) const;

    char front() const;
    char &front();
    char back() const;
    char &back();
    char at(const size_t pos) const;
    char& operator[](const size_t idx);
    const char& operator[](const size_t idx) const;

    bool operator<(const HString &rhs) const;

    HString& operator+=(const HString& rhs);
    HString& operator+=(const char *rhs);
    HString& operator+=(const char rhs);

    HString& operator=(const char* rhs);
    HString& operator=(const char rhs);
    HString& operator=(const HString &rhs);
};

inline bool operator==(const HString& lhs, const HString& rhs){return lhs.compare(rhs);}
inline bool operator!=(const HString& lhs, const HString& rhs){return !operator==(lhs,rhs);}
inline bool operator> (const HString& lhs, const HString& rhs){return rhs<lhs;}
inline bool operator<=(const HString& lhs, const HString& rhs){return !operator> (lhs,rhs);}
inline bool operator>=(const HString& lhs, const HString& rhs){return !(lhs<rhs);}

inline HString operator+(HString lhs, const HString& rhs)
{
  lhs += rhs;
  return lhs;
}

inline HString operator+(HString lhs, const char rhs)
{
  lhs += rhs;
  return lhs;
}


template<typename T>
class HVector
{
private:
    T *mpDataArray;
    size_t mSize;

public:
    HVector()
    {
        mpDataArray = 0;
        mSize = 0;
    }

    //! @brief copy constructor
    HVector(const HVector<T> &rOther)
    {
        mpDataArray = new T[rOther.size()];
        mSize = rOther.size();
        for (size_t i=0; i<rOther.size(); ++i)
        {
            mpDataArray[i] = rOther[i];
        }
    }

    ~HVector()
    {
        clear();
    }

    //! @brief Clear the array
    void clear()
    {
        if (mpDataArray)
        {
            delete[] mpDataArray;
            mSize = 0;
            mpDataArray=0;
        }
    }

    //! @brief Resize the array, keeping old data if any.
    //! @details If new size is smaller than old, old data will be truncated
    //! If new size is larger than old, the additional elements will be uninitialized
    //! @param [in] s New size
    void resize(const size_t s)
    {
        // Create new dummy array
        T* pNewArray = new T[s];

        // Check how many elements to copy
        size_t nCopy = s;
        if (size() < s)
        {
            nCopy = size();
        }

        // Copy old data to new array
        for (size_t i=0; i<nCopy; ++i)
        {
            pNewArray[i] = mpDataArray[i];
        }

        // Clear old data
        clear();

        // Set new data
        mpDataArray = pNewArray;
        mSize = s;
    }

    //! @brief Resize the array, initializing all values to defaultValue
    //! @param [in] s New size
    //! @param [in] rDefaultValue initialize value for all elements
    void resize(const size_t s, const T &rDefaultValue)
    {
        clear();
        mpDataArray = new T[s];
        mSize = s;
        for (size_t i=0; i<mSize; ++i)
        {
            mpDataArray[i] = rDefaultValue;
        }
    }

    //! @brief Append data
    //! @note This function is slow, it will reallocate all array memory every time
    //! @param [in] rData Data to append
    void append(const T &rData)
    {
        resize(size()+1);
        mpDataArray[size()-1] = rData;
    }

    const T& operator[] (const size_t i) const
    {
        return mpDataArray[i];
    }

    T& operator[] (const size_t i)
    {
        return mpDataArray[i];
    }

    //! @brief Returns the number of elements in the array
    //! @returns Number of elements in the array
    size_t size() const
    {
        return mSize;
    }

    //! @brief Check if the array is empty
    //! @returns true if the array is empty
    bool empty() const
    {
        return (mSize==0);
    }
};

}

#endif // HOPSANTYPES_H
