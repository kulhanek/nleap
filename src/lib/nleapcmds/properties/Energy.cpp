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

#include <Energy.hpp>
#include <engine/Context.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CEnergyCommand::CEnergyCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CEnergyCommand::CEnergyCommand(const string& cmd_name, CEntityPtr& unit)
    : CCommand( cmd_name, cmd_name ), m_unit( unit )
{
}

//------------------------------------------------------------------------------

const char* CEnergyCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("properties");
    }

    if( type == help_short )
    {
        return("calculate energy of object");
    }

    return(
    "<b>NAME:</b>\n"
    "       <b>energy</b> - calculate energy of object\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>energy</b> <u>unit</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "This command calculates the total energy of the <u>unit</u>.\n"
    );
}

//------------------------------------------------------------------------------

void CEnergyCommand::Exec( CContext* p_ctx )
{
    throw runtime_error("this command is not implemented");
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CEnergyCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 1, 1 );

    CEntityPtr  unit;
    ExpandArgument( p_ctx , cmdline, 0, unit, UNIT );

    return shared_ptr< CCommand >( new CEnergyCommand( m_action, unit) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}


