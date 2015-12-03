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

#include <types/Unit.hpp>
#include <core/PredefinedKeys.hpp>
#include <types/Factory.hpp>
#include <iomanip>
#include <core/ForwardIterator.hpp>

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CUnit::CUnit(void)
: CEntity(UNIT)
{
    m_atoms = 0;
    m_bonds = 0;
    m_residues = 0;
}

// -------------------------------------------------------------------------

CUnit::CUnit(int& top_id)
: CEntity(UNIT)
{
    SetId( top_id++ );

    CEntityPtr node;

    node = CFactory::CreateNode( top_id, "residues" );
    AddChild(node);

    node = CFactory::CreateNode( top_id, "bonds" );
    AddChild(node);

    m_atoms = 0;
    m_bonds = 0;
    m_residues = 0;
}

// -------------------------------------------------------------------------

void CUnit::DescSubObj(const string& oname,ostream& ofs)
{
    // get object name
    string obj_name = oname;
    string sub_obj_name;

    int ndot = count( oname.begin(), oname.end(), '.' );

    if( ndot >= 1 ) {
        unsigned int dot = oname.find('.');
        obj_name = oname.substr(0,dot);
        if( dot + 1 < oname.size() ){
            sub_obj_name =  oname.substr(dot+1,oname.size()-1);
        }
    }

    // find residue
    CEntityPtr res;

    if( CFactory::IsInteger(obj_name) ){
        res = FindResidue( CFactory::GetNumber(obj_name) );
    } else {
        res = FindResidue(obj_name);
    }

    // is object valid?
    if( ! res ){
        throw runtime_error("unable to find residue '" + obj_name + "' in '" + GetName() + "'" );
    }

    // describe
    if( sub_obj_name.empty() ){
        res->Desc(ofs);
    } else {
        res->DescSubObj(sub_obj_name,ofs);
    }
}

// -------------------------------------------------------------------------

CEntityPtr CUnit::GetSubObj(const string& oname)
{
    // get object name
    string obj_name = oname;
    string sub_obj_name;

    int ndot = count( oname.begin(), oname.end(), '.' );

    if( ndot >= 1 ) {
        unsigned int dot = oname.find('.');
        obj_name = oname.substr(0,dot);
        if( dot + 1 < oname.size() ){
            sub_obj_name =  oname.substr(dot+1,oname.size()-1);
        }
    }

    // find residue
    CEntityPtr res;

    if( CFactory::IsInteger(obj_name) ){
        res = FindResidue( CFactory::GetNumber(obj_name) );
    } else {
        res = FindResidue(obj_name);
    }

    // is object valid?
    if( ! res ){
        throw runtime_error("unable to find residue '" + obj_name + "' in '" + GetName() + "'" );
    }

    // describe
    if( sub_obj_name.empty() ){
        return( res );
    } else {
        return( res->GetSubObj(sub_obj_name) );
    }
}

// -------------------------------------------------------------------------

void CUnit::Desc(ostream& ofs)
{
    DescHeader( ofs );

    if( m_residues > 0 ){
        ofs <<  endl;
        ofs << "   >>> RESIDUES                                                          " << endl;
        ofs << "     ID    Name    ID    Name    ID    Name    ID    Name    ID    Name  " << endl;
        ofs << "   ------- ----  ------- ----  ------- ----  ------- ----  ------- ----  " << endl;

        CForwardIterator it = BeginResidues();
        CForwardIterator ie = EndResidues();
        CForwardIterator pt;

        int pos = 0;
        while( it != ie ){
            if( pt && (pt->GetName() == it->GetName()) ){
                CForwardIterator bt = it;
                // sequence detected - count sequence
                int seq_size = 2;
                it++;
                while( it != ie ){
                    if( pt->GetName() != it->GetName() ) break;
                    pt = it;
                    it++;
                    seq_size++;
                }

                if( seq_size <= 3 ){
                    it = bt; // too short sequence
                } else {
                    it = pt; // last item in sequence
                    ofs << setw(14) << "<<==========>  ";
                    pos++;
                }
            }
            if( pos % 5 == 0 ) ofs << "   ";
            ofs << resetiosflags(ios_base::left) << setiosflags(ios_base::right);
            ofs << setw(7) << it->Get<int>(RID) << " ";
            ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
            ofs << setw(4) << it->GetName();
            ofs << "  ";
            pt = it;
            it++;
            pos++;
            if( pos % 5 == 0 ) ofs << endl;
        }

        if( pos % 5 != 0 ) ofs << endl;
    }
}

