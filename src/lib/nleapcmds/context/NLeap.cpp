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

#include <NLeap.hpp>
#include <iostream>
#include <engine/Context.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CNLeapCommand::CNLeapCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
    m_change_state = false;
}

//------------------------------------------------------------------------------

CNLeapCommand::CNLeapCommand( const string& cmd_name,  const vector<string>& args )
    : CCommand( cmd_name, cmd_name ), m_args( args )
{
    m_change_state = false;
}

//------------------------------------------------------------------------------

const char* CNLeapCommand::Info( CCommand::EHelp type ) const
{
    if( type == help_group )
    {
        return("context");
    }

    if( type == help_short )
    {
        return("nLEaP administrative command");
    }
    return(
    "<b>NAME:</b>\n"
    "   <b>nleap</b> - nLEaP administrative command\n"
    "\n"
    "<b>USAGE:</b>\n"
    "   <b>nleap</b>\n"
    "       print the current context setup\n"
    "\n"
    "   <b>nleap</b> verbosity [<u>level</u>]\n"
    "       either set verbosity to <u>level</u> or print current verbosity level\n"
    "       supported levels: 0 - none, 1 - low, 2 - medium, 3 - high, 4 - debug\n"
    "\n"
    "   <b>nleap</b> echo [on/off]\n"
    "       either set the echoing of excuted commands or print current echoing setup\n"
    "\n"
    "   <b>nleap</b> buffer [<u>size</u>]\n"
    "       either set history buffer to <u>size</u> or print current history buffer size\n"
    "\n"
    "   <b>nleap</b> path\n"
    "       shows the set of directories that are used to search nLEaP scripts and parameter files\n"
    "\n"
    "   <b>nleap</b> addpath <u>path</u>\n"
    "       add <u>path</u> to the set of directories that are used to search nLEaP scripts and parameter files\n"
    "\n"
    "   <b>nleap</b> rmpath <u>path</u>\n"
    "       remove <u>path</u> from the set of directories that are used to search nLEaP scripts and parameter files\n"
    "\n"
    "   <b>nleap</b> aliases\n"
    "       show currently defined command aliases\n"
    "\n"
    "   <b>nleap</b> addalias <u>alias</u> <u>command</u>\n"
    "       set command <u>alias</u> for nLEaP <u>command</u>\n"
    "\n"
    "   <b>nleap</b> rmalias <u>alias</u>\n"
    "       remove command <u>alias</u>\n"
    "\n"
    "   <b>nleap</b> deafult\n"
    "       reset context setup to nLEaP default values\n"
    "\n"
    "   <b>nleap</b> loadSetup\n"
    "       load user predefined context setup\n"
    "\n"
    "   <b>nleap</b> saveSetup\n"
    "       save current context setup as user predefined one\n"
    );
}

//------------------------------------------------------------------------------

void CNLeapCommand::Exec( CContext* p_ctx )
{

    // ------------------------------------------
    // describe context
    if( m_args.size() == 0 ){
        // describe context
        p_ctx->Desc(p_ctx->out());
        return;
    }

    // ------------------------------------------
    // handle verbosity

    if( m_args[0] == "verbosity" ) {
        CheckNumberOfArguments( m_args, 1, 2 );

        if( m_args.size() == 1 ){
            p_ctx->out() << low << "Verbosity: " << p_ctx->Get<int>(VERBOSITY) << endl;
            return;
        }

        if( m_args.size() == 2 ) {
            int vlevel = 0;
            ExpandArgument( p_ctx, m_args, 1, vlevel );

            if( (vlevel < 0) || (vlevel > 5) ){
                WrongArgument( m_args, 1, "level has to be in the range from 0 to 5");
            }

            p_ctx->SetVerbosity( vlevel );
            p_ctx->out() << medium << "Verbosity changed to " << p_ctx->Get<int>(VERBOSITY) << endl;
            return;
        }
    }

    // ------------------------------------------
    // handle echo

    if( m_args[0] == "echo" ) {
        CheckNumberOfArguments( m_args, 1, 2 );

        if( m_args.size() == 1 ){
            p_ctx->out() << low << "Echo: " << p_ctx->Get<string>(ECHO) << endl;
            return;
        }

        if( m_args.size() == 2 ) {
            string emode;
            ExpandArgument( p_ctx, m_args, 1, emode );
            if( (emode != "off") && (emode != "on" ) ){
                WrongArgument( m_args, 1, "on/off is allowed parameter");
            }
            p_ctx->Set( ECHO, emode );
            p_ctx->out() << medium << "Verbosity changed to '" << p_ctx->Get<string>(ECHO) << "'" << endl;
            return;
        }
    }

    // ------------------------------------------
    // handle buffer

    if( m_args[0] == "buffer" ) {
        CheckNumberOfArguments( m_args, 1, 2 );

        if( m_args.size() == 1 ){
            p_ctx->out() << low << "Max buffer size: " << p_ctx->Get<int>(MAXHIST) << endl;
            return;
        }

        if( m_args.size() == 2 ) {
            int vsize = 0;
            ExpandArgument( p_ctx, m_args, 1, vsize );
            if( vsize < 0 ){
                WrongArgument( m_args, 1, "buffer size cannot be a negative number");
            }
            p_ctx->Set( MAXHIST, vsize );
            p_ctx->out() << medium << "Max buffer size changed to " << p_ctx->Get<int>(MAXHIST) << endl;
            return;
        }
    }

    // ------------------------------------------
    // handle path

    if( m_args[0] == "path" ) {
        CheckNumberOfArguments( m_args, 1, 1 );

        p_ctx->PrintPaths( p_ctx->out() );
        return;
    }

    // ------------------------------------------
    // handle addpath

    if( m_args[0] == "addpath" ) {
        CheckNumberOfArguments( m_args, 2, 2 );

        string path;
        ExpandArgument( p_ctx, m_args, 1, path );

        p_ctx->AddPath( path );
        return;
    }

    // ------------------------------------------
    // handle rmpath

    if( m_args[0] == "rmpath" ) {
        CheckNumberOfArguments( m_args, 2, 2 );

        string path;
        ExpandArgument( p_ctx, m_args, 1, path );

        p_ctx->RemovePath( path );
        return;
    }

    // ------------------------------------------
    // handle aliases
    if( m_args[0] == "aliases" ) {
        CheckNumberOfArguments( m_args, 1, 1 );

        p_ctx->PrintAliases( p_ctx->out() );
        return;
    }

    // ------------------------------------------
    // handle addalias
    if( m_args[0] == "addalias" ) {
        CheckNumberOfArguments( m_args, 3, 3 );

        string alias,cmd;
        ExpandArgument( p_ctx, m_args, 1, alias );
        ExpandArgument( p_ctx, m_args, 2, cmd );

        p_ctx->AddAlias( alias, cmd );
        return;
    }

    // ------------------------------------------
    // handle rmalias
    if( m_args[0] == "rmalias" ) {
        CheckNumberOfArguments( m_args, 2, 2 );

        string alias;
        ExpandArgument( p_ctx, m_args, 1, alias );

        p_ctx->RemoveAlias( alias );
        return;
    }

    // ------------------------------------------
    // handle default

    if( m_args[0] == "default" ) {
        CheckNumberOfArguments( m_args, 1, 1 );

        p_ctx->InitDefault( );
        return;
    }

    // ------------------------------------------
    WrongArgument( m_args, 0, "illegal action");
}

//-------------------------------------------------------------------------------

shared_ptr< CCommand > CNLeapCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );

    return shared_ptr< CCommand >( new CNLeapCommand( m_action, cmdline.GetArgs() ) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}


