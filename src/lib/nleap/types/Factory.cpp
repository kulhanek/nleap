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

#include <sstream>
#include <cctype>
#include <sstream>
#include <vector>
#include <types/Factory.hpp>
#include <core/PredefinedKeys.hpp>
#include <boost/algorithm/string.hpp>

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CKey CFactory::DetermineType(const string& value)
{
    if( value.empty() ) return(UNKNOWN);

    // string
    if( value[0] == '"' ) return(STRING);
    if( isalpha(value[0]) ) return(STRING);

    for( unsigned int i = 0; i < value.size(); i++ ){
        if( (! isdigit(value[i])) && ( value[i] != '.' ) ) return(STRING);
    }

    return(NUMBER);
}

// -------------------------------------------------------------------------

bool CFactory::IsInteger(const string& value)
{
    int start = 0;
    if( value.size() > 0 ){
        if( value[0] == '+' ) start = 1;
        if( value[0] == '-' ) start = 1;
    }
    for( unsigned int i = start; i < value.size(); i++ ){
        if( ! isdigit(value[i]) ) return(false);
    }

    return(true);
}

// -------------------------------------------------------------------------

int CFactory::GetIntegerNumber(const string& value)
{
    stringstream str(value);
    int num = 0;
    str >> num;
    return( num );
}

// -------------------------------------------------------------------------

bool CFactory::IsNumber(const string& value)
{
    bool first_dot = false;
    int start = 0;
    if( value.size() > 0 ){
        if( value[0] == '+' ) start = 1;
        if( value[0] == '-' ) start = 1;
    }
    for( unsigned int i = start; i < value.size(); i++ ){
        if( value[i] == '.' ){
            if( first_dot == true ) return(false);
            first_dot = true;
        }
        if( ! ( isdigit(value[i]) || (value[i] == '.') ) ) return(false);
    }

    return(true);
}

// -------------------------------------------------------------------------

double CFactory::GetNumber(const string& value)
{
    stringstream str(value);
    double num = 0;
    str >> num;
    return( num );
}

// -------------------------------------------------------------------------

bool CFactory::IsValidVariableName(const string& value)
{
    if( value.empty() ) return(false);
    if( ! isalpha(value[0]) ) return(false);

    for( unsigned int i = 0; i < value.size(); i++ ){
        if( (! isdigit(value[i])) && (! isalpha(value[i])) && (value[i] != '_') ) return(false);
    }
    return(true);
}

// -------------------------------------------------------------------------

CEntityPtr CFactory::Create(const CKey& type)
{
    CEntityPtr obj;

    if( type == NODE ){
        obj = CEntityPtr( new CEntity(NODE) );
    } else if( type == DATABASE ) {
        obj = CDatabasePtr( new CDatabase );
    } else if( type == VARIABLE ) {
        obj = CVariablePtr( new CVariable );
    } else if( type == STRING ) {
        obj = CStringPtr( new CString );
    } else if( type == NUMBER ) {
        obj = CNumberPtr( new CNumber );
    } else if( type == UNIT ) {
        obj = CUnitPtr( new CUnit );
    } else if( type == RESIDUE ) {
        obj = CResiduePtr( new CResidue );
    } else if( type == ATOM ) {
        obj = CAtomPtr( new CAtom );
    } else if( type == BOND ) {
        obj = CBondPtr( new CBond );
    } else if( type == AMBERFF ) {
        obj = CAmberFFPtr( new CAmberFF );
    } else if( type == LIST ) {
        obj = CListPtr( new CList );
    } else if( type == ATOMTYPES ) {
        obj = CAtomTypesPtr( new CAtomTypes );
    } else if( type == PDBATOMMAP ) {
        obj = CPDBAtomMapPtr( new CPDBAtomMap );
    } else if( type == PDBRESMAP ) {
        obj = CPDBResMapPtr( new CPDBResMap );
    } else {
        // wrong
        return( obj );
    }

    return( obj );
}

// -------------------------------------------------------------------------

CDatabasePtr CFactory::CreateDatabase(int& top_id)
{
    CDatabasePtr obj = CDatabasePtr( new CDatabase( top_id ) );
    return(obj);
}

// -------------------------------------------------------------------------

CVariablePtr CFactory::CreateVariable(int& top_id, const string& name)
{
    CVariablePtr obj = CVariablePtr( new CVariable( top_id ) );
    obj->SetName(name);
    return(obj);
}

// -------------------------------------------------------------------------

CEntityPtr CFactory::CreateNode(int& top_id)
{
    CEntityPtr obj = CEntityPtr( new CEntity( NODE, top_id ) );
    return(obj);
}

// -------------------------------------------------------------------------

CEntityPtr CFactory::CreateNode(int& top_id, const string& name)
{
    CEntityPtr obj = CEntityPtr( new CEntity( NODE, top_id ) );
    obj->SetName(name);
    return(obj);
}

