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

#include <Remove.hpp>
#include <iostream>
#include <engine/Context.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
    
CRemoveCommand::CRemoveCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CRemoveCommand::CRemoveCommand( const string& cmd_name, CEntityPtr& owner, CEntityPtr& object )
    : CCommand( cmd_name, cmd_name ), m_owner( owner ), m_object( object )
{
}

//------------------------------------------------------------------------------

const char* CRemoveCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("control");
    }

    if( type == help_short )
    {
        return("remove an object");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>remove</b> - remove an object\n"
    "\n"
    "<b>SYNOPSIS:\n"
    "       <b>remove</b> <u>owner</u> <u>object</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Remove the <u>object</u> from the <u>owner</u>. This command is "
    "used to remove ATOMs from RESIDUEs, and RESIDUEs from UNITs. If "
    "the <u>object</u> is not further referenced by any object or variable "
    "then it will be destroyed.\n"
    );
}

//------------------------------------------------------------------------------

void CRemoveCommand::Exec( CContext* p_ctx )
{
    // remove ATOM from RESIDUE object
    if( (m_object->GetType() == ATOM) && (m_owner->GetType() == RESIDUE) ){
        if( m_owner->IsChild( m_object ) ){
            stringstream str;
            str << "object " << m_object->GetPathName() << " is not owned by ";
            str << m_owner->GetPathName();
            throw runtime_error( str.str() );
        }
        CResiduePtr res = dynamic_pointer_cast<CResidue>(m_owner);
        CAtomPtr    atm = dynamic_pointer_cast<CAtom>(m_object);
        res->RemoveAtom( atm );
        CUnitPtr unit = res->GetUnit();
        if( unit ){
            unit->FixCounters();
        }
        return;
    }

    // remove RESIDUE from UNIT object
    if( (m_object->GetType() == RESIDUE) && (m_owner->GetType() == UNIT) ){
        CUnitPtr    unit = dynamic_pointer_cast<CUnit>(m_owner);
        CResiduePtr res = dynamic_pointer_cast<CResidue>(m_object);
        if( unit->HasResidue( res ) ){
            stringstream str;
            str << "object " << m_object->GetPathName() << " is not owned by ";
            str << m_owner->GetPathName();
            throw runtime_error( str.str() );
        }
        unit->RemoveResidue( res );
        unit->FixCounters();
        return;
    }

    // unsupported object types
    stringstream str;
    str << "unable to remove the object of type " << m_object->GetType().GetName();
    str << " from the object of type " << m_owner->GetType().GetName();
    throw runtime_error( str.str() );
}

//------------------------------------------------------------------------------

CCommandPtr CRemoveCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 2 , 2 );

    CEntityPtr obj1,obj2;
    ExpandArgument( p_ctx, cmdline, 0, obj1, ANY );
    ExpandArgument( p_ctx, cmdline, 1, obj2, ANY );

    return shared_ptr< CCommand >( new CRemoveCommand(m_action, obj1, obj2 ) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}




        
