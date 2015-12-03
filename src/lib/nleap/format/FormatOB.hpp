#ifndef NLEAP_FORMAT_OB_HPP
#define NLEAP_FORMAT_OB_HPP
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
#include <iosfwd>
#include <types/Unit.hpp>
#include <VerboseStr.hpp>

namespace nleap {
//------------------------------------------------------------------------------

using namespace std;

//------------------------------------------------------------------------------

class NLEAP_PACKAGE CFormatOB {
public:

    CFormatOB( CVerboseStr& debug );

    /// read mol2 file
    void Read( istream& is, CUnitPtr& unit, int& top_id, const string& format = "auto" );

    /// write mol2 file
    void Write( ostream& os, CUnitPtr& unit, const string& format = "auto" );

// private section -------------------------------------------------------------
private:
    CVerboseStr&            m_debug;
    int                     m_line_no;
    string                  m_line;

    int                     m_atoms;
    int                     m_bonds;
    int                     m_residues;
    string                  m_moltype;
    string                  m_chgtype;
    string                  m_status;
    string                  m_comment;

    map< int, CAtomPtr >    m_atom_map;

    void ReadHead( istream& is, CUnitPtr& unit );
    void ReadAtoms( istream& is, CUnitPtr& unit, int& top_id );
    void ReadBonds( istream& is, CUnitPtr& unit, int& top_id );

    void WriteHead( ostream& os, CUnitPtr& unit );
    void WriteAtoms( ostream& os, CUnitPtr& unit );
    void WriteBonds( ostream& os, CUnitPtr& unit );

    void rw_error( const string& reason );
};

//------------------------------------------------------------------------------
}

#endif
