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

#include <AddPDBAtomMap.hpp>
#include <iostream>
#include <engine/Context.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CAddPDBAtomMapCommand::CAddPDBAtomMapCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CAddPDBAtomMapCommand::CAddPDBAtomMapCommand( const string& cmd_name, const CListPtr& list )
    : CCommand( cmd_name, cmd_name ), m_list( list )
{
}

//------------------------------------------------------------------------------

const char* CAddPDBAtomMapCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("control");
    }

    if( type == help_short )
    {
        return("add new PDB atom map entry");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>addPDBAtomType</b> - add new PDB atom map entry\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>addPDBAtomType</b> <u>list</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "The atom Name Map is used to try to map atom names read from PDB files "
    "to atoms within residue UNITs when the atom name in the PDB file does "
    "not match an atom in the residue. This enables PDB files to be read "
    "in without extensive editing of atom names.\n"
    "The <u>list</u> is a LIST of LISTs:\n"
    "       { {sublist} {sublist} ... }\n"
    "where each sublist is of the form\n"
    "      { \"OddAtomName\" \"LibAtomName\" }\n"
    "Many 'odd' atom names can map to one 'standard' atom name, but any single "
    "odd atom name maps only to the last standard atom name it was mapped to.\n"
    );
}

//------------------------------------------------------------------------------

void CAddPDBAtomMapCommand::Exec( CContext* p_ctx )
{
    CPDBAtomMapPtr types = p_ctx->database()->GetPDBAtomMap();
    int top_id = p_ctx->m_index_counter.GetTopIndex();

    // we must add list to tmp container for proper iterator functionality
    p_ctx->tmp()->AddChild(m_list);
    types->AddPDBAtomMap( top_id, m_list, p_ctx->out() );
    p_ctx->tmp()->RemoveChild(m_list);

    p_ctx->m_index_counter.SetTopIndex( top_id );
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CAddPDBAtomMapCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 1 , 1);

    CListPtr list;
    ExpandArgument(p_ctx, cmdline, 0, list);

    return shared_ptr< CCommand >( new CAddPDBAtomMapCommand( m_action, list ) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}

