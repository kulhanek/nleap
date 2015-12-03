#ifndef NLEAP_CORE_KEY_HPP
#define NLEAP_CORE_KEY_HPP
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

#include <NLEaPMainHeader.hpp>
#include <map>
#include <string>

namespace nleap {
//------------------------------------------------------------------------------

using namespace std;

//------------------------------------------------------------------------------

class NLEAP_PACKAGE CKey {
public:
    //! constructor
    CKey(const string& name);

    //! comparisons
    bool operator == (const CKey& left) const;
    bool operator != (const CKey& left) const;
    bool operator < (const CKey& left) const;

    //! return name of the key
    string GetName(void) const;

    //! return key by name
    static const CKey GetKey(const string& name);

// section of private data -----------------------------------------------------
private:
    short                           m_id;
    static map< short, string>      m_map;
    static short                    m_top_id;

    CKey(short id);
};

//------------------------------------------------------------------------------

#define DECLARE_KEY(id) extern NLEAP_PACKAGE CKey id
#define DEFINE_KEY(id,name) CKey id(name)

//------------------------------------------------------------------------------
}

#endif


