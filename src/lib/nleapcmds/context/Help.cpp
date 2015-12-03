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

#include <Help.hpp>
#include <iomanip>
#include <engine/Context.hpp>
#include <stdexcept>
#include <string.h>
#include <sstream>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CHelpCommand::CHelpCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
    m_change_state = false;
}

//------------------------------------------------------------------------------

CHelpCommand::CHelpCommand( const string& cmd_name, const string& name )
    : CCommand( cmd_name, cmd_name ), m_name(name)
{
    m_change_state = false;
}

//------------------------------------------------------------------------------

const char* CHelpCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("context");
    }
    if( type == help_short )
    {
        return("help subsystem");
    }

    static stringstream str;

    str.str("");
    str <<
    "<b>NAME:</b>\n"
    "       <b>help</b> - help subsystem\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>help</b>\n"
    "       <b>help</b> <u>command</u>\n"
    "       <b>help</b> <u>section</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "List available commands, print info about given <u>command</u>, or print info about "
    "given <u>section</u>.\n"
    "Following sections are recognized: ";

    map< string, CCommand* >::const_iterator   i;
    list< string >                              sections;

    for(i = CCommand::Begin(); i != CCommand::End(); ++i ) {
        sections.push_back(i->second->Info(CCommand::help_group));
    }

    sections.sort();
    sections.unique();

    list< string >::iterator is;

    int items = 0;
    for(is = sections.begin(); is != sections.end(); ++is ) {
        if( items > 0 ) str << ", ";
        str << "<i>" << (*is) << "</i>";
        items++;
    }
    str << "." << endl;

    return( str.str().c_str() );
}

//------------------------------------------------------------------------------

void CHelpCommand::Exec( CContext* p_ctx )
{
    // list all commands except deprecated
    if( m_name.empty() ) {

        map< string, CCommand* >::iterator i;
        unsigned int                        cmd_len = 0;

        for(i = CCommand::Begin(); i != CCommand::End(); ++i ) {
            if( cmd_len < i->first.size() ) cmd_len = i->first.size();
        }

        p_ctx->out() <<  endl;
        for(i = CCommand::Begin(); i != CCommand::End(); ++i ) {
            if( strcmp(i->second->Info(CCommand::help_group),"deprecated") != 0 ){
                p_ctx->out() << "<b>" << resetiosflags(ios_base::right) << setiosflags(ios_base::left) << setw(cmd_len+2);
                p_ctx->out() << i->first << setw(0) << "</b>";
                p_ctx->out() << i->second->Info(CCommand::help_short) <<  endl;
            }
        }
        p_ctx->out() <<  endl;
        return;
    }

    // print command help
    CCommand* p_cmd = CCommand::Find(m_name);
    if( p_cmd != NULL ) {
        p_ctx->out() << endl;
        p_ctx->out() << p_cmd->Info();
        p_ctx->out() << endl;
        return;
    }

    // print section

    // find section
    map< string, CCommand* >::iterator i;
    bool                                found = false;
    for(i = CCommand::Begin(); i != CCommand::End(); ++i ) {
        if( strcmp(i->second->Info(CCommand::help_group),m_name.c_str()) == 0 ){
            found = true;
            break;
        }
    }
    if( found == false ){
        throw runtime_error( "Command or command section <b>" + m_name + "</b> not found." );
    }

    unsigned int                        cmd_len = 0;

    for(i = CCommand::Begin(); i != CCommand::End(); ++i ) {
        if( cmd_len < i->first.size() ) cmd_len = i->first.size();
    }

    p_ctx->out() <<  endl;
    for(i = CCommand::Begin(); i != CCommand::End(); ++i ) {
        if( strcmp(i->second->Info(CCommand::help_group),m_name.c_str()) == 0 ){
            p_ctx->out() << "<b>" << resetiosflags(ios_base::right) << setiosflags(ios_base::left) << setw(cmd_len+2);
            p_ctx->out() << i->first << setw(0) << "</b>";
            p_ctx->out() << i->second->Info(CCommand::help_short) <<  endl;
        }
    }
    p_ctx->out() <<  endl;
    return;
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CHelpCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 0 , 1);

    string arg;
    if( cmdline.GetArgs().size() == 1 ){
        ExpandArgument( p_ctx, cmdline, 0, arg);
    }

    return shared_ptr< CCommand >( new CHelpCommand( m_action, arg ) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}


