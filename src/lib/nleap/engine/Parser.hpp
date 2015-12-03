#ifndef NLEAP_ENGINE_PARSER_H
#define NLEAP_ENGINE_PARSER_H
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
#include <vector>
#include <string>

namespace nleap {

//------------------------------------------------------------------------------

using namespace std;

//------------------------------------------------------------------------------

//! \brief command line parser
class NLEAP_PACKAGE CParser {
public:
    enum ESyntaxStatus
    {
        syntax_error,
        syntax_intermediate,
        syntax_valid
    };

    //! check syntax of line
    //! functionality is very limited, it only detects open curly brackets as syntax_intermediate
    static ESyntaxStatus CheckSyntax(const string& line);

    //! parse line
    void Parse(const string& line);

    //! return left hand side
    const string& GetLHS(void) const;

    //! return command name
    const string& GetCmd(void) const;

    //! return parsed arguments
    const vector<string>& GetArgs(void) const;

// private data and methods ----------------------------------------------------
private:
    string          m_lhs;          // left hand side
    string          m_cmd;          // command
    vector<string>  m_args;         // arguments

};

//------------------------------------------------------------------------------
}

#endif
