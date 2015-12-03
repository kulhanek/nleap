#ifndef NLAtomH
#define NLAtomH
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
#include <Point.hpp>

//---------------------------------------------------------------------------

class CNLResidue;

//---------------------------------------------------------------------------

/** \brief simple topology atom
*/

class NLEAP_PACKAGE CNLAtom {
public:
// constructor and destructor -------------------------------------------------
    CNLAtom(void);
    ~CNLAtom(void);

// information methods --------------------------------------------------------
    //! get atom index
    int                     GetIndex(void) const;

    //! get atom name
    const char*     GetName(void) const;

    //! get atom type
    const char*     GetType(void) const;

    //! get atom mass
    double                  GetMass(void) const;

    //! get atom position
    const CPoint&           GetPosition(void) const;

    //! get residue
    CNLResidue*            GetResidue(void) const;

// section of private data ----------------------------------------------------
private:
    int             Index;      // atom index
    CNLResidue*    Residue;    // residue
    CSmallString    Name;       // atom name
    CSmallString    Type;       // atom type
    double          Mass;       // atom mass
    CPoint          Position;   // atom position

    friend class CNLTopology;
};

//---------------------------------------------------------------------------

#endif
