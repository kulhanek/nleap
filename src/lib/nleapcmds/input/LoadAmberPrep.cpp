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

#include <input/LoadAmberPrep.hpp>
#include <fstream>
#include <engine/Context.hpp>
#include <types/Factory.hpp>
#include <format/AmberPrep.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CLoadAmberPrepCommand::CLoadAmberPrepCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CLoadAmberPrepCommand::CLoadAmberPrepCommand( const string& cmd_name, const string& file )
    : CCommand( cmd_name, cmd_name ), m_file( file )
{
}

//------------------------------------------------------------------------------

const char* CLoadAmberPrepCommand::Info(  EHelp type ) const
{
    if( type == help_group )
    {
        return("input");
    }

    if( type == help_short )
    {
        return("load an AMBER PREP input file");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>loadAmberPrep</b> - load an AMBER PREP input file\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>loadAmberPrep</b> <u>filename</u> [<i>prefix</i>]\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "This command loads an AMBER PREP input file <u>filename</u>. For each residue that is loaded, a new UNIT "
    "is constructed that contains a single RESIDUE. The UNIT is created with the same name as "
    "the name of the residue within the PREP file. If the optional argument <i>prefix</i> is provided it will "
    "be prefixed to each UNIT name. If <u>filename</u> does not contain a slash, file names in PATH (see <b>mortenv</b> command) "
    "are used to find the directory containing <u>filename</u>.\n"
    );
}

//------------------------------------------------------------------------------

void CLoadAmberPrepCommand::Exec( CContext* p_ctx )
{
    CDatabasePtr db = p_ctx->database();

    // open file
    ifstream is( m_file.c_str() );

    if( ! is ) {
        throw std::runtime_error( "Cannot open file '" + m_file + "'' for reading." );
    }

    // TODO
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CLoadAmberPrepCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 1, 1);

    string file;
    ExpandArgument( p_ctx, cmdline, 0, file );

    return shared_ptr< CCommand >( new CLoadAmberPrepCommand(m_action, file) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}



