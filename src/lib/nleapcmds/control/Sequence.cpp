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

#include <Sequence.hpp>
#include <iostream>
#include <engine/Context.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSequenceCommand::CSequenceCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CSequenceCommand::CSequenceCommand( const string& cmd_name, const string& name, CListPtr& list )
    : CCommand( cmd_name, cmd_name ), m_name( name ), m_list( list )
{
}

//------------------------------------------------------------------------------

const char* CSequenceCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("control");
    }

    if( type == help_short )
    {
        return("sequence");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>sequence</b> - sequence\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <u>variable</u> = <b>sequence</b> <u>list</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "The sequence command is used to create a new UNIT by copying the "
    "contents of a <u>list</u> of UNITs. As each UNIT in the list is copied, "
    "a bond is created between its head atom and the tail ATOM of the  "
    "previous UNIT, if both connect ATOMs are defined. If only one of "
    "the connect pair is defined, a warning is generated and no bond is "
    "created. If neither connection ATOM is defined then no bond is "
    "created. As each RESIDUE within a UNIT is copied, it is assigned a "
    "sequence number reflecting the order added. The order of RESIDUEs "
    "in multi-RESIDUE UNITs is maintained.\n"
    "This command builds reasonable starting coordinates for the new UNIT "
    "by assigning internal coordinates to the linkages between the component "
    "UNITs and building the Cartesian coordinates from these and the "
    "internal coordinates of the component UNITs.\n"
    );
}

//------------------------------------------------------------------------------

void CSequenceCommand::Exec( CContext* p_ctx )
{
    // TODO
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CSequenceCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    AssigmentRequired( cmdline );
    CheckNumberOfArguments( cmdline, 1 , 1 );

    string var;
    CListPtr list;

    ExpandLHS( p_ctx , cmdline , var );
    ExpandArgument( p_ctx, cmdline, 0, list );

    return shared_ptr< CCommand >( new CSequenceCommand(m_action, var, list) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}

