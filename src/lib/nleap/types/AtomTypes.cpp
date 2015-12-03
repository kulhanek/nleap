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

#include <types/AtomTypes.hpp>
#include <core/PredefinedKeys.hpp>
#include <core/ForwardIterator.hpp>
#include <types/Factory.hpp>
#include <iomanip>
#include <sstream>
#include <PeriodicTable.hpp>
#include <engine/Context.hpp>

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CAtomTypes::CAtomTypes( void )
: CEntity(ATOMTYPES)
{

}

// -----------------------------------------------------------------------------

CAtomTypes::CAtomTypes(int& top_id)
: CEntity(ATOMTYPES)
{
    SetId( top_id++ );
}

// -----------------------------------------------------------------------------

void CAtomTypes::Desc( ostream& ofs )
{
    ofs << "ATOMTYPES" << endl;
    ofs << "   Name           : " << GetName() << endl;
    ofs << "   GID            : " << GetId() << endl;
    ofs << "   Num of entries : " << NumberOfChildren() << endl;

    ofs << endl;
    ofs << "   >>> TYPES" << endl;
    ofs << "    Type   Element   Hybridization " << endl;
    ofs << "   ------ --------- ---------------" << endl;

    CForwardIterator it = BeginChildren();
    CForwardIterator ie = EndChildren();

    while( it != ie ){
        ofs << "   ";
        ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
        ofs << setw(6) << it->GetName() << " ";
        ofs << resetiosflags(ios_base::left) << setiosflags(ios_base::right);
        ofs << setw(9) << it->Get<string>(ELEMENT) << " ";
        ofs << setw(15) << it->Get<string>(HYBRIDIZATION);
        ofs << endl;
        it++;
    }
    ofs << endl;
}

// -----------------------------------------------------------------------------

void CAtomTypes::AddAtomTypes( int& top_id, CListPtr& types, CVerboseStr& vout )
{
    CForwardIterator it = types->BeginChildren();
    CForwardIterator ie = types->EndChildren();

    while( it != ie ){
        CForwardIterator tit = it->BeginChildren();
        CForwardIterator tie = it->EndChildren();

        string type,element,hybridization;

        if( it->GetType() != LIST ){
            stringstream str;
            str << "item in the list is not of type LIST, but it is " << it->GetType().GetName();
            str << " with value '" << it->Get<string>(VALUE) << "'";
            throw runtime_error(str.str());
        }

        if( tit != tie ) {
            if( tit->GetType() != STRING ){
                throw runtime_error("type has to be a string");
            }
            type = tit->Get<string>(VALUE);
            tit++;
        } else {
            throw runtime_error("missing atom type in the list");
        }

        if( tit != tie ) {
            if( tit->GetType() != STRING ){
                throw runtime_error("element has to be a string");
            }
            element = tit->Get<string>(VALUE);
            tit++;
        } else {
            throw runtime_error("missing atom element in the list");
        }

        if( tit != tie ) {
            if( tit->GetType() != STRING ){
                throw runtime_error("hybridization has to be a string");
            }
            hybridization = tit->Get<string>(VALUE);
            tit++;
        } else {
            throw runtime_error("missing atom hybridization in the list");
        }

        if( tit != tie ) {
            stringstream str;
            str << "too many items (>3) for the atom type '" << type << "'";
            throw runtime_error( str.str() );
        }

        CEntityPtr obj;

        // try to find the type
        obj = FindChild( type );
        bool found = false;
        if( obj == NULL ){
            // create new one
            obj = CFactory::CreateNode(top_id, type);
        } else {
            vout << medium;
            vout << "<blue>Warning:</blue> Overwriting already existing type '" << type << "'" << endl;
            vout << low;
            found = true;
        }

        obj->Set(ELEMENT,element);
        obj->Set(HYBRIDIZATION,hybridization);

        if( ! found ){
            AddChild(obj);
        }

        it++;
    }
}

//------------------------------------------------------------------------------

double CAtomTypes::GetMass( CContext* p_ctx, const string& type )
{
    CEntityPtr types = p_ctx->database()->FindChild("_atom_types");

    CForwardIterator it = types->BeginChildren();
    CForwardIterator ie = types->EndChildren();

    string element;
    while( it != ie ){
        if( type == it->GetName() ){
            element = it->Get<string>(ELEMENT);
            break;
        }
        it++;
    }

    if( element.empty() ){
        stringstream str;
        str << "atom type " << type << " is not defined via <b>addAtomTypes</b> command";
        throw runtime_error(str.str());
    }

    const CElement* p_ele = PeriodicTable.SearchBySymbol(element.c_str());
    if( p_ele == NULL ){
        stringstream str;
        str << "atom type " << type << " has defined symbol " << element;
        str << ", which is not valid element";
        throw runtime_error(str.str());
    }

    return( p_ele->GetMass() );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}
