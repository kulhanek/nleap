// =============================================================================
// nLEaP - A molecular manipulation program and coding environment
// -----------------------------------------------------------------------------
//     Copyright (C) 2010 Petr Kulhanek, kulhanek@chemi.muni.cz
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

#include <Clear.hpp>
#include <iomanip>
#include <engine/Context.hpp>
#include <stdexcept>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

using namespace std;

CClearCommand::CClearCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

// -------------------------------------------------------------------------

CClearCommand::CClearCommand( const string& cmd_name,  const string& action )
    : CCommand( cmd_name, cmd_name )
{
}

// -------------------------------------------------------------------------

const char* CClearCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("control");
    }
    if( type == help_short )
    {
        return("clear nLEaP database");
    }

    return(
    "<b>NAME:</b>\n"
    "       <b>clear</b> - clear nLEaP database"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>clear</b>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Clear the entire nLEaP database. This means that all variables, maps, type maps are released or deleted.</u>.\n"
    );
}

//------------------------------------------------------------------------------

void CClearCommand::Exec( CContext* p_ctx )
{
    p_ctx->database()->ClearDatabase();
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CClearCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 0 , 0);

    return shared_ptr< CCommand >( new CClearCommand( m_action, m_action ) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}

