#ifndef NLEAP_ENGINE_CONTEXT_H
#define NLEAP_ENGINE_CONTEXT_H
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

#include <NLEaPMainHeader.hpp>
#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include <types/Database.hpp>
#include <core/PredefinedKeys.hpp>
#include <IndexCounter.hpp>
#include <VerboseStr.hpp>
#include <IndexCounter.hpp>
#include <TerminalStr.hpp>

namespace nleap {
// -----------------------------------------------------------------------------
/*

context (+ context setup)
 |-> history (history of database changes)
       |-> database1
       |-> database2
       |-> ....

*/
// -----------------------------------------------------------------------------

using namespace std;
using boost::shared_ptr;

// -----------------------------------------------------------------------------

/// \brief context of leap session
class NLEAP_PACKAGE CContext : public CEntity
{
public:

    CContext(void);
    virtual ~CContext(void);

public:
    /// init default settings
    virtual void InitDefault(void);

    /// set context setup file name
    void SetSetupName(const string& name);

    /// set log file name
    void SetLogFileName(const string& name);

    /// close log file name
    void CloseLogFile(bool error);

    /// write header
    void WriteHeader(ostream& str);

    /// write footer
    void WriteFooter(ostream& str,bool error);

    /// load context setup
    bool LoadContextSetup(void);

    /// save context setup
    bool SaveContextSetup(void);

    /// set output stream
    void SetOut(ostream* p_out);

    /// return output stream
    CVerboseStr& out(void);

    /// return current database object
    CDatabasePtr database(void);

    /// return tmp node
    CEntityPtr tmp(void);

    /// return nleap installation prefix
    static string GetPrefix(void);

    /// process line
    bool Process(const string& line);

    /// start transaction
    void StartTransaction(void);

    /// commit transaction
    void CommitTransaction(void);

    /// rollback transaction
    void RollbackTransaction(void);

    /// source commands from a stream in a single transaction
    bool Source(const istream& is);

    /// run command in given context
    bool Run(const string& command);

    /// add path to PATH
    void AddPath(const string& path);

    /// remove path to PATH
    void RemovePath(const string& path);

    /// find file in search path
    string FindFile(const string& name);

    /// set verbosity
    void SetVerbosity(int verbosity);

    /// describe context setup
    void Desc(ostream& ofs);

    /// print paths
    void PrintPaths(ostream& ofs);

    /// undo last action
    void Undo(int level);

    /// redo last action
    void Redo(int level);

    /// add alias
    void AddAlias(const string& str, const string& cmd);

    /// remove alias
    void RemoveAlias(const string& str);

    /// list all aliases
    void PrintAliases(ostream& ofs);

// section of public data ------------------------------------------------------
public:
    CIndexCounter   m_index_counter;

// section of private data -----------------------------------------------------
private:
    string          m_setup_file;
    string          m_pending;
    CVerboseStr     m_out;
    CTerminalStr    m_log_stream;
    int             m_undo_level;           // current undo level
    int             m_trans_level;
    bool            m_trans_rollback;
};

// -----------------------------------------------------------------------------

typedef shared_ptr< CContext > CContextPtr;

// -----------------------------------------------------------------------------
}

#endif

