#ifndef NLTopologyH
#define NLTopologyH
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
#include <mask/NLResidue.hpp>
#include <mask/NLAtom.hpp>
#include <SimpleVector.hpp>

//---------------------------------------------------------------------------

/** \brief simple system topology for atom selection by masks
*/

class NLEAP_PACKAGE CNLTopology {
public:
// constructor and destructor -------------------------------------------------
    CNLTopology(void);
    ~CNLTopology(void);

// setup topology -------------------------------------------------------------
    void    Init(int natoms,int nres,bool has_box,const CPoint& box_centre);
    void    Clear(void);

    void    SetResidue(int idx,const CSmallString& name,int first_atom);
    void    SetAtom(int idx,const CSmallString& name,const CSmallString& type);
    void    SetAtom(int idx,double mass,double x,double y,double z);
    void    GetAtom(int idx,double& mass,double& x,double& y,double& z,
                    CSmallString& name,CSmallString& type,
                    int& resid,CSmallString& resname);

    void    Finalize(void);

// informational methods ------------------------------------------------------
    int         GetNumberOfAtoms(void) const;
    CNLAtom*    GetAtom(int index);

    int         GetNumberOfResidues(void) const;
    CNLResidue* GetResidue(int index);

    bool            HasBox(void) const;
    const CPoint&   GetBoxCenter(void) const;

    void        PrintTopology(void);

// section of private data ----------------------------------------------------
private:
    CSimpleVector<CNLAtom>      Atoms;
    CSimpleVector<CNLResidue>   Residues;
    bool                        BoxPresent;
    CPoint                      BoxCenter;
};

//---------------------------------------------------------------------------

#endif