// -------------------------------------------------------------------------

CAmberFFPtr CFactory::CreateAmberFF(int& top_id)
{
    CAmberFFPtr obj = CAmberFFPtr( new CAmberFF( top_id ) );
    return(obj);
}

// -------------------------------------------------------------------------

CStringPtr CFactory::CreateString(int& top_id, const string& str)
{
    CStringPtr obj = CStringPtr( new CString( top_id ) );
    obj->SetValue(str);
    return(obj);
}

// -------------------------------------------------------------------------

CNumberPtr CFactory::CreateNumber(int& top_id, const string& num)
{
    // convert number
    double n = 0;
    stringstream str(num);
    str >> n;

    // create object
    CNumberPtr obj = CNumberPtr( new CNumber( top_id ) );
    obj->SetValue(n);
    return(obj);
}

// -------------------------------------------------------------------------

CUnitPtr CFactory::CreateUnit(int& top_id)
{
    CUnitPtr obj = CUnitPtr( new CUnit( top_id ) );
    return(obj);
}

// -------------------------------------------------------------------------

CResiduePtr CFactory::CreateResidue(int& top_id)
{
    CResiduePtr obj = CResiduePtr( new CResidue( top_id ) );
    return(obj);
}

// -------------------------------------------------------------------------

CAtomPtr CFactory::CreateAtom(int& top_id)
{
    CAtomPtr obj = CAtomPtr( new CAtom( top_id ) );
    return(obj);
}

// -------------------------------------------------------------------------

CBondPtr CFactory::CreateBond(int& top_id)
{
    CBondPtr obj = CBondPtr( new CBond( top_id ) );
    return(obj);
}

// -------------------------------------------------------------------------

CListPtr CFactory::CreateList(int& top_id)
{
    CListPtr obj = CListPtr( new CList( top_id ) );
    return(obj);
}

// -------------------------------------------------------------------------

CListPtr CFactory::CreateList( int& top_id, const string& list_src )
{
    CListPtr list = CListPtr( new CList( top_id ) );

    vector<string> items;
    split(items, list_src, is_any_of(" \t\n"), token_compress_on);

    vector<string>::iterator it = items.begin();
    vector<string>::iterator ie = items.end();

    if( it == ie ){
        throw runtime_error("list is empty");
    }

    if( (*it) != "{" ){
        stringstream str;
        str << "list is not opened by '{' but by '" << (*it) << "'";
        throw runtime_error( str.str() );
    }

    it++;

    while( it != ie ) {
        string item = (*it);
        if( item.empty() ){
            it++;
            continue;
        }

        if( item == "}" ){
            // end of list
            it++;
            break;
        }

        if( item == "{" ) {
            // new sublist
            vector<string>::iterator nit = it;

            int count = 0;
            while( it != ie ){
                string item = (*it);
                if( item == "{" ) count++;
                if( item == "}" ) count--;
                it++;
                if( count == 0 ) break;
            }
            if( count != 0 ){
                stringstream str;
                str << "sublist is not closed, number of opening = " << count;
                throw runtime_error( str.str() );
            }
            vector<string>::iterator nie = it;

            // create sublist string
            stringstream str;
            while( nit != nie ){
                str << (*nit);
                str << " ";
                nit++;
            }
            CListPtr sublist = CreateList( top_id, str.str() );
            list->AddChild(sublist);
        } else {
            // new object
            CEntityPtr obj;
            CKey type = CFactory::DetermineType( item );
            if( type == STRING ){
                obj = CreateString( top_id, item);
            } else if( type == NUMBER ) {
                obj = CreateNumber( top_id, item);
            } else {
                // wrong type
                stringstream str;
                str << "unrecognized type for list item '" << item << "'";
                throw runtime_error( str.str() );
            }
            list->AddChild(obj);
            it++;
        }
    }

    if( it != ie ){
        if( ! (*it).empty() ) {
            stringstream str;
            str << "list is not correctly terminated, unprocessed item '" << (*it) << "'";
            throw runtime_error( str.str() );
        }
    }

    return(list);
}

// -------------------------------------------------------------------------

CAtomTypesPtr CFactory::CreateAtomTypes(int& top_id)
{
    CAtomTypesPtr obj = CAtomTypesPtr( new CAtomTypes( top_id ) );
    return(obj);
}

//------------------------------------------------------------------------------

CPDBAtomMapPtr CFactory::CreatePDBAtomMap(int& top_id)
{
    CPDBAtomMapPtr obj = CPDBAtomMapPtr( new CPDBAtomMap( top_id ) );
    return(obj);
}

//------------------------------------------------------------------------------

CPDBResMapPtr CFactory::CreatePDBResMap(int& top_id)
{
    CPDBResMapPtr obj = CPDBResMapPtr( new CPDBResMap( top_id ) );
    return(obj);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

}
