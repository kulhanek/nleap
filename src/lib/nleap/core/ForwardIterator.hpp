#ifndef NLEAP_CORE_FORWARD_ITERATOR_H
#define NLEAP_CORE_FORWARD_ITERATOR_H
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
#include <core/Entity.hpp>

namespace nleap {
//------------------------------------------------------------------------------

using namespace std;
using namespace boost;

class   CKey;
class   CEntity;
typedef shared_ptr< CEntity >   CEntityPtr;

//------------------------------------------------------------------------------

//! CForwardIterator is forward entity iterator with possible object filtering
class NLEAP_PACKAGE CForwardIterator {
public:

    //! constructor
    CForwardIterator(void);

    //! constructor
    CForwardIterator(const CEntityPtr& container);

    //! set filter
    void SetFilter(const CKey& filter);

    //! set to begin - set filter first
    void SetToBegin(void);

    //! set to end
    void SetToEnd(void);

    //! access operator *
    CEntityPtr operator * (void) const;

    //! access operator ->
    CEntity* operator -> (void) const;

    //! postincrement operator
    void operator ++ (int);

    //! points to valid item
    operator bool(void) const;

    //! comparisons operator
    bool operator != (const CForwardIterator& left) const;
    bool operator == (const CForwardIterator& left) const;

// private data and methods ------------------------------------------------
private:
    CEntityPtr  m_root;
    CEntityPtr  m_item;
    CKey        m_filter;
};

//------------------------------------------------------------------------------
}

#endif

