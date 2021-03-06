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

#include <geometry/Center.hpp>
#include <engine/Context.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CCenterCommand::CCenterCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

// -------------------------------------------------------------------------

CCenterCommand::CCenterCommand( const string& cmd_name,  const CUnitPtr& unit, const string& mask )
    : CCommand( cmd_name, cmd_name ), m_unit( unit ), m_mask( mask )
{
}

// -------------------------------------------------------------------------

const char* CCenterCommand::Info(  EHelp type ) const
{
    if( type == help_group )
    {
        return("geometry");
    }

    if( type == help_short )
    {
        return("center an unit");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>center</b> - center an unit\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>center</b> <u>unit</u> [<u>mask</u>]\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "This command moves the center of mass of the <u>unit</u> either to "
    "the origin or the center of box. If <u>mask</u> is provided, only "
    "those in the <u>mask</u> are used to calculate the center of mass."
    );
}

// -------------------------------------------------------------------------

void CCenterCommand::Exec( CContext* p_ctx )
{

}

// -------------------------------------------------------------------------

shared_ptr< CCommand > CCenterCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 2, 4);

    CEntityPtr  unit;
    string      mask;

    ExpandArgument( p_ctx , cmdline, 0, unit, UNIT );
    ExpandArgument( p_ctx , cmdline, 1, mask );

    return shared_ptr< CCommand >( new CCenterCommand(m_action, dynamic_pointer_cast<CUnit>(unit), mask) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}



