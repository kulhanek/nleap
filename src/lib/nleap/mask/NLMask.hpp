#ifndef NLMaskH
#define NLMaskH
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

#include <NLEaPMainHeader.hpp>
#include <SmallString.hpp>
#include <NLEaPMainHeader.hpp>

//---------------------------------------------------------------------------

class CNLTopology;
class CNLAtom;
class CNLMaskSelection;

//---------------------------------------------------------------------------

//! mask support class
/*!
 mask support is now fully compatible with AMBER 9.0
*/

class NLEAP_PACKAGE CNLMask {
public:
    CNLMask(void);
    ~CNLMask(void);

    // topology assigment ------------------------------------------------------
    bool            AssignTopology(CNLTopology* p_top);
    CNLTopology*    GetTopology(void) const;

    // mask setup --------------------------------------------------------------
    //! select all atoms
    bool SelectAllAtoms(void);

    //! set mask from string specification
    bool SetMask(const CSmallString& mask);

    //! return current mask specification
    const CSmallString& GetMask(void) const;

    // results -----------------------------------------------------------------
    int         GetNumberOfTopologyAtoms(void);
    int         GetNumberOfSelectedAtoms(void);
    CNLAtom*    GetSelectedAtom(int index);
    bool        IsAtomSelected(int index);

// section of private data ----------------------------------------------------
private:
    CNLTopology*        Topology;
    CSmallString        Mask;
    CNLMaskSelection*   Selection;
};

//---------------------------------------------------------------------------

#endif
