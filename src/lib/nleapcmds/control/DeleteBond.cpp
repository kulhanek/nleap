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

#include <DeleteBond.hpp>
#include <iostream>
#include <engine/Context.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
    
CDeleteBondCommand::CDeleteBondCommand(  const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CDeleteBondCommand::CDeleteBondCommand(  const string& cmd_name, CEntityPtr& atom1, CEntityPtr& atom2 )
    : CCommand( cmd_name, cmd_name ), m_atom1( atom1 ), m_atom2( atom2 )
{
}

//------------------------------------------------------------------------------

const char* CDeleteBondCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("control");
    }

    if( type == help_short )
    {
        return("delete a bond between two atoms");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>deleteBond</b> - delete a bond between two atoms\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>deleteBond</b> <u>atom1</u> <u>atom2</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Delete the bond between the ATOMs <i>atom1</i> and <i>atom2</i>.\n"
    );

}

//------------------------------------------------------------------------------

void CDeleteBondCommand::Exec( CContext* p_ctx )
{
    CAtomPtr at1 = dynamic_pointer_cast<CAtom>(m_atom1);
    CAtomPtr at2 = dynamic_pointer_cast<CAtom>(m_atom2);

    if( ! at1->GetUnit() ){
        stringstream str;
        str << "the first atom " << at1->GetPathName() << " has to be the part of unit";
        throw runtime_error( str.str() );
    }

    if( ! at2->GetUnit() ){
        stringstream str;
        str << "the second atom " << at2->GetPathName() << " has to be the part of unit";
        throw runtime_error( str.str() );
    }

    if( at1->GetUnit() != at2->GetUnit() ){
        stringstream str;
        str << "both atoms must be the part of the same unit ";
        str << "the first atom unit is " << at1->GetUnit()->GetPathName() << ", ";
        str << "the second atom unit is " << at2->GetUnit()->GetPathName();
        throw runtime_error( str.str() );
    }

    CUnitPtr unit = dynamic_pointer_cast<CUnit>(at1->GetUnit());
    if( ! unit->AreBonded(at1,at2) ){
        stringstream str;
        str << "the atom " << at1->GetPathName() << " and atom " <<  at1->GetPathName();
        str << " are not bonded";
        throw runtime_error( str.str() );
    }

    // remove bond
    unit->RemoveBond(at1,at2);
}

//------------------------------------------------------------------------------

CCommandPtr CDeleteBondCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 2, 2 );

    CEntityPtr at1,at2;
    ExpandArgument( p_ctx, cmdline, 0, at1, ATOM );
    ExpandArgument( p_ctx, cmdline, 1, at2, ATOM );

    return shared_ptr< CCommand >( new CDeleteBondCommand(m_action, at1, at2 ) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}




        
