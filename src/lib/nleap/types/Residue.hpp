#ifndef NLEAP_TYPE_RESIDUE_HPP
#define NLEAP_TYPE_RESIDUE_HPP
// =============================================================================
// nLEaP - prepare input for the AMBER molecular mechanics programs
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
#include <types/Atom.hpp>

namespace nleap {

//------------------------------------------------------------------------------

class CUnit;
typedef shared_ptr< CUnit > CUnitPtr;

//------------------------------------------------------------------------------

/// CResidue is a residue type
class NLEAP_PACKAGE CResidue : public CEntity {
public:

    CResidue(void);
    CResidue(int& top_id);

// -------------------------------------------------------------------------

    //! describe subobject
    virtual void DescSubObj(const string& oname,ostream& ofs);

    /// get subobj
    virtual CEntityPtr GetSubObj(const string& oname);

// -------------------------------------------------------------------------

    /// describe residue
    virtual void Desc(ostream& ofs);

    /// create new atom
    CAtomPtr CreateAtom(const string& name, int& top_id);

    /// add atom into residue
    void AddAtom(CAtomPtr atom);

    /// remove atom from the residue
    void RemoveAtom(CAtomPtr atom);

    /// get unit
    CUnitPtr GetUnit(void);

    /// get residue type description
    string GetResidueType(void);

    /// find atom
    CAtomPtr FindAtom(int lid);
};

//------------------------------------------------------------------------------

typedef shared_ptr< CResidue > CResiduePtr;

//------------------------------------------------------------------------------
}

#endif
