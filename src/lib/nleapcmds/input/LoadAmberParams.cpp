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

#include <input/LoadAmberParams.hpp>
#include <fstream>
#include <engine/Context.hpp>
#include <types/Factory.hpp>
#include <format/AmberParams.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CLoadAmberParamsCommand::CLoadAmberParamsCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CLoadAmberParamsCommand::CLoadAmberParamsCommand( const string& cmd_name, const string& var, const string& file )
    : CCommand( cmd_name, cmd_name ), m_var( var ), m_file( file )
{
}

//------------------------------------------------------------------------------

const char* CLoadAmberParamsCommand::Info(  EHelp type ) const
{
    if( type == help_group )
    {
        return("input");
    }

    if( type == help_short )
    {
        return("load an AMBER format parameter set file");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>loadAmberParams</b> - load an AMBER format parameter set file\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <u>variable</u> = <b>loadAmberParams</b> <u>filename</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Load an AMBER format parameter set <u>filename</u> and place it into <u>variable</u>. "
    "All interactions defined in the parameter set will be contained within "
    "<u>variable</u>. This command causes the loaded parameter set to be included "
    "in LEaP's list of parameter sets that are searched when parameters are "
    "required.  General proper and improper torsion parameters are modified, "
    "the AMBER general type \"X\" is replaced with the LEaP general type \"?\".\n"
    );
}

//------------------------------------------------------------------------------

void CLoadAmberParamsCommand::Exec( CContext* p_ctx )
{
    CDatabasePtr db = p_ctx->database();

    // open file
    string real_file = p_ctx->FindFile( m_file  );
    p_ctx->out() << "Loading " << real_file << endl;

    ifstream is( real_file.c_str() );

    if( ! is ) {
        throw runtime_error( "Cannot open file '" + real_file + "'' for reading." );
    }

    // create amberff
    int top_id = p_ctx->m_index_counter.GetTopIndex();
    CAmberFFPtr ff = db->CreateAmberFF( top_id );

    CAmberParams  reader( p_ctx->out() );
    reader.Read( is, ff, top_id );

    // set variable
    db->SetVariable( top_id, m_var, ff );
    ff->SetName(m_var);

    p_ctx->m_index_counter.SetTopIndex( top_id );
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CLoadAmberParamsCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    AssigmentRequired( cmdline );
    CheckNumberOfArguments( cmdline, 1, 1);

    string var;
    string file;

    ExpandLHS( p_ctx, cmdline, var );
    ExpandArgument( p_ctx, cmdline, 0, file );

    return shared_ptr< CCommand >( new CLoadAmberParamsCommand(m_action, var, file) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}



