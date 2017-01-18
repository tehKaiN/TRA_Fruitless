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
//! @file   ClassFactory.hpp
//! @author <peter.nordin@liu.se>
//! @date   2009-12-26
//!
//! @brief Contains template class ClassFactory for automatic object instantiation through key value
//!
//$Id: ClassFactory.hpp 8131 2015-06-17 12:02:37Z petno25 $

#ifndef CLASFACTORY_HPP_INCLUDED
#define CLASFACTORY_HPP_INCLUDED

#include <map>
#include <vector>
#include <cstddef>

namespace hopsan {

    //! @brief Template class for automatic object instantiation by key-value.
    //!
    //! This code is based on:
    //! http://www.codeproject.com/KB/architecture/SimpleDynCreate.aspx
    //!
    template <typename _Key, typename _Base, typename _Predicator = std::less<_Key> >
    class ClassFactory
    {
    public:
        typedef std::vector< std::pair<_Key, int> > RegStatusVectorT;
        enum RegisterStatusEnumT {RegisteredOk, AllreadyRegistered, NotRegistered};

    protected:
        typedef _Base* (*CreatorFunctionT) (void);
        typedef std::map<_Key, CreatorFunctionT, _Predicator> FactoryMapT;
        typedef std::pair<_Key, CreatorFunctionT> FactoryPairT;

        //! Map where the construction info is stored
        FactoryMapT mFactoryMap;
        //! Error status map
        RegStatusVectorT mRegStatusVector;


    public:
        //! @brief Used to register creator functions
        void registerCreatorFunction(const _Key &rIdKey, CreatorFunctionT classCreator)
        {
            std::pair<typename FactoryMapT::iterator, bool> rc;
            rc = mFactoryMap.insert(FactoryPairT(rIdKey, classCreator));
            if (!rc.second)
            {
                mRegStatusVector.push_back(std::pair<_Key, int>(rIdKey, AllreadyRegistered));
            }
            else
            {
                mRegStatusVector.push_back(std::pair<_Key, int>(rIdKey, RegisteredOk));
            }
        }

        //! @brief Reserve keyword by inserting NULL ptr,
        bool reserveKey(const _Key &rIdKey)
        {
            std::pair<typename FactoryMapT::iterator, bool> rc;
            rc = mFactoryMap.insert(FactoryPairT(rIdKey, static_cast<CreatorFunctionT>(0)));
            return rc.second;
        }

        //! @brief Creates an instance based on the key using creator function (if registered)
        _Base* createInstance(const _Key &rIdKey)
        {
            //std::cout << "Create: Size: " << mFactoryMap.size() << std::endl;
            typename FactoryMapT::iterator it = mFactoryMap.find(rIdKey);
            if (it != mFactoryMap.end())
            {
                if (it->second)
                {
                    return it->second();
                }
            }
            mRegStatusVector.push_back(std::pair<_Key, int>(rIdKey, NotRegistered));
            //std::cout << "Warning key: " << idKey << " not found!" << std::endl;
            return 0;
        }

        //! @brief Check if the factory has key registered
        bool hasKey(const _Key &rIdKey) const
        {
            if (mFactoryMap.find(rIdKey) != mFactoryMap.end())
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        //! @brief Return a vector with all registered keys
        const std::vector<_Key> getRegisteredKeys() const
        {
            std::vector<_Key> keys;
            keys.reserve(mFactoryMap.size());
            typename FactoryMapT::const_iterator cit;
            for (cit=mFactoryMap.begin(); cit!=mFactoryMap.end(); ++cit)
            {
                keys.push_back(cit->first);
            }
            return keys;
        }

        //! @brief Unregister creator functions for given key
        void unRegisterCreatorFunction(const _Key &rIdKey)
        {
            size_t rc;
            rc = mFactoryMap.erase(rIdKey);
            if (rc > 0)
            {
                //std::cout << "Sucessfully unregistered: " << idKey << std::endl;
                //! @todo Do we need a status message here to ??
            }
            else
            {
                mRegStatusVector.push_back(std::pair<_Key, int>(rIdKey, NotRegistered));
                //std::cout << "Failed to unregister: " << idKey << std::endl;
            }
        }

        //! @brief Get a copy of the internal error vector, it maps key values against error codes, error codes come from registration or unregistration
        RegStatusVectorT getRegisterStatus()
        {
            return mRegStatusVector;
        }

        //! @brief Clears the internal error status vector
        void clearRegisterStatus()
        {
            mRegStatusVector.clear();
        }

        //! @brief Clear the entire factory map (unregister everything)
        void clearFactory()
        {
            mFactoryMap.clear();
            mRegStatusVector.clear();
        }
    };
}

#endif // CLASFACTORY_HPP_INCLUDED
