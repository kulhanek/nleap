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

#include <engine/Context.hpp>
#include <engine/prefix.h>
#include <vector>
#include <engine/Parser.hpp>
#include <engine/Command.hpp>
#include <stdexcept>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <FileName.hpp>
#include <types/Factory.hpp>
#include <SmallTimeAndDate.hpp>
#include <iomanip>

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CContext::CContext()
    : CEntity(CONTEXT)
{
    int id;
    id = m_index_counter.GetIndex();
    SetId(id);

    InitDefault();

    m_log_stream.DisconnectFromStdout();
    m_out.AttachLogStream(m_log_stream);

    m_undo_level = 0;
    m_trans_level = 0;

    // create nodes
    int top_id = m_index_counter.GetTopIndex();

    // create command aliases node
    CEntityPtr aliases = CFactory::CreateNode( top_id, "aliases" );
    AddChild(aliases);

    // create database dbhistory node
    CEntityPtr dbhistory = CFactory::CreateNode( top_id, "dbhistory" );
    AddChild(dbhistory);

    // temporary node
    CEntityPtr tmp = CFactory::CreateNode( top_id, "tmp" );
    AddChild(tmp);

    m_index_counter.SetTopIndex( top_id );

    // access database - to create a initial node
    database();
}

// -------------------------------------------------------------------------

CContext::~CContext(void)
{

}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CContext::InitDefault(void)
{
    // default settings
    Set(ECHO,"off");
    SetVerbosity(1);
    Set(MAXHIST,5);

    // add paths
    CFileName prefix_path( GetPrefix().c_str() );
    CFileName lib_path;

    lib_path = prefix_path / "share" / "leap" / "cmd";
    AddPath((const char*)lib_path);

    lib_path = prefix_path / "share" / "leap" / "lib";
    AddPath((const char*)lib_path);

    lib_path = prefix_path / "share" / "leap" / "parm";
    AddPath((const char*)lib_path);
}

// -------------------------------------------------------------------------

void CContext::SetSetupName(const string& name)
{
    m_setup_file = name;
}

// -------------------------------------------------------------------------

void CContext::SetLogFileName(const string& name)
{
    m_log_stream.Open(name.c_str());
    WriteHeader(m_log_stream);
}

// -------------------------------------------------------------------------

void CContext::CloseLogFile(bool error)
{
    WriteFooter(m_log_stream,error);
    m_log_stream.Close();
}

// -------------------------------------------------------------------------

void CContext::WriteHeader(ostream& str)
{
    CSmallTimeAndDate dt;
    dt.GetActualTimeAndDate();

    str << endl;
    str << "# ==============================================================================" << endl;
    str << "# nLEaP started at " << dt.GetSDateAndTime() << endl;
    str << "# " << endl;
    str << "# (c) 2011 Petr Kulhanek" << endl;
    str << "#     Central European Institute of Technology (CEITEC), Masaryk University" << endl;
    str << "# (c) 2010 Petr Kulhanek" << endl;
    str << "#     Faculty of Science, Masaryk University" << endl;
    str << "# " << endl;
    str << "# This program is released under the GNU GPL license v2.0. See doc/licenses for " << endl;
    str << "# further details." << endl;
    str << "# ==============================================================================" << endl;
    str << endl;
}

// -------------------------------------------------------------------------

void CContext::WriteFooter(ostream& str,bool error)
{
    CSmallTimeAndDate dt;
    dt.GetActualTimeAndDate();

    str << endl;
    str << "# ==============================================================================" << endl;
    if( error ){
        str << "# nLEaP terminated <red><b>unexpectedly</b></red> at " << dt.GetSDateAndTime() << endl;
    } else {
        str << "# nLEaP terminated <blue><b>succesfully</b></blue> at " << dt.GetSDateAndTime() << endl;
    }
    str << "# ==============================================================================" << endl;
    str << endl;
}

// -------------------------------------------------------------------------

bool CContext::LoadContextSetup(void)
{
    return( false );
}

// -------------------------------------------------------------------------

bool CContext::SaveContextSetup(void)
{
    return( false );
}

// -------------------------------------------------------------------------
// #########################################################################
// -------------------------------------------------------------------------

void CContext::SetOut(ostream* p_out)
{
    m_out.Attach(*p_out);
}

// -------------------------------------------------------------------------

