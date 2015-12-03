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

#include <misc/Geometry.hpp>
#include <types/AtomTypes.hpp>
#include <core/PredefinedKeys.hpp>
#include <core/RecursiveIterator.hpp>
#include <sstream>

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CPoint GetCOM(CContext* p_ctx,CEntityPtr& obj)
{
    if( ! obj ){
        throw runtime_error("object is NULL in GetCOM");
    }

    CPoint com;

    // atom is special case
    if( obj->GetType() == ATOM ){
        com.x = obj->Get<double>(POSX);
        com.y = obj->Get<double>(POSY);
        com.z = obj->Get<double>(POSZ);
        return(com);
    }

    // handle unit and residue
    if( (obj->GetType() == UNIT) ||
        (obj->GetType() == RESIDUE) ){

        CRecursiveIterator it = CRecursiveIterator(obj);
        it.SetFilter(ATOM);
        it.SetToBegin();
        CRecursiveIterator ie = it;
        ie.SetToEnd();

        // calculate COM -----------------------
        double tmass = 0.0;
        while( it != ie ){
            CPoint pos;
            double mass;
            pos.x = it->Get<double>(POSX);
            pos.y = it->Get<double>(POSY);
            pos.z = it->Get<double>(POSZ);
            mass  = CAtomTypes::GetMass( p_ctx, it->Get<string>(TYPE) );
            com   += pos*mass;
            tmass += mass;
            it++;
        }

        if( tmass == 0 ) {
            stringstream str;
            str << "object " << obj->GetPathName() << " has zero mass";
            throw runtime_error(str.str());
        }

        com /= tmass;
        return(com);
    }

    // unsupported type
    stringstream str;
    str << "unsupported object type: expecting UNIT, RESIDUE or ATOM, but ";
    str << obj->GetType().GetName() << " provided";
    throw runtime_error(str.str());
}

//------------------------------------------------------------------------------

double GetDistance(const CPoint& p1,const CPoint& p2)
{
    CPoint dif = p2-p1;
    return(Size(dif));
}

//------------------------------------------------------------------------------

double GetAngle(const CPoint& p1,const CPoint& p2,const CPoint& p3)
{
    CPoint v1,v2;
    v1 = p1-p2;
    v2 = p3-p2;
    return(Angle(v1,v2));
}

//------------------------------------------------------------------------------

#define SIGN(a,b)  ((b >= 0.0) ? (a) : -(a))

double GetTorsion(const CPoint& p1,const CPoint& p2,const CPoint& p3,const CPoint& p4)
{
    float iax ,iay ,iaz ,ibx ,iby ,ibz ;
    float icx ,icy ,icz ,idx ,idy ,idz ;

    iax = p1.x;
    iay = p1.y;
    iaz = p1.z;

    ibx =p2.x;
    iby = p2.y;
    ibz = p2.z;

    icx = p3.x;
    icy = p3.y;
    icz = p3.z;

    idx = p4.x;
    idy = p4.y;
    idz = p4.z;

    float eabx,eaby,eabz;
    float ebcx,ebcy,ebcz;
    float ecdx,ecdy,ecdz;
    float abbcx,abbcy,abbcz;
    float dccbx,dccby,dccbz;
    float abcdx,abcdy,abcdz,signum;
    float cosdel,xrcd;
    float rab,rbc,rcd,xrbc,xrab,cosb,phib,xsinb,cosc,phic,xsinc;

    //       bond lengths and unit vectors

    eabx = ibx - iax;
    eaby = iby - iay;
    eabz = ibz - iaz;

    rab = sqrt (eabx * eabx + eaby * eaby + eabz * eabz);
    xrab = 1.0 / rab;
    eabx = eabx * xrab;
    eaby = eaby * xrab;
    eabz = eabz * xrab;
    ebcx = icx - ibx;
    ebcy = icy - iby;
    ebcz = icz - ibz;

    rbc = sqrt (ebcx * ebcx + ebcy * ebcy + ebcz * ebcz);
    xrbc = 1.0 / rbc;
    ebcx = ebcx * xrbc;
    ebcy = ebcy * xrbc;
    ebcz = ebcz * xrbc;
    ecdx = idx - icx;
    ecdy = idy - icy;
    ecdz = idz - icz;

    rcd = sqrt (ecdx * ecdx + ecdy * ecdy + ecdz * ecdz);
    xrcd = 1.0 / rcd;
    ecdx = ecdx * xrcd;
    ecdy = ecdy * xrcd;
    ecdz = ecdz * xrcd;

    //       cross and dot products between unit vectors, and bond (!)
    //       angles

    abbcx = eaby * ebcz - eabz * ebcy;
    abbcy = eabz * ebcx - eabx * ebcz;
    abbcz = eabx * ebcy - eaby * ebcx;
    cosb = - (eabx * ebcx + eaby * ebcy + eabz * ebcz);
    phib = acos(cosb);
    xsinb = 1.0 / sin(phib);
    dccbx = ecdy * ebcz - ecdz * ebcy;
    dccby = ecdz * ebcx - ecdx * ebcz;
    dccbz = ecdx * ebcy - ecdy * ebcx;
    cosc = - (ecdx * ebcx + ecdy * ebcy + ecdz * ebcz);
    phic = acos(cosc);
    xsinc = 1.0 / sin(phic);

    //        torsional angle

    abcdx = - ( abbcy * dccbz - abbcz * dccby );
    abcdy = - ( abbcz * dccbx - abbcx * dccbz );
    abcdz = - ( abbcx * dccby - abbcy * dccbx );
    signum = SIGN(1.0,
                  (abcdx * ebcx + abcdy * ebcy + abcdz * ebcz) );
    cosdel = - (abbcx * dccbx + abbcy * dccby + abbcz * dccbz)
             *  xsinb * xsinc;

    if(cosdel < -1.0) cosdel = -1.0;
    if(cosdel >  1.0) cosdel =  1.0;

    float  angle = signum * acos (cosdel);

    return(angle);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}
