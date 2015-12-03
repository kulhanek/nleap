#ifndef NLEAP_TYPE_AMBERFF_HPP
#define NLEAP_TYPE_AMBERFF_HPP
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
#include <vector>

namespace nleap {
//------------------------------------------------------------------------------

class   CContext;
class   CDatabase;
class   CAmberFF;
class   CUnit;

typedef shared_ptr< CDatabase >     CDatabasePtr;
typedef shared_ptr< CUnit >         CUnitPtr;
typedef shared_ptr< CAmberFF >      CAmberFFPtr;

//------------------------------------------------------------------------------

/// CAmberFF is a amberff type
class NLEAP_PACKAGE CAmberFF : public CEntity {
public:

    CAmberFF(void);
    CAmberFF(int& top_id);

// -----------------------------------------------------------------------------

    static void CheckUnit(CDatabasePtr db, CUnitPtr unit, ostream& ofs);

// -----------------------------------------------------------------------------

    //! list available parameter sets
    static void ListFFs(CDatabasePtr db, ostream& ofs);

    //! describe given FF type
    static void DescType(CDatabasePtr db, const string& t1, ostream& ofs);

    //! describe given bond between two types
    static void DescBond(CDatabasePtr db, const string& t1, const string& t2, ostream& ofs);

    //! describe given angle between three types
    static void DescAngle(CDatabasePtr db, const string& t1, const string& t2,
                          const string& t3, ostream& ofs);

    //! describe given torsion between four types
    static void DescTorsion(CDatabasePtr db,const string& t1, const string& t2,
                               const string& t3,const string& t4, ostream& ofs);

    //! describe given improper between four types
    static void DescImproper(CDatabasePtr db,const string& t1, const string& t2,
                               const string& t3,const string& t4, ostream& ofs);

// -----------------------------------------------

    //! cache all Amber FFs in correct order, e.g. high priority first
    static void CacheFFs(CDatabasePtr db, list<CAmberFFPtr>& ffs);

// -----------------------------------------------

    //! find parameters for a type
    static CEntityPtr FindType(list<CAmberFFPtr>& ffs, const string& t1,
                               CAmberFFPtr& ff);

    //! find parameters for a type
    static CEntityPtr FindType(list<CAmberFFPtr>& ffs, const string& t1);

    //! find parameters for a type
    CEntityPtr FindType(const string& t1);

// -----------------------------------------------

    //! find parameters for a bond
    static CEntityPtr FindBond(list<CAmberFFPtr>& ffs, const string& t1,
                               const string& t2, CAmberFFPtr& ff);

    //! find parameters for a bond
    static CEntityPtr FindBond(list<CAmberFFPtr>& ffs, const string& t1,
                               const string& t2);

    //! find parameters for a bond
    CEntityPtr FindBond(const string& t1, const string& t2);

// -----------------------------------------------

    //! find parameters for an angle
    static CEntityPtr FindAngle(list<CAmberFFPtr>& ffs, const string& t1,
                               const string& t2, const string& t3, CAmberFFPtr& ff);

    //! find parameters for an angle
    static CEntityPtr FindAngle(list<CAmberFFPtr>& ffs, const string& t1,
                               const string& t2, const string& t3);

    //! find parameters for an angle
    CEntityPtr FindAngle(const string& t1, const string& t2, const string& t3);

// -----------------------------------------------

    //! find parameters for a torsion
    static void FindTorsion(list<CAmberFFPtr>& ffs, const string& t1,
                            const string& t2, const string& t3, const string& t4,
                            CAmberFFPtr& ff, vector<CEntityPtr>& tors);

    //! find parameters for a torsion
    static void FindTorsion(list<CAmberFFPtr>& ffs, const string& t1,
                            const string& t2, const string& t3, const string& t4,
                            vector<CEntityPtr>& tors);

    //! find parameters for a torsion
    void FindTorsion(const string& t1, const string& t2, const string& t3, const string& t4,
                     vector<CEntityPtr>& tors);

// -----------------------------------------------

    //! find parameters for an improper
    static void FindImproper(list<CAmberFFPtr>& ffs, const string& t1,
                             const string& t2, const string& t3, const string& t4,
                             CAmberFFPtr& ff, vector<CEntityPtr>& improps);

    //! find parameters for an improper
    static void FindImproper(list<CAmberFFPtr>& ffs, const string& t1,
                             const string& t2, const string& t3, const string& t4,
                             vector<CEntityPtr>& improps);

    //! find parameters for an improper
    void FindImproper(const string& t1, const string& t2, const string& t3, const string& t4,
                      vector<CEntityPtr>& improps);

// -----------------------------------------------------------------------------
    /// \brief describe amberff
    virtual void Desc(ostream& ofs);

    /// describe header
    void DescHeader(ostream& ofs);

    /// describe types
    void DescTypes(ostream& ofs);

    /// describe bonds
    void DescBonds(ostream& ofs);

    /// describe angles
    void DescAngles(ostream& ofs);

    /// describe torsions
    void DescTorsions(ostream& ofs);

    /// describe impropers
    void DescImpropers(ostream& ofs);

    /// describe vdW parameters
    void DescvdW(ostream& ofs);

    /// describe subobject
    virtual void DescSubObj(const string& name,ostream& ofs);

    /// get number of types
    int NumberOfTypes(void);

    /// get number of bonds
    int NumberOfBonds(void);

    /// get number of angles
    int NumberOfAngles(void);

    /// get number of torsions
    int NumberOfTorsions(void);

    /// get number of impropers
    int NumberOfImpropers(void);

};

//------------------------------------------------------------------------------
}

#endif