CVerboseStr& CContext::out(void)
{
    return(m_out);
}

// -------------------------------------------------------------------------

CDatabasePtr CContext::database(void)
{
    CEntityPtr last_child;
    CEntityPtr dbs = FindChild( "dbhistory" );
    if( dbs->NumberOfChildren() > 0 ) {
        // get top child
        last_child = dbs->GetChild( dbs->NumberOfChildren() - m_undo_level - 1 );
    } else {
        // no top database create new one
        int top_id = m_index_counter.GetTopIndex();
        last_child = CFactory::CreateDatabase( top_id );
        m_index_counter.SetTopIndex( top_id );
        dbs->AddChild(last_child);
        m_undo_level = 0;
    }
    if( ! last_child ) {
        throw runtime_error( "internal error CContext::database");
    }

    return( dynamic_pointer_cast<CDatabase>(last_child) );
}

// -------------------------------------------------------------------------

CEntityPtr CContext::tmp(void)
{
    return( FindChild("tmp") );
}

// -------------------------------------------------------------------------

string CContext::GetPrefix(void)
{
    return(PREFIX);
}

// -------------------------------------------------------------------------

bool CContext::Process(const string& line)
{
    out() << high;
    out() << "[nleap]$ " << line << endl;
    out() << low;

    if( line.empty() ) {
        return true;
    }

    // is line comment? - skip it here - parser process all data
    bool comment = false;
    for(unsigned int i = 0; i < line.size(); i++){
        if( line[i] == '#' ) {
            comment = true;
            break;
        }
        if( isspace(line[i]) == false ) break;
    }

    if( comment == true ){
        return(true);
    }

    m_pending += line + " ";
    CParser::ESyntaxStatus result = CParser::CheckSyntax( m_pending );

    switch(result) {
    case CParser::syntax_error:
        return false;

    case CParser::syntax_intermediate:
        return true;

    default:
        break;
    }

    string cmd = m_pending;

    m_pending = "";

    bool status = Run(cmd);

    return status;
}

// -------------------------------------------------------------------------

void CContext::StartTransaction(void)
{
    if( m_trans_level > 0 ) {
        // another transaction already in progress
        m_trans_level++;
        return;
    }

    // remove redo records
    CEntityPtr dbs = FindChild( "dbhistory" );
    if( m_undo_level > 0 ) {
        for(int i = 0; i < m_undo_level; i++ ) {
            dbs->RemoveLastChild( );
        }
    }

    // duplicate current record
    CEntityPtr last_child;
    if( dbs->NumberOfChildren() > 0 ) {
        // clone the last database
        last_child = dbs->GetChild( dbs->NumberOfChildren() - 1);
        int top_id = m_index_counter.GetTopIndex();
        last_child = last_child->Clone( top_id, top_id );
        m_index_counter.SetTopIndex( top_id );
    } else {
        // no top database create new one
        int top_id = m_index_counter.GetTopIndex();
        last_child = CFactory::CreateDatabase( top_id );
        m_index_counter.SetTopIndex( top_id );
    }
    if( ! last_child ) {
        throw runtime_error( "internal error CContext::start_transaction");
    }
    dbs->AddChild(last_child);
    m_undo_level = 0;

    // flag transaction active
    m_trans_rollback = false;
    m_trans_level = 1;
}

// -------------------------------------------------------------------------

void CContext::CommitTransaction(void)
{
    if( m_trans_level > 1 ) {
        // another transaction already in progress
        m_trans_level--;
        return;
    }
    if( m_trans_level <= 0 ) return; // unbalanced commit_transaction

    // was something wrong in past?
    if( m_trans_rollback ) {
        RollbackTransaction();
        return;
    }

    // commit transaction - actually nothing to do
    m_trans_rollback = false;
    m_trans_level = 0;
}

// -------------------------------------------------------------------------

void CContext::RollbackTransaction(void)
{
    if( m_trans_level > 1 ) {
        // another transaction already in progress
        m_trans_level--;
        m_trans_rollback = true;
        return;
    }
    if( m_trans_level <= 0 ) return; // unbalanced rollback_transaction

    // rollback transaction - remove last database
    CEntityPtr dbs = FindChild( "dbhistory" );

    if( dbs->NumberOfChildren() > 0 ) {
        dbs->RemoveLastChild( );
    }

    // unflag transaction
    m_trans_rollback = false;
    m_trans_level = 0;
}

