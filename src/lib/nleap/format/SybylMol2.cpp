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

#include <format/SybylMol2.hpp>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <iomanip>
#include <core/PredefinedKeys.hpp>

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSybylMol2::CSybylMol2( CVerboseStr& debug )
    : m_debug( debug )
{
    m_line_no = 0;
}

// -------------------------------------------------------------------------

void CSybylMol2::Read( istream& is, CUnitPtr& unit, int& top_id )
{
    m_atoms = 0;
    m_bonds = 0;
    m_residues = 0;

    if( ! unit ){
        // invalid unit
        throw runtime_error( "unit is NULL in CSybylMol2::read" );
    }

    m_atom_map.clear();

    getline( is, m_line );
    while( is ){
        m_line_no++;
        if( m_line.find("@<TRIPOS>MOLECULE") != string::npos ){
            ReadHead( is, unit );
        } else
        if( m_line.find("@<TRIPOS>ATOM") != string::npos ){
            ReadAtoms( is, unit, top_id );
        } else
        if( m_line.find("@<TRIPOS>BOND") != string::npos ){
            ReadBonds( is, unit, top_id );
        }

        getline( is, m_line );
    }

    // fix unit counters
    unit->FixCounters();
}

// -------------------------------------------------------------------------

void CSybylMol2::ReadHead( istream& is, CUnitPtr& unit )
{
    m_debug << "> Reading molecule header ..." << endl;
    string name, moltype, chgtype, status, comment, temp;

    // line 0
    getline( is, m_line );
    m_line_no++;
    unit->SetName( m_line );
    m_debug << "  Name : " << m_line << endl;

    // line 1
    getline( is, m_line );
    m_line_no++;

    stringstream str(m_line);
    str >> m_atoms >> m_bonds >> m_residues;

    m_debug << "  Atoms = " << setw(8) << m_atoms;
    m_debug << "  Bonds = " << setw(8) << m_bonds;
    m_debug << "  Residues = " << setw(8) << m_residues << endl;

    // line 2
    getline( is, m_moltype );
    m_line_no++;

    // line 3
    getline( is, m_chgtype );
    m_line_no++;

    // line 4 : optional
    if( is.peek() != '@' ) {
        getline( is, m_status );
        m_line_no++;
    }

    // line 5: optional
    if( is.peek() != '@' ) {
        getline( is, m_comment );
        m_line_no++;
        if( ! empty(m_comment) ){
            unit->Set( TITLE, m_comment );
            m_debug << "  Comment  : " << m_comment << endl;
        }
    }
}

// -------------------------------------------------------------------------

void CSybylMol2::ReadAtoms( istream& is, CUnitPtr& unit, int& top_id )
{
    m_debug << "> Reading atoms ..." << endl;

    int         natoms = 0;
    int         nres = 0;
    int         prev_resid = 0;
    CResiduePtr res;

    getline( is, m_line );
    while( is ){
        m_line_no++;

        int         atid = 0;
        string      atname;
        double      x = 0;
        double      y = 0;
        double      z = 0;
        string      type;
        int         resid = 0;
        string      resname;
        double      charge = 0;

        stringstream str( m_line );
        str >> atid >> atname >> x >> y >> z >> type >> resid >> resname >> charge;

        if( prev_resid < resid ){
            prev_resid = resid;
            // create new residue
            res = unit->CreateResidue( resname, top_id );
            nres++;
        }

        if( ! res ) {
            throw runtime_error("errr");
        }

        // create atom within residue
        CAtomPtr atm = res->CreateAtom( atname, top_id );
        m_atom_map[atid] = atm;

        // populate atom with data
        atm->Set(POSX, x);
        atm->Set(POSY, y);
        atm->Set(POSZ, z);
        atm->Set(TYPE, type);
        atm->Set(CHARGE, charge);

        natoms++;
        if( natoms >= m_atoms ) break;

        getline( is, m_line );
    }

    m_debug << "  Atoms = " << setw(8) << natoms;
    m_debug << "  Residues = " << setw(8) << nres << endl;
}

