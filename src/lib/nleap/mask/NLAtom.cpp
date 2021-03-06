// =============================================================================
// nLEaP - A molecular manipulation program and coding environment
// -----------------------------------------------------------------------------
//     Copyright (C) 2011 Petr Kulhanek, kulhanek@chemi.muni.cz
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

#include <mask/NLAtom.hpp>
#include <string.h>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CNLAtom::CNLAtom(void)
{
    Index = -1;
    Residue = NULL;
    Mass = 0.0;
}

//------------------------------------------------------------------------------

CNLAtom::~CNLAtom(void)
{

}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

int CNLAtom::GetIndex(void) const
{
    return(Index);
}

//------------------------------------------------------------------------------

const char* CNLAtom::GetName(void) const
{
    return(Name);
}

//------------------------------------------------------------------------------

const char* CNLAtom::GetType(void) const
{
    return(Type);
}

//------------------------------------------------------------------------------

double CNLAtom::GetMass(void) const
{
    return(Mass);
}

//------------------------------------------------------------------------------

const CPoint& CNLAtom::GetPosition(void) const
{
    return(Position);
}

//------------------------------------------------------------------------------

CNLResidue* CNLAtom::GetResidue(void) const
{
    return(Residue);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
