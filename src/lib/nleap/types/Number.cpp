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

#include <types/Number.hpp>
#include <core/PredefinedKeys.hpp>
#include <cmath>

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CNumber::CNumber(void)
: CEntity(NUMBER)
{
    SetValue(0.0);
}

// -------------------------------------------------------------------------

CNumber::CNumber(int& top_id)
: CEntity(NUMBER)
{
    SetId( top_id++ );
    SetValue(0.0);
}

// -------------------------------------------------------------------------

void CNumber::Desc(ostream& ofs)
{
    ofs << "NUMBER" << endl;
    ofs << "   Name  : " << GetName() << endl;
    ofs << "   GID   : " << GetId() << endl;
    ofs << "   Value : " << Get<double>(VALUE) << endl;
}

// -------------------------------------------------------------------------

double CNumber::GetValue(void)
{
    return( Get<double>(VALUE) );
}

// -------------------------------------------------------------------------

void CNumber::SetValue(double value)
{
    Set(VALUE,value);
}

// -------------------------------------------------------------------------

bool CNumber::IsInteger(void)
{
    double value = GetValue();
    int    ivalue = GetValue();
    if( fabs(value - ivalue) <= 1.e-7 ) return( true );
    return( false );
}

// -------------------------------------------------------------------------

int CNumber::GetNumber(void)
{
    if( ! IsInteger() ){
        throw std::runtime_error( "CNumber::GetNumber(): not an integer number" );
    }
    return( GetValue() );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}
