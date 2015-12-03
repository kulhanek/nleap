#ifndef NLEAP_TYPE_ATOM_TYPES_HPP
#define NLEAP_TYPE_ATOM_TYPES_HPP
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
#include <types/List.hpp>
#include <VerboseStr.hpp>

namespace nleap {
//------------------------------------------------------------------------------

class CContext;

//------------------------------------------------------------------------------

//! CAtomTypes list of registered atom types
class NLEAP_PACKAGE CAtomTypes : public CEntity
{
public:
// constructor/destructor ------------------------------------------------------
    CAtomTypes(void);
    CAtomTypes(int& top_id);

// information methods ---------------------------------------------------------
    //! describe string
    virtual void Desc( ostream& ofs );

// executive methods -----------------------------------------------------------
    //! add atom types
    void AddAtomTypes( int& top_id, CListPtr& types, CVerboseStr& vout );

// executive methods -----------------------------------------------------------
    //! get atom type mass
    static double GetMass( CContext* p_ctx, const string& type );
};

//------------------------------------------------------------------------------

typedef shared_ptr< CAtomTypes > CAtomTypesPtr;

//------------------------------------------------------------------------------
}

#endif
