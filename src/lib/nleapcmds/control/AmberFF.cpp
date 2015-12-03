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

#include <control/AmberFF.hpp>
#include <iostream>
#include <engine/Context.hpp>
#include <types/Unit.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CAmberFFCommand::CAmberFFCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
    m_change_state = false;
}

//------------------------------------------------------------------------------

CAmberFFCommand::CAmberFFCommand( const string& cmd_name, CUnitPtr unit, const string& action )
    : CCommand( cmd_name, cmd_name ), m_unit( unit ), m_ffaction( action )
{
    m_change_state = false;
}

//------------------------------------------------------------------------------

CAmberFFCommand::CAmberFFCommand( const string& cmd_name, const string& t1, const string& t2, const string& t3, const string& t4 )
    : CCommand( cmd_name, cmd_name ), m_t1( t1 ), m_t2( t2 ), m_t3( t3 ), m_t4( t4 )
{
    m_change_state = false;
}

//------------------------------------------------------------------------------

const char* CAmberFFCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("control");
    }

    if( type == help_short )
    {
        return("print info about AMBER force field");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>amberFF</b> - print info about AMBER force field\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>amberFF</b> <u>unit</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "\n"
    );
}

//------------------------------------------------------------------------------

void CAmberFFCommand::Exec( CContext* p_ctx )
{
    if( (! m_unit) && m_t1.empty() ){
        // list available AMBER parameter sets
        CAmberFF::ListFFs(p_ctx->database(),p_ctx->out());
        return;
    }

    if( m_unit ){
        // list parameters for given unit
    }

    if( (! m_t1.empty()) && m_t2.empty() && m_t3.empty() && m_t4.empty() ){
        // get type parameters
        CAmberFF::DescType(p_ctx->database(),m_t1,p_ctx->out());
        return;
    }

    if( (! m_t1.empty()) && (! m_t2.empty()) && m_t3.empty() && m_t4.empty() ){
        // get bond parameters
        CAmberFF::DescBond(p_ctx->database(),m_t1,m_t2,p_ctx->out());
        return;
    }

    if( (! m_t1.empty()) && (! m_t2.empty()) && (! m_t3.empty()) && m_t4.empty() ){
        // get angle parameters
        CAmberFF::DescAngle(p_ctx->database(),m_t1,m_t2,m_t3,p_ctx->out());
        return;
    }

    if( (! m_t1.empty()) && (! m_t2.empty()) && (! m_t3.empty()) && (! m_t4.empty()) ){
        // get torsion parameters
        CAmberFF::DescTorsion(p_ctx->database(),m_t1,m_t2,m_t3,m_t4,p_ctx->out());
        return;
    }
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CAmberFFCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 0, 4 );

    int nargs = cmdline.GetArgs().size();

    CEntityPtr  obj;
    CUnitPtr    unit;
    string      action = "all";
    string      t1,t2,t3,t4;

    if( nargs == 0 ){
        return shared_ptr< CCommand >( new CAmberFFCommand( m_action, unit, action ) );
    }

    if( nargs == 1 ){
        try {
            ExpandArgument( p_ctx, cmdline, 0, obj, UNIT );
            unit = dynamic_pointer_cast<CUnit>(obj);
            return shared_ptr< CCommand >( new CAmberFFCommand( m_action, unit, action ) );
        } catch(...) {
            ExpandArgument( p_ctx, cmdline, 0, t1 );
            return shared_ptr< CCommand >( new CAmberFFCommand( m_action, t1, t2, t3, t4 ) );
        }
    }

    if( nargs == 2 ){
        try {
            ExpandArgument( p_ctx, cmdline, 0, obj, UNIT );
            unit = dynamic_pointer_cast<CUnit>(obj);
        } catch(...) {
            ExpandArgument( p_ctx, cmdline, 0, t1 );
            ExpandArgument( p_ctx, cmdline, 1, t2 );
            return shared_ptr< CCommand >( new CAmberFFCommand( m_action, t1, t2, t3, t4 ) );
        }

        ExpandArgument( p_ctx, cmdline, 1, action );
        return shared_ptr< CCommand >( new CAmberFFCommand( m_action, unit, action ) );
    }

    ExpandArgument( p_ctx, cmdline, 0, t1 );
    ExpandArgument( p_ctx, cmdline, 1, t2 );
    ExpandArgument( p_ctx, cmdline, 2, t3 );
    if( nargs == 4 ){
        ExpandArgument( p_ctx, cmdline, 3, t4 );
    }
    return shared_ptr< CCommand >( new CAmberFFCommand( m_action, t1, t2, t3, t4 ) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}

