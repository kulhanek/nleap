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

#include <types/Database.hpp>
#include <types/Factory.hpp>
#include <core/PredefinedKeys.hpp>
#include <engine/Context.hpp>
#include <types/Variable.hpp>
#include <core/ForwardIterator.hpp>

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CDatabase::CDatabase(  )
: CEntity(DATABASE)
{
}

//------------------------------------------------------------------------------

CDatabase::CDatabase( int& top_id  )
: CEntity(DATABASE)
{
    SetId( top_id++ );
    SetName( "database" );

    CEntityPtr node;

    node = CFactory::CreateNode( top_id, "_objects" );
    AddChild(node);

    node = CFactory::CreateNode( top_id, "_variables" );
    AddChild(node);

    node = CFactory::CreateAtomTypes( top_id);
    node->SetName("_atom_types");
    AddChild(node);

    node = CFactory::CreatePDBAtomMap( top_id);
    node->SetName("_pdb_atom_map");
    AddChild(node);

    node = CFactory::CreatePDBResMap( top_id);
    node->SetName("_pdb_res_map");
    AddChild(node);

    node = CFactory::CreateNode( top_id, "_histcmds" );
    AddChild(node);
}

//------------------------------------------------------------------------------

void CDatabase::DescSubObj(const string& oname,ostream& ofs)
{
    // get object name
    string obj_name = oname;
    string sub_obj_name;

    int ndot = count( oname.begin(), oname.end(), '.' );

    if( ndot >= 1 ) {
        unsigned int dot = oname.find('.');
        obj_name = oname.substr(0,dot);
        if( dot + 1 < oname.size() ){
            sub_obj_name =  oname.substr(dot+1,oname.size()-1);
        }
    }

    CEntityPtr obj = GetVariableObject( obj_name );

    if( obj ){
        if( sub_obj_name.empty() ){
            obj->Desc(ofs);
            return;
        } else {
            return( obj->DescSubObj(sub_obj_name,ofs) );
        }
    } else {
        if( (! obj_name.empty() ) && (obj_name[0] == '_') ){
            return( CEntity::DescSubObj(oname,ofs) );
        } else {
            throw runtime_error("variable '" + obj_name + "' was not found");
        }
    }
}

//------------------------------------------------------------------------------

