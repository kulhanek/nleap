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

#include <geometry/Translate.hpp>
#include <engine/Context.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CTranslateCommand::CTranslateCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

// -------------------------------------------------------------------------

CTranslateCommand::CTranslateCommand( const string& cmd_name, const string& obj, const string& vec )
    : CCommand( cmd_name, cmd_name ), m_object( obj ), m_vector( vec )
{
}

// -------------------------------------------------------------------------

const char* CTranslateCommand::Info(  EHelp type ) const
{
    if( type == help_group )
    {
        return("geometry");
    }

    if( type == help_short )
    {
        return("translate atoms");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>translate</b> - translate atoms\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>translate</b> <u>atoms</u> <u>direction</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Translate all of the ATOMs within <u>atoms</u> (UNIT, RESIDUE, ATOM) by the vector defined by "
    "the three NUMBERs in the LIST <u>direction</u>."
    );
}

// -------------------------------------------------------------------------

void CTranslateCommand::Exec( CContext* p_ctx )
{

}

// -------------------------------------------------------------------------

shared_ptr< CCommand > CTranslateCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 2 , 2);

    string object;
    string vector;

    ExpandArgument( p_ctx , cmdline.GetArgs() , 0 , object );
    ExpandArgument( p_ctx , cmdline.GetArgs() , 1 , vector );

    return shared_ptr< CCommand >( new CTranslateCommand(m_action, object, vector) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}



