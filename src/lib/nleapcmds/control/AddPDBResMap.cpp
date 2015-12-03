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

#include <AddPDBResMap.hpp>
#include <iostream>
#include <engine/Context.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CAddPDBResMapCommand::CAddPDBResMapCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CAddPDBResMapCommand::CAddPDBResMapCommand( const string& cmd_name, const CListPtr& list )
    : CCommand( cmd_name, cmd_name ), m_list( list )
{
}

//------------------------------------------------------------------------------

const char* CAddPDBResMapCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("control");
    }

    if( type == help_short )
    {
        return("add new PDB residue map entry");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>addPDBResMap</b> - add new PDB residue map entry\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>addPDBResMap</b> <u>list</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "The map is used to map residue names read from PDB files to variable "
    "names within LEaP.  The <u>list</u> is a LIST of LISTs:\n"
    "      { {sublist} {sublist} ... }\n"
    "Each sublist contains two or three entries to add to the map:\n"
    "      { [terminalflag] PDBName LEaPVar }\n"
    "where the PDBName will be mapped to the LEaPVar. The terminalflag indicates "
    "the special cases of terminal residues: allowable values are 0 for beginning "
    "residues (N-terminal for proteins, 5' for nucleic acids) and 1 for ending "
    "residues (C-terminal for proteins, 3' for nucleic acids).  If the "
    "terminalflag is given, the PDBName->LEaPVar name map will only be applied "
    "for the appropriate terminal residue.  The `leaprc' file included with "
    "the distribution contains default mappings.\n"
    );
}

//------------------------------------------------------------------------------

void CAddPDBResMapCommand::Exec( CContext* p_ctx )
{
    CPDBResMapPtr types = p_ctx->database()->GetPDBResMap();
    int top_id = p_ctx->m_index_counter.GetTopIndex();

    // we must add list to tmp container for proper iterator functionality
    p_ctx->tmp()->AddChild(m_list);
    types->AddPDBResMap( top_id, m_list, p_ctx->out() );
    p_ctx->tmp()->RemoveChild(m_list);

    p_ctx->m_index_counter.SetTopIndex( top_id );
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CAddPDBResMapCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 1 , 1);

    CListPtr list;
    ExpandArgument(p_ctx, cmdline, 0, list);

    return shared_ptr< CCommand >( new CAddPDBResMapCommand( m_action, list ) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}

