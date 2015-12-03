#ifndef NLEAP_MISC_GEOMETRY_HPP
#define NLEAP_MISC_GEOMETRY_HPP
// =============================================================================
// nLEaP - prepare input for the AMBER molecular mechanics programs
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
#include <core/Entity.hpp>
#include <Point.hpp>

namespace nleap {
//------------------------------------------------------------------------------

class CContext;

//------------------------------------------------------------------------------

//! return COM of unit, residue or atom
CPoint NLEAP_PACKAGE GetCOM(CContext* p_ctx,CEntityPtr& obj);

//! get distance between two points
double NLEAP_PACKAGE GetDistance(const CPoint& p1,const CPoint& p2);

//! get angle among three points
double NLEAP_PACKAGE GetAngle(const CPoint& p1,const CPoint& p2,const CPoint& p3);

//! get torsion among four atoms
double NLEAP_PACKAGE GetTorsion(const CPoint& p1,const CPoint& p2,const CPoint& p3,const CPoint& p4);

//------------------------------------------------------------------------------
}

#endif
