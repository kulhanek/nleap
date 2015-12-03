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

#include <core/RecursiveIterator.hpp>
#include <core/PredefinedKeys.hpp>

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CRecursiveIterator::CRecursiveIterator(void)
    : m_filter(ANY)
{
}

// -------------------------------------------------------------------------

CRecursiveIterator::CRecursiveIterator(const CEntityPtr& container)
    : m_filter(ANY)
{
    m_root = container;
}

// -------------------------------------------------------------------------

void CRecursiveIterator::SetFilter(const CKey& filter)
{
    m_filter = filter;
}

// -------------------------------------------------------------------------

void CRecursiveIterator::SetToBegin(void)
{
    m_ite_stack.clear();

    // init top-level iterator
    CForwardIterator it(m_root);
    it.SetToBegin();

    // add top level iterator on the stack
    m_ite_stack.push_back(it);

    // go down as possible
    GoDownAsPossible( );

    // get item
    m_item = (*m_ite_stack.back());

    if( m_filter == ANY ) return;

    // find the first element satysfying the filter condition
    while( m_item ){
        if( m_item->GetType() == m_filter ) return;
        MoveNext();
    }
}

// -------------------------------------------------------------------------

void CRecursiveIterator::SetToEnd(void)
{
    m_item = CEntityPtr();
}

// -------------------------------------------------------------------------

CEntityPtr CRecursiveIterator::operator * (void) const
{
    return( m_item );
}

// -------------------------------------------------------------------------

CEntity* CRecursiveIterator::operator -> (void) const
{
    CEntity* p_ptr = m_item.get();
    assert( p_ptr != NULL );
    return( p_ptr );
}

// -------------------------------------------------------------------------

void CRecursiveIterator::operator ++ (int)
{
    MoveNext();
    if( m_filter == ANY ) return;

    while( m_item ){
        if( m_item->GetType() == m_filter ) return;
        MoveNext();
    }
}

// -------------------------------------------------------------------------

CRecursiveIterator::operator bool(void) const
{
    return( m_item );
}

// -------------------------------------------------------------------------

bool CRecursiveIterator::operator != (const CRecursiveIterator& left) const
{
    return( m_item != left.m_item );
}

// -------------------------------------------------------------------------

bool CRecursiveIterator::operator == (const CRecursiveIterator& left) const
{
    return( m_item == left.m_item );
}

// -------------------------------------------------------------------------

void CRecursiveIterator::GoDownAsPossible(void)
{
    if( m_ite_stack.size() == 0 ) return;

    CForwardIterator ite = m_ite_stack.back();
    if( (! (*ite)) || (! (*ite)->GetFirstChild() ) ){
        return;
    }

    CForwardIterator nit( *ite );
    nit.SetToBegin();
    m_ite_stack.push_back(nit);

    GoDownAsPossible();
}

// -------------------------------------------------------------------------

void CRecursiveIterator::GoUp(void)
{
    m_ite_stack.pop_back();
}

// -------------------------------------------------------------------------

void CRecursiveIterator::MoveNext(void)
{
    // -> X -> Y -> Z - Z1
    //         |    | - Z2
    //         |    | - Z3
    //         |
    //         | -> V -> V1

    // sequence Z1, Z2, Z3, Z, Y, V1, V, X

    if( m_ite_stack.size() == 0 ) return;

    CForwardIterator ite = m_ite_stack.back();
    if( ! *ite ){
        // end of list
        m_item = CEntityPtr();
        return;
    }

    // does have children?
    if( (*ite)->GetFirstChild() ){
        GoDownAsPossible();
    } else {
        // move iterator
        CForwardIterator nte = m_ite_stack.back();
        nte++;
        m_ite_stack.pop_back();
        m_ite_stack.push_back(nte);

        if( ! *nte ){
            // end of current level
            if( m_ite_stack.size() == 1 ){
                // end of list
                m_item = CEntityPtr();
                return;
            }
            // - go up
            GoUp();

            // get new item and move
            CForwardIterator nte = m_ite_stack.back();
            m_item = *nte;
            nte++;
            m_ite_stack.pop_back();
            m_ite_stack.push_back(nte);
            return;
        }
    }

    CForwardIterator ute = m_ite_stack.back();
    m_item = *ute;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}


