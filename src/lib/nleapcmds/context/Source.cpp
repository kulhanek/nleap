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

#include <Source.hpp>
#include <iostream>
#include <engine/Context.hpp>
#include <fstream>

using std::string;
using std::vector;
using std::ifstream;

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSourceCommand::CSourceCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CSourceCommand::CSourceCommand( const string& cmd_name, const string& file_name )
    : CCommand( cmd_name, cmd_name ), m_file( file_name )
{
}

//------------------------------------------------------------------------------

const char* CSourceCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("context");
    }

    if( type == help_short )
    {
        return("read and execute commands from a file");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>source</b> - read and execute commands from a file\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>source</b> <u>filename</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Read and execute commands from <u>filename</u>. "
    "If <u>filename</u> does not contain a slash, file names in PATH (see <b>mortenv</b> command) "
    "are used to find the directory containing <u>filename</u>.\n"
    );
}

//------------------------------------------------------------------------------

void CSourceCommand::Exec( CContext* p_ctx )
{
    string real_file;
    real_file = p_ctx->FindFile( m_file  );
    p_ctx->out() << "Loading " << real_file << endl;

    ifstream stream( real_file.c_str() );
    if( ! stream ){
        throw runtime_error("unable to open file");
    }
    string line;

    while( getline( stream, line ) )
    {
        if( p_ctx->Process( line ) == false ){
            throw runtime_error("file processing failed");
        }
    }
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CSourceCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 1 , 1);

    string name;
    ExpandArgument( p_ctx , cmdline , 0 , name );

    return shared_ptr< CCommand >( new CSourceCommand(m_action, name) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}
 
