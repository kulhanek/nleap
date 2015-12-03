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

#include <format/AmberParm.hpp>
#include <vector>
#include <sstream>
#include <types/Factory.hpp>
#include <boost/algorithm/string.hpp>
#include <core/PredefinedKeys.hpp>

using namespace boost;

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CAmberParm::CAmberParm(CVerboseStr& debug)
    : m_debug( debug )
{

}

// -------------------------------------------------------------------------

void CAmberParm::Read(const string& parm, const string& rst,
                      CUnitPtr& unit, CAmberFFPtr& ff, int& top_id)
{

}

// -------------------------------------------------------------------------

void CAmberParm::Write(const string& parm, const string& rst,
                       CUnitPtr& unit, CDatabasePtr& db)
{

}

// -------------------------------------------------------------------------

void CAmberParm::BuildAmberParams(CAmberFFPtr& ff,int& top_id)
{

}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}







