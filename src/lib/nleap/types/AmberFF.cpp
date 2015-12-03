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

#include <types/AmberFF.hpp>
#include <core/PredefinedKeys.hpp>
#include <iomanip>
#include <core/ForwardIterator.hpp>
#include <engine/Context.hpp>
#include <types/Factory.hpp>

namespace nleap {

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CAmberFF::CAmberFF(void)
: CEntity(AMBERFF)
{

}

// -------------------------------------------------------------------------

CAmberFF::CAmberFF(int& top_id)
: CEntity(AMBERFF)
{
    SetId( top_id++ );

    CEntityPtr node;

    node = CFactory::CreateNode( top_id, "types" );
    top_id++;
    AddChild(node);

    node = CFactory::CreateNode( top_id, "bonds" );
    top_id++;
    AddChild(node);

    node = CFactory::CreateNode( top_id, "angles" );
    top_id++;
    AddChild(node);

    node = CFactory::CreateNode( top_id, "torsions" );
    top_id++;
    AddChild(node);

    node = CFactory::CreateNode( top_id, "impropers" );
    top_id++;
    AddChild(node);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CAmberFF::CheckUnit(CDatabasePtr db,CUnitPtr unit,ostream& ofs)
{
    ofs << ">> Checking AmberFF parameters" << endl;

    // cache FFs ---------------------------------
    list<CAmberFFPtr> ffs;
    CacheFFs(db,ffs);

    int count;
    int missing;

    // check bonds -------------------------------
    ofs << "   Bonds ..." << endl;
    CForwardIterator bit = unit->BeginBonds();
    CForwardIterator bie = unit->EndBonds();

    count = 0;
    missing = 0;
    while( bit != bie ){
        CEntityPtr at1 = bit->Get<CEntityPtr>(ATOM1);
        CEntityPtr at2 = bit->Get<CEntityPtr>(ATOM2);
        CEntityPtr bt = FindBond(ffs,at1->Get<string>(TYPE),at2->Get<string>(TYPE));
        if( ! bt ){
            ofs << "     missing " << at1->Get<string>(TYPE) << " - " << at2->Get<string>(TYPE) << endl;
            missing++;
        }
        bit++;
        count++;
    }
    ofs << "     Number of bonds  : " << count;
    if( missing > 0 ){
        ofs << " (Missing parameters for " << missing << " bonds)";
    }
    ofs << endl;

    // check angles ------------------------------
    ofs << "   Angles ..." << endl;
//    CForwardIterator ait = unit->BeginBonds();
//    CForwardIterator aie = unit->EndBonds();

    count = 0;
    missing = 0;
//    while( bit != bie ){
//        CEntityPtr at1 = bit->Get<CEntityPtr>(ATOM1);
//        CEntityPtr at2 = bit->Get<CEntityPtr>(ATOM2);
//        CEntityPtr bt = FindBond(ffs,at1->Get<string>(TYPE),at2->Get<string>(TYPE));
//        if( ! bt ){
//            ofs << "     missing " << at1->Get<string>(TYPE) << " - " << at2->Get<string>(TYPE) << endl;
//        }
//        bit++;
//    }
    ofs << "     Number of angles : " << count;
    if( missing > 0 ){
        ofs << " (Missing parameters for " << missing << " angles)";
    }
    ofs << endl;


}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CAmberFF::ListFFs(CDatabasePtr db,ostream& ofs)
{
    // cache FFs
    list<CAmberFFPtr> ffs;
    CacheFFs(db,ffs);

    // print them
    ofs << ">>> AVAILABLE AMBERFFs" << endl;
    ofs << "   Name      Pri  Description" << endl;
    ofs << "------------ --- -------------------------------------------------" << endl;

    list<CAmberFFPtr>::reverse_iterator it = ffs.rbegin();
    list<CAmberFFPtr>::reverse_iterator ie = ffs.rend();

    int pri = 1;
    while( it != ie ) {
        ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
        ofs << setw(12) << (*it)->GetName() << " ";
        ofs << resetiosflags(ios_base::left) << setiosflags(ios_base::right);
        ofs << setw(3) << pri << " ";
        ofs << (*it)->Get<string>(TITLE) << endl;
        pri++;
        it++;
    }
}

//------------------------------------------------------------------------------

void CAmberFF::DescType(CDatabasePtr db,const string& t1,ostream& ofs)
{
    // cache FFs
    list<CAmberFFPtr> ffs;
    CacheFFs(db,ffs);

    CAmberFFPtr ff;
    CEntityPtr type = FindType(ffs,t1,ff);
    if( ! type ){
        stringstream str;
        str << "no atom parameters for " << t1 << " type";
        throw runtime_error( str.str() );
    }

    // find type
    ofs << ">> TYPE" << endl;
    ofs << "   Type A1 : " << t1 << endl;
    ofs << "   Parmset : " << ff->GetName() << endl;
    ofs << endl;
    ofs << "   Type   Mass    Polar   vdW eps   vdw R*       Comment type               Comment van der Waals  " << endl;
    ofs << "   ---- -------- ------- --------- --------     ------------------------- -------------------------" << endl;

    ofs << "   ";
    ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
    ofs << setw(4) << type->GetName() << " ";
    ofs << resetiosflags(ios_base::left) << setiosflags(ios_base::right);
    ofs << fixed << setw(8) << setprecision(3) << type->Get<double>(MASS) << " ";
    ofs << fixed << setw(7) << setprecision(4) << type->Get<double>(POLAR) << " ";
    ofs << fixed << setw(9) << setprecision(6) << type->Get<double>(DEPTH) << " ";
    ofs << fixed << setw(8) << setprecision(5) << type->Get<double>(RSTAR) << "     ";
    ofs << fixed << type->Get<string>(TITLE) << " ";
    ofs << fixed << type->Get<string>(TITLE2) << " ";
    ofs << endl;

}

//------------------------------------------------------------------------------

void CAmberFF::DescBond(CDatabasePtr db, const string& t1, const string& t2, ostream& ofs)
{
    // cache FFs
    list<CAmberFFPtr> ffs;
    CacheFFs(db,ffs);

    // find bond parameters
    CAmberFFPtr ff;
    CEntityPtr bond = FindBond(ffs,t1,t2,ff);
    if( ! bond ){
        stringstream str;
        str << "no bond parameters for bond between " << t1 << " and " << t2 << " types";
        throw runtime_error( str.str() );
    }

    ofs << ">> BOND" << endl;
    ofs << "   Type A1 : " << t1 << endl;
    ofs << "   Type A2 : " << t2 << endl;
    ofs << "   Parmset : " << ff->GetName() << endl;
    ofs << endl;
    ofs << "    A1   A2   Force   Length       Comment                               " << endl;
    ofs << "   ---- ---- ------- --------     ---------------------------------------" << endl;
    ofs << "   ";
    ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
    ofs << setw(4) << bond->Get<string>(ATOM1) << " ";
    ofs << setw(4) << bond->Get<string>(ATOM2) << " ";
    ofs << resetiosflags(ios_base::left) << setiosflags(ios_base::right);
    ofs << fixed << setw(7) << setprecision(2) << bond->Get<double>(FORCE) << " ";
    ofs << fixed << setw(8) << setprecision(4) << bond->Get<double>(EQUIL) << "      ";
    ofs << fixed << bond->Get<string>(TITLE);
    ofs << endl;
}

//------------------------------------------------------------------------------

void CAmberFF::DescAngle(CDatabasePtr db,const string& t1, const string& t2,
                         const string& t3, ostream& ofs)
{
    // cache FFs
    list<CAmberFFPtr> ffs;
    CacheFFs(db,ffs);

    // find angle parameters
    CAmberFFPtr ff;
    CEntityPtr angle = FindAngle(ffs,t1,t2,t3,ff);
    if( ! angle ){
        stringstream str;
        str << "no angle parameters for angle among " << t1 << ", " << t2 << ", and " << t3 << " types";
        throw runtime_error( str.str() );
    }

    ofs << ">> ANGLE" << endl;
    ofs << "   Type A1 : " << t1 << endl;
    ofs << "   Type A2 : " << t2 << endl;
    ofs << "   Type A3 : " << t3 << endl;
    ofs << "   Parmset : " << ff->GetName() << endl;
    ofs << endl;
    ofs << "    A1   A2   A3   Force   Angle       Comment                           " << endl;
    ofs << "   ---- ---- ---- ------- -------     -----------------------------------" << endl;

    ofs << "   ";
    ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
    ofs << setw(4) << angle->Get<string>(ATOM1) << " ";
    ofs << setw(4) << angle->Get<string>(ATOM2) << " ";
    ofs << setw(4) << angle->Get<string>(ATOM3) << " ";
    ofs << resetiosflags(ios_base::left) << setiosflags(ios_base::right);
    ofs << fixed << setw(7) << setprecision(2) << angle->Get<double>(FORCE) << " ";
    ofs << fixed << setw(7) << setprecision(2) << angle->Get<double>(EQUIL) << "     ";
    ofs << fixed << angle->Get<string>(TITLE);
    ofs << endl;
}

//------------------------------------------------------------------------------

void CAmberFF::DescTorsion(CDatabasePtr db,const string& t1, const string& t2,
                           const string& t3,const string& t4, ostream& ofs)
{
    // cache FFs
    list<CAmberFFPtr> ffs;
    CacheFFs(db,ffs);

    // find torsion parameters
    CAmberFFPtr ff;
    vector<CEntityPtr> tors;
    FindTorsion(ffs,t1,t2,t3,t4,ff,tors);
    if( tors.empty() ){
        stringstream str;
        str << "no torsion parameters for torsion among " << t1 << ", " << t2 << ", " << t3 << ", and " << t4 << " types";
        throw runtime_error( str.str() );
    }

    ofs << ">> TORSION" << endl;
    ofs << "   Type A1 : " << t1 << endl;
    ofs << "   Type A2 : " << t2 << endl;
    ofs << "   Type A3 : " << t3 << endl;
    ofs << "   Type A4 : " << t4 << endl;
    ofs << "   Parmset : " << ff->GetName() << endl;
    ofs << endl;
    ofs << "    A1   A2   A3   A4  Div  Force   Phase   PK      Comment                     " << endl;
    ofs << "   ---- ---- ---- ---- --- ------- ------- ----     ----------------------------" << endl;

    vector<CEntityPtr>::iterator it = tors.begin();
    vector<CEntityPtr>::iterator ie = tors.end();

    while( it != ie ){
        ofs << "   ";
        ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
        ofs << setw(4) << (*it)->Get<string>(ATOM1) << " ";
        ofs << setw(4) << (*it)->Get<string>(ATOM2) << " ";
        ofs << setw(4) << (*it)->Get<string>(ATOM3) << " ";
        ofs << setw(4) << (*it)->Get<string>(ATOM4) << " ";
        ofs << resetiosflags(ios_base::left) << setiosflags(ios_base::right);
        ofs << fixed << setw(3) << setprecision(0) << (*it)->Get<double>(DIVIDE) << " ";
        ofs << fixed << setw(7) << setprecision(2) << (*it)->Get<double>(FORCE) << " ";
        ofs << fixed << setw(7) << setprecision(2) << (*it)->Get<double>(EQUIL) << " ";
        ofs << fixed << setw(4) << setprecision(1) << (*it)->Get<double>(PERIOD) << "     ";
        ofs << fixed << (*it)->Get<string>(TITLE) << " ";
        ofs << endl;
        it++;
    }

}

//------------------------------------------------------------------------------

void CAmberFF::DescImproper(CDatabasePtr db,const string& t1, const string& t2,
                           const string& t3,const string& t4, ostream& ofs)
{
    // cache FFs
    list<CAmberFFPtr> ffs;
    CacheFFs(db,ffs);

    // find improper parameters
    CAmberFFPtr         ff;
    vector<CEntityPtr>  improps;
    FindImproper(ffs,t1,t2,t3,t4,ff,improps);
    if( improps.empty() ){
        stringstream str;
        str << "no improper parameters for improper among " << t1 << ", " << t2 << ", " << t3 << ", and " << t4 << " types";
        throw runtime_error( str.str() );
    }

    ofs << ">> IMPROPER" << endl;
    ofs << "   Type A1 : " << t1 << endl;
    ofs << "   Type A2 : " << t2 << endl;
    ofs << "   Type A3 : " << t3 << endl;
    ofs << "   Type A4 : " << t4 << endl;
    ofs << "   Parmset : " << ff->GetName() << endl;
    ofs << endl;
    ofs << "    A1   A2   A3   A4   Force   Phase   PK       Comment                 " << endl;
    ofs << "   ---- ---- ---- ---- ------- ------- ----     -------------------------" << endl;

    vector<CEntityPtr>::iterator it = improps.begin();
    vector<CEntityPtr>::iterator ie = improps.end();

    while( it != ie ){
        ofs << "   ";
        ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
        ofs << setw(4) << (*it)->Get<string>(ATOM1) << " ";
        ofs << setw(4) << (*it)->Get<string>(ATOM2) << " ";
        ofs << setw(4) << (*it)->Get<string>(ATOM3) << " ";
        ofs << setw(4) << (*it)->Get<string>(ATOM4) << " ";
        ofs << resetiosflags(ios_base::left) << setiosflags(ios_base::right);
        ofs << fixed << setw(7) << setprecision(2) << (*it)->Get<double>(FORCE) << " ";
        ofs << fixed << setw(7) << setprecision(2) << (*it)->Get<double>(EQUIL) << " ";
        ofs << fixed << setw(4) << setprecision(1) << (*it)->Get<double>(PERIOD) << "     ";
        ofs << fixed << (*it)->Get<string>(TITLE) << " ";
        ofs << endl;
        it++;
    }

}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CAmberFF::CacheFFs(CDatabasePtr db, list<CAmberFFPtr>& ffs)
{
    CForwardIterator it = db->BeginVariables();
    CForwardIterator et = db->EndVariables();

    ffs.clear();

    while( it != et ) {
        CEntityPtr obj = it->Get<CEntityPtr>(VALUE);
        if( obj && (obj->GetType() == AMBERFF) ) {
            CAmberFFPtr ff = dynamic_pointer_cast<CAmberFF>(obj);
            ffs.push_front(ff);
        }
        it++;
    }
}


//------------------------------------------------------------------------------

CEntityPtr CAmberFF::FindType(list<CAmberFFPtr>& ffs, const string& t1,
                           CAmberFFPtr& ff)
{
    list<CAmberFFPtr>::iterator it = ffs.begin();
    list<CAmberFFPtr>::iterator ie = ffs.end();

    ff = CAmberFFPtr();
    while( it != ie ) {
        CEntityPtr type = (*it)->FindType(t1);
        if( type ){
            ff = *it;
            return( type );
        }
        it++;
    }

    return( CEntityPtr() );
}

//------------------------------------------------------------------------------

CEntityPtr CAmberFF::FindType(list<CAmberFFPtr>& ffs, const string& t1)
{
    CAmberFFPtr ff;
    return( FindType(ffs,t1,ff) );
}

//------------------------------------------------------------------------------

CEntityPtr CAmberFF::FindType(const string& t1)
{
    CEntityPtr list = FindChild( "types" );

    CForwardIterator it = list->BeginChildren();
    CForwardIterator ie = list->EndChildren();

    CEntityPtr type;

    // find the latest type definition
    while( it != ie ){
        if( it->GetName() == t1 ){
            type = *it;
        }
        it++;
    }

    return( type );
}

//------------------------------------------------------------------------------

CEntityPtr CAmberFF::FindBond(list<CAmberFFPtr>& ffs, const string& t1,
                           const string& t2, CAmberFFPtr& ff)
{
    list<CAmberFFPtr>::iterator it = ffs.begin();
    list<CAmberFFPtr>::iterator ie = ffs.end();

    ff = CAmberFFPtr();
    while( it != ie ) {
        CEntityPtr bond = (*it)->FindBond(t1,t2);
        if( bond ){
            ff = *it;
            return( bond );
        }
        it++;
    }

    return( CEntityPtr() );
}

//------------------------------------------------------------------------------

CEntityPtr CAmberFF::FindBond(list<CAmberFFPtr>& ffs,const string& t1,
                           const string& t2)
{
    CAmberFFPtr ff;
    return( FindBond(ffs,t1,t2,ff) );
}

//------------------------------------------------------------------------------

CEntityPtr CAmberFF::FindBond(const string& t1, const string& t2)
{
    CEntityPtr list = FindChild( "bonds" );

    CForwardIterator it = list->BeginChildren();
    CForwardIterator ie = list->EndChildren();

    CEntityPtr bond;

    // find the latest bond definition
    while( it != ie ){
        if( ((it->Get<string>(ATOM1) == t1) && (it->Get<string>(ATOM2) == t2)) ||
            ((it->Get<string>(ATOM1) == t2) && (it->Get<string>(ATOM2) == t1)) ){
            bond = *it;
        }
        it++;
    }

    return( bond );
}

//------------------------------------------------------------------------------

CEntityPtr CAmberFF::FindAngle(list<CAmberFFPtr>& ffs, const string& t1,
                           const string& t2, const string& t3, CAmberFFPtr& ff)
{
    list<CAmberFFPtr>::iterator it = ffs.begin();
    list<CAmberFFPtr>::iterator ie = ffs.end();

    ff = CAmberFFPtr();
    while( it != ie ) {
        CEntityPtr angle = (*it)->FindAngle(t1,t2,t3);
        if( angle ){
            ff = *it;
            return( angle );
        }
        it++;
    }

    return( CEntityPtr() );
}

//------------------------------------------------------------------------------

CEntityPtr CAmberFF::FindAngle(list<CAmberFFPtr>& ffs, const string& t1,
                           const string& t2, const string& t3)
{
    CAmberFFPtr ff;
    return( FindAngle(ffs,t1,t2,t3,ff) );
}

//------------------------------------------------------------------------------

CEntityPtr CAmberFF::FindAngle(const string& t1, const string& t2, const string& t3)
{
    CEntityPtr list = FindChild( "angles" );

    CForwardIterator it = list->BeginChildren();
    CForwardIterator ie = list->EndChildren();

    CEntityPtr angle;

    // find the latest angle definition
    while( it != ie ){
        if( ( ((it->Get<string>(ATOM1) == t1) && (it->Get<string>(ATOM3) == t3)) ||
            ((it->Get<string>(ATOM1) == t3) && (it->Get<string>(ATOM3) == t1)) ) &&
            (it->Get<string>(ATOM2) == t2) ) {
            angle = *it;
        }
        it++;
    }

    return( angle );
}

//------------------------------------------------------------------------------

void CAmberFF::FindTorsion(list<CAmberFFPtr>& ffs, const string& t1,
                           const string& t2, const string& t3, const string& t4,
                           CAmberFFPtr& ff, vector<CEntityPtr>& tors)
{
    list<CAmberFFPtr>::iterator it = ffs.begin();
    list<CAmberFFPtr>::iterator ie = ffs.end();

    ff = CAmberFFPtr();
    while( it != ie ) {
        (*it)->FindTorsion(t1,t2,t3,t4,tors);
        if( ! tors.empty() ){
            ff = *it;
            return;
        }
        it++;
    }
}

//------------------------------------------------------------------------------

void CAmberFF::FindTorsion(list<CAmberFFPtr>& ffs, const string& t1,
                           const string& t2, const string& t3, const string& t4,
                           vector<CEntityPtr>& tors)
{
    CAmberFFPtr ff;
    return( FindTorsion(ffs,t1,t2,t3,t4,ff,tors) );
}

//------------------------------------------------------------------------------

void CAmberFF::FindTorsion(const string& t1, const string& t2, const string& t3, const string& t4,
                           vector<CEntityPtr>& tors)
{
    CEntityPtr list = FindChild( "torsions" );

    CForwardIterator it = list->BeginChildren();
    CForwardIterator ie = list->EndChildren();

    tors.clear();
    bool ok = false;

    // find the latest torsion definition
    while( it != ie ){
        if( ok == false ){
            bool found = false;
            if( (it->Get<string>(ATOM2) == t2) && (it->Get<string>(ATOM3) == t3) ){
                found = true;
                if( it->Get<string>(ATOM1) != "X" ){
                    found &= it->Get<string>(ATOM1) == t1;
                }
                if( it->Get<string>(ATOM4) != "X" ){
                    found &= it->Get<string>(ATOM4) == t4;
                }
            }
            if( (it->Get<string>(ATOM2) == t3) && (it->Get<string>(ATOM3) == t2) ){
                found = true;
                if( it->Get<string>(ATOM1) != "X" ){
                    found &= it->Get<string>(ATOM1) == t4;
                }
                if( it->Get<string>(ATOM4) != "X" ){
                    found &= it->Get<string>(ATOM4) == t1;
                }
            }
            if( found ){
                ok = true;
                tors.clear();
            }
        }

        if( ok ){
            // add record to the list
            tors.push_back(*it);
            // if period is not negative there is no more records for given torsion
            if( (*it)->Get<double>(PERIOD) > 0 ){
                ok = false;
            }
        }

        it++;
    }
}

//------------------------------------------------------------------------------

void CAmberFF::FindImproper(list<CAmberFFPtr>& ffs, const string& t1,
                                  const string& t2, const string& t3,
                                  const string& t4, CAmberFFPtr& ff,
                                  vector<CEntityPtr>& improps)
{
    list<CAmberFFPtr>::iterator it = ffs.begin();
    list<CAmberFFPtr>::iterator ie = ffs.end();

    ff = CAmberFFPtr();
    while( it != ie ) {
        (*it)->FindImproper(t1,t2,t3,t4,improps);
        if( ! improps.empty() ){
            ff = *it;
            return;
        }
        it++;
    }
}

//------------------------------------------------------------------------------

void CAmberFF::FindImproper(list<CAmberFFPtr>& ffs, const string& t1,
                                  const string& t2, const string& t3, const string& t4,
                                  vector<CEntityPtr>& improps)
{
    CAmberFFPtr ff;
    FindImproper(ffs,t1,t2,t3,t4,ff,improps);
}

//------------------------------------------------------------------------------

void CAmberFF::FindImproper(const string& t1, const string& t2, const string& t3,
                                  const string& t4, vector<CEntityPtr>& improps)
{
    CEntityPtr list = FindChild( "impropers" );

    CForwardIterator it = list->BeginChildren();
    CForwardIterator ie = list->EndChildren();

    bool ok = false;
    improps.clear();

    // find the latest improper definition
    while( it != ie ){
        if( ok == false ){
            bool found = false;
            if( (it->Get<string>(ATOM2) == t2) && (it->Get<string>(ATOM3) == t3) ){
                found = true;
                if( it->Get<string>(ATOM1) != "X" ){
                    found &= it->Get<string>(ATOM1) == t1;
                }
                if( it->Get<string>(ATOM4) != "X" ){
                    found &= it->Get<string>(ATOM4) == t4;
                }
            }
            if( (it->Get<string>(ATOM2) == t3) && (it->Get<string>(ATOM3) == t2) ){
                found = true;
                if( it->Get<string>(ATOM1) != "X" ){
                    found &= it->Get<string>(ATOM1) == t4;
                }
                if( it->Get<string>(ATOM4) != "X" ){
                    found &= it->Get<string>(ATOM4) == t1;
                }
            }
            if( found ){
                ok = true;
                improps.clear();
            }
        }

        if( ok ){
            // add record to the list
            improps.push_back(*it);
            // if period is not negative there is no more records for given improper
            if( (*it)->Get<double>(PERIOD) > 0 ){
                ok = false;
            }
        }

        it++;
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CAmberFF::Desc(ostream& ofs)
{
    DescHeader(ofs);

    if( NumberOfTypes() > 0 ){
        ofs << endl;
        DescTypes( ofs );
    }
    if( NumberOfBonds() > 0 ){
        ofs << endl;
        DescBonds( ofs );
    }
    if( NumberOfAngles() > 0 ){
        ofs << endl;
        DescAngles( ofs );
    }
    if( NumberOfTorsions() > 0 ){
        ofs << endl;
        DescTorsions( ofs );
    }
    if( NumberOfImpropers() > 0 ){
        ofs << endl;
        DescImpropers( ofs );
    }
    if( NumberOfTypes() > 0 ){
        ofs << endl;
        DescvdW( ofs );
    }
}

// -------------------------------------------------------------------------

void CAmberFF::DescSubObj(const string& oname, ostream& ofs)
{
    // check name
    int ndot = count( oname.begin(), oname.end(), '.' );
    if( ndot >= 1 ) {
        throw runtime_error( "too many dots - only *, types, bonds, angles, torsions, and impropers categories are supported");
    }

    if( oname == "types" ){
        DescHeader(ofs);
        ofs << endl;
        DescTypes( ofs );
        return;
    }
    if( oname == "bonds" ){
        DescHeader(ofs);
        ofs << endl;
        DescBonds( ofs );
        return;
    }
    if( oname == "angles" ){
        DescHeader(ofs);
        ofs << endl;
        DescAngles( ofs );
        return;
    }
    if( oname == "torsions" ){
        DescHeader(ofs);
        ofs << endl;
        DescTorsions( ofs );
        return;
    }
    if( oname == "impropers" ){
        DescHeader(ofs);
        ofs << endl;
        DescImpropers( ofs );
        return;
    }

    throw runtime_error( "unrecognized category: only types, bonds, angles, torsions, and impropers are supported");
}

// -------------------------------------------------------------------------

void CAmberFF::DescHeader(ostream& ofs)
{
    ofs << "AMBERFF" << endl;
    ofs << "   Name   : " << GetName() << endl;
    ofs << "   GID    : " << GetId() << endl;
    ofs << "   Title  : " << Get<string>(TITLE) << endl;
    ofs << "   Number of types     : " << NumberOfTypes() << endl;
    ofs << "   Number of bonds     : " << NumberOfBonds() << endl;
    ofs << "   Number of angles    : " << NumberOfAngles() << endl;
    ofs << "   Number of torsions  : " << NumberOfTorsions() << endl;
    ofs << "   Number of impropers : " << NumberOfImpropers() << endl;
}

// -------------------------------------------------------------------------

void CAmberFF::DescTypes(ostream& ofs)
{
    ofs << "   >>> TYPES" << endl;
    ofs << "   Type   Mass    Polar       Comment                 " << endl;
    ofs << "   ---- -------- -------     -------------------------" << endl;

    CEntityPtr list = FindChild( "types" );

    CForwardIterator it = list->BeginChildren();
    CForwardIterator ie = list->EndChildren();

    while( it != ie ){
        ofs << "   ";
        ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
        ofs << setw(4) << it->GetName() << " ";
        ofs << resetiosflags(ios_base::left) << setiosflags(ios_base::right);
        ofs << fixed << setw(8) << setprecision(3) << it->Get<double>(MASS) << " ";
        ofs << fixed << setw(7) << setprecision(4) << it->Get<double>(POLAR) << "     ";
        ofs << fixed << it->Get<string>(TITLE);
        ofs << endl;
        it++;
    }
}

// -------------------------------------------------------------------------

void CAmberFF::DescBonds(ostream& ofs)
{
    ofs << "   >>> BONDS                                                             " << endl;
    ofs << "    A1   A2   Force   Length       Comment                               " << endl;
    ofs << "   ---- ---- ------- --------     ---------------------------------------" << endl;

    CEntityPtr list = FindChild( "bonds" );

    CForwardIterator it = list->BeginChildren();
    CForwardIterator ie = list->EndChildren();
    while( it != ie ){
        ofs << "   ";
        ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
        ofs << setw(4) << it->Get<string>(ATOM1) << " ";
        ofs << setw(4) << it->Get<string>(ATOM2) << " ";
        ofs << resetiosflags(ios_base::left) << setiosflags(ios_base::right);
        ofs << fixed << setw(7) << setprecision(2) << it->Get<double>(FORCE) << " ";
        ofs << fixed << setw(8) << setprecision(4) << it->Get<double>(EQUIL) << "      ";
        ofs << fixed << it->Get<string>(TITLE);
        ofs << endl;
        it++;
    }

}

// -------------------------------------------------------------------------

void CAmberFF::DescAngles(ostream& ofs)
{
    ofs << "   >>> ANGLES                                                            " << endl;
    ofs << "    A1   A2   A3   Force   Angle       Comment                           " << endl;
    ofs << "   ---- ---- ---- ------- -------     -----------------------------------" << endl;

    CEntityPtr list = FindChild( "angles" );

    CForwardIterator it = list->BeginChildren();
    CForwardIterator ie = list->EndChildren();

    while( it != ie ){
        ofs << "   ";
        ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
        ofs << setw(4) << it->Get<string>(ATOM1) << " ";
        ofs << setw(4) << it->Get<string>(ATOM2) << " ";
        ofs << setw(4) << it->Get<string>(ATOM3) << " ";
        ofs << resetiosflags(ios_base::left) << setiosflags(ios_base::right);
        ofs << fixed << setw(7) << setprecision(2) << it->Get<double>(FORCE) << " ";
        ofs << fixed << setw(7) << setprecision(2) << it->Get<double>(EQUIL) << "     ";
        ofs << fixed << it->Get<string>(TITLE);
        ofs << endl;
        it++;
    }

}

// -------------------------------------------------------------------------

void CAmberFF::DescTorsions(ostream& ofs)
{
    ofs << "   >>> TORSIONS                                                                 " << endl;
    ofs << "    A1   A2   A3   A4  Div  Force   Phase   PK      Comment                     " << endl;
    ofs << "   ---- ---- ---- ---- --- ------- ------- ----     ----------------------------" << endl;

    CEntityPtr list = FindChild( "torsions" );

    CForwardIterator it = list->BeginChildren();
    CForwardIterator ie = list->EndChildren();

    while( it != ie ){
        ofs << "   ";
        ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
        ofs << setw(4) << it->Get<string>(ATOM1) << " ";
        ofs << setw(4) << it->Get<string>(ATOM2) << " ";
        ofs << setw(4) << it->Get<string>(ATOM3) << " ";
        ofs << setw(4) << it->Get<string>(ATOM4) << " ";
        ofs << resetiosflags(ios_base::left) << setiosflags(ios_base::right);
        ofs << fixed << setw(3) << setprecision(0) << it->Get<double>(DIVIDE) << " ";
        ofs << fixed << setw(7) << setprecision(2) << it->Get<double>(FORCE) << " ";
        ofs << fixed << setw(7) << setprecision(2) << it->Get<double>(EQUIL) << " ";
        ofs << fixed << setw(4) << setprecision(1) << it->Get<double>(PERIOD) << "     ";
        ofs << fixed << it->Get<string>(TITLE);
        ofs << endl;
        it++;
    }

}

// -------------------------------------------------------------------------

void CAmberFF::DescImpropers(ostream& ofs)
{
    ofs << "   >>> IMPROPERS                                                         " << endl;
    ofs << "    A1   A2   A3   A4   Force   Phase   PK       Comment                 " << endl;
    ofs << "   ---- ---- ---- ---- ------- ------- ----     -------------------------" << endl;

    CEntityPtr list = FindChild( "impropers" );

    CForwardIterator it = list->BeginChildren();
    CForwardIterator ie = list->EndChildren();

    while( it != ie ){
        ofs << "   ";
        ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
        ofs << setw(4) << it->Get<string>(ATOM1) << " ";
        ofs << setw(4) << it->Get<string>(ATOM2) << " ";
        ofs << setw(4) << it->Get<string>(ATOM3) << " ";
        ofs << setw(4) << it->Get<string>(ATOM4) << " ";
        ofs << resetiosflags(ios_base::left) << setiosflags(ios_base::right);
        ofs << fixed << setw(7) << setprecision(2) << it->Get<double>(FORCE) << " ";
        ofs << fixed << setw(7) << setprecision(2) << it->Get<double>(EQUIL) << " ";
        ofs << fixed << setw(4) << setprecision(1) << it->Get<double>(PERIOD) << "     ";
        ofs << fixed << it->Get<string>(TITLE);
        ofs << endl;
        it++;
    }

}

// -------------------------------------------------------------------------

void CAmberFF::DescvdW(ostream& ofs)
{
    ofs << "   >>> van der Waals parameters " << endl;
    ofs << "   Type  vdW eps   vdw R*        Comment  " << endl;
    ofs << "   ---- --------- --------     -------------------------" << endl;

    CEntityPtr list = FindChild( "types" );

    CForwardIterator it = list->BeginChildren();
    CForwardIterator ie = list->EndChildren();

    while( it != ie ){
        ofs << "   ";
        ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
        ofs << setw(4) << it->GetName() << " ";
        ofs << resetiosflags(ios_base::left) << setiosflags(ios_base::right);
        ofs << fixed << setw(9) << setprecision(6) << it->Get<double>(DEPTH) << " ";
        ofs << fixed << setw(8) << setprecision(5) << it->Get<double>(RSTAR) << "     ";
        ofs << fixed << it->Get<string>(TITLE2);
        ofs << endl;
        it++;
    }
}

// -------------------------------------------------------------------------

int CAmberFF::NumberOfTypes(void)
{
    CEntityPtr list = FindChild( "types" );
    return( list->NumberOfChildren() );
}

// -------------------------------------------------------------------------

int CAmberFF::NumberOfBonds(void)
{
    CEntityPtr list = FindChild( "bonds" );
    return( list->NumberOfChildren() );
}

// -------------------------------------------------------------------------

int CAmberFF::NumberOfAngles(void)
{
    CEntityPtr list = FindChild( "angles" );
    return( list->NumberOfChildren() );
}

// -------------------------------------------------------------------------

int CAmberFF::NumberOfTorsions(void)
{
    CEntityPtr list = FindChild( "torsions" );
    return( list->NumberOfChildren() );
}

// -------------------------------------------------------------------------

int CAmberFF::NumberOfImpropers(void)
{
    CEntityPtr list = FindChild( "impropers" );
    return( list->NumberOfChildren() );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

}
