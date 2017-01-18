/*-----------------------------------------------------------------------------
 This source file is part of Hopsan NG

 Copyright (c) 2011
    Mikael Axin, Robert Braun, Alessandro Dell'Amico, Björn Eriksson,
    Peter Nordin, Karl Pettersson, Petter Krus, Ingo Staack

 This file is provided "as is", with no guarantee or warranty for the
 functionality or reliability of the contents. All contents in this file is
 the original work of the copyright holders at the Division of Fluid and
 Mechatronic Systems (Flumes) at Linköping University. Modifying, using or
 redistributing any part of this file is prohibited without explicit
 permission from the copyright holders.
-----------------------------------------------------------------------------*/

//!
//! @file   hopsan_rapidxml.hpp
//! @author Peter Nordin <peter.nordin@liu.se>
//! @date   2012-05-30
//!
//! @brief Contains some convenient help functions for using rapid_xml in Hopsan
//! @note This file is not included with HopsanCore because we want to keep the include folder free from external dependencies
//!
//$Id: hopsan_rapidxml.hpp 7572 2015-01-27 11:31:17Z petno25 $

#ifndef HOPSAN_RABITXML_HPP
#define HOPSAN_RABITXML_HPP

#include <string>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

//! @brief Reads a double value from node attribute or return default value if attribute does not exist
double readDoubleAttribute(const rapidxml::xml_node<> *pNode, const std::string attrName, const double defaultValue)
{
    if (pNode!=0)
    {
        rapidxml::xml_attribute<> *pAttr = pNode->first_attribute(attrName.c_str());
        if (pAttr)
        {
            //Convert char* to double, assume null terminated strings
            return atof(pAttr->value());
        }
    }

    return defaultValue;
}

//! @brief Reads an integer value from node attribute or return default value if attribute does not exist
int readIntAttribute(const rapidxml::xml_node<> *pNode, const std::string attrName, const int defaultValue)
{
    if (pNode!=0)
    {
        rapidxml::xml_attribute<> *pAttr = pNode->first_attribute(attrName.c_str());
        if (pAttr)
        {
            //Convert char* to int, assume null terminated strings
            return atoi(pAttr->value());
        }
    }

    return defaultValue;
}

//! @brief Reads a string value from node attribute or return default value if attribute does not exist
std::string readStringAttribute(const rapidxml::xml_node<> *pNode, const std::string attrName, const std::string defaultValue="")
{
    if (pNode)
    {
        rapidxml::xml_attribute<> *pAttr = pNode->first_attribute(attrName.c_str());
        if (pAttr)
        {
            //Convert char* to string, assume null terminated strings
            return std::string(pAttr->value());
        }
    }

    return defaultValue;
}

void writeStringAttribute(rapidxml::xml_node<> *pNode, const std::string attrName, const std::string value)
{
    // First allocate memory for and copy this string so that it becomes persistent
    char* pRapidAllocatedAttributeValue = pNode->document()->allocate_string(value.c_str());

    // Assign if already exist, else create new
    rapidxml::xml_attribute<>* pAttribute = pNode->first_attribute(attrName.c_str());
    if (pAttribute)
    {
        pAttribute->value(pRapidAllocatedAttributeValue);
    }
    else
    {
        // Allocate memory and copy to remember the attribute name
        char* pRapidAllocatedAttributeName = pNode->document()->allocate_string(attrName.c_str());
        // Create attribute and then append it
        pAttribute = pNode->document()->allocate_attribute(pRapidAllocatedAttributeName, pRapidAllocatedAttributeValue);
        pNode->append_attribute(pAttribute);
    }
}

//! @brief Reads a bool value from node attribute or return default value if attribute does not exist
bool readBoolAttribute(const rapidxml::xml_node<> *pNode, const std::string attrName, const bool defaultValue)
{
    if (pNode)
    {
        rapidxml::xml_attribute<> *pAttr = pNode->first_attribute(attrName.c_str());
        if (pAttr)
        {
            //Convert char* to string, assume null terminated strings
            std::string boolStr = std::string(pAttr->value());
            if ((boolStr == "true") || (boolStr == "True") || (boolStr == "1") )
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    return defaultValue;
}

//! @brief Check if node has attribute
bool hasAttribute(rapidxml::xml_node<> *pNode, std::string attrName)
{
    if (pNode)
    {
        if(pNode->first_attribute(attrName.c_str()))
        {
            return true;
        }
    }
    return false;
}

//! @brief Reads a string value from node or return default value if node = 0
std::string readStringNodeValue(rapidxml::xml_node<> *pNode, const std::string defaultValue)
{
    if (pNode)
    {
        return std::string(pNode->value());
    }
    else
    {
        return defaultValue;
    }
}

//! @brief Reads a int value from node or return default value if node = 0
int readIntNodeValue(rapidxml::xml_node<> *pNode, const int defaultValue)
{
    if (pNode)
    {
        return atoi(pNode->value());
    }
    else
    {
        return defaultValue;
    }
}

//! @brief Reads a double value from node or return default value if node = 0
double readDoubleNodeValue(rapidxml::xml_node<> *pNode, const double defaultValue)
{
    if (pNode)
    {
        return atof(pNode->value());
    }
    else
    {
        return defaultValue;
    }
}

//! @brief Help function to get the first grandchild node, in one go
//! @param [in] pNode The grandparent node
//! @param [in] child The name of the child node
//! @param [in] grandChild The name of the child nodes child node that you want
//! @returns A pointer to the node or 0 if child or grandChild was not found
rapidxml::xml_node<> *getGrandChild(rapidxml::xml_node<> *pNode, const std::string child, const std::string grandChild)
{
    rapidxml::xml_node<> *pParent = pNode->first_node(child.c_str());
    if (pParent)
    {
        return pParent->first_node(grandChild.c_str());
    }
    return 0;
}

rapidxml::xml_node<> *appendEmptyNode(rapidxml::xml_node<> *pParentNode, const std::string &rName)
{
    char *pRapidAllocatedNodeName = pParentNode->document()->allocate_string(rName.c_str());
    rapidxml::xml_node<> *pNewNode = pParentNode->document()->allocate_node(rapidxml::node_element, pRapidAllocatedNodeName);
    pParentNode->append_node(pNewNode);
    return pNewNode;
}

rapidxml::xml_node<> *appendValueNode(rapidxml::xml_node<> *pParentNode, const std::string &rName, const std::string &rValue)
{
    char *pRapidAllocatedNodeName = pParentNode->document()->allocate_string(rName.c_str());
    char *pRapidAllocatedNodeValue = pParentNode->document()->allocate_string(rValue.c_str());
    rapidxml::xml_node<> *pNewNode = pParentNode->document()->allocate_node(rapidxml::node_element, pRapidAllocatedNodeName, pRapidAllocatedNodeValue);
    pParentNode->append_node(pNewNode);
    return pNewNode;
}

void addXmlDeclaration(rapidxml::xml_document<> *pDoc)
{
    rapidxml::xml_node<> *pDecl = pDoc->allocate_node(rapidxml::node_declaration);
    pDecl->append_attribute(pDoc->allocate_attribute("version", "1.0"));
    pDecl->append_attribute(pDoc->allocate_attribute("encoding", "UTF-8"));
    pDoc->append_node(pDecl);
}

#endif
