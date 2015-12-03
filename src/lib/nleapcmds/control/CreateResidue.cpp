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

#include <CreateResidue.hpp>
#include <iostream>
#include <engine/Context.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CCreateResidueCommand::CCreateResidueCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CCreateResidueCommand::CCreateResidueCommand( const string& cmd_name, const string& var_name, const string& object_name )
    : CCommand( cmd_name, cmd_name ), m_var( var_name ), m_name( object_name )
{
}

//------------------------------------------------------------------------------

const char* CCreateResidueCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("control");
    }

    if( type == help_short )
    {
        return("create a new and empty RESIDUE");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>createResidue</b> - create a new and empty RESIDUE\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <i>variable</i> = <b>createResidue</b> <u>name</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Create a new and empty RESIDUE named <u>name</u> and put it to <i>variable</i>.\n"
    );
}

//------------------------------------------------------------------------------

void CCreateResidueCommand::Exec( CContext* p_ctx )
{
    CDatabasePtr db = p_ctx->database();
    int top_id = p_ctx->m_index_counter.GetTopIndex();
    CEntityPtr obj = db->CreateResidue( top_id, m_name);
    db->SetVariable( top_id, m_var, obj );
    p_ctx->m_index_counter.SetTopIndex( top_id );
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CCreateResidueCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    AssigmentRequired( cmdline );
    CheckNumberOfArguments( cmdline, 1, 1);

    string var;
    string name;

    ExpandLHS( p_ctx, cmdline, var );
    ExpandArgument( p_ctx, cmdline, 0, name );

    return shared_ptr< CCommand >( new CCreateResidueCommand(m_action, var, name) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}