// -------------------------------------------------------------------------

void CUnit::DescHeader(ostream& ofs)
{
        ofs << "UNIT" << endl;
        ofs << "   Name      : " << GetName() << endl;
        ofs << "   GID       : " << GetId() << endl;
        ofs << "   Residues  : " << m_residues << endl;
        ofs << "   Atoms     : " << m_atoms << endl;
        ofs << "   Bonds     : " << m_bonds << endl;

    int head = Get<int>(HEAD);
    int tail = Get<int>(TAIL);
    if( head > 0 ){
        ofs << "   Head atom : " << head << endl;
    } else {
        ofs << "   Head atom : " << "n.d." << endl;
    }

    if( tail > 0 ){
        ofs << "   Tail atom : " << tail << endl;
    } else {
        ofs << "   Tail atom : " << "n.d." << endl;
    }
}

// -------------------------------------------------------------------------

void CUnit::DescAllResidues(ostream& ofs)
{
    DescHeader( ofs );

    ofs <<  endl;
    ofs << "   >>> RESIDUES         " << endl;
    ofs << "   RID     Name NoAtoms " << endl;
    ofs << "   ------- ---- ------- " << endl;

    CForwardIterator it = BeginResidues();
    CForwardIterator ie = EndResidues();

    while( it != ie ){
        ofs << "   ";
        ofs << resetiosflags(ios_base::left) << setiosflags(ios_base::right);
        ofs << setw(7) << it->Get<int>(RID) << " ";
        ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
        ofs << setw(4) << it->GetName() << " ";
        ofs << resetiosflags(ios_base::left) << setiosflags(ios_base::right);
        ofs << setw(7) << it->NumberOfChildren() << endl;
        it++;
    }
}

// -------------------------------------------------------------------------

void CUnit::DescAllAtoms(ostream& ofs)
{
    DescHeader( ofs );

    ofs <<  endl;
    ofs << "   >>> ALL ATOMS                                                                  " << endl;
    ofs << "     SID   LID  Name  Type   RID   RName     X          Y          Z       Charge " << endl;
    ofs << "   ------- --- ------ ---- ------- ----- ---------- ---------- ---------- --------" << endl;

    CRecursiveIterator it = BeginAtoms();
    CRecursiveIterator ie = EndAtoms();

    while( it != ie ){
        CEntityPtr res = (*it)->GetRoot();
        int     resid = 0;
        string  resname;
        if( res ){
            resid = res->Get<int>(RID);
            resname = res->GetName();
        }

        ofs << "   ";
        ofs << resetiosflags(ios_base::left) << setiosflags(ios_base::right);
        ofs << setw(7) << it->Get<int>(SID) << " ";
        ofs << setw(3) << it->Get<int>(LID) << " ";
        ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
        ofs << setw(6) << it->GetName() << " ";
        ofs << setw(4) << it->Get<string>(TYPE) << " ";
        ofs << resetiosflags(ios_base::left) << setiosflags(ios_base::right);
        ofs << setw(7) << resid << " ";
        ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
        ofs << setw(5) << resname << " ";
        ofs << resetiosflags(ios_base::left) << setiosflags(ios_base::right);
        ofs << fixed << setw(10) << setprecision(4) << it->Get<double>(POSX) << " ";
        ofs << fixed << setw(10) << setprecision(4) << it->Get<double>(POSY) << " ";
        ofs << fixed << setw(10) << setprecision(4) << it->Get<double>(POSZ) << " ";
        ofs << fixed << setw(8) << setprecision(4) << it->Get<double>(CHARGE);
        ofs << endl;
        it++;
    }
}

// -------------------------------------------------------------------------
// #########################################################################
// -------------------------------------------------------------------------

int CUnit::NumberOfAtoms(void)
{
    return( m_atoms );
}

// -------------------------------------------------------------------------

