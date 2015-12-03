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

#include <types/Residue.hpp>
#include <core/PredefinedKeys.hpp>
#include <types/Factory.hpp>
#include <types/Unit.hpp>
#include <iomanip>
#include <core/ForwardIterator.hpp>

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//=============================================================================

CResidue::CResidue(void)
: CEntity(RESIDUE)
{
    m_properties.SetInitialBlockSize(1);
}

// -------------------------------------------------------------------------

CResidue::CResidue(int& top_id)
: CEntity(RESIDUE)
{
    SetId( top_id++ );
    m_properties.SetInitialBlockSize(1);
}

// -------------------------------------------------------------------------

void CResidue::DescSubObj(const string& oname,ostream& ofs)
{
    // get object name
    string obj_name = oname;

    int ndot = count( oname.begin(), oname.end(), '.' );

    if( ndot >= 1 ) {
        throw runtime_error("only atom name or local id is expected (too many dots)" );
    }

    CEntityPtr atm;
    if( CFactory::IsInteger(obj_name) ){
        atm = FindAtom( CFactory::GetNumber(obj_name) );
        if( ! atm ) {
            throw runtime_error("unable to find atom with the sequence number '" + obj_name + "'" );
        }
    } else {
        atm = FindChild( obj_name );
        if( ! atm ) {
            throw runtime_error("unable to find atom with name '" + obj_name + "'" );
        }
    }

    atm->Desc(ofs);
}

// -------------------------------------------------------------------------

CEntityPtr CResidue::GetSubObj(const string& oname)
{
    // get object name
    string obj_name = oname;

    int ndot = count( oname.begin(), oname.end(), '.' );

    if( ndot >= 1 ) {
        throw runtime_error("only atom name or local id is expected (too many dots)" );
    }

    CEntityPtr atm;
    if( CFactory::IsInteger(obj_name) ){
        atm = FindAtom( CFactory::GetNumber(obj_name) );
        if( ! atm ) {
            throw runtime_error("unable to find atom with the sequence number '" + obj_name + "'" );
        }
    } else {
        atm = FindChild( obj_name );
        if( ! atm ) {
            throw runtime_error("unable to find atom with name '" + obj_name + "'" );
        }
    }

    return( atm );
}

// -------------------------------------------------------------------------

void CResidue::Desc(ostream& ofs)
{
        ofs << "RESIDUE" << endl;
        ofs << "   Name            : " << GetName() << endl;
        ofs << "   GID             : " << GetId() << endl;
        ofs << "   RID             : " << Get<int>(RID) << endl;
        ofs << "   Residue type    : " << GetResidueType() << endl;

    CEntityPtr unit = GetUnit();
    if( unit ){
        ofs << "   Unit name       : " << unit->GetName() << endl;
    } else {
        ofs << "   Unit name       : not contained within an unit" << endl;
    }

        ofs << "   Number of atoms : " << NumberOfChildren() << endl;

    int at0 = Get<int>(ATOM1);
    int at1 = Get<int>(ATOM2);
    int at2 = Get<int>(ATOM3);

    if( at0 > 0 ) {
        ofs << "   Connect 0 atom  : " << at0 << endl;
    } else {
        ofs << "   Connect 0 atom  : " << "n.d." << endl;
    }

    if( at1 > 0 ) {
        ofs << "   Connect 1 atom  : " << at1 << endl;
    } else {
        ofs << "   Connect 1 atom  : " << "n.d." << endl;
    }

    if( at2 > 0 ) {
        ofs << "   Connect 2 atom  : " << at2 << endl;
    } else {
        ofs << "   Connect 2 atom  : " << "n.d." << endl;
    }

    if( NumberOfChildren() > 0 ){
        ofs <<  endl;
        ofs << "   >>> ATOMS                                                         " << endl;
        ofs << "   LID   SID    Name  Type     X          Y          Z       Charge  " << endl;
        ofs << "   --- ------- ------ ---- ---------- ---------- ---------- -------- " << endl;

        CForwardIterator it = BeginChildren();
        CForwardIterator ie = EndChildren();

        while( it != ie ){
            CEntityPtr obj = *it;
            ofs << "   ";
            ofs << resetiosflags(ios_base::left) << setiosflags(ios_base::right);
            ofs << setw(3) << obj->Get<int>(LID) << " ";
            ofs << setw(7) << obj->Get<int>(SID) << " ";
            ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
            ofs << setw(6) << obj->GetName() << " ";
            ofs << setw(4) << obj->Get<string>(TYPE) << " ";
            ofs << resetiosflags(ios_base::left) << setiosflags(ios_base::right);
            ofs << fixed << setw(10) << setprecision(4) << obj->Get<double>(POSX) << " ";
            ofs << fixed << setw(10) << setprecision(4) << obj->Get<double>(POSY) << " ";
            ofs << fixed << setw(10) << setprecision(4) << obj->Get<double>(POSZ) << " ";
            ofs << fixed << setw(8) << setprecision(4) << obj->Get<double>(CHARGE);
            ofs << endl;
            it++;
        }
    }
}

// -------------------------------------------------------------------------

CAtomPtr CResidue::CreateAtom(const string& name, int& top_id)
{
    CAtomPtr atm = CFactory::CreateAtom( top_id );
    AddChild( atm );
    atm->SetName( name );
    return( atm );
}

// -------------------------------------------------------------------------

void CResidue::AddAtom(CAtomPtr atom)
{
    if( ! atom ){
        throw runtime_error("atom is NULL in CResidue::AddAtom");
    }

    AddChild( atom );
}

// -------------------------------------------------------------------------

void CResidue::RemoveAtom(CAtomPtr atom)
{
    if( ! atom ){
        throw runtime_error("atom is NULL in CResidue::RemoveAtom");
    }

    CUnitPtr unit = GetUnit();
    if( unit ){
        // remove all bonds referencing this atom
        unit->RemoveBonds(atom);
    }

    RemoveChild( atom );
}

// -------------------------------------------------------------------------

CUnitPtr CResidue::GetUnit(void)
{
    CEntityPtr root = GetRoot();
    if( ! root ){
        return( CUnitPtr() );
    }

    if( root->GetName() != "residues" ){
        return( CUnitPtr() );
    }

    CUnitPtr unit = dynamic_pointer_cast<CUnit>(root->GetRoot());
    return( unit );
}

// -------------------------------------------------------------------------

string CResidue::GetResidueType( )
{
    return("unknow - not implemented");
}

// -------------------------------------------------------------------------

CAtomPtr CResidue::FindAtom(int lid)
{
    CForwardIterator it = BeginChildren();
    CForwardIterator ie = EndChildren();

    while( it != ie ){
        if( it->Get<int>(LID) == lid ){
            return( dynamic_pointer_cast<CAtom>(*it) );
        }
        it++;
    }

    return( CAtomPtr() );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

}
