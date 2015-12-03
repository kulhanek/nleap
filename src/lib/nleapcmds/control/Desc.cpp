// =============================================================================
// nLEaP - A molecular manipulation program and coding environment
// -----------------------------------------------------------------------------
//     Copyright (C) 2010 Petr Kulhanek, kulhanek@chemi.muni.cz
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

#include <Desc.hpp>
#include <iomanip>
#include <engine/Context.hpp>
#include <stdexcept>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

using namespace std;

CDescCommand::CDescCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
    m_change_state = false;
}

// -------------------------------------------------------------------------

CDescCommand::CDescCommand( const string& cmd_name,  string& obj )
    : CCommand( cmd_name, cmd_name ), m_object( obj )
{
    m_change_state = false;
}

// -------------------------------------------------------------------------

const char* CDescCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("control");
    }
    if( type == help_short )
    {
        return("print a description of object");
    }

    return(
    "<b>NAME:</b>\n"
    "       <b>desc</b> - print a description of object\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>desc</b>\n"
    "       <b>desc</b>     <u>variable>\n"
    "       <b>desc</b>     <u>object</u>\n"
     "\n"
    "<b>DESCRIPTION:</b>\n"
    "Print a description of the <u>object</u> or the object referenced by the <u>variable>.\n"
    );
}

//------------------------------------------------------------------------------

void CDescCommand::Exec( CContext* p_ctx )
{
    if( m_object.empty() ){
        p_ctx->database()->Desc( p_ctx->out() );
    } else {
        p_ctx->database()->DescSubObj( m_object, p_ctx->out() );
    }
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CDescCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 0, 1);

    string obj;
    if( cmdline.GetArgs().size() == 1 ){
        ExpandArgument(p_ctx, cmdline, 0, obj);
    }

    return shared_ptr< CCommand >( new CDescCommand( m_action, obj ) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}

