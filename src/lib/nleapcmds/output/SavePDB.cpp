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

#include <SavePDB.hpp>
#include <fstream>
#include <engine/Context.hpp>
#include <types/Factory.hpp>
#include <format/SybylMol2.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSavePDBCommand::CSavePDBCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CSavePDBCommand::CSavePDBCommand( const string& cmd_name, const string& var, const string& file )
    : CCommand( cmd_name, cmd_name ), m_var( var ), m_file( file )
{
}

//------------------------------------------------------------------------------

const char* CSavePDBCommand::Info(  EHelp type ) const
{
    if( type == help_group )
    {
        return("output");
    }

    if( type == help_short )
    {
        return("save a Protein Data Bank (PDB) format file");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>savePdb</b> - save a Protein Data Bank (PDB) format file\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>savePdb</b> <u>unit</u> <u>filename</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Write the UNIT <u>unit</u> to the file <u>filename</u> as a PDB format file."
    );
}

//------------------------------------------------------------------------------

void CSavePDBCommand::Exec( CContext* p_ctx )
{
    CDatabasePtr db = p_ctx->database();

    // open file
    ofstream is( m_file.c_str() );

    if( ! is ) {
        throw std::runtime_error( "Cannot open file '" + m_file + "'' for writing." );
    }

}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CSavePDBCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 2 , 2);

    string var;
    string file;

    ExpandArgument( p_ctx, cmdline, 0, var );
    ExpandArgument( p_ctx, cmdline, 1, file );

    return shared_ptr< CCommand >( new CSavePDBCommand(m_action, var, file) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}



