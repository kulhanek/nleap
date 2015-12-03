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

#include <List.hpp>
#include <iomanip>
#include <engine/Context.hpp>
#include <stdexcept>
#include <core/ForwardIterator.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
using namespace std;

CListCommand::CListCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
    m_change_state = false;
}

//------------------------------------------------------------------------------

CListCommand::CListCommand( const string& cmd_name, const string& type_name )
    : CCommand( cmd_name, cmd_name ), m_type( type_name )
{
    m_change_state = false;
}

//------------------------------------------------------------------------------

const char* CListCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("control");
    }
    if( type == help_short )
    {
        return("list objects currently defined");
    }

    return(
    "<b>NAME:</b>\n"
    "       <b>list</b> - list objects currently defined\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>list</b>\n"
    "       <b>list</b> includetypes\n"
    "       <b>list</b> [<u>type</u>]\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "List either all variables (without or with their included types) or variables of given <u>type</u> that are currently "
    "defined in the LEaP environment. Recognized types are as follows:\n"
    "   UNIT    - all units\n"
    "   RESIDUE - all residues\n"
    "   ATOM    - all atoms\n"
    "   LIST    - any list\n"
    "   STRING  - any string\n"
    "   NUMBER  - any number\n"
    "   AMBERFF - AMBER force field parameters\n"
    );
}

//------------------------------------------------------------------------------

void CListCommand::Exec( CContext* p_ctx )
{
    CForwardIterator it = p_ctx->database()->BeginVariables();
    CForwardIterator et = p_ctx->database()->EndVariables();

    if( m_type == "" ) {
        // list all variables as name sequence
        while( it != et ) {
            p_ctx->out() <<  it->GetName() << " ";
            it++;
        }
        p_ctx->out() << endl;
        return;
    }

    if( m_type == "includetypes" ) {
        // list all variables as the list, include also type of objects
        size_t len = 0;
        while( it != et ) {
            if( len < it->GetName().size() ) len = it->GetName().size();
            it++;
        }

        p_ctx->out() << resetiosflags(ios_base::right) << setiosflags(ios_base::left);

        it = p_ctx->database()->BeginVariables();
        while( it != et ) {
            p_ctx->out() << setw(len+2) << it->GetName();
            CEntityPtr obj = it->Get<CEntityPtr>(VALUE);
            if( obj ) {
                p_ctx->out() << " (" << obj->GetType().GetName() << ")" << endl;
            } else {

            }
            it++;
        }
        return;
    }

    // list only variable of given type
    while( it != et ) {
        CEntityPtr obj = it->Get<CEntityPtr>(VALUE);
        if( obj ) {
            if( obj->GetType().GetName() == m_type ) {
                p_ctx->out() <<  it->GetName() << " ";
            }
        }
        it++;
    }
    p_ctx->out() << endl;
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CListCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 0 , 1);

    string type;

    if( cmdline.GetArgs().size() == 1 ){
        ExpandArgument( p_ctx, cmdline, 0, type );
        if( (type != "UNIT")    &&
            (type != "RESIDUE") &&
            (type != "ATOM")    &&
            (type != "AMBERFF") &&
            (type != "LIST")    &&
            (type != "STRING")  &&
            (type != "NUMBER")  &&
            (type != "includetypes") )
        {
            WrongArgument( cmdline.GetArgs(), 0, "unsupported type or action" );
        }
    }
    return shared_ptr< CCommand >( new CListCommand( m_action, type ) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}

    

    
