#ifndef NLEAP_TYPE_DATABASE_HPP
#define NLEAP_TYPE_DATABASE_HPP
// =============================================================================
// nLEaP - A molecular manipulation program and coding environment
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
#include <types/String.hpp>
#include <types/Number.hpp>
#include <types/Unit.hpp>
#include <types/Residue.hpp>
#include <types/Atom.hpp>
#include <types/AmberFF.hpp>
#include <types/AtomTypes.hpp>
#include <types/PDBAtomMap.hpp>
#include <types/PDBResMap.hpp>

namespace nleap {
// -----------------------------------------------------------------------------
/*

 database
   |->objects   (top-level objects)
   |->variables (variables)
   |->histcmds  (list of commands used)

*/
// -----------------------------------------------------------------------------


using namespace std;
using namespace boost;

class CDatabase;
class CContext;
typedef shared_ptr< CDatabase >        CDatabasePtr;

// -----------------------------------------------------------------------------

/// CDatabase is a database type
class NLEAP_PACKAGE CDatabase : public CEntity {
public:

    CDatabase(void);
    CDatabase(int& top_id);

    //! describe subobject
    virtual void DescSubObj(const string& oname,ostream& ofs);

    //! get subobject
    virtual CEntityPtr GetSubObj(const string& oname);

// objects ---------------------------------------------------------------------
    /// create string
    CStringPtr CreateString(int& top_id, const string& value);

    /// create number
    CNumberPtr CreateNumber(int& top_id, const string& value);

    /// create unit
    CUnitPtr CreateUnit(int& top_id, const string& name = string());

    /// create residue
    CResiduePtr CreateResidue(int& top_id, const string& name);

    /// create atom
    CAtomPtr CreateAtom(int& top_id, const string& name);

    /// create amberfft
    CAmberFFPtr CreateAmberFF(int& top_id);

// objects ---------------------------------------------------------------------
    /// get atom types
    CAtomTypesPtr GetAtomTypes(void);

    /// get PDB atom map
    CPDBAtomMapPtr GetPDBAtomMap(void);

    /// get PDB residue map
    CPDBResMapPtr GetPDBResMap(void);

// variables -------------------------------------------------------------------
    /// beginning of variables
    CForwardIterator   BeginVariables(void);

    /// end of variables
    CForwardIterator   EndVariables(void);

    /// does variable exists in the database?
    bool IsVariable(const string& name);

    /// release variable
    void ReleaseVariable(const string& name);

    /// set variable
    void SetVariable(int& top_id, const string& name, CEntityPtr object);

    /// get variable object
    CEntityPtr GetVariableObject(const string& name);

// executive methods -----------------------------------------------------------
    /// clear the entire database
    void ClearDatabase(void);

};
// -----------------------------------------------------------------------------
}

#endif

