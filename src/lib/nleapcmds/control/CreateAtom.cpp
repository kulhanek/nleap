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

#include <CreateAtom.hpp>
#include <iostream>
#include <engine/Context.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CCreateAtomCommand::CCreateAtomCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CCreateAtomCommand::CCreateAtomCommand( const string& cmd_name, const string& var_name, const string& object_name,
                                          const string& object_type, double object_charge )
    : CCommand( cmd_name, cmd_name ), m_var( var_name ), m_name( object_name ),
      m_type( object_type ), m_charge( object_charge )
{
}

//------------------------------------------------------------------------------

const char* CCreateAtomCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("control");
    }

    if( type == help_short )
    {
        return("create a new atom");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>createAtom</b> - create a new atom\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <u>variable</u> = <b>createAtom</b> <u>name</u> <u>type</u> <u>charge</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Create a new ATOM with <u>name</u>, <u>type</u>, and <u>charge</u> and put it to <u>variable</u>.\n"
    );
}

//------------------------------------------------------------------------------

void CCreateAtomCommand::Exec( CContext* p_ctx )
{
    CDatabasePtr db = p_ctx->database();
    int top_id = p_ctx->m_index_counter.GetTopIndex();
    CEntityPtr obj = db->CreateAtom( top_id, m_name);
    obj->Set(TYPE,m_type);
    obj->Set(CHARGE,m_charge);
    db->SetVariable( top_id, m_var, obj );
    p_ctx->m_index_counter.SetTopIndex( top_id );
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CCreateAtomCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    AssigmentRequired( cmdline );
    CheckNumberOfArguments( cmdline, 3, 3);

    string var;
    string name;
    string type;
    double charge;

    ExpandLHS( p_ctx, cmdline, var );
    ExpandArgument( p_ctx, cmdline, 0, name );
    ExpandArgument( p_ctx, cmdline, 1, type );
    ExpandArgument( p_ctx, cmdline, 2, charge );

    return shared_ptr< CCommand >( new CCreateAtomCommand(m_action, var, name, type, charge) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}


