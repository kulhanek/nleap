#ifndef NLEAP_TYPE_INUMBER_HPP
#define NLEAP_TYPE_INUMBER_HPP
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

namespace nleap {
//------------------------------------------------------------------------------

/// CNumber is a number type
/// value is accessible via VALUE property
class NLEAP_PACKAGE CNumber : public CEntity {
public:

    CNumber(void);
    CNumber(int& top_id);

    /// \brief describe number
    virtual void Desc(ostream& ofs);

    /// get value
    double GetValue(void);

    /// set value
    void SetValue(double value);

    /// is integer?
    bool IsInteger(void);

    /// get integer value
    int GetNumber(void);
};

//------------------------------------------------------------------------------

typedef shared_ptr< CNumber > CNumberPtr;

//------------------------------------------------------------------------------
}

#endif
