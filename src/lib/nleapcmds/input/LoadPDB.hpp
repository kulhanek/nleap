#ifndef NLEAPSCMDS_LOADPDB_H
#define NLEAPSCMDS_LOADPDB_H
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

#include <engine/Command.hpp>

namespace nleapcmds {
//------------------------------------------------------------------------------

using namespace nleap;

//------------------------------------------------------------------------------
/// loadpdb structure
/// \ingroup nleapcmds
class CLoadPDBCommand : public CCommand {
public:

    CLoadPDBCommand(const string& cmd_name);

    CLoadPDBCommand(const string& cmd_name, const string& var, const string& file);

    virtual const char* Info(EHelp type = help_full) const;

    virtual void Exec(CContext* p_ctx);

    virtual shared_ptr< CCommand > Clone(CContext* p_ctx, const CParser& cmdline) const;

// private data and methods ----------------------------------------------------
private:
    string m_var;
    string m_file;
};

//------------------------------------------------------------------------------
}
#endif

