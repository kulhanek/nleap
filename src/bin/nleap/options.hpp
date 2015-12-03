#ifndef NLEAP_OPTIONS_H
#define NLEAP_OPTIONS_H
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

#include <SimpleOptions.hpp>
#include <NLEaPMainHeader.hpp>

//------------------------------------------------------------------------------

class CNLEaPOptions : public CSimpleOptions {
    public:
        // constructor - tune option setup
        CNLEaPOptions(void);

 // program name and description -----------------------------------------------
    CSO_PROG_NAME_BEGIN
        "nleap"
    CSO_PROG_NAME_END

    CSO_PROG_DESC_BEGIN
        "<b><red>nLEaP</red></b> is the environment to create a new system in Amber, or to modify old systems. "
        "The name nLEaP is an acronym constructed from the names of the older AMBER software "
        "modules it replaces: link, edit, and parm, where prefix \"n\" stands for the Nemesis program. Nemesis is "
        "a graphical suite used for molecular modeling. However this particular application is a standalone program that does not actually "
        "run in a graphical mode."
    CSO_PROG_DESC_END

    CSO_PROG_VERS_BEGIN
        NLEAP_VERSION
    CSO_PROG_VERS_END

 // list of all options and arguments ------------------------------------------
    CSO_LIST_BEGIN
        // options ------------------------------
        CSO_OPT(CSmallString,ScriptName)
        CSO_OPT(CSmallString,Variables)
        CSO_OPT(bool,Interactive)
        CSO_OPT(bool,DefaultSetup)
        CSO_OPT(bool,NoHistory)
        CSO_OPT(bool,ClearHistory)
        CSO_OPT(bool,Help)
        CSO_OPT(bool,Version)
    CSO_LIST_END

    CSO_MAP_BEGIN
 // description of options -----------------------------------------------------
        CSO_MAP_OPT(CSmallString,                           /* option type */
                    ScriptName,                        /* option name */
                    NULL,                          /* default value */
                    false,                          /* is option mandatory */
                    'f',                           /* short option name */
                    "script",                      /* long option name */
                    "FILE",                           /* parametr name */
                    "name of file to be loaded as nLEaP script")   /* option description */
        //----------------------------------------------------------------------
        CSO_MAP_OPT(CSmallString,                           /* option type */
                    Variables,                        /* option name */
                    NULL,                          /* default value */
                    false,                          /* is option mandatory */
                    'v',                           /* short option name */
                    "variables",                      /* long option name */
                    "LIST",                           /* parametr name */
                    "comma separated list of variables and values, if a value is not provided then the value of the environment variable is used instead")   /* option description */
        //----------------------------------------------------------------------
         CSO_MAP_OPT(bool,                           /* option type */
                    Interactive,                        /* option name */
                    false,                          /* default value */
                    false,                          /* is option mandatory */
                    'i',                           /* short option name */
                    "interactive",                      /* long option name */
                    NULL,                           /* parametr name */
                    "keep running in interactive mode (usuful together with <blue>--script</blue> option)")   /* option description */
        //----------------------------------------------------------------------
        CSO_MAP_OPT(bool,                           /* option type */
                    DefaultSetup,                        /* option name */
                    false,                          /* default value */
                    false,                          /* is option mandatory */
                    'd',                           /* short option name */
                    "default",                      /* long option name */
                    NULL,                           /* parametr name */
                    "use default nLEaP context setup")   /* option description */
        //----------------------------------------------------------------------
        CSO_MAP_OPT(bool,                           /* option type */
                    NoHistory,                        /* option name */
                    false,                          /* default value */
                    false,                          /* is option mandatory */
                    'n',                           /* short option name */
                    "nohistory",                      /* long option name */
                    NULL,                           /* parametr name */
                    "do not load readline history file")   /* option description */
        //----------------------------------------------------------------------
        CSO_MAP_OPT(bool,                           /* option type */
                    ClearHistory,                        /* option name */
                    false,                          /* default value */
                    false,                          /* is option mandatory */
                    'c',                           /* short option name */
                    "clearhistory",                      /* long option name */
                    NULL,                           /* parametr name */
                    "clear readline history file at the startup")   /* option description */
        //----------------------------------------------------------------------
        CSO_MAP_OPT(bool,                           /* option type */
                    Version,                        /* option name */
                    false,                          /* default value */
                    false,                          /* is option mandatory */
                    '\0',                           /* short option name */
                    "version",                      /* long option name */
                    NULL,                           /* parametr name */
                    "output version information and exit")   /* option description */
        //----------------------------------------------------------------------
        CSO_MAP_OPT(bool,                           /* option type */
                    Help,                        /* option name */
                    false,                          /* default value */
                    false,                          /* is option mandatory */
                    'h',                           /* short option name */
                    "help",                      /* long option name */
                    NULL,                           /* parametr name */
                    "display this help and exit")   /* option description */
    CSO_MAP_END

// final operation with options ------------------------------------------------
    private:
    virtual int CheckOptions(void);
    virtual int FinalizeOptions(void);
    virtual int CheckArguments(void);
    };

//------------------------------------------------------------------------------

#endif
