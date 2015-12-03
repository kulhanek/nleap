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

#include <History.hpp>
#include <iostream>
#include <engine/Context.hpp>
#include <core/ForwardIterator.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CHistoryCommand::CHistoryCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
    m_change_state = false;
}

//------------------------------------------------------------------------------

CHistoryCommand::CHistoryCommand( const string& cmd_name, const string& file_name )
    : CCommand( cmd_name, cmd_name ), m_file( file_name )
{
    m_change_state = false;
}

//------------------------------------------------------------------------------

const char* CHistoryCommand::Info( EHelp type ) const
{
    if( type == help_group ) {
        return("context");
    }

    if( type == help_short ) {
        return("print the history of commands used so far");
    }

    return(
    "<b>NAME:</b>\n"
    "       <b>history</b> - print the history of commands used so far\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>history</b> [<u>filename</u>]\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Print the history of commands used so far either on screen or to the specified file <u>filename</u>.\n"
    );
}

//------------------------------------------------------------------------------

void CHistoryCommand::Exec( CContext* p_ctx )
{
    CEntityPtr list = p_ctx->database()->FindChild("_histcmds");

    ostream* ofs = NULL;
    fstream  fofs;

    if( ! m_file.empty() ){
        fofs.open( m_file.c_str(), ios_base::out );
        if( ! fofs ){
            stringstream str;
            str << "unable to open file '" << m_file << "' for writing";
            throw runtime_error( str.str() );
        }
        ofs = &fofs;
    } else {
        ofs = &p_ctx->out();
    }

    // list all childs
    CForwardIterator it = list->BeginChildren();
    CForwardIterator ie = list->EndChildren();

    while( it != ie ){
        *ofs << (*it)->Get<string>(VALUE) << endl;
        it++;
    }
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CHistoryCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 0, 1);

    string filename;
    if( cmdline.GetArgs().size() > 0 ){
        ExpandArgument( p_ctx, cmdline, 0, filename );
    }

    return shared_ptr< CCommand >( new CHistoryCommand( m_action, filename ) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}
 
