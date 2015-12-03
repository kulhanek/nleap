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

#include <LoadMol2.hpp>
#include <fstream>
#include <engine/Context.hpp>
#include <types/Factory.hpp>
#include <format/SybylMol2.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CLoadMol2Command::CLoadMol2Command( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CLoadMol2Command::CLoadMol2Command( const string& cmd_name, const string& var, const string& file )
    : CCommand( cmd_name, cmd_name ), m_var( var ), m_file( file )
{
}

//------------------------------------------------------------------------------

const char* CLoadMol2Command::Info(  EHelp type ) const
{
    if( type == help_group )
    {
        return("input");
    }

    if( type == help_short )
    {
        return("load a Sybyl MOL2 format file");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>loadMol2</b> - load a Sybyl MOL2 format file\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <u>variable</u> = <b>loadMol2</b> <u>filename</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Load the Sybyl MOL2 format file <u>filename</u> into the UNIT referenced by the <u>variable</u>. "
    "This command is very much like <b>loadOff</b> command, except that it only creates a single unit. "
    "The file <u>filename</u> is not searched in the PATH (see <b>nleap</b> command).\n"
    );
}

//------------------------------------------------------------------------------

void CLoadMol2Command::Exec( CContext* p_ctx )
{
    CDatabasePtr db = p_ctx->database();

    // open file
    ifstream is( m_file.c_str() );

    if( ! is ) {
        throw std::runtime_error( "Cannot open file '" + m_file + "'' for reading." );
    }

    // create unit
    int top_id = p_ctx->m_index_counter.GetTopIndex();
    CUnitPtr unit = db->CreateUnit( top_id );

    CSybylMol2  reader( p_ctx->out() );
    reader.Read( is, unit, top_id );

    // set variable
    db->SetVariable( top_id, m_var, unit );
    p_ctx->m_index_counter.SetTopIndex( top_id );
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CLoadMol2Command::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    AssigmentRequired( cmdline );
    CheckNumberOfArguments( cmdline, 1 , 1);

    string var;
    string file;

    ExpandLHS( p_ctx, cmdline, var );
    ExpandArgument( p_ctx, cmdline, 0, file );

    return shared_ptr< CCommand >( new CLoadMol2Command(m_action, var, file) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}



