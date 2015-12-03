#ifndef NLEAP_ENGINE_COMMAND_H
#define NLEAP_ENGINE_COMMAND_H
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
#include <map>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <engine/Parser.hpp>
#include <core/PredefinedKeys.hpp>

namespace nleap {
//------------------------------------------------------------------------------

using namespace boost;
using namespace std;

class   CContext;
class   CEntity;
class   CList;
typedef shared_ptr< CEntity >   CEntityPtr;
typedef shared_ptr< CList >     CListPtr;

//------------------------------------------------------------------------------
/// base class of all commands
/// \ingroup nleap
class NLEAP_PACKAGE CCommand {
public:
    enum EHelp {
        help_full,      // full help
        help_short,     // short help (single line)
        help_group      // group name
    };

    /// add an entry in control's command dictionary
    CCommand(const string& cmd_name);

    /// constructor used in clone
    CCommand(const string& cmd_name, const string& action);

    /// deconstructor
    virtual ~CCommand(void);

    /// return true if the command changing the state of database
    bool IsChangingState(void);

    /// print info about command
    virtual const char* Info(EHelp type = help_full) const = 0;

    /// command execution
    virtual void Exec(CContext* p_ctx) = 0;

    /// making a new command object with given arguments
    virtual shared_ptr<CCommand> Clone(CContext* p_ctx, const CParser& cmdline) const = 0;

// command library ---------------------------------------------------------
    /// find command in the list of available commands
    static CCommand* Find(const string& name);

    /// begininig of the list of commands
    std::map< string, CCommand* >::iterator Begin(void);

    /// end of the list of commands
    std::map< string, CCommand* >::iterator End(void);

    /// begininig of the list of commands
    std::map< string, CCommand* >::const_iterator Begin(void) const;

    /// end of the list of commands
    std::map< string, CCommand* >::const_iterator End(void) const;

// command arguments processing --------------------------------------------
    /// throw an error if command does not return any object
    void NoAssigmentPossible(const CParser& cmdline) const;

    /// throw an error if assigment is not specified, it also check lhs
    void AssigmentRequired(const CParser& cmdline) const;

    /// throw an error if number of arguments is not correct
    void CheckNumberOfArguments(const CParser& cmdline, unsigned int min, unsigned int max) const;

    /// throw an error if number of arguments is not correct
    void CheckNumberOfArguments(const vector<string>& args, unsigned int min, unsigned int max) const;

    /// throw an error if wrong argument
    void WrongArgument(const CParser& cmdline, unsigned int pos, const string& reason = string()) const;

    /// throw an error if wrong argument
    void WrongArgument(const vector<string>& args, unsigned int pos, const string& reason = string()) const;

    /// expand argument - string
    void ExpandArgument(CContext* p_ctx, const CParser& cmdline, unsigned int pos, string& output) const;

    /// expand argument - string
    void ExpandArgument(CContext* p_ctx, const vector<string>& args, unsigned int pos, string& output) const;

    /// expand argument - integer
    void ExpandArgument(CContext* p_ctx, const CParser& cmdline, unsigned int pos, int& output) const;

    /// expand argument - integer
    void ExpandArgument(CContext* p_ctx, const vector<string>& args, unsigned int pos, int& output) const;

    /// expand argument - double
    void ExpandArgument(CContext* p_ctx, const CParser& cmdline, unsigned int pos, double& output) const;

    /// expand argument - double
    void ExpandArgument(CContext* p_ctx, const vector<string>& args, unsigned int pos, double& output) const;

    /// expand argument - list
    void ExpandArgument(CContext* p_ctx, const CParser& cmdline, unsigned int pos, CListPtr& list) const;

    /// expand argument - list
    void ExpandArgument(CContext* p_ctx, const vector<string>& args, unsigned int pos, CListPtr& list) const;

    /// expand argument - object
    void ExpandArgument(CContext* p_ctx, const CParser& cmdline, unsigned int pos, CEntityPtr& object, const CKey& type) const;

    /// expand argument - object
    void ExpandArgument(CContext* p_ctx, const vector<string>& args, unsigned int pos, CEntityPtr& object, const CKey& type) const;

    //------------------------------

    /// throw an error if wrong lhs
    void WrongLHS(const CParser& cmdline, const string& reason = string()) const;

    /// expand left hand side
    void ExpandLHS(CContext* p_ctx, const CParser& cmdline, string& output) const;

// private data and methods ----------------------------------------------------
protected:
    string                          m_action;
    bool                            m_change_state;

private:
    static map<string, CCommand*>   g_commands;

    /// throw an error for unexpected situation
    void InternalError(void) const;
};

//------------------------------------------------------------------------------

typedef shared_ptr< CCommand > CCommandPtr;

//------------------------------------------------------------------------------

}

#endif