// -------------------------------------------------------------------------

void CSybylMol2::ReadBonds( istream& is, CUnitPtr& unit, int& top_id )
{
    m_debug << "> Reading bonds ..." << endl;
    int nbonds = 0;

    getline( is, m_line );
    while( is ){
        m_line_no++;

        int         boid  = 0;
        int         atid1 = 0;
        int         atid2 = 0;

        stringstream str( m_line );
        str >> boid >> atid1 >> atid2;

        unit->CreateBond(m_atom_map[atid1], m_atom_map[atid1], 1, top_id);

        nbonds++;
        if( nbonds >= m_bonds ) break;

        getline( is, m_line );
    }

    m_debug << "  Bonds = " << setw(8) << nbonds << endl;
}

// -------------------------------------------------------------------------
// #########################################################################
// -------------------------------------------------------------------------

void CSybylMol2::Write( ostream& os, CUnitPtr& unit )
{
    WriteHead( os, unit );
    WriteAtoms( os, unit );
    WriteBonds( os, unit );
}

// -------------------------------------------------------------------------

void CSybylMol2::WriteHead( ostream& os, CUnitPtr& unit )
{
    os << "@<TRIPOS>MOLECULE" << endl;

    string molname = unit->GetName();
    os << (  molname.empty() ? "untitled" : molname ) << endl;

    // ----------------
    os << format( "%8d " ) % unit->NumberOfAtoms();
    os << format( "%8d " ) % unit->NumberOfBonds();
    os << format( "%8d " ) % unit->NumberOfResidues() << endl;

    // ----------------
    os << ( unit->NumberOfResidues() < 2 ? "SMALL" : "POLYMER" ) << endl;

    // ----------------
    os << "USER" << endl;
    os << endl;
    os << endl;
}

// -------------------------------------------------------------------------

void CSybylMol2::WriteAtoms( ostream& os, CUnitPtr& unit )
{
    if( unit->NumberOfAtoms() == 0 ) {
        return;
    }

    os << "@<TRIPOS>ATOM" << endl;

    CRecursiveIterator it = unit->BeginAtoms();
    CRecursiveIterator ie = unit->EndAtoms();

    while( it != ie ) {
        CAtomPtr atm = dynamic_pointer_cast<CAtom>(*it);
        if( atm ){
            os << format( "%8d " ) % atm->Get<int>(SID);
            os << format( "%-8s " ) % atm->GetName();
            os << format( "%9.3f " ) % atm->Get<double>(POSX);
            os << format( "%9.3f " ) % atm->Get<double>(POSY);
            os << format( "%9.3f " ) % atm->Get<double>(POSZ);
            os << format( "%-8s " ) % atm->Get<string>(TYPE);
            os << format( "%8d " ) % atm->GetResidueId();
            os << format( "%-8s " ) % atm->GetResidueName();
            os << format( "%9.3f" ) % atm->Get<double>(CHARGE);
            os << endl;
        }
        it++;
    }
}

// -------------------------------------------------------------------------

void CSybylMol2::WriteBonds( ostream& os, CUnitPtr& unit )
{
    if( unit->NumberOfBonds() == 0 ) {
        return;
    }
    os << "@<TRIPOS>BOND" << std::endl;

    CForwardIterator it = unit->BeginBonds();
    CForwardIterator ie = unit->EndBonds();

    int id = 1;
    while( it != ie ) {
        os << format("%8d ") % id;
        int first = it->Get<CEntityPtr>(ATOM1)->Get<int>(SID);
        int second = it->Get<CEntityPtr>(ATOM2)->Get<int>(SID);
        os << format( "%8d " ) % std::min( first, second );
        os << format( "%8d " ) % std::max( first, second );
        os << format( "%8d " ) % it->Get<int>(ORDER);
        os << std::endl;
        id++;
        it++;
    }
}

// -------------------------------------------------------------------------

void CSybylMol2::rw_error( const string& reason )
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
 
