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

#include <BondByDistance.hpp>
#include <iostream>
#include <engine/Context.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CBondByDistanceCommand::CBondByDistanceCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CBondByDistanceCommand::CBondByDistanceCommand( const string& cmd_name, CEntityPtr& unit, double cutoff )
    : CCommand( cmd_name, cmd_name ), m_unit( unit ), m_cutoff( cutoff )
{
}

//------------------------------------------------------------------------------

const char* CBondByDistanceCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("control");
    }

    if( type == help_short )
    {
        return("create single bonds by distance criteria");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>bondByDistance</b> - create single bonds by distance criteria\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>bondByDistance</b> <u>object</u> [<u>maxBond</u>]\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Create single bonds between all ATOMs in container <u>object</u> that are within <u>maxBond</u> angstroms of "
    "each other. If <i>maxBond</i> is not specified then a default distance (1.6 A) will be used. This command is "
    "especially useful in building molecules. The container <u>object</u> can be of type UNIT or RESIDUE.\n"
    );
}

//------------------------------------------------------------------------------

void CBondByDistanceCommand::Exec( CContext* p_ctx )
{
    // TODO
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CBondByDistanceCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 1 , 2 );

    CEntityPtr unit;
    ExpandArgument( p_ctx, cmdline, 0, unit, UNIT );

    double cutoff = 1.6;
    if( cmdline.GetArgs().size() == 2 ){
        ExpandArgument( p_ctx, cmdline, 1, cutoff );
    }

    return shared_ptr< CCommand >( new CBondByDistanceCommand(m_action, unit, cutoff) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}