// -------------------------------------------------------------------------

bool CContext::Run(const string& command)
{
    CParser parser;

    try {
        parser.Parse(command);

        if( parser.GetCmd().empty() ) return true; // empty line or comment

        // echo
        if( Get<string>(ECHO) == "on" ) {
            out() << low;
            if( ! parser.GetLHS().empty() ) {
                out() << parser.GetLHS() << " = ";
            }
            out() << parser.GetCmd() << " ";
            for(size_t i = 0; i < parser.GetArgs().size(); i++) {
                out() << parser.GetArgs()[i] << " ";
            }
            out() << endl;
        }

        CCommand* pcmd = CCommand::Find( parser.GetCmd() );
        if( pcmd == NULL ) {
            throw runtime_error( "unknown command <b>" + parser.GetCmd() + "</b>");
        }

        if( pcmd->IsChangingState() ) {
            StartTransaction();
        }

        try {
            // clonning must be after StartTransaction since it already manipulate with the database
            CCommandPtr comm = pcmd->Clone(this, parser);
            if( ! comm ) {
                throw runtime_error( "command not cloned - internal error");
            }

            // record command to command history list
            if( m_trans_level <= 1 ) {
                // do not record context changing commands
                if( strcmp(pcmd->Info(CCommand::help_group),"context") != 0 ) {
                    // well, this part should not fail
                    CEntityPtr list = database()->FindChild("_histcmds");
                    if( ! list ) {
                        throw runtime_error("list is NULL, internal error in CContext::run");
                    }
                    int index = m_index_counter.GetIndex();
                    CEntityPtr hist = CFactory::CreateString(index, command);
                    list->AddChild(hist);
                }
            }

            m_out << low;   // set default low verbosity output
            comm->Exec(this);
        } catch( std::exception& e ) {
            if( pcmd->IsChangingState() ) {
                RollbackTransaction();
            }
            throw runtime_error( e.what() );
        }

        if( pcmd->IsChangingState() ) {
            CommitTransaction();
        }
    } catch( std::exception& e ) {
        out() << "<b><red>Error:</red></b> " <<  e.what() << endl;
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------

void CContext::AddPath(const string& path)
{
    // remove path
    RemovePath( path );

    // add path to the beggining
    string old_path;
    string new_path;

    Get(PATH, old_path);

    if( ! old_path.empty() ) {
        new_path = path + "|" + old_path;
    } else {
        new_path = path;
    }

    Set(PATH, new_path);
}

// -------------------------------------------------------------------------

void CContext::RemovePath(const string& path)
{
    vector<string> dirs;
    try {
        split( dirs, Get<string>(PATH), is_any_of("|") );
    } catch(...) {

    }

    string new_paths;

    for( size_t i=0; i < dirs.size(); ++i ) {
        if( path != dirs[i] ) {
            if( ! new_paths.empty() ) {
                new_paths += "|";
            }
            new_paths += dirs[i];
        }
    }

    Set(PATH, new_paths);
}

// -------------------------------------------------------------------------

string CContext::FindFile(const string& name)
{
    std::ifstream is( name.c_str() );
    if( is ) {
        return name;
    }

    // does it contain slash?
    if( name.find('/') != string::npos ){
        throw std::runtime_error( "cannot find file " + name + " in specified path" );
    }

    string path;
    Get(PATH, path);

    if( path.empty() ) {
        throw std::runtime_error( "nLEaP PATH not set and file " + name + " was not found" );
    }

    vector<string> dirs;
    split( dirs, path, is_any_of("|") );

    for( size_t i=0; i < dirs.size(); ++i ) {
        CFileName full_name = CFileName(dirs[i].c_str()) / CFileName(name.c_str());

        std::ifstream is( full_name );

        if(is) {
            return (const char*)full_name;
        }
    }
    throw std::runtime_error( "cannot find file " + name + " in nLEaP PATH" );
}

// -------------------------------------------------------------------------

void CContext::SetVerbosity(int verbosity)
{
    Set(VERBOSITY, verbosity);
    switch(verbosity){
        case 0:
            m_out.Verbosity( CVerboseStr::none );
            break;
        case 1:
            m_out.Verbosity( CVerboseStr::low );
            break;
        case 2:
            m_out.Verbosity( CVerboseStr::medium );
            break;
    }


}

// -------------------------------------------------------------------------

void CContext::Desc(ostream& ofs)
{
    ofs << "NLEaP CONTEXT" << endl;

    ofs << "   # General" << endl;
    ofs << "   # ----------------------------------------------" << endl;
    ofs << "     Echo commands     : " << Get<string>(ECHO) << endl;
    ofs << "     Verbosity level   : " << Get<int>(VERBOSITY) << endl;

    CEntityPtr dbs = FindChild( "dbhistory" );

    ofs << endl;
    ofs << "   # Changes recording" << endl;
    ofs << "   # ----------------------------------------------" << endl;
    ofs << "     Max buffer size      : " << Get<int>(MAXHIST) << endl;
    ofs << "     Current buffer size  : " << dbs->NumberOfChildren() << endl;
    ofs << "     Available changes    : " << dbs->NumberOfChildren() - 1 << endl;
    if( dbs->NumberOfChildren() > 0 ) {
        ofs << "     Undo level           : " <<  m_undo_level << endl;
    } else {
        ofs << "     Undo level           : " << 0 << endl;
    }

    ofs << endl;
    PrintPaths( ofs );

    ofs << endl;
    PrintAliases( ofs );
}

// -------------------------------------------------------------------------

void CContext::PrintPaths(ostream& ofs)
{
    ofs << "   # Searching paths" << endl;
    ofs << "   # ----------------------------------------------" << endl;
    vector<string> dirs;
    split( dirs, Get<string>(PATH), is_any_of("|") );

    for( size_t i=0; i < dirs.size(); ++i ) {
        ofs << "     > " << dirs[i] << endl;
    }
}

// -------------------------------------------------------------------------

void CContext::Undo(int level)
{
    CEntityPtr dbs = FindChild( "dbhistory" );
    m_undo_level += level;
    if( m_undo_level < 0 ) m_undo_level = 0;
    if( m_undo_level > (int)dbs->NumberOfChildren() - 1 ) m_undo_level = dbs->NumberOfChildren() - 1;
}

//------------------------------------------------------------------------------

void CContext::Redo(int level)
{
    CEntityPtr dbs = FindChild( "dbhistory" );
    m_undo_level -= level;
    if( m_undo_level < 0 ) m_undo_level = 0;
    if( m_undo_level > (int)dbs->NumberOfChildren() - 1 ) m_undo_level = dbs->NumberOfChildren() - 1;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CContext::AddAlias(const string& str, const string& cmd)
{
    // is cmd nleap command?
    if( CCommand::Find(cmd) == NULL ){
        throw runtime_error("command '" + cmd + "' is not valid nLEaP command");
    }

    CEntityPtr aliases = FindChild( "aliases" );

    // is alias already defined?
    CEntityPtr alias = aliases->FindChild(str);
    if( alias ) {
        // update value
        CStringPtr salias = dynamic_pointer_cast<CString>(alias);
        salias->SetValue(cmd);
    } else {
        // create new alias
        int top_id = m_index_counter.GetTopIndex();
        CStringPtr salias = CFactory::CreateString(top_id,cmd);
        salias->SetName(str);
        aliases->AddChild(salias);
        m_index_counter.SetTopIndex( top_id );
    }
}

//------------------------------------------------------------------------------

void CContext::RemoveAlias(const string& str)
{
    CEntityPtr aliases = FindChild( "aliases" );

    // is alias already defined?
    CEntityPtr alias = aliases->FindChild(str);
    if( ! alias ){
        throw runtime_error("alias '" + str + "' does not exist");
    }

    // remove alias
    aliases->RemoveChild(alias);
}

//------------------------------------------------------------------------------

void CContext::PrintAliases(ostream& ofs)
{
    CEntityPtr aliases = FindChild( "aliases" );

    ofs << "   # Command Aliases" << endl;
    ofs << "   # Alias            Command                      " << endl;
    ofs << "   # ---------------- -----------------------------" << endl;

    CForwardIterator it = aliases->BeginChildren();
    CForwardIterator ie = aliases->EndChildren();

    while( it != ie ){
        ofs << "     ";
        ofs << setiosflags(ios_base::left);
        ofs << setw(16) << it->GetName() << " ";
        ofs << setw(20) << it->Get<string>(VALUE) << endl;
        it++;
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}



