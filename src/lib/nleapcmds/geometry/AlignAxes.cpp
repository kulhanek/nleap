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

#include <geometry/AlignAxes.hpp>
#include <engine/Context.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CAlignAxesCommand::CAlignAxesCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

// -------------------------------------------------------------------------

CAlignAxesCommand::CAlignAxesCommand( const string& cmd_name, const CEntityPtr& unit )
    : CCommand( cmd_name, cmd_name ), m_unit( unit )
{

}

// -------------------------------------------------------------------------

const char* CAlignAxesCommand::Info(  EHelp type ) const
{
    if( type == help_group )
    {
        return("geometry");
    }

    if( type == help_short )
    {
        return("align the principal axes");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>alignAxes</b> - align the principal axes\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>alignAxes</b> <u>unit</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Align the principal axes of the ATOMs within <u>unit</u> along the "
    "coordinate axes. This is done by calculating the moment of inertia "
    "of the UNIT, and then diagonalizing the resulting matrix and aligning "
    "the eigenvectors along the coordinate axes.\n"
    "This command modifies the coordinates of the UNIT. It may be "
    "especially useful for preparing long solutes such as nucleic acids "
    "for solvation.\n"
    );
}

// -------------------------------------------------------------------------

void CAlignAxesCommand::Exec( CContext* p_ctx )
{

}

// -------------------------------------------------------------------------

shared_ptr< CCommand > CAlignAxesCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 1, 1 );

    CEntityPtr object;

    ExpandArgument( p_ctx, cmdline.GetArgs(), 0, object, UNIT );

    return shared_ptr< CCommand >( new CAlignAxesCommand(m_action, object) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}



