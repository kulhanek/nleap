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

#include <format/CommonIO.hpp>
#include <stdexcept>
#include <sstream>

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CCommonIO::CCommonIO( CVerboseStr& debug )
    : m_debug( debug )
{
    m_line_no = 0;
}

//------------------------------------------------------------------------------

void CCommonIO::EmitRWError( const string& reason )
{
    stringstream str;
    str << reason << endl;
    str << "       Line " << m_line_no << " : " << m_line;

    throw runtime_error( str.str() );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}
 
