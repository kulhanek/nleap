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

#include <types/PDBResMap.hpp>
#include <core/PredefinedKeys.hpp>
#include <core/ForwardIterator.hpp>
#include <types/Factory.hpp>
#include <iomanip>
#include <sstream>

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CPDBResMap::CPDBResMap( void )
: CEntity(PDBRESMAP)
{

}

// -----------------------------------------------------------------------------

CPDBResMap::CPDBResMap(int& top_id)
: CEntity(PDBRESMAP)
{
    SetId( top_id++ );
}

// -----------------------------------------------------------------------------

void CPDBResMap::Desc( ostream& ofs )
{
    ofs << "PDBRESMAP" << endl;
    ofs << "   Name           : " << GetName() << endl;
    ofs << "   GID            : " << GetId() << endl;
    ofs << "   Num of entries : " << NumberOfChildren() << endl;

    ofs << endl;
    ofs << "   >>> MAP" << endl;
    ofs << "    PDB Residue       Lib Residue   Term " << endl;
    ofs << "   ------------- --> ------------- ------" << endl;

    CForwardIterator it = BeginChildren();
    CForwardIterator ie = EndChildren();

    while( it != ie ){
        ofs << "   ";
        ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
        ofs << setw(13) << it->GetName() << " --> ";
        ofs << setw(13) << it->Get<string>(VALUE) << " ";
        ofs << resetiosflags(ios_base::left) << setiosflags(ios_base::right);
        if( it->Get<int>(TERM) >= 0 ){
            ofs << setw(6) << it->Get<int>(TERM);
        }
        ofs << endl;
        it++;
    }
}

// -----------------------------------------------------------------------------

CEntityPtr CPDBResMap::FindMap( const string& oddname, int term)
{
    CForwardIterator it = BeginChildren();
    CForwardIterator ie = EndChildren();

    while( it != ie ){
        if( (it->GetName() == oddname) && (it->Get<int>(TERM) == term) ){
            return(it->GetSelf());
        }

        it++;
    }

    return(CEntityPtr());
}

// -----------------------------------------------------------------------------

void CPDBResMap::AddPDBResMap( int& top_id, CListPtr& types, CVerboseStr& vout )
{
    CForwardIterator it = types->BeginChildren();
    CForwardIterator ie = types->EndChildren();

    while( it != ie ){
        CForwardIterator tit = it->BeginChildren();
        CForwardIterator tie = it->EndChildren();

        string oddname,libname;
        int    term = -1;

        if( it->NumberOfChildren() == 3 ){
            if( tit != tie ) {
                if( tit->GetType() != NUMBER ){
                    throw runtime_error("term has to be a number");
                }
                term = tit->Get<double>(VALUE);
                tit++;
            } else {
                throw runtime_error("missing term in the list");
            }
        }

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
            throw runtime_error("missing odd residue name in the list");
        }

        if( tit != tie ) {
            if( tit->GetType() != STRING ){
                throw runtime_error("lib name has to be a string");
            }
            libname = tit->Get<string>(VALUE);
            tit++;
        } else {
            throw runtime_error("missing lib residue name in the list");
        }

        if( tit != tie ) {
            stringstream str;
            vout << "too many items (>3) for the residue map '";
            vout << oddname << "' (Term: " << term << ")";
            throw runtime_error( str.str() );
        }

        CEntityPtr obj;

        // try to find the type
        obj = FindMap( oddname, term );
        bool found = false;
        if( obj == NULL ){
            // create new one
            obj = CFactory::CreateNode(top_id, oddname);
        } else {
            vout << medium;
            vout << "<blue>Warning:</blue> Overwriting already existing residue map '";
            vout << oddname << "' (Term: " << term << ")" << endl;
            vout << low;
            found = true;
        }

        obj->Set(VALUE,libname);
        obj->Set(TERM,term);

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
