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

#include <LoadOB.hpp>
#include <fstream>
#include <engine/Context.hpp>
#include <types/Factory.hpp>
#include <format/FormatOB.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CLoadOBCommand::CLoadOBCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CLoadOBCommand::CLoadOBCommand( const string& cmd_name, const string& var, const string& file, const string& fmt )
    : CCommand( cmd_name, cmd_name ), m_var( var ), m_file( file ), m_format( fmt )
{
}

//------------------------------------------------------------------------------

const char* CLoadOBCommand::Info(  EHelp type ) const
{
    if( type == help_group )
    {
        return("openbabel");
    }

    if( type == help_short )
    {
        return("load a file using OpenBabel");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>loadOB</b> - load a file using OpenBabel\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <u>variable</u> = <b>loadOB</b> <u>filename</u> [<u>format</u>]\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Load the file <u>filename</u> into the UNIT referenced by the <u>variable</u>. "
    "The format of the file is autodetected. However if the autodetection fails the user can "
    "specify the format using the <u>format</u> option. The list of formats can be obtained by "
    " <b>listOBFormats</b>. "
    "The file <u>filename</u> is not searched in the PATH (see <b>nleap</b> command).\n"
    );
}

//------------------------------------------------------------------------------

void CLoadOBCommand::Exec( CContext* p_ctx )
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

    CFormatOB  reader( p_ctx->out() );
    reader.Read( is, unit, top_id, m_format );

    // set variable
    db->SetVariable( top_id, m_var, unit );
    p_ctx->m_index_counter.SetTopIndex( top_id );
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CLoadOBCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    AssigmentRequired( cmdline );
    CheckNumberOfArguments( cmdline, 1 , 2);

    string var;
    string file;
    string fmt = "auto";

    ExpandLHS( p_ctx, cmdline, var );
    ExpandArgument( p_ctx, cmdline, 0, file );

    if( cmdline.GetArgs().size() == 2 ){
        ExpandArgument( p_ctx, cmdline, 1, fmt );
    }

    return shared_ptr< CCommand >( new CLoadOBCommand(m_action, var, file, fmt) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}



