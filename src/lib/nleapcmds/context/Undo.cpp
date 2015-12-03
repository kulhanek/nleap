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

#include <Undo.hpp>
#include <iostream>
#include <engine/Context.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CUndoCommand::CUndoCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
    m_change_state = false;
}

//------------------------------------------------------------------------------

CUndoCommand::CUndoCommand( const string& cmd_name, int undo_level )
    : CCommand( cmd_name, cmd_name ), m_undo_level( undo_level )
{
    m_change_state = false;
}

//------------------------------------------------------------------------------

const char* CUndoCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("context");
    }

    if( type == help_short )
    {
        return("undo previously performed nLEaP actions");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>undo</b> - undo previously performed nLEaP actions\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>undo</b> <u>level</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "TODO\n"
    );
}

//------------------------------------------------------------------------------

void CUndoCommand::Exec( CContext* p_ctx )
{
    p_ctx->Undo( m_undo_level );
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CUndoCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 0 , 1 );

    int level = 1;
    if( cmdline.GetArgs().size() > 0 ){
        ExpandArgument( p_ctx , cmdline , 0 , level );
        if( level < 1 ){
            WrongArgument( cmdline , 0, "undo level must be greater than zero");
        }
    }

    return shared_ptr< CCommand >( new CUndoCommand( m_action, level ) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}
 
