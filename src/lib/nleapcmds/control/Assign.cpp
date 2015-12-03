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

#include <Assign.hpp>
#include <iostream>
#include <engine/Context.hpp>
#include <types/Factory.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CAssignCommand::CAssignCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CAssignCommand::CAssignCommand( const string& cmd_name, const string& var, const string& object_value )
    : CCommand( cmd_name, cmd_name ), m_var( var ), m_value( object_value )
{
}

//------------------------------------------------------------------------------

const char* CAssignCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("control");
    }

    if( type == help_short )
    {
        return("assign a value to an object");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>assign</b> - assign a value to an object\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <u>variable</u> = <b>assign</b> <u>value</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Assign a <u>value</u> to an object with <u>name</u>.\n"
    );
}

//------------------------------------------------------------------------------

void CAssignCommand::Exec( CContext* p_ctx )
{
    CDatabasePtr db = p_ctx->database();
    CEntityPtr obj;

    int top_id = p_ctx->m_index_counter.GetTopIndex();

    // is m_value also variable?
    if( db->IsVariable( m_value ) ){
        obj = db->GetVariableObject( m_value );
    } else {
        // create new object
        CKey type = CFactory::DetermineType(m_value);
        if( type == STRING ){
            obj = db->CreateString( top_id, m_value);
        } else if( type == NUMBER ) {
            obj = db->CreateNumber( top_id, m_value);
        } else {
            // wrong type
            throw runtime_error( "unrecognized type" );
        }
    }

    // set variable
    db->SetVariable( top_id, m_var, obj );

    p_ctx->m_index_counter.SetTopIndex( top_id );
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CAssignCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    AssigmentRequired( cmdline );
    CheckNumberOfArguments( cmdline, 1, 1 );

    string var;
    string value;

    ExpandLHS( p_ctx, cmdline, var );
    ExpandArgument( p_ctx, cmdline, 0, value );

    return shared_ptr< CCommand >( new CAssignCommand(m_action, var, value) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}


