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

#include <SaveAmberParm.hpp>
#include <fstream>
#include <engine/Context.hpp>
#include <types/Factory.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSaveAmberParmCommand::CSaveAmberParmCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CSaveAmberParmCommand::CSaveAmberParmCommand( const string& cmd_name, const CEntityPtr& unit, const string& file1, const string& file2 )
    : CCommand( cmd_name, cmd_name ), m_unit( unit ), m_file1( file1 ), m_file2( file2 )
{
}

//------------------------------------------------------------------------------

const char* CSaveAmberParmCommand::Info(  EHelp type ) const
{
    if( type == help_group )
    {
        return("output");
    }

    if( type == help_short )
    {
        return("save AMBER/NAB topology and coordinate files");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>saveAmberParm</b> - save AMBER/NAB topology and coordinate files\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>saveAmberParm</b> <u>unit</u> <u>topologyfilename</u> <u>coordinatefilename</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Save the AMBER/NAB topology and coordinate files for the <u>unit</u> into the files "
    "named <u>topologyfilename</u> and <u>coordinatefilename</u> respectively. "
    "This command will cause LEaP to search its list of PARMSETs for parameters defining "
    "all of the interactions between the ATOMs within the <u>unit</u>. This command produces "
    "topology files and coordinate files that are identical in format to those produced by AMBER PARM "
    " and can be read into AMBER and NAB for calculations. The output of this operation "
    " can be used for minimizations, dynamics, and thermodynamic integration calculations.\n"
    );
}

//------------------------------------------------------------------------------

void CSaveAmberParmCommand::Exec( CContext* p_ctx )
{
    // TODO
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CSaveAmberParmCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 3, 3);

    CEntityPtr  unit;
    string      file1,file2;

    ExpandArgument( p_ctx, cmdline, 0, unit, UNIT );
    ExpandArgument( p_ctx, cmdline, 1, file1 );
    ExpandArgument( p_ctx, cmdline, 2, file2 );

    return shared_ptr< CCommand >( new CSaveAmberParmCommand(m_action, unit, file1, file2) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}

