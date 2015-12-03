#ifndef NLEAPSCMDS_BOND_H
#define NLEAPSCMDS_BOND_H
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
/// create a bond between two atoms
/// \ingroup nleapcmds
class CBondCommand : public CCommand {
public:

    CBondCommand(const string& cmd_name);

    CBondCommand(const string& cmd_name, CEntityPtr& atom1, CEntityPtr& atom2, int order);

    virtual const char* Info(EHelp type = help_full) const;

    virtual void Exec(CContext* p_ctx);

    virtual shared_ptr< CCommand > Clone(CContext* p_ctx, const CParser& cmdline) const;

// private data and methods ----------------------------------------------------
private:
    CEntityPtr  m_atom1;
    CEntityPtr  m_atom2;
    int         m_order;
};

//------------------------------------------------------------------------------
}
#endif

