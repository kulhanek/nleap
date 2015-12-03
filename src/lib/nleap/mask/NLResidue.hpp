#ifndef NLResidueH
#define NLResidueH
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

//------------------------------------------------------------------------------

/** \brief simple topology residue
*/

class NLEAP_PACKAGE CNLResidue {
public:
// constructor and destructor -------------------------------------------------
    CNLResidue(void);
    ~CNLResidue(void);

// information methods --------------------------------------------------------
    //! get residue name
    int         GetIndex(void) const;

    //! get residue name
    const char* GetName(void) const;

    //! return index of the first atom
    /*! index starts from zero and has regular counting order
    */
    int         GetFirstAtomIndex(void) const;

    //! return number of atoms which belongs to residue
    int         GetNumberOfAtoms(void) const;

// section of private data ----------------------------------------------------
private:
    int             Index;              //! residue index
    CSmallString    Name;               //! residue name
    int             FirstAtomIndex;     //! index of first atom in residue
    int             NumOfAtoms;         //! number of atoms in residues

    friend class CNLTopology;
};

//------------------------------------------------------------------------------

#endif
