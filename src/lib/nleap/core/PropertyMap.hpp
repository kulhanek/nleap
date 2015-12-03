#ifndef NLEAP_CORE_PROPERTY_MAP_H
#define NLEAP_CORE_PROPERTY_MAP_H
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
#include <boost/shared_ptr.hpp>
#include <core/Key.hpp>

namespace nleap {
//------------------------------------------------------------------------------

using namespace std;
using namespace boost;

class   CEntity;
class   CProperty;
typedef shared_ptr< CEntity >          CEntityPtr;

//------------------------------------------------------------------------------

//! CPropertyMap is a dynamic container of entity properties
class NLEAP_PACKAGE CPropertyMap {
public:

    //! constructor
    CPropertyMap(void);

    //! constructor
    CPropertyMap(const CPropertyMap& src);

    //! destructor
    ~CPropertyMap(void);

// -----------------------------------------------------------------------------
    //! set initial block size
    void SetInitialBlockSize(size_t block_size);

    //! clone weakly
    void CloneWeakly(const CPropertyMap& src, int base_id);

    //! bind weakly cloned object properties
    void BindWeak(map< int, CEntityPtr >&  obj_map);

// -----------------------------------------------------------------------------

    //! return number of all properties
    size_t  NumberOfProperties(void) const;

    //! return number of object properties
    size_t  NumberOfObjectProperties(void) const;

    //! describe
    void Desc(ostream& ofs);

    //! unset object property
    void UnsetObjectProperty(const CEntityPtr& obj);

// -----------------------------------------------------------------------------
    //! property setter method - int
    void Set(const CKey& parmid, const int& value);

    //! property setter method - double
    void Set(const CKey& parmid, const double& value);

    //! property setter method - string
    void Set(const CKey& parmid, const string& value);

    //! property setter method - CEntityPtr
    void Set(const CKey& parmid, const CEntityPtr& value);

// -----------------------------------------------------------------------------
    //! property getter method - int
    void Get(const CKey& parmid, int& value);

    //! property getter method - double
    void Get(const CKey& parmid, double& value);

    //! property getter method - string
    void Get(const CKey& parmid, string& value);

    //! property getter method - CEntityPtr
    void Get(const CKey& parmid, CEntityPtr& value);

// private data and methods ----------------------------------------------------
private:
    CProperty*      m_first_block;
    static size_t   m_default_size;

    CProperty*      AllocateBlock(size_t size);
    CProperty&      GetNode(size_t index) const;
};

//------------------------------------------------------------------------------
}

#endif

