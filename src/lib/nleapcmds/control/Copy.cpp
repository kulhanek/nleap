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

#include <Copy.hpp>
#include <iostream>
#include <engine/Context.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
    
CCopyCommand::CCopyCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

// -------------------------------------------------------------------------

CCopyCommand::CCopyCommand( const string& cmd_name, const string& var_name, const string& srcvar_name )
    : CCommand( cmd_name, cmd_name ), m_var(var_name), m_srcvar(srcvar_name)
{
}

// -------------------------------------------------------------------------

const char* CCopyCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("control");
    }

    if( type == help_short )
    {
        return("creates an exact duplicate of an object");
    }

    return(
    "<b>NAME:</b>\n"
    "       <b>copy</b> - creates an exact duplicate of an object\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <u>newvariable</u> = <b>copy</b> <u>srcvariable</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Create an exact duplicate of the object <u>srcvariable</u>. Changing the object "
    "<u>srcvariable</u> will not affect the object <u>newvariable</u>. "
    "This is in contrast to the situation created by <u>newvariable</u> = <u>srcvariable</u> "
    "in which both names reference the same object.\n"
    );
}

// -------------------------------------------------------------------------

void CCopyCommand::Exec( CContext* p_ctx )
{
    // TODO
}

// -------------------------------------------------------------------------

shared_ptr< CCommand > CCopyCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    AssigmentRequired( cmdline );
    CheckNumberOfArguments( cmdline, 1 , 1);

    string var;
    string srcvar;

    ExpandLHS( p_ctx , cmdline , var );
    ExpandArgument( p_ctx, cmdline.GetArgs(), 0, srcvar );

    return shared_ptr< CCommand >( new CCopyCommand( m_action, var, srcvar ) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}

