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

#include <SaveGromos.hpp>
#include <fstream>
#include <engine/Context.hpp>
#include <types/Factory.hpp>


namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CSaveGromosCommand::CSaveGromosCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CSaveGromosCommand::CSaveGromosCommand( const string& cmd_name, const CEntityPtr& unit, const string& file )
    : CCommand( cmd_name, cmd_name ), m_unit( unit ), m_file( file )
{
}

//------------------------------------------------------------------------------

const char* CSaveGromosCommand::Info(  EHelp type ) const
{
    if( type == help_group )
    {
        return("output");
    }

    if( type == help_short )
    {
        return("");
    }
    return(
    ""
    );
}

//------------------------------------------------------------------------------

void CSaveGromosCommand::Exec( CContext* p_ctx )
{
    // open file
    ofstream os( m_file.c_str() );

    if( ! os ) {
        throw std::runtime_error( "Cannot open file '" + m_file + "'' for writing." );
    }

    // write unit


    CUnitPtr unit = dynamic_pointer_cast<CUnit>( m_unit );
    // TODO
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CSaveGromosCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 2, 2);

    CEntityPtr  unit;
    string      file;

    ExpandArgument( p_ctx, cmdline, 0, unit, UNIT );
    ExpandArgument( p_ctx, cmdline, 1, file );

    return shared_ptr< CCommand >( new CSaveGromosCommand(m_action, unit, file) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}



