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

#include <Check.hpp>
#include <iostream>
#include <engine/Context.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CCheckCommand::CCheckCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

// -------------------------------------------------------------------------

CCheckCommand::CCheckCommand( const string& cmd_name,  CEntityPtr& unit )
    : CCommand( cmd_name, cmd_name ), m_unit( unit )
{
}

// -------------------------------------------------------------------------

const char* CCheckCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("control");
    }

    if( type == help_short )
    {
        return("check the unit for internal inconsistencies");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>check</b> - check the unit for internal inconsistencies\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>check</b> <u>unit</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "This command can be used to check the <u>unit</u> for internal inconsistencies "
    "that could cause problems when performing calculations. This is "
    "a very useful command that should be used before a UNIT is saved "
    "with saveAmberParm or its variations.\n"
    "Currently it checks for the following possible problems:\n"
    "  * Long bonds.\n"
    "  * Short bonds.\n"
    "  * Non-integral total charge of the <u>unit</u>.\n"
    "  * Missing types.\n"
    "  * Close contacts between non-bonded ATOMs. A close contact is less than 1.5 angstroms.\n"
    );
}

// -------------------------------------------------------------------------

void CCheckCommand::Exec( CContext* p_ctx )
{
    CUnitPtr unit = dynamic_pointer_cast<CUnit>(m_unit);
    CAmberFF::CheckUnit(p_ctx->database(),unit,p_ctx->out());
}

// -------------------------------------------------------------------------

shared_ptr< CCommand > CCheckCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 1, 1);

    CEntityPtr unit;
    ExpandArgument( p_ctx, cmdline, 0, unit, UNIT );

    return shared_ptr< CCommand >( new CCheckCommand(m_action, unit) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}

