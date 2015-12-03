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

#include <types/String.hpp>
#include <core/PredefinedKeys.hpp>

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CString::CString(void)
: CEntity(STRING)
{
    SetValue("");
}

// -------------------------------------------------------------------------

CString::CString(int& top_id)
: CEntity(STRING)
{
    SetId( top_id++ );
    SetValue("");
}

// -------------------------------------------------------------------------

void CString::Desc(ostream& ofs)
{
    ofs << "STRING" << endl;
    ofs << "   Name  : " << GetName() << endl;
    ofs << "   GID   : " << GetId() << endl;
    ofs << "   Value : " << Get<string>(VALUE) << endl;
}

// -------------------------------------------------------------------------

string CString::GetValue(void)
{
    return( Get<string>(VALUE) );
}

// -------------------------------------------------------------------------

void CString::SetValue(const string& value)
{
    string newvalue = value;

    if( ! value.empty() ){
        if( value[0] == '"' ){
            if( (value.size() < 2) || ( value[value.size()-1] != '"' ) ){
                throw runtime_error("unbalanced quotation");
            }
        // strip quotation
        newvalue = value.substr(1,value.size()-2);
        }
    }

    Set(VALUE,newvalue);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

}
