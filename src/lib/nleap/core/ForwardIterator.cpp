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

#include <core/ForwardIterator.hpp>
#include <core/PredefinedKeys.hpp>

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CForwardIterator::CForwardIterator(void)
    : m_filter(ANY)
{

}

// -------------------------------------------------------------------------

CForwardIterator::CForwardIterator(const CEntityPtr& container)
    : m_filter(ANY)
{
    m_root = container;
}

// -------------------------------------------------------------------------

void CForwardIterator::SetFilter(const CKey& filter)
{
    m_filter = filter;
}

// -------------------------------------------------------------------------

void CForwardIterator::SetToBegin(void)
{
    if( m_root ){
        m_item = m_root->GetFirstChild();
    }

    if( m_filter == ANY ) return;

    // find first item satisfying the condition
    while( m_item ){
        if( m_item->GetType() == m_filter ) return;
        m_item = m_item->GetNext();
    }
}

// -------------------------------------------------------------------------

void CForwardIterator::SetToEnd(void)
{
    m_item = CEntityPtr();
}

// -------------------------------------------------------------------------

CEntityPtr CForwardIterator::operator * (void) const
{
    return( m_item );
}

// -------------------------------------------------------------------------

CEntity* CForwardIterator::operator -> (void) const
{
    CEntity* p_ptr = m_item.get();
    assert( p_ptr != NULL );
    return( p_ptr );
}

// -------------------------------------------------------------------------

void CForwardIterator::operator ++ (int)
{
    if( ! m_item ) return;
    m_item = m_item->GetNext( );

    if( m_filter == ANY ) return;

    // find next item satisfying the condition
    while( m_item ){
        if( m_item->GetType() == m_filter ) return;
        m_item = m_item->GetNext( );
    }
}

// -------------------------------------------------------------------------

CForwardIterator::operator bool(void) const
{
    return( m_item );
}

// -------------------------------------------------------------------------

bool CForwardIterator::operator != (const CForwardIterator& left) const
{
    return( m_item != left.m_item );
}

// -------------------------------------------------------------------------

bool CForwardIterator::operator == (const CForwardIterator& left) const
{
    return( m_item == left.m_item );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}


