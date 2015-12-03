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

#include <Alias.hpp>
#include <iostream>
#include <engine/Context.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CAliasCommand::CAliasCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
    m_change_state = false;
}

//------------------------------------------------------------------------------

CAliasCommand::CAliasCommand( const string& cmd_name, const vector<string>& args )
    : CCommand( cmd_name, cmd_name ), m_args( args )
{
    m_change_state = false;
}

//------------------------------------------------------------------------------

const char* CAliasCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("control");
    }

    if( type == help_short )
    {
        return("manage command aliases");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>alias</b> - manage command aliases\n"
    "\n"
    "<b>USAGE:</b>\n"
    "       <b>alias</b> <u>string</u> <u>command</u>\n"
    "       equivalence <u>string</u> to <u>command</u>\n"
    "\n"
    "       <b>alias</b> <u>string</u>\n"
    "       delete the alias for <u>string</u>\n"
    "\n"
    "       <b>alias</b>\n"
    "       report all current aliases\n"
    );
}

//------------------------------------------------------------------------------

void CAliasCommand::Exec( CContext* p_ctx )
{
    // ------------------------------------------
    // list all aliases
    if( m_args.size() == 0 ){
        p_ctx->PrintAliases(p_ctx->out());
        return;
    }

    // ------------------------------------------
    // handle remove alias
    if( m_args.size() == 1 ) {
        string str;
        ExpandArgument( p_ctx, m_args, 0, str );
        p_ctx->RemoveAlias(str);
        return;
    }

    // add alias
    string str,cmd;
    ExpandArgument( p_ctx, m_args, 0, str );
    ExpandArgument( p_ctx, m_args, 1, cmd );
    p_ctx->AddAlias(str,cmd);
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CAliasCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 0 , 2);

    return shared_ptr< CCommand >( new CAliasCommand( m_action, cmdline.GetArgs() ) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}

