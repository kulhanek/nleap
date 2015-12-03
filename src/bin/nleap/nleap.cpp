// =============================================================================
// nLEaP - prepare input for the AMBER molecular mechanics programs
// -----------------------------------------------------------------------------
//    Copyright (C) 2010 Petr Kulhanek, kulhanek@chemi.muni.cz
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

#include <stdio.h>
#include <ErrorSystem.hpp>
#include "nleap.hpp"
#ifdef HAVE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif
#include <iostream>
#include <TerminalStr.hpp>
#include <FileSystem.hpp>
#include <boost/algorithm/string.hpp>
#include <openbabel/obconversion.h>

using namespace std;
using namespace boost;

//------------------------------------------------------------------------------

#define MAX_HISTORY_LENGTH 100

//------------------------------------------------------------------------------

const char*  CNLEaP::Prompt = NULL;
char*        CNLEaP::Line = NULL;

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CNLEaP::CNLEaP(void)
{
    Error = false;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

int CNLEaP::Init(int argc,char* argv[])
{
    // encode program options
    int result = Options.ParseCmdLine(argc,argv);

    // should we exit or was it error?
    if( result != SO_CONTINUE ) return(result);

    // attach verbose stream to terminal stream and set desired verbosity level
    vout.Attach(Console);
    vout.Verbosity(CVerboseStr::low);

    vout << low;
    Context.WriteHeader(vout);

    // init openbabel
    OpenBabel::OBConversion     co;

    // ----------------------------------------------------
    // init NLEaP configuration storage
    CFileName home_dir = GetSystemVariable("HOME");
    if( home_dir == NULL ){
        ES_ERROR("environmental variable HOME is not defined");
        return( SO_USER_ERROR );
    }

    NLEaPConfigDir = home_dir / ".nleap";

    // create directory - does not matter if it exists
    CFileSystem::CreateDir( NLEaPConfigDir );

    // set configuration names
    HistoryFileName = NLEaPConfigDir / "history";
    UserConfigName = NLEaPConfigDir / "context";

    if( Options.GetOptClearHistory() ) {
        FILE* p_file = fopen( HistoryFileName, "wn" );
        vout << "> Clearing command history file ..." << endl;
        if( p_file ) fclose(p_file);
    }

#ifdef HAVE_READLINE
    if( ! Options.GetOptNoHistory() ){
        // load ReadLine history
        vout << "> Reading command history file ..." << endl;
        read_history(HistoryFileName);
    }
#endif

    // load nleap context setup
    Context.SetSetupName( std::string(UserConfigName) );

    if( ! Options.GetOptDefaultSetup() ){
        vout << "> Loading user context setup ..." << endl;
        Context.LoadContextSetup();
    } else {
        vout << "> Skipping user context setup (using default setup) ..." << endl;
    }

    Context.SetOut(&vout);

    if( Options.GetOptVariables() != NULL ){
        vout << "> External variables ..." << endl;
        // get variables
        vector<string> variables;
        string varlist = string(Options.GetOptVariables());
        try {
            split( variables, varlist, is_any_of(",") );
        } catch(...) {

        }

        vector<string>::iterator it = variables.begin();
        vector<string>::iterator et = variables.end();

        while( it != et ){
            string var = *it;

            if( var.find('=') == string::npos ){
                var = var + " = " + string(GetSystemVariable(var.c_str()));
            }
            vout << "  " << var << endl;
            if( Context.Process(var) == false ) return(false);
            it++;
        }
    }

    return( result );
}

//------------------------------------------------------------------------------

bool CNLEaP::Run(void)
{
    //
    vout << endl;

    if( CTerminal::IsTerminal(stdin) && (Options.GetOptScriptName() == NULL) ) {
        if( RunInteractive() == false ) return(false);
    } else {
        if( Options.GetOptScriptName() == NULL ){
            if( RunNonInteractive(std::cin) == false ) return(false);
        } else {
            string file;
            try {
                file = Context.FindFile(string(Options.GetOptScriptName()));
            } catch(...) {
                vout << "<red><b>Error:</b></red> unable to find script file '" << Options.GetOptScriptName() << "'" << endl;
                return(false);
            }
            vout << "Loading " << file << endl;
            std::ifstream ifs(file.c_str());
            if( ! ifs ){
                vout << "<red>Error:</red> unable to open script file" << endl;
                return(false);
            }

            if( RunNonInteractive(ifs) == false ){
                if( Options.GetOptInteractive() == false ) return(false);
            }
            if( Options.GetOptInteractive() ){
                if( RunInteractive() == false ) return(false);
            }
        }
    }

    return(true);
}

//------------------------------------------------------------------------------

bool CNLEaP::RunNonInteractive(std::istream& stream)
{
    string line;
    while( getline(stream,line) != NULL ){
        if( line == "exit" ) return(true);
        if( line == "quit" ) return(true);
        if( Context.Process(line) == false ){
            Error = true;
            return(false);
        }
    }

    return(true);
}

//------------------------------------------------------------------------------

bool CNLEaP::RunInteractive(void)
{
    Prompt = "[nleap]$ ";
    while( RLGets() != NULL ){
        if( strstr(Line,"exit") != NULL ) return(true);
        if( strstr(Line,"quit") != NULL ) return(true);
        Context.Process(Line);
    }

    printf("\n");
    return(true);
}

//------------------------------------------------------------------------------

/* Read a string, and return a pointer to it. Returns NULL on EOF. */
char* CNLEaP::RLGets(void)
{
#ifdef HAVE_READLINE
    // If the buffer has already been allocated,
    // return the memory to the free pool.
    if( Line ) {
        free (Line);
        Line = NULL;
    }

    // Get a line from the user.
    Line = readline(Prompt);

    // If the line has any text in it,
    //   save it on the history.
    if( Line && *Line ){
        add_history(Line);
    }
#else
    static char line_buffer[65536];
    Line = fgets(line_buffer,65535,stdin);
    Line[65535] = '\n';
#endif
    return(Line);
}

//------------------------------------------------------------------------------

bool CNLEaP::Finalize(void)
{
    flush(Console);

    // save command history
    FILE* p_file = fopen( HistoryFileName, "wa" );
    if( p_file ) fclose(p_file);

#ifdef HAVE_READLINE
    // append the history
    append_history( MAX_HISTORY_LENGTH, HistoryFileName );
    history_truncate_file( HistoryFileName, MAX_HISTORY_LENGTH );
#endif

    // save nleap context setup
    vout << endl;
    vout << "> Saving user context setup ..." << endl;
    Context.SaveContextSetup();   

    Context.WriteFooter(vout,Error);
    Context.CloseLogFile(Error);

    return(true);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

const CSmallString CNLEaP::GetSystemVariable(const CSmallString& name)
{
    if( name == NULL ) return("");
    return(CSmallString(getenv(name)));
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

