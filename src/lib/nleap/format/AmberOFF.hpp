#ifndef NLEAP_FORMAT_OFF_HPP
#define NLEAP_FORMAT_OFF_HPP
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
#include <types/Database.hpp>
#include <VerboseStr.hpp>
#include <vector>

namespace nleap {
//------------------------------------------------------------------------------

using namespace std;

//------------------------------------------------------------------------------

/// OFF (Object File Format) reader
/// only limited functionality is provided
class NLEAP_PACKAGE CAmberOFF {
public:

    CAmberOFF( CVerboseStr& debug );

    /// read off file
    void Read( istream& is, CDatabasePtr& db, int& top_id );

// private section -------------------------------------------------------------
private:
    CVerboseStr&                m_debug;
    int                         m_line_no;
    string                      m_line;

    // units
    std::vector< string >       m_index;    // index of units
    int                         m_unit_index;
    CUnitPtr                    m_unit;     // current processed unit
    std::vector< CAtomPtr >     m_atom_map;

    void ReadIndex( istream& is );
    void ReadAtoms( istream& is, int& top_id );
    void ReadBonds( istream& is, int& top_id );
    void ReadPositions( istream& is, int& top_id );
    void ReadUnitName( istream& is, int& top_id );

    // remove quotation
    string get_str( const string& str );

    void ReadError( const string& reason );
};

//------------------------------------------------------------------------------
}

#endif
