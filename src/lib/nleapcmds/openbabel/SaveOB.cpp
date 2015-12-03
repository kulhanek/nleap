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

#include <SaveOB.hpp>
#include <fstream>
#include <engine/Context.hpp>
#include <types/Factory.hpp>
#include <format/FormatOB.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSaveOBCommand::CSaveOBCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CSaveOBCommand::CSaveOBCommand( const string& cmd_name, const CEntityPtr& unit, const string& file, const string& fmt )
    : CCommand( cmd_name, cmd_name ), m_unit( unit ), m_file( file ), m_format( fmt )
{
}

//------------------------------------------------------------------------------

const char* CSaveOBCommand::Info(  EHelp type ) const
{
    if( type == help_group )
    {
        return("openbabel");
    }

    if( type == help_short )
    {
        return("save a file using OpenBabel");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>saveOB</b> - save a file using OpenBabel\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>loadOB</b> <u>unit</u> <u>filename</u> [<u>format</u>]\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Save the UNIT referenced by the variable <u>unit</u> to the file <u>filename</u>. "
    "The format of the file is autodetected from <u>filename</u> extension. If the autodetection fails the user may "
    "specify the format using the <u>format</u> option. The list of formats can be obtained by "
    " <b>listOBFormats</b> command.\n"
    );
}

//------------------------------------------------------------------------------

void CSaveOBCommand::Exec( CContext* p_ctx )
{
    // open file
    ofstream os( m_file.c_str() );

    if( ! os ) {
        throw std::runtime_error( "Cannot open file '" + m_file + "'' for writing." );
    }

    CUnitPtr unit = dynamic_pointer_cast<CUnit>( m_unit );

    CFormatOB  writer( p_ctx->out() );
    writer.Write( os, unit, m_format );
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CSaveOBCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 2, 3);

    CEntityPtr  unit;
    string      file;
    string      fmt = "auto";

    ExpandArgument( p_ctx, cmdline, 0, unit, UNIT );
    ExpandArgument( p_ctx, cmdline, 1, file );

    if( cmdline.GetArgs().size() == 3 ){
        ExpandArgument( p_ctx, cmdline, 2, fmt );
    }

    return shared_ptr< CCommand >( new CSaveOBCommand(m_action, unit, file, fmt) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}