CRecursiveIterator CUnit::BeginAtoms(void)
{
    CEntityPtr residues = FindChild( "residues" );
    CRecursiveIterator it( residues );
    it.SetFilter( ATOM );
    it.SetToBegin();
    return( it );
}

// -------------------------------------------------------------------------

CRecursiveIterator CUnit::EndAtoms(void)
{
    CEntityPtr residues = FindChild( "residues" );
    CRecursiveIterator it( residues );
    it.SetFilter( ATOM );
    it.SetToEnd();
    return( it );
}

// -------------------------------------------------------------------------

CAtomPtr CUnit::FindAtom(int sid)
{
    CRecursiveIterator it = BeginAtoms();
    CRecursiveIterator ie = EndAtoms();

    while( it != ie ){
        if( it->Get<int>(SID) == sid ){
            return( dynamic_pointer_cast<CAtom>(*it) );
        }
        it++;
    }

    return( CAtomPtr() );
}

// -------------------------------------------------------------------------
// #########################################################################
// -------------------------------------------------------------------------

CResiduePtr CUnit::CreateResidue(const string& name, int& top_id)
{
    CEntityPtr residues = FindChild( "residues" );

    CResiduePtr res = CFactory::CreateResidue( top_id );
    residues->AddChild( res );

    res->SetName( name );
    m_residues++;

    return( res );
}

// -------------------------------------------------------------------------

void CUnit::AddResidue(CResiduePtr residue)
{
    if( ! residue ){
        throw runtime_error(" residue is NULL in CUnit::AddResidue");
    }

    CEntityPtr residues = FindChild( "residues" );
    residues->AddChild(residue);

    m_residues++;
}

// -------------------------------------------------------------------------

void CUnit::RemoveResidue(CResiduePtr residue)
{
    if( ! residue ){
        throw runtime_error(" residue is NULL in CUnit::RemoveResidue");
    }

    // remove all bonds that belongs to any residue atom
    CForwardIterator it = residue->BeginChildren();
    CForwardIterator ie = residue->EndChildren();

    while( it != ie ){
        CAtomPtr atm = dynamic_pointer_cast<CAtom>(*it);
        RemoveBonds(atm);
        it++;
    }

    // remove residue
    CEntityPtr residues = FindChild( "residues" );
    residues->RemoveChild(residue);

    // fix counters
    m_residues--;
}

// -------------------------------------------------------------------------

int CUnit::NumberOfResidues(void)
{
    CEntityPtr residues = FindChild( "residues" );

    return( residues->NumberOfChildren() );
}

// -------------------------------------------------------------------------

CForwardIterator CUnit::BeginResidues(void)
{
    CEntityPtr residues = FindChild( "residues" );

    CForwardIterator it( residues );
    it.SetToBegin();
    return( it );
}

// -------------------------------------------------------------------------

CForwardIterator CUnit::EndResidues(void)
{
    CEntityPtr residues = FindChild( "residues" );

    CForwardIterator it( residues );
    it.SetToEnd();
    return( it );
}

// -------------------------------------------------------------------------

CResiduePtr CUnit::FindResidue(const string& name)
{
    CForwardIterator it = BeginResidues();
    CForwardIterator ie = EndResidues();

    while( it != ie ){
        if( it->GetName() == name ){
            return( dynamic_pointer_cast<CResidue>(*it) );
        }
        it++;
    }
    return( CResiduePtr() );
}

// -------------------------------------------------------------------------

CResiduePtr CUnit::FindResidue(int rid)
{
    CForwardIterator it = BeginResidues();
    CForwardIterator ie = EndResidues();

    while( it != ie ){
        int lsid;
        lsid = it->Get<int>(RID);
        if( lsid == rid ){
            return( dynamic_pointer_cast<CResidue>(*it) );
        }
        it++;
    }
    return( CResiduePtr() );
}

// -------------------------------------------------------------------------

bool CUnit::HasResidue(CResiduePtr cres)
{
    if( ! cres ){
        throw runtime_error(" residue is NULL in CUnit::HasResidue");
    }

    CEntityPtr residues = FindChild( "residues" );

    return( residues != cres->GetRoot() );
}

// -------------------------------------------------------------------------
// #########################################################################
// -------------------------------------------------------------------------

