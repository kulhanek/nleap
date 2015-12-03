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

#include <mask/NLTopology.hpp>
#include <ErrorSystem.hpp>
#include <string.h>
#include <errno.h>

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CNLTopology::CNLTopology(void)
{
    BoxPresent = false;
}

//------------------------------------------------------------------------------

CNLTopology::~CNLTopology(void)
{

}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CNLTopology::Init(int natoms,int nres,bool has_box,const CPoint& box_centre)
{
    try {
        Atoms.CreateVector(natoms);
        Residues.CreateVector(nres);
    } catch(...) {
        ES_ERROR("unable to allocate arrays for atoms and residues");
        return;
    }

    BoxPresent = has_box;
    BoxCenter = box_centre;
}

//------------------------------------------------------------------------------

void CNLTopology::Clear(void)
{
    Atoms.FreeVector();
    Residues.FreeVector();
    BoxPresent = false;
    BoxCenter = CPoint();
}

//------------------------------------------------------------------------------

void CNLTopology::SetResidue(int idx,const CSmallString& name,int first_atom)
{
    if((idx < 0) || (idx >= Residues.GetLength())) {
        ES_ERROR("residue out-of-range");
        return;
    }
    Residues[idx].Index = idx;
    Residues[idx].Name = name;
    Residues[idx].FirstAtomIndex = first_atom;
}

//------------------------------------------------------------------------------

void CNLTopology::SetAtom(int idx,const CSmallString& name,const CSmallString& type)
{
    if((idx < 0) || (idx >= Atoms.GetLength())) {
        ES_ERROR("atom out-of-range");
        return;
    }
    Atoms[idx].Index = idx;
    Atoms[idx].Name = name;
    Atoms[idx].Type = type;
}

//------------------------------------------------------------------------------

void CNLTopology::SetAtom(int idx,double mass,double x,double y,double z)
{
    if((idx < 0) || (idx >= Atoms.GetLength())) {
        ES_ERROR("atom out-of-range");
        return;
    }
    Atoms[idx].Mass = mass;
    Atoms[idx].Position = CPoint(x,y,z);
}

//------------------------------------------------------------------------------

void CNLTopology::GetAtom(int idx,double& mass,double& x,double& y,double& z,
                CSmallString& name,CSmallString& type,
                int& resid,CSmallString& resname)
{
    if((idx < 0) || (idx >= Atoms.GetLength())) {
        ES_ERROR("atom out-of-range");
        return;
    }
    mass = Atoms[idx].Mass;
    x = Atoms[idx].Position.x;
    y = Atoms[idx].Position.y;
    z = Atoms[idx].Position.z;
    name = Atoms[idx].Name;
    type = Atoms[idx].Type;

    CNLResidue* p_res = Atoms[idx].GetResidue();
    if(p_res != NULL) {
        resid = p_res->GetIndex()+1;
        resname = p_res->GetName();
    } else {
        resid = 1;
        resname = "XXX";
    }
}

//------------------------------------------------------------------------------

void CNLTopology::Finalize(void)
{
    for(int i=0; i < Residues.GetLength(); i++) {
        int last_atm;
        if(i == Residues.GetLength() - 1) {
            last_atm = Atoms.GetLength();
        } else {
            last_atm = Residues[i+1].FirstAtomIndex;
        }
        Residues[i].NumOfAtoms = last_atm - Residues[i].FirstAtomIndex;
        for(int j= Residues[i].FirstAtomIndex; j < last_atm; j++) {
            Atoms[j].Residue = &Residues[i];
        }
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

int CNLTopology::GetNumberOfAtoms(void) const
{
    return(Atoms.GetLength());
}
//------------------------------------------------------------------------------

CNLAtom* CNLTopology::GetAtom(int index)
{
    return(&Atoms[index]);
}
//------------------------------------------------------------------------------

int CNLTopology::GetNumberOfResidues(void) const
{
    return(Residues.GetLength());
}

//------------------------------------------------------------------------------

CNLResidue* CNLTopology::GetResidue(int index)
{
    return(&Residues[index]);
}
//------------------------------------------------------------------------------

bool CNLTopology::HasBox(void) const
{
    return(BoxPresent);
}

//------------------------------------------------------------------------------

const CPoint&   CNLTopology::GetBoxCenter(void) const
{
    return(BoxCenter);
}

//------------------------------------------------------------------------------

void CNLTopology::PrintTopology(void)
{
    printf("#    ID    Name  ResID  Res     X [A]      Y [A]      Z [A]    Mass  Type\n");
    printf("# -------- ---- ------- ---- ---------- ---------- ---------- ------ ----\n");

    for(int i=0; i < Atoms.GetLength(); i++) {

        CNLResidue* p_res = Atoms[i].GetResidue();
        if(p_res != NULL) {
            printf("  %8d %-4s %7d %-4s %10.3f %10.3f %10.3f %6.2f %-4s\n",i+1,
                   (const char*)Atoms[i].GetName(),
                   p_res->GetIndex()+1,(const char*)p_res->GetName(),
                   Atoms[i].Position.x,Atoms[i].Position.y,Atoms[i].Position.z,
                   Atoms[i].Mass,
                   (const char*)Atoms[i].GetType());
        } else {
            printf("  %8d %-4s              %10.3f %10.3f %10.3f %6.2f %-4s\n",i+1,
                   (const char*)Atoms[i].GetName(),
                   Atoms[i].Position.x,Atoms[i].Position.y,Atoms[i].Position.z,
                   Atoms[i].Mass,
                   (const char*)Atoms[i].GetType());
        }
    }

    fflush(stdout);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
