#ifndef NLEAP_TYPE_ATOM_HPP
#define NLEAP_TYPE_ATOM_HPP
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
#include <Point.hpp>

namespace nleap {
//------------------------------------------------------------------------------

class CUnit;
class CResidue;

typedef shared_ptr< CUnit > CUnitPtr;
typedef shared_ptr< CResidue > CResiduePtr;

//------------------------------------------------------------------------------

/// CAtom is a atom type
class NLEAP_PACKAGE CAtom : public CEntity {
public:

    CAtom(void);
    CAtom(int& top_id);

    /// \brief describe atom
    virtual void Desc(ostream& ofs);

    /// get unit
    CUnitPtr GetUnit(void);

    /// get residue
    CResiduePtr GetResidue(void);

    /// get residue id
    int GetResidueId(void);

    /// get residue name
    string GetResidueName(void);

    /// return atom position
    const CPoint  GetPos(void);
};

//------------------------------------------------------------------------------

typedef shared_ptr< CAtom > CAtomPtr;

//------------------------------------------------------------------------------
}

#endif
