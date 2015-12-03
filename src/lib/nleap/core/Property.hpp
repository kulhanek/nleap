#ifndef NLEAP_CORE_PROPERTY_H
#define NLEAP_CORE_PROPERTY_H
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
#include <ostream>

namespace nleap {
//------------------------------------------------------------------------------

using namespace std;
using namespace boost;

class   CEntity;
typedef shared_ptr< CEntity >   CEntityPtr;

//--------------------------------------------------------------------------

//! CProperty is a simple variant object
class NLEAP_PACKAGE CProperty {
public:
    //! constructor
    CProperty(void);

    //! destructor
    ~CProperty(void);

    //! clone weakly
    void CloneWeakly(const CProperty& src,int base_id);

    //! bind weak objects
    void BindWeak(map< int, CEntityPtr >& obj_map);

// ---------------------------------------------------------------------
    //! get property key
    const CKey& GetKey(void) const;

    //! get property type
    const CKey& GetType(void) const;

// ---------------------------------------------------------------------
    //! property setter method - int
    void Set(const int& value);

    //! property setter method - int
    void SetId(const int& value);

    //! property setter method - double
    void Set(const double& value);

    //! property setter method - string
    void Set(const string& value);

    //! property setter method - CEntityPtr
    void Set(const CEntityPtr& value);

// ---------------------------------------------------------------------
    //! property getter method - int
    void Get(int& value) const;

    //! property getter method - int
    void GetId(int& value) const;

    //! property getter method - double
    void Get(double& value) const;

    //! property getter method - string
    void Get(string& value) const;

    //! property getter method - CEntityPtr
    void Get(CEntityPtr& value) const;

// private data and methods --------------------------------------------
private:
    union UProperty {
        int     m_i_value;
        double  m_d_value;
        void*   m_x_value;
    };

    CKey        m_key;
    CKey        m_type;
    UProperty   m_value;

    //! property setter method - CProperty
    void Set(CProperty* value);

    //! property getter method - CProperty
    void Get(CProperty* &value) const;

    // alocate special objects
    void Allocate(void);
    void Deallocate(void);

    friend class CPropertyMap;
};

//--------------------------------------------------------------------------

//! describe property
ostream& operator << ( ostream& ofs, const CProperty& obj );

//--------------------------------------------------------------------------

}

#endif

