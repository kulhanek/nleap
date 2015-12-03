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

#include <types/PDBAtomMap.hpp>
#include <core/PredefinedKeys.hpp>
#include <core/ForwardIterator.hpp>
#include <types/Factory.hpp>
#include <iomanip>
#include <sstream>

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CPDBAtomMap::CPDBAtomMap( void )
: CEntity(PDBATOMMAP)
{

}

// -----------------------------------------------------------------------------

CPDBAtomMap::CPDBAtomMap(int& top_id)
: CEntity(PDBATOMMAP)
{
    SetId( top_id++ );
}

// -----------------------------------------------------------------------------

void CPDBAtomMap::Desc( ostream& ofs )
{
    ofs << "PDBATOMMAP" << endl;
    ofs << "   Name           : " << GetName() << endl;
    ofs << "   GID            : " << GetId() << endl;
    ofs << "   Num of entries : " << NumberOfChildren() << endl;

    ofs << endl;
    ofs << "   >>> MAP" << endl;
    ofs << "    Odd Name       Lib Name " << endl;
    ofs << "   ----------     ----------" << endl;

    CForwardIterator it = BeginChildren();
    CForwardIterator ie = EndChildren();

    ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
    while( it != ie ){
        ofs << "   ";
        ofs << setw(10) << it->GetName() << " --> ";
        ofs << setw(10) << it->Get<string>(VALUE);
        ofs << endl;
        it++;
    }
}

// -----------------------------------------------------------------------------

void CPDBAtomMap::AddPDBAtomMap( int& top_id, CListPtr& types, CVerboseStr& vout )
{
    CForwardIterator it = types->BeginChildren();
    CForwardIterator ie = types->EndChildren();

    while( it != ie ){
        CForwardIterator tit = it->BeginChildren();
        CForwardIterator tie = it->EndChildren();

        string oddname,libname;

        if( it->GetType() != LIST ){
            stringstream str;
            str << "item in the list is not of type LIST, but it is " << it->GetType().GetName();
            str << " with value '" << it->Get<string>(VALUE) << "'";
            throw runtime_error(str.str());
        }

        if( tit != tie ) {
            if( tit->GetType() != STRING ){
                throw runtime_error("odd name has to be a string");
            }
            oddname = tit->Get<string>(VALUE);
            tit++;
        } else {
            throw runtime_error("missing odd name in the list");
        }

        if( tit != tie ) {
            if( tit->GetType() != STRING ){
                throw runtime_error("lib name has to be a string");
            }
            libname = tit->Get<string>(VALUE);
            tit++;
        } else {
            throw runtime_error("missing lib name in the list");
        }

        if( tit != tie ) {
            stringstream str;
            str << "too many items (>2) for the PDB atom map '";
            str << oddname << "' -> '" << libname << "'";
            throw runtime_error( str.str() );
        }

        CEntityPtr obj;

        // try to find existing map
        obj = FindChild( oddname );
        bool found = false;
        if( obj == NULL ){
            // create new one
            obj = CFactory::CreateNode(top_id, oddname);
        } else {
            vout << medium;
            vout << "<blue>Warning:</blue> Overwriting already existing PDB atom map for '" << oddname << "'" << endl;
            vout << low;
            found = true;
        }

        obj->Set(VALUE,libname);

        if( ! found ){
            AddChild(obj);
        }

        it++;
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}
