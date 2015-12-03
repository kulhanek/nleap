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

#include <core/Key.hpp>
#include <core/PredefinedKeys.hpp>

namespace nleap {

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

map<short, string>  CKey::m_map;
short               CKey::m_top_id = 0;

//------------------------------------------------------------------------------

DEFINE_KEY(NULL_PROP,"NULLID");
DEFINE_KEY(NEXT_PROP,"NEXTID");
DEFINE_KEY(IPTR_PROP,"IPTRID");
DEFINE_KEY(PTR__PROP,"PTRID");
DEFINE_KEY(INT__PROP,"INTID");
DEFINE_KEY(DBL__PROP,"DBLID");
DEFINE_KEY(STR__PROP,"STRID");

DEFINE_KEY(UNKNOWN,"UNKNOWN");
DEFINE_KEY(ANY,"ANY");
DEFINE_KEY(NODE,"NODE");
DEFINE_KEY(CONTEXT,"CONTEXT");
DEFINE_KEY(DATABASE,"DATABASE");
DEFINE_KEY(VARIABLE,"VARIABLE");
DEFINE_KEY(STRING,"STRING");
DEFINE_KEY(NUMBER,"NUMBER");
DEFINE_KEY(LIST,"LIST");
DEFINE_KEY(UNIT,"UNIT");
DEFINE_KEY(RESIDUE,"RESIDUE");
DEFINE_KEY(ATOM,"ATOM");
DEFINE_KEY(BOND,"BOND");
DEFINE_KEY(AMBERFF,"AMBERFF");
DEFINE_KEY(ATOMTYPES,"ATOMTYPES");
DEFINE_KEY(PDBATOMMAP,"PDBATOMMAP");
DEFINE_KEY(PDBRESMAP,"PDBRESMAP");

DEFINE_KEY(VALUE,"VALUE");
DEFINE_KEY(PATH,"PATH");
DEFINE_KEY(ECHO,"ECHO");
DEFINE_KEY(VERBOSITY,"VERBOSITY");
DEFINE_KEY(MAXHIST,"MAXHIST");
DEFINE_KEY(TITLE,"TITLE");
DEFINE_KEY(TITLE2,"TITLE2");
DEFINE_KEY(MASS,"MASS");
DEFINE_KEY(POLAR,"POLAR");
DEFINE_KEY(DEPTH,"DEPTH");
DEFINE_KEY(RSTAR,"RSTAR");
DEFINE_KEY(ATOM1,"ATOM1");
DEFINE_KEY(ATOM2,"ATOM2");
DEFINE_KEY(ATOM3,"ATOM3");
DEFINE_KEY(ATOM4,"ATOM4");
DEFINE_KEY(EQUIL,"EQUIL");
DEFINE_KEY(FORCE,"FORCE");
DEFINE_KEY(DIVIDE,"DIVIDE");
DEFINE_KEY(PERIOD,"PERIOD");
DEFINE_KEY(CHARGE,"CHARGE");
DEFINE_KEY(POSX,"POSX");
DEFINE_KEY(POSY,"POSY");
DEFINE_KEY(POSZ,"POSZ");
DEFINE_KEY(TYPE,"TYPE");
DEFINE_KEY(RID,"RID");
DEFINE_KEY(SID,"SID");
DEFINE_KEY(LID,"LID");
DEFINE_KEY(ORDER,"ORDER");
DEFINE_KEY(TERM,"TERM");

DEFINE_KEY(ELEMENT,"ELEMENT");
DEFINE_KEY(HYBRIDIZATION,"HYBRIDIZATION");

DEFINE_KEY(HEAD,"HEAD");
DEFINE_KEY(TAIL,"TAIL");

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CKey::CKey(const string& name)
{
    m_id = m_top_id++;
    m_map[m_id] = name;
}

//------------------------------------------------------------------------------

CKey::CKey(short id)
{
    m_id = id;
}

// -------------------------------------------------------------------------

bool CKey::operator == (const CKey& left) const
{
    return( m_id == left.m_id );
}

// -------------------------------------------------------------------------

bool CKey::operator != (const CKey& left) const
{
    return( m_id != left.m_id );
}

// -------------------------------------------------------------------------

bool CKey::operator < (const CKey& left) const
{
    return( m_id < left.m_id );
}

// -------------------------------------------------------------------------

string CKey::GetName(void) const
{
    return( m_map[m_id] );
}

// -------------------------------------------------------------------------

const CKey CKey::GetKey(const string& name)
{
    // TODO
    return( CKey(0) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}