CBondPtr CUnit::CreateBond(const CAtomPtr& at1, const CAtomPtr& at2, int order, int& top_id)
{
    CEntityPtr bonds = FindChild( "bonds" );

    CBondPtr bnd = CFactory::CreateBond( top_id );
    bonds->AddChild( bnd );

    bnd->Set( ATOM1, at1 );
    bnd->Set( ATOM2, at2 );
    bnd->Set( ORDER, order );

    m_bonds++;

    return( bnd );
}

// -------------------------------------------------------------------------

bool CUnit::AreBonded(const CAtomPtr& at1, const CAtomPtr& at2)
{
    CForwardIterator it = BeginBonds();
    CForwardIterator ie = EndBonds();

    while( it != ie ){
        if( ((it->Get<CEntityPtr>(ATOM1) == at1) && (it->Get<CEntityPtr>(ATOM2) == at2)) ||
            ((it->Get<CEntityPtr>(ATOM1) == at2) && (it->Get<CEntityPtr>(ATOM2) == at1)) ){
            return(true);
        }
        it++;
    }

    return(false);
}

// -------------------------------------------------------------------------

void CUnit::RemoveBond(const CAtomPtr& at1, const CAtomPtr& at2)
{
    CForwardIterator it = BeginBonds();
    CForwardIterator ie = EndBonds();
    CEntityPtr bonds = FindChild( "bonds" );

    while( it != ie ){
        if( ((it->Get<CEntityPtr>(ATOM1) == at1) && (it->Get<CEntityPtr>(ATOM2) == at2)) ||
            ((it->Get<CEntityPtr>(ATOM1) == at2) && (it->Get<CEntityPtr>(ATOM2) == at1)) ){
            bonds->RemoveChild(*it);
            m_bonds--;
            return;
        }
        it++;
    }
}

// -------------------------------------------------------------------------

void CUnit::RemoveBonds(const CAtomPtr& at1)
{
    CForwardIterator it = BeginBonds();
    CForwardIterator ie = EndBonds();
    CEntityPtr bonds = FindChild( "bonds" );

    while( it != ie ){
        CEntityPtr bond = *it;
        it++;
        if( (it->Get<CEntityPtr>(ATOM1) == at1) || (it->Get<CEntityPtr>(ATOM2) == at1) ){
            bonds->RemoveChild(bond);
            m_bonds--;
        }
    }
}

// -------------------------------------------------------------------------

int CUnit::NumberOfBonds(void)
{
    CEntityPtr bonds = FindChild( "bonds" );

    return( bonds->NumberOfChildren() );
}

// -------------------------------------------------------------------------

CForwardIterator CUnit::BeginBonds(void)
{
    CEntityPtr bonds = FindChild( "bonds" );

    CForwardIterator it( bonds );
    it.SetToBegin();
    return( it );
}

// -------------------------------------------------------------------------

CForwardIterator CUnit::EndBonds(void)
{
    CEntityPtr bonds = FindChild( "bonds" );

    CForwardIterator it( bonds );
    it.SetToEnd();
    return( it );
}

// -------------------------------------------------------------------------
// #########################################################################
// -------------------------------------------------------------------------

void CUnit::FixCounters(void)
{
    m_atoms = 0;
    m_bonds = 0;
    m_residues = 0;

    CEntityPtr residues = FindChild( "residues" );
    if( ! residues ){
        return;
    }

    m_residues = residues->NumberOfChildren();

    CEntityPtr bonds = FindChild( "bonds" );
    if( bonds ){
        m_bonds = bonds->NumberOfChildren();
    } else {
        m_bonds = 0;
    }

    CForwardIterator rit = residues->BeginChildren();
    CForwardIterator rie = residues->EndChildren();

    int resid = 1;
    int atmid = 1;
    while( rit != rie ){
        rit->Set( RID, resid );
        resid++;

        CForwardIterator ait = rit->BeginChildren();
        CForwardIterator aie = rit->EndChildren();

        int latmid = 1;
        while( ait != aie ){
            ait->Set( SID, atmid );
            ait->Set( LID, latmid );
            atmid++;
            latmid++;
            m_atoms++;
            ait++;
        }

        rit++;
    }

}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

}