CEntityPtr CDatabase::GetSubObj(const string& oname)
{
    // get object name
    string obj_name = oname;
    string sub_obj_name;

    int ndot = count( oname.begin(), oname.end(), '.' );

    if( ndot >= 1 ) {
        unsigned int dot = oname.find('.');
        obj_name = oname.substr(0,dot);
        if( dot + 1 < oname.size() ){
            sub_obj_name =  oname.substr(dot+1,oname.size()-1);
        }
    }

    CEntityPtr obj = GetVariableObject( obj_name );

    if( obj ){
        if( sub_obj_name.empty() ){
            return(obj);
        } else {
            return( obj->GetSubObj(sub_obj_name) );
        }
    } else {
        if( (! obj_name.empty() ) && (obj_name[0] == '_') ){
            return( CEntity::GetSubObj(oname) );
        } else {
            throw runtime_error("variable '" + obj_name + "' was not found");
        }
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CStringPtr CDatabase::CreateString( int& top_id, const string& value )
{
    CEntityPtr objs = FindChild( "_objects" );

    CStringPtr str = CFactory::CreateString(top_id, value);
    objs->AddChild(str);

    return(str);
}

//------------------------------------------------------------------------------

CNumberPtr CDatabase::CreateNumber( int& top_id, const string& value )
{
    CEntityPtr objs = FindChild( "_objects" );

    CNumberPtr num = CFactory::CreateNumber( top_id, value);
    objs->AddChild(num);

    return(num);
}

//------------------------------------------------------------------------------

CUnitPtr CDatabase::CreateUnit( int& top_id, const string& name )
{
    CUnitPtr unit = CFactory::CreateUnit( top_id );
    unit->SetName(name);

    CEntityPtr objs = FindChild( "_objects" );
    objs->AddChild(unit);

    return(unit);
}

//------------------------------------------------------------------------------

CResiduePtr CDatabase::CreateResidue( int& top_id, const string& name )
{
    CEntityPtr objs = FindChild( "_objects" );

    CResiduePtr res = CFactory::CreateResidue( top_id );
    res->SetName(name);
    objs->AddChild(res);

    return(res);
}

//------------------------------------------------------------------------------

CAtomPtr CDatabase::CreateAtom( int& top_id, const string& name )
{
    CEntityPtr objs = FindChild( "_objects" );

    CAtomPtr atm = CFactory::CreateAtom( top_id );
    atm->SetName(name);
    objs->AddChild(atm);

    return(atm);
}

//------------------------------------------------------------------------------

CAmberFFPtr CDatabase::CreateAmberFF( int& top_id )
{
    CEntityPtr objs = FindChild( "_objects" );

    CAmberFFPtr ff = CFactory::CreateAmberFF( top_id );
    objs->AddChild(ff);

    return(ff);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CAtomTypesPtr CDatabase::GetAtomTypes(void)
{
    CEntityPtr objs = FindChild( "_atom_types" );
    return( dynamic_pointer_cast< CAtomTypes >(objs) );
}

//------------------------------------------------------------------------------

CPDBAtomMapPtr CDatabase::GetPDBAtomMap(void)
{
    CEntityPtr objs = FindChild( "_pdb_atom_map" );
    return( dynamic_pointer_cast< CPDBAtomMap >(objs) );
}

//------------------------------------------------------------------------------

CPDBResMapPtr CDatabase::GetPDBResMap(void)
{
    CEntityPtr objs = FindChild( "_pdb_res_map" );
    return( dynamic_pointer_cast< CPDBResMap >(objs) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CForwardIterator CDatabase::BeginVariables( )
{
    CEntityPtr vars = FindChild( "_variables" );
    return( vars->BeginChildren() );
}

//------------------------------------------------------------------------------

CForwardIterator CDatabase::EndVariables( )
{
    CEntityPtr vars = FindChild( "_variables" );
    return( vars->EndChildren() );
}

//------------------------------------------------------------------------------

bool CDatabase::IsVariable( const string& name )
{
    CEntityPtr vars = FindChild( "_variables" );
    return( vars->FindChild(name) );
}

//------------------------------------------------------------------------------

void CDatabase::ReleaseVariable( const string& name )
{
    CEntityPtr vars = FindChild( "_variables" );
    CEntityPtr var = vars->FindChild( name );
    if( var ){
        vars->RemoveChild( var );
    }
}

//------------------------------------------------------------------------------

void CDatabase::SetVariable( int& top_id, const string& name, CEntityPtr object )
{
    if( IsVariable(name) ){
        ReleaseVariable(name);
    }

    CVariablePtr var = CFactory::CreateVariable( top_id, name);
    var->SetObject( object );

    CEntityPtr vars = FindChild( "_variables" );
    vars->AddChild(var);
}

//------------------------------------------------------------------------------

CEntityPtr CDatabase::GetVariableObject( const string& name )
{
    CEntityPtr vars = FindChild( "_variables" );
    CEntityPtr ovar = vars->FindChild( name );
    if( ! ovar ){
        return( CEntityPtr() ); // not found
    }
    CVariablePtr var = dynamic_pointer_cast< CVariable >(ovar);
    if( ! var ){
        return( CEntityPtr() ); // incorrect type
    }
    return( var->GetObject() );
}

//------------------------------------------------------------------------------

void CDatabase::ClearDatabase(void)
{
    CEntityPtr node;

    node = FindChild( "_objects" );
    if( node ){
        node->RemoveAllChildren();
    }

    node = FindChild( "_variables" );
    if( node ){
        node->RemoveAllChildren();
    }

    node = FindChild( "_atom_types" );
    if( node ){
        node->RemoveAllChildren();
    }

    node = FindChild( "_pdb_atom_map" );
    if( node ){
        node->RemoveAllChildren();
    }

    node = FindChild( "_pdb_res_map" );
    if( node ){
        node->RemoveAllChildren();
    }

    node = FindChild( "_histcmds" );
    if( node ){
        node->RemoveAllChildren();
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

}
