#ifndef NLEAP_CORE_PREDEFINED_KEY_HPP
#define NLEAP_CORE_PREDEFINED_KEY_HPP
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
#include <core/Key.hpp>

namespace nleap {
//------------------------------------------------------------------------------

//! property types
DECLARE_KEY(NULL_PROP);
DECLARE_KEY(NEXT_PROP);
DECLARE_KEY(IPTR_PROP);
DECLARE_KEY(PTR__PROP);
DECLARE_KEY(INT__PROP);
DECLARE_KEY(DBL__PROP);
DECLARE_KEY(STR__PROP);

//! types
DECLARE_KEY(UNKNOWN);
DECLARE_KEY(ANY);            // any type - used by iterators for anonymous iterating
DECLARE_KEY(NODE);
DECLARE_KEY(CONTEXT);
DECLARE_KEY(DATABASE);
DECLARE_KEY(VARIABLE);
DECLARE_KEY(STRING);
DECLARE_KEY(NUMBER);
DECLARE_KEY(LIST);
DECLARE_KEY(UNIT);
DECLARE_KEY(RESIDUE);
DECLARE_KEY(ATOM);
DECLARE_KEY(BOND);
DECLARE_KEY(AMBERFF);
DECLARE_KEY(ATOMTYPES);
DECLARE_KEY(PDBATOMMAP);
DECLARE_KEY(PDBRESMAP);

//! properties
DECLARE_KEY(VALUE);          // value (used by variables)
DECLARE_KEY(PATH);           // context PATH value
DECLARE_KEY(ECHO);           // context ECHO value
DECLARE_KEY(VERBOSITY);      // context verbosity
DECLARE_KEY(MAXHIST);        // context maximum of changes recording

DECLARE_KEY(ATOM1);          // atom1 type (used by amberff)
DECLARE_KEY(ATOM2);
DECLARE_KEY(ATOM3);
DECLARE_KEY(ATOM4);

DECLARE_KEY(TITLE);          // object title
DECLARE_KEY(TITLE2);         // secondary title
DECLARE_KEY(RID);            // residue ID (atom and residue)
DECLARE_KEY(SID);            // sequence ID
DECLARE_KEY(LID);            // local ID (atoms in residues)
DECLARE_KEY(MASS);           // atom mass
DECLARE_KEY(POLAR);          // atom polarizability
DECLARE_KEY(DEPTH);          // vdw energy
DECLARE_KEY(RSTAR);          // vdw radii
DECLARE_KEY(EQUIL);
DECLARE_KEY(FORCE);
DECLARE_KEY(DIVIDE);
DECLARE_KEY(PERIOD);
DECLARE_KEY(CHARGE);         // charge
DECLARE_KEY(POSX);           // cartesian position
DECLARE_KEY(POSY);
DECLARE_KEY(POSZ);
DECLARE_KEY(TYPE);           // atom type
DECLARE_KEY(ORDER);          // bond order
DECLARE_KEY(ELEMENT);
DECLARE_KEY(HYBRIDIZATION);
DECLARE_KEY(TERM);

DECLARE_KEY(HEAD);           // unit head atom
DECLARE_KEY(TAIL);           // unit tail atom

//------------------------------------------------------------------------------
}

#endif


