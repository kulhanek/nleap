// =============================================================================
// nLEaP - A molecular manipulation program and coding environment
// -----------------------------------------------------------------------------
//     Copyright (C) 2010 Petr Kulhanek, kulhanek@chemi.muni.cz
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

#include <types/List.hpp>
#include <core/PredefinedKeys.hpp>

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CList::CList( void )
: CEntity(LIST)
{

}

// -----------------------------------------------------------------------------

CList::CList(int& top_id)
: CEntity(LIST)
{
    SetId( top_id++ );
}

// -----------------------------------------------------------------------------

void CList::Desc( ostream& ofs )
{
    ofs << "LIST" << endl;
    ofs << "   Name  : " << GetName() << endl;
    ofs << "   GID   : " << GetId() << endl;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}
