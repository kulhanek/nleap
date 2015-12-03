#ifndef NLEAP_NLEAP_H
#define NLEAP_NLEAP_H
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

#include "options.hpp"
#include <fstream>
#include <engine/Context.hpp>
#include <FileName.hpp>
#include <IndexCounter.hpp>

//------------------------------------------------------------------------------

class CNLEaP {
public:
        // constructor
        CNLEaP(void);

// main methods ---------------------------------------------------------------
    //! init options
    int Init(int argc,char* argv[]);

    //! main part of program
    bool Run(void);

    //! finalize program
    bool Finalize(void);

// section of private data ----------------------------------------------------
private:
    CNLEaPOptions       Options;
    nleap::CContext     Context;
    CTerminalStr        Console;
    CVerboseStr         vout;
    CFileName           NLEaPConfigDir;
    CFileName           HistoryFileName;
    CFileName           UserConfigName;
    bool                Error;

    //! run interpreter in interactive mode
    bool RunInteractive(void);

    //! run interpreter in non-interactive mode reading input from stdin
    bool RunNonInteractive(std::istream& stream);

// readline support -----------------------------------------------------------
    static const char*  Prompt;                 // readline prompt
    static char*        Line;                   // line read by readline

    //! read line using readline library
    static char* RLGets(void);

    //! get system variable
    const CSmallString GetSystemVariable(const CSmallString& name);
};

//------------------------------------------------------------------------------

#endif
