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

#include <Set.hpp>
#include <iostream>
#include <engine/Context.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSetCommand::CSetCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CSetCommand::CSetCommand( const string& cmd_name, CEntityPtr& object, const string& parm, const string& value )
    : CCommand( cmd_name, cmd_name ), m_object( object ), m_param( parm ), m_value( value )
{
}

//------------------------------------------------------------------------------

const char* CSetCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("control");
    }

    if( type == help_short )
    {
        return("set properties of object");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>set</b> - set properties of object\n"
    "\n"
    "<b>SYNOPSIS:\n"
    "       <b>set</b> <u>object</u> <u>property</u> <u>value</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "TODO\n"
    );
}

//------------------------------------------------------------------------------

void CSetCommand::Exec( CContext* p_ctx )
{
    // TODO
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CSetCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 3 , 3 );

    CEntityPtr  obj;
    string      prop,value;
    ExpandArgument( p_ctx, cmdline, 0, obj, ANY );
    ExpandArgument( p_ctx, cmdline, 1, prop );
    ExpandArgument( p_ctx, cmdline, 2, value );

    return shared_ptr< CCommand >( new CSetCommand(m_action, obj, prop, value ) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}


