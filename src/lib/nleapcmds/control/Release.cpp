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

#include <Release.hpp>
#include <iostream>
#include <engine/Context.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================


CReleaseCommand::CReleaseCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

// -------------------------------------------------------------------------

CReleaseCommand::CReleaseCommand( const string& cmd_name, const string& var_name )
    : CCommand( cmd_name, cmd_name ), m_var( var_name )
{
}

// -------------------------------------------------------------------------

const char* CReleaseCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("control");
    }

    if( type == help_short )
    {
        return("release a variable");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>release</b> - release a variable\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>release</b> <u>variable</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Release <u>variable</u>. If the object referenced by <u>variable</u> is not "
    "further referenced by other objects or variables it is destroyed.\n"
    );
}

// -------------------------------------------------------------------------

void CReleaseCommand::Exec( CContext* p_ctx )
{
    // is it variable?
    if( ! p_ctx->database()->IsVariable(m_var) ){
        stringstream str;
        str << m_var << " is not a valid variable";
        throw runtime_error( str.str() );
    }

    // release variable
    p_ctx->database()->ReleaseVariable(m_var);
}

// -------------------------------------------------------------------------

shared_ptr< CCommand > CReleaseCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 1, 1 );

    string variable;
    ExpandArgument( p_ctx, cmdline, 0, variable );

    return shared_ptr< CCommand >( new CReleaseCommand( m_action, variable ) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}


