#ifndef NLEAP_TYPE_FACTORY_HPP
#define NLEAP_TYPE_FACTORY_HPP
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
#include <types/String.hpp>
#include <types/Number.hpp>
#include <types/Unit.hpp>
#include <types/Residue.hpp>
#include <types/Database.hpp>
#include <types/Variable.hpp>
#include <types/Atom.hpp>
#include <types/Bond.hpp>
#include <types/List.hpp>
#include <types/AtomTypes.hpp>

namespace nleap {

//------------------------------------------------------------------------------

/// object factory
class NLEAP_PACKAGE CFactory {
public:
    /// determine type
    static CKey DetermineType(const string& value);

    /// is integer number
    static bool IsInteger(const string& value);

    /// get integer number
    static int GetIntegerNumber(const string& value);

    /// is number
    static bool IsNumber(const string& value);

    /// get number
    static double GetNumber(const string& value);

    /// is valid variable name
    static bool IsValidVariableName(const string& value);

    // objects ---------------------------------------------------------------------

    /// create object by type
    static CEntityPtr Create(const CKey& type);

    /// create database
    static CDatabasePtr CreateDatabase(int& top_id);

    /// create variable
    static CVariablePtr CreateVariable(int& top_id, const string& name);

    /// create node
    static CEntityPtr CreateNode(int& top_id, const string& name);

    /// create node
    static CEntityPtr CreateNode(int& top_id);

    /// create amberff
    static CAmberFFPtr CreateAmberFF(int& top_id);

    /// create string - str has to be a valid string
    static CStringPtr CreateString(int& top_id, const string& str);

    /// create number - num has to be a valid number
    static CNumberPtr CreateNumber(int& top_id, const string& num);

    /// create unit
    static CUnitPtr CreateUnit(int& top_id);

    /// create residue
    static CResiduePtr CreateResidue(int& top_id);

    /// create atom
    static CAtomPtr CreateAtom(int& top_id);

    /// create bond
    static CBondPtr CreateBond(int& top_id);

    /// create empty list
    static CListPtr CreateList(int& top_id);

    /// create list
    static CListPtr CreateList(int& top_id, const string& list_src);

    /// create atom map
    static CAtomTypesPtr CreateAtomTypes(int& top_id);

    /// create atom map
    static CPDBAtomMapPtr CreatePDBAtomMap(int& top_id);

    /// create res map
    static CPDBResMapPtr CreatePDBResMap(int& top_id);
};

//------------------------------------------------------------------------------
}

#endif
