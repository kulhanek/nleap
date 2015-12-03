#ifndef NLEAP_CORE_ENTITY_H
#define NLEAP_CORE_ENTITY_H
// =============================================================================
// nLEaP - A molecular manipulation program and coding environment
// -----------------------------------------------------------------------------
//     Copyright (C) 2011 Petr Kulhanek, kulhanek@chemi.muni.cz
//     Copyright (C) 2006 gLEaP authors, see AUTHORS file in the main directory
//
//     This program is free software; you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation; either version 2 of the License, or
//     (at your option) any later version.
//
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     You should have received a copy of the GNU General Public License along
//     with this program; if not, write to the Free Software Foundation, Inc.,
//     51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
// =============================================================================

#include <NLEaPMainHeader.hpp>
#include <map>
#include <list>
#include <string>
#include <iostream>
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <core/PropertyMap.hpp>

// -----------------------------------------------------------------------------

class   CXMLElement;

namespace nleap {
//------------------------------------------------------------------------------

using namespace std;
using namespace boost;

class   CKey;
class   CEntity;
class   CForwardIterator;
typedef shared_ptr< CEntity >   CEntityPtr;
typedef weak_ptr< CEntity >     CEntityWPtr;

//------------------------------------------------------------------------------

//! CEntity is a container of other entities and properties
class NLEAP_PACKAGE CEntity {
public:

    //! constructor
    CEntity(const CKey& type);

    //! constructor
    CEntity(const CKey& type, int& top_id);

    //! destructor
    virtual ~CEntity(void);

// object clonning  ------------------------------------------------------------

    //! clone
    CEntityPtr Clone(int& top_id, int base_id = 0);

    //! clone entity weakly
    CEntityPtr CloneWeakly(int& top_id, int base_id = 0);

    //! bind weakly cloned object properties
    void BindWeakProperties(map< int, CEntityPtr >& obj_map);

    //! update object map with children objects
    void UpdateObjectMap(map< int, CEntityPtr >& obj_map);

// object references  ----------------------------------------------------------

    //! get root object
    CEntityPtr  GetRoot(void);

    //! get self reference
    CEntityPtr  GetSelf(void);

    //! get this reference
    CEntity*    GetThis(void);

    //! get previous sibling entity
    CEntityPtr  GetPrev(void);

    //! get next sibling entity
    CEntityPtr  GetNext(void);

// object identification  ------------------------------------------------------

    //! return entity type
    const CKey& GetType(void) const;

    //! get entity name
    const string& GetName(void) const;

    //! get entity path name
    const string GetPathName(void) const;

    //! set entity name
    void SetName(const string& name);

    //! get entity ID
    int GetId(void) const;

    //! set entity ID
    void SetId(int id);

    //! describe entity
    virtual void Desc(ostream& ofs);

    //! describe subobject
    virtual void DescSubObj(const string& oname,ostream& ofs);

    //! get subobject
    virtual CEntityPtr GetSubObj(const string& oname);

// properties ------------------------------------------------------------------

    //! property setter method - int
    void Set(const CKey& parmid, const int& value);

    //! property setter method - double
    void Set(const CKey& parmid, const double& value);

    //! property setter method - string
    void Set(const CKey& parmid, const string& value);

    //! property setter method - CEntityPtr
    void Set(const CKey& parmid, const CEntityPtr& value);

    //! property getter method - int
    void Get(const CKey& parmid, int& value);

    //! property getter method - double
    void Get(const CKey& parmid, double& value);

    //! property getter method - string
    void Get(const CKey& parmid, string& value);

    //! property getter method - CEntityPtr
    void Get(const CKey& parmid, CEntityPtr& value);

    template <typename T1>
    inline const T1 Get(const CKey& parmid);

    //! remove object property
    void RemoveObjectProperty(CEntityPtr value);

// children objects ------------------------------------------------------------

    //! beginning of children objects
    CForwardIterator   BeginChildren(void);

    //! end of children objects
    CForwardIterator   EndChildren(void);

    //! add child
    void AddChild(CEntityPtr child);

    //! remove child
    void RemoveChild(CEntityPtr child);

    //! remove first child
    void RemoveFirstChild(void);

    //! remove last child
    void RemoveLastChild(void);

    //! remove all childern
    void RemoveAllChildren(void);

    //! find child object by name
    CEntityPtr FindChild(const string& name, bool recursive = false);

    //! find child object by id
    CEntityPtr FindChild(int id, bool recursive = false);

    //! is it child object?
    bool IsChild(CEntityPtr cobj, bool recursive = false);

    //! get number of children
    size_t NumberOfChildren(bool recursive = false );

    //! get child
    CEntityPtr GetChild(size_t index);

    //! get first child
    CEntityPtr GetFirstChild(void);

    //! get last child
    CEntityPtr GetLastChild(void);

// input/output methods --------------------------------------------------------
    //! load from XML
    virtual bool Load(CXMLElement* p_ele);

    //! save to XML
    virtual bool Save(CXMLElement* p_ele);

// private data and methods ----------------------------------------------------
protected:
    int                     m_id;           // unique object ID (-1 - invalid)
    CKey                    m_type;
    string                  m_name;
    CEntity*                m_root;         // root object
    CEntityPtr              m_first;        // first child entity
    CEntityPtr              m_sibling;      // sibling entity
    CEntity*                m_self;         // node owing this node (prev or root)
    CEntity*                m_last;         // last child entity
    CPropertyMap            m_properties;   // entity properties
    list< CEntityWPtr >     m_related;      // related objects

    void RemoveRelated(CEntityPtr value);
};

//--------------------------------------------------------------------------

template <typename T1>
inline const T1 CEntity::Get(const CKey& parmid)
{
    T1 value;
    Get(parmid,value);
    return(value);
}

//--------------------------------------------------------------------------
}

#endif

