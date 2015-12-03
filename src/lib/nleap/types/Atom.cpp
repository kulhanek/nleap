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

#include <types/Atom.hpp>
#include <types/Residue.hpp>
#include <core/PredefinedKeys.hpp>
#include <iomanip>

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CAtom::CAtom(void)
: CEntity(ATOM)
{
    m_properties.SetInitialBlockSize(7);
}

// -------------------------------------------------------------------------

CAtom::CAtom(int& top_id)
: CEntity(ATOM)
{
    SetId( top_id++ );
    m_properties.SetInitialBlockSize(7);
}

// -------------------------------------------------------------------------

void CAtom::Desc(ostream& ofs)
{
    ofs << "ATOM" << endl;
    ofs << "   Name     : " << GetName() << endl;
    ofs << "   GID      : " << GetId() << endl;
    ofs << "   SID      : " << Get<int>(SID) << endl;
    ofs << "   LID      : " << Get<int>(LID) << endl;
    ofs << "   Type     : " << Get<string>(TYPE) << endl;
    ofs << "   Position : {";
    ofs << fixed << setw(10) << setprecision(4) << Get<double>(POSX) << " ";
    ofs << fixed << setw(10) << setprecision(4) << Get<double>(POSY) << " ";
    ofs << fixed << setw(10) << setprecision(4) << Get<double>(POSZ) << " }" << endl;
    ofs << "   Charge   :  ";
    ofs << fixed << setw(10) << setprecision(4) << Get<double>(CHARGE) << endl;
}

// -------------------------------------------------------------------------

CUnitPtr CAtom::GetUnit(void)
{
    CEntityPtr root = GetRoot();
    if( ! root ){
        return( CUnitPtr() );
    }

    CResiduePtr res = dynamic_pointer_cast<CResidue>(root);
    if( res ){
        return( res->GetUnit() );
    }
    return( CUnitPtr() );
}

// -------------------------------------------------------------------------

CResiduePtr CAtom::GetResidue(void)
{
    CEntityPtr root = GetRoot();
    if( ! root ){
        return( CResiduePtr() );
    }

    CResiduePtr unit = dynamic_pointer_cast<CResidue>(root);
    return( unit );
}

// -------------------------------------------------------------------------

int CAtom::GetResidueId(void)
{
    CEntityPtr root = GetRoot();
    if( ! root ){
        return( 0 );
    }
    return( root->Get<int>(RID) );
}

// -------------------------------------------------------------------------

string CAtom::GetResidueName(void)
{
    CEntityPtr root = GetRoot();
    if( ! root ){
        return( string() );
    }
    return( root->GetName() );
}

// -------------------------------------------------------------------------

const CPoint CAtom::GetPos(void)
{
    return( CPoint(Get<double>(POSX), Get<double>(POSY), Get<double>(POSZ)) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

}
