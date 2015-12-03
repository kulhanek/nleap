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

#include <LoadOFF.hpp>
#include <fstream>
#include <engine/Context.hpp>
#include <types/Factory.hpp>
#include <format/AmberOFF.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CLoadOFFCommand::CLoadOFFCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CLoadOFFCommand::CLoadOFFCommand( const string& cmd_name, const string& file )
    : CCommand( cmd_name, cmd_name ), m_file( file )
{
}

//------------------------------------------------------------------------------

const char* CLoadOFFCommand::Info(  EHelp type ) const
{
    if( type == help_group )
    {
        return("input");
    }

    if( type == help_short )
    {
        return("load an OFF library format file");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>loadOff</b> - load an OFF library format file\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>loadOff</b> <u>filename</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Load the OFF library <u>filename</u>. If <u>filename</u> does not contain a slash, file names in PATH (see <b>mortenv</b> command) "
    "are used to find the directory containing <u>filename</u>."
    );
}

//------------------------------------------------------------------------------

void CLoadOFFCommand::Exec( CContext* p_ctx )
{
    CDatabasePtr db = p_ctx->database();

    // open file
    string real_file = p_ctx->FindFile( m_file  );
    p_ctx->out() << "Loading " << real_file << endl;

    ifstream is( real_file.c_str() );

    if( ! is ) {
        throw runtime_error( "Cannot open file '" + real_file + "'' for reading." );
    }

    // load OFF file
    nleap::CAmberOFF     reader( p_ctx->out() );
    int                 top_id = p_ctx->m_index_counter.GetTopIndex();

    reader.Read( is, db, top_id );
    p_ctx->m_index_counter.SetTopIndex( top_id );

}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CLoadOFFCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 1, 1);

    string file;
    ExpandArgument( p_ctx, cmdline, 0, file );

    return shared_ptr< CCommand >( new CLoadOFFCommand(m_action, file) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}



