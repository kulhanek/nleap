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

#include <Combine.hpp>
#include <iostream>
#include <engine/Context.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
    
CCombineCommand::CCombineCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CCombineCommand::CCombineCommand( const string& cmd_name, const string& var, CListPtr& list )
    : CCommand( cmd_name, cmd_name ), m_var( var ), m_list( list )
{
}

//------------------------------------------------------------------------------

const char* CCombineCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("control");
    }

    if( type == help_short )
    {
        return("combine units into a single unit");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>combine</b> - combine units into a single unit\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <u>variable</u> = <b>combine</b> <u>list</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Combine the contents of the UNITs within <u>list</u> into a single UNIT. "
    "The new UNIT is placed in <u>variable</u>. This command is similar to "
    "the <b>sequence</b> command except it does not link the ATOMs of the UNITs "
    "together.\n"
    );
}

//------------------------------------------------------------------------------

void CCombineCommand::Exec( CContext* p_ctx )
{
    // TODO
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CCombineCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    AssigmentRequired( cmdline );
    CheckNumberOfArguments( cmdline, 1 , 1 );

    string var;
    CListPtr list;

    ExpandLHS( p_ctx , cmdline , var );
    ExpandArgument( p_ctx, cmdline, 0, list );

    return shared_ptr< CCommand >( new CCombineCommand(m_action, var, list) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}

