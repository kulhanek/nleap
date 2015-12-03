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

#include <format/AmberOFF.hpp>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <iomanip>
#include <types/Factory.hpp>
#include <core/PredefinedKeys.hpp>

using namespace boost;

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CAmberOFF::CAmberOFF( CVerboseStr& debug )
    : m_debug( debug )
{
    m_line_no = 0;
    m_unit_index = -1;
}

// -------------------------------------------------------------------------

void CAmberOFF::Read( istream& is, CDatabasePtr& db, int& top_id )
{
    if( ! db ){
        // invalid unit
        throw runtime_error( "db is NULL in CAmberOFF::read" );
    }

    m_unit_index = -1;
    m_index.clear();
    m_unit.reset();

    getline( is, m_line );
    while( is ){
        m_line_no++;

        // parse line
        stringstream    str( m_line );
        string          keyword;

        str >> keyword;
        if( keyword.size() == 0 ){
            // skip empty lines
            getline( is, m_line );
            continue;
        }

        if( keyword == "!!index" ){
            // read index
            ReadIndex( is );
            if( m_index.size() == 0 ) return; // no records
            continue;
        }

        if( keyword[0] != '!' ){
            // skip this line - line from unsupported section
            getline( is, m_line );
            continue;
        }

        // split keyword into subkeys
        vector<string>  subkeys;
        split( subkeys, keyword, is_any_of(".") );

        // check subkeys
        if( subkeys.size() != 4 ){
            ReadError("keyword does not contain four subkeys.");
        }

        if( subkeys[0] != "!entry" ){
            ReadError("first subkey is not <b>!entry</b>");
        }

        if( subkeys[2] != "unit" ){
            ReadError("third subkey is not <b>unit</b>");
        }

        // new unit?
        if( (m_unit_index == -1) || ( m_index[m_unit_index] != subkeys[1] ) ){
            m_unit_index++;
            if( m_unit_index >= (int) m_index.size() ){
                stringstream str;
                str << "out-off number of predefined units, attempted to read " << m_unit_index+1;
                str << " but " << m_index.size() << " defined";
                ReadError( str.str() );
            }
            if( m_unit ){
                m_unit->FixCounters();
            }
            // create unit
            m_unit = db->CreateUnit( top_id, m_index[m_unit_index] );
            // and create variable in the database
            db->SetVariable( top_id, m_index[m_unit_index], m_unit );
        }

        if( ! m_unit ){
            ReadError("unit is not valid object");
        }

        // read unit parts
        if( subkeys[3] == "atoms" ){
            // read atoms
            ReadAtoms( is, top_id );
            continue;
        } else if ( subkeys[3] == "connectivity" ) {
            // read bonds
            ReadBonds( is, top_id );
            continue;
        } else if ( subkeys[3] == "positions" ) {
            // read atom positions
            ReadPositions( is, top_id );
            continue;
        }

        // read next line
        getline( is, m_line );

    }

    if( m_unit ){
        m_unit->FixCounters();
    }

}

// -------------------------------------------------------------------------

void CAmberOFF::ReadIndex( istream& is )
{
    getline( is, m_line );
    while( is ){
        m_line_no++;
        if( m_line.size() == 0 ){
            // skip empty lines
            getline( is, m_line );
            continue;
        }
        if( m_line[0] == '!' ){
            m_line_no--; // new section return
            return;
        }
        m_index.push_back( get_str( m_line ) );
        getline( is, m_line );
    }
}

// -------------------------------------------------------------------------

void CAmberOFF::ReadAtoms( istream& is, int& top_id )
{
    m_debug << medium << "> Reading atoms ..." << endl;

    int         natoms = 0;
    int         nres = 0;
    int         prev_resid = 0;
    CResiduePtr res;

    m_atom_map.clear();

    getline( is, m_line );
    while( is ){
        m_line_no++;
        if( m_line.size() == 0 ){
            // skip empty lines
            getline( is, m_line );
            continue;
        }
        if( m_line[0] == '!' ){
            m_line_no--; // new section return
            return;
        }

        //parse line
        string      atname;
        string      type;
        int         typex = 0;
        int         resix = 0;
        int         flag = 0;
        int         atid = 0;
        int         atz = 0;
        double      charge = 0;

        stringstream str( m_line );
        str >> atname >> type >> typex >> resix >> flag >> atid >> atz >> charge;

        if( prev_resid < resix ){
            prev_resid = resix;
            // create new residue
            res = m_unit->CreateResidue( "X", top_id );
            nres++;
        }

        if( ! res ) {
            throw runtime_error("errr");
        }

        // create atom within residue
        CAtomPtr atm = res->CreateAtom( get_str(atname), top_id );
        m_atom_map.push_back(atm);

        // populate atom with data
        atm->Set(TYPE, get_str(type) );
        atm->Set(CHARGE, charge);

        natoms++;

        getline( is, m_line );
    }

    m_debug << high << "  Atoms = " << setw(8) << natoms;
    m_debug << "  Residues = " << setw(8) << nres << endl;
}

// -------------------------------------------------------------------------

void CAmberOFF::ReadBonds( istream& is, int& top_id )
{
    getline( is, m_line );
    while( is ){
        m_line_no++;
        if( m_line.size() == 0 ){
            // skip empty lines
            getline( is, m_line );
            continue;
        }
        if( m_line[0] == '!' ){
            m_line_no--; // new section return
            return;
        }

        getline( is, m_line );
    }
}

// -------------------------------------------------------------------------

void CAmberOFF::ReadPositions( istream& is, int& top_id )
{
    m_debug << medium << "> Reading atom positions ..." << endl;

    size_t         natoms = 0;

    getline( is, m_line );
    while( is ){
        m_line_no++;
        if( m_line.size() == 0 ){
            // skip empty lines
            getline( is, m_line );
            continue;
        }
        if( m_line[0] == '!' ){
            m_line_no--; // new section return
            return;
        }

        //parse line
        double      x = 0;
        double      y = 0;
        double      z = 0;

        stringstream str( m_line );
        str >> x >> y >> z;

        if( natoms >= m_atom_map.size() ){
            ReadError("too many atoms in positions");
        }

        // get atoms
        CAtomPtr atm = m_atom_map[natoms];

        // populate atom with data
        atm->Set(POSX, x );
        atm->Set(POSY, y );
        atm->Set(POSZ, z );

        natoms++;

        getline( is, m_line );
    }
}

// -------------------------------------------------------------------------

void CAmberOFF::ReadUnitName( istream& is, int& top_id )
{
    m_debug << medium << "> Reading unit name ..." << endl;

    getline( is, m_line );
    while( is ){
        m_line_no++;
        if( m_line.size() == 0 ){
            // skip empty lines
            getline( is, m_line );
            continue;
        }
        if( m_line[0] == '!' ){
            m_line_no--; // new section return
            return;
        }

        //parse line
        string name;

        stringstream str( m_line );
        str >> name;

        // populate unit with data
        m_unit->SetName( get_str(name) );

        getline( is, m_line );
    }
}

// -------------------------------------------------------------------------

string CAmberOFF::get_str( const string& str )
{
    return( trim_copy_if( str, is_any_of(" \"") ) );
}

// -------------------------------------------------------------------------

void CAmberOFF::ReadError( const string& reason )
{
    stringstream str;
    str << reason << endl;
    str << "       Line " << m_line_no << " : " << m_line;

    throw runtime_error( str.str() );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}




