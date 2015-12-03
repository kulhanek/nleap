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

#include <core/Property.hpp>
#include <core/PredefinedKeys.hpp>
#include <core/Entity.hpp>

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CProperty::CProperty( )
    : m_key( NULL_PROP ), m_type( NULL_PROP )
{
}

// -------------------------------------------------------------------------

CProperty::~CProperty( )
{
    Deallocate();
}

// -------------------------------------------------------------------------

void CProperty::CloneWeakly(const CProperty& src, int base_id)
{
    m_key = src.m_key;
    if( src.m_type == INT__PROP ){
        int value;
        src.Get(value);
        Set(value);
    } else if ( src.m_type == DBL__PROP ) {
        double value;
        src.Get(value);
        Set(value);
    } else if ( src.m_type == STR__PROP ) {
        string value;
        src.Get(value);
        Set(value);
    } else if ( src.m_type == PTR__PROP ) {
        CEntityPtr value;
        int        id = -1;
        src.Get(value);
        if( value ){
            id = value->GetId() + base_id;
        }
        SetId(id); // copy weakly
    }
}

// ---------------------------------------------------------------------

void CProperty::BindWeak(map< int, CEntityPtr >&  obj_map)
{
    if( m_type != IPTR_PROP ) return;
    int id;
    GetId(id);
    CEntityPtr obj = obj_map[id];
    Set(obj);
}

// ---------------------------------------------------------------------

const CKey& CProperty::GetKey(void) const
{
    return( m_key );
}

// ---------------------------------------------------------------------

const CKey& CProperty::GetType(void) const
{
    return( m_type );
}

// ---------------------------------------------------------------------

void CProperty::Set(const int& value)
{
    if( (m_type == STR__PROP) || (m_type == PTR__PROP) ) Deallocate();
    m_type = INT__PROP;
    m_value.m_i_value = value;
}

// ---------------------------------------------------------------------

void CProperty::SetId(const int& value)
{
    if( (m_type == STR__PROP) || (m_type == PTR__PROP) ) Deallocate();
    m_type = IPTR_PROP;
    m_value.m_i_value = value;
}

// ---------------------------------------------------------------------

void CProperty::Set(const double& value)
{
    if( (m_type == STR__PROP) || (m_type == PTR__PROP) ) Deallocate();
    m_type = DBL__PROP;
    m_value.m_d_value = value;
}

// ---------------------------------------------------------------------

void CProperty::Set(const string& value)
{
    if( m_type == PTR__PROP ) Deallocate();
    if( m_type != STR__PROP ) {
        m_type = STR__PROP;
        Allocate();
    }
    *((string*)m_value.m_x_value) = value;
}

// ---------------------------------------------------------------------

void CProperty::Set(const CEntityPtr& value)
{
    if( m_type == STR__PROP ) Deallocate();
    if( m_type != PTR__PROP ) {
        m_type = PTR__PROP;
        Allocate();
    }
    *((CEntityPtr*)m_value.m_x_value) = value;
}

// ---------------------------------------------------------------------

void CProperty::Set(CProperty* value)
{
    if( (m_type == STR__PROP) || (m_type == PTR__PROP) ) Deallocate();
    m_type = NEXT_PROP;
    m_value.m_x_value = value;
}

// ---------------------------------------------------------------------

void CProperty::Get(int& value) const
{
    if( m_type != INT__PROP ){
        value = 0;
        return;
    }
    value = m_value.m_i_value;
}

// ---------------------------------------------------------------------

void CProperty::GetId(int& value) const
{
    if( m_type != IPTR_PROP ){
        value = 0;
        return;
    }
    value = m_value.m_i_value;
}

// ---------------------------------------------------------------------

void CProperty::Get(double& value) const
{
    if( m_type != DBL__PROP ){
        value = 0;
        return;
    }
    value = m_value.m_d_value;
}

// ---------------------------------------------------------------------

void CProperty::Get(string& value) const
{
    if( m_type != STR__PROP ){
        value = string();
        return;
    }
    value = *((string*)m_value.m_x_value);
}

// ---------------------------------------------------------------------

void CProperty::Get(CEntityPtr& value) const
{
    if( m_type != PTR__PROP ){
        value = CEntityPtr();
        return;
    }
    value = *((CEntityPtr*)m_value.m_x_value);
}

// ---------------------------------------------------------------------

void CProperty::Get(CProperty* &value) const
{
    if( m_type != NEXT_PROP ){
        value = NULL;
        return;
    }
    value = (CProperty*)m_value.m_x_value;
}

// -------------------------------------------------------------------------

void CProperty::Allocate(void)
{
    if( m_type == STR__PROP ){
        m_value.m_x_value = new string;
        return;
    }
    if( m_type == PTR__PROP ){
        m_value.m_x_value = new CEntityPtr;
        return;
    }
}

// -------------------------------------------------------------------------

void CProperty::Deallocate(void)
{
    if( m_type == STR__PROP ){
        delete ((string*)m_value.m_x_value);
        return;
    }
    if( m_type == PTR__PROP ){
        delete ((CEntityPtr*)m_value.m_x_value);
        return;
    }
}

// -------------------------------------------------------------------------

ostream& operator << ( ostream& ofs, const CProperty& obj )
{
    if( obj.GetType() == INT__PROP ){
        int value;
        obj.Get(value);
        ofs << value;
    } else if ( obj.GetType() == DBL__PROP ) {
        double value;
        obj.Get(value);
        ofs << value;
    } else if ( obj.GetType() == STR__PROP ) {
        string value;
        obj.Get(value);
        ofs << value;
    } else if ( obj.GetType() == PTR__PROP ) {
        ofs << "OBJECT";
    }
    return( ofs );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}


