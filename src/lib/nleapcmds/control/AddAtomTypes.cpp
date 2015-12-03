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

#include <AddAtomTypes.hpp>
#include <iostream>
#include <engine/Context.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CAddAtomTypesCommand::CAddAtomTypesCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

// -------------------------------------------------------------------------

CAddAtomTypesCommand::CAddAtomTypesCommand( const string& cmd_name, const CListPtr& list )
    : CCommand( cmd_name, cmd_name ), m_list( list )
{
}

// -------------------------------------------------------------------------

const char* CAddAtomTypesCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("control");
    }

    if( type == help_short )
    {
        return("add new AMBER atom type entry");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>addAtomTypes</b> - add new AMBER atom type entry\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>addAtomTypes</b> <u>list</u>\n"
    "\n"
    "       <u>list</u> = { { \"type\" \"element\" \"hybridization\"} { ... } ... }\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Add mapping of AMBER atom type to element and hybridization.\n"
    );
}

// -------------------------------------------------------------------------

void CAddAtomTypesCommand::Exec( CContext* p_ctx )
{
    CAtomTypesPtr types = p_ctx->database()->GetAtomTypes();
    int top_id = p_ctx->m_index_counter.GetTopIndex();

    // we must add list to tmp container for proper iterator functionality
    p_ctx->tmp()->AddChild(m_list);
    types->AddAtomTypes( top_id, m_list, p_ctx->out() );
    p_ctx->tmp()->RemoveChild(m_list);

    p_ctx->m_index_counter.SetTopIndex( top_id );
}

// -------------------------------------------------------------------------

shared_ptr< CCommand > CAddAtomTypesCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 1 , 1);

    CListPtr list;
    ExpandArgument(p_ctx, cmdline, 0, list);

    return shared_ptr< CCommand >( new CAddAtomTypesCommand( m_action, list ) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}

