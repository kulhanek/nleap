#ifndef NLEAP_TYPE_UNIT_HPP
#define NLEAP_TYPE_UNIT_HPP
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
#include <types/Residue.hpp>
#include <types/Atom.hpp>
#include <types/Bond.hpp>
#include <core/ForwardIterator.hpp>
#include <core/RecursiveIterator.hpp>

namespace nleap {
//------------------------------------------------------------------------------

/// CUnit is a unit type
class NLEAP_PACKAGE CUnit : public CEntity
{
public:

    CUnit(void);
    CUnit(int& top_id);

// -------------------------------------------------------------------------

    //! describe subobject
    virtual void DescSubObj(const string& oname,ostream& ofs);

    /// get subobj
    virtual CEntityPtr GetSubObj(const string& oname);

// -------------------------------------------------------------------------

    /// \brief describe unit
    virtual void Desc(ostream& ofs);

    /// describe unit header
    void DescHeader(ostream& ofs);

    /// describe all residues
    void DescAllResidues(ostream& ofs);

    /// describe all atoms
    void DescAllAtoms(ostream& ofs);

// -------------------------------------------------------------------------

    /// get number of atoms
    int NumberOfAtoms(void);

    /// beginning of atoms
    CRecursiveIterator   BeginAtoms(void);

    /// end of atoms
    CRecursiveIterator   EndAtoms(void);

    /// find atom
    CAtomPtr FindAtom(int sid);

// -------------------------------------------------------------------------

    /// create new residue
    CResiduePtr CreateResidue(const string& name, int& top_id);

    /// add residue
    void AddResidue(CResiduePtr residue);

    /// remove residue
    void RemoveResidue(CResiduePtr residue);

    /// get number of residues
    int NumberOfResidues(void);

    /// beginning of residues objects
    CForwardIterator   BeginResidues(void);

    /// end of residues objects
    CForwardIterator   EndResidues(void);

    /// find residue
    CResiduePtr FindResidue(const string& name);

    /// find residue
    CResiduePtr FindResidue(int rid);

    /// has residue owned by this unit?
    bool HasResidue(CResiduePtr cres);

// -------------------------------------------------------------------------

    /// create bond between two atoms
    CBondPtr    CreateBond(const CAtomPtr& at1, const CAtomPtr& at2, int order, int& top_id);

    /// are atoms bonded?
    bool AreBonded(const CAtomPtr& at1, const CAtomPtr& at2);

    /// remove bond
    void RemoveBond(const CAtomPtr& at1, const CAtomPtr& at2);

    /// remove bonds containing atom
    void RemoveBonds(const CAtomPtr& at1);

    /// get number of bonds
    int NumberOfBonds(void);

    /// beginning of bonds objects
    CForwardIterator   BeginBonds(void);

    /// end of bonds objects
    CForwardIterator   EndBonds(void);

// -------------------------------------------------------------------------

    /// fix counters and numbering
    void FixCounters(void);

// -------------------------------------------------------------------------
private:
    int m_atoms;
    int m_bonds;
    int m_residues;
};

//------------------------------------------------------------------------------

typedef shared_ptr< CUnit > CUnitPtr;

//------------------------------------------------------------------------------
}

#endif
