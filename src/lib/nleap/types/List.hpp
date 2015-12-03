#ifndef NLEAP_TYPE_LIST_HPP
#define NLEAP_TYPE_LIST_HPP
// =============================================================================
// nLEaP - prepare input for the AMBER molecular mechanics programs
// -----------------------------------------------------------------------------
//     Copyright (C) 2010 Petr Kulhanek, kulhanek@chemi.muni.cz
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

/// CList is a list type
class NLEAP_PACKAGE CList : public CEntity {
public:
// constructor/destructor ------------------------------------------------------
    CList( void );
    CList(int& top_id);

// information methods ---------------------------------------------------------
    //! brief describe string
    virtual void Desc( ostream& ofs );
};

//------------------------------------------------------------------------------

typedef shared_ptr< CList > CListPtr;

//------------------------------------------------------------------------------
}

#endif
