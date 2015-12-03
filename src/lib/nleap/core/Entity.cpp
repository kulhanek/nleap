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

#include <iomanip>
#include <XMLElement.hpp>
#include <ErrorSystem.hpp>
#include <core/Entity.hpp>
#include <core/ForwardIterator.hpp>
#include <types/Factory.hpp>
#include <core/PredefinedKeys.hpp>

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CEntity::CEntity(const CKey& type)
: m_type( type )
{
    m_id = -1;
    m_root = NULL;
    m_self = NULL;
    m_last = NULL;
}

// -------------------------------------------------------------------------

CEntity::CEntity(const CKey& type, int& top_id)
: m_type( type )
{
    m_id = -1;
    SetId( top_id++ );
    m_root = NULL;
    m_self = NULL;
    m_last = NULL;
}

// -------------------------------------------------------------------------

CEntity::~CEntity(void)
{
    m_root = NULL;
    m_self = NULL;
    m_last = NULL;

    // unregister related objects
    list< CEntityWPtr >::iterator it = m_related.begin();
    list< CEntityWPtr >::iterator ie = m_related.end();

    while( it != ie ){
        if( ! (*it).expired() ){
            (*it).lock()->m_properties.UnsetObjectProperty( GetSelf() );
        }
        it++;
    }

    // now destroy all children - in linear fashion
    // if this is not used then very long destruction chain (m_sibling->m_sibling->...)
    // can occur which can lead to stack overflow
    while( m_first ){
        RemoveFirstChild();
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CEntityPtr CEntity::Clone(int& top_id, int base_id)
{
    // clone objects weakly
    CEntityPtr cloned = CloneWeakly(top_id, base_id);

    // create object map from cloned objects
    map< int, CEntityPtr > obj_map;
    cloned->UpdateObjectMap(obj_map);

    // bind weakly bound properties
    cloned->BindWeakProperties(obj_map);

    return( cloned );
}

// -------------------------------------------------------------------------

CEntityPtr CEntity::CloneWeakly(int& top_id, int base_id)
{
    CEntityPtr cloned = CFactory::Create(GetType());

    if( ! cloned ){
        throw runtime_error("unable to create object '" + GetType().GetName() + "' in CEntity::clone_weakly");
    }

    // id
    cloned->m_name = m_name;
    cloned->m_id =  m_id + base_id;
    if( top_id < cloned->m_id ) top_id = cloned->m_id;

    // properties
    cloned->m_properties.CloneWeakly(m_properties, base_id);

    // children
    CEntityPtr obj = m_first;

    while( obj ){
        CEntityPtr child_cloned = obj->CloneWeakly(top_id, base_id);
        cloned->AddChild(child_cloned);
        obj = obj->m_sibling;
    }

    return(cloned);
}

// -------------------------------------------------------------------------

void CEntity::BindWeakProperties(map< int, CEntityPtr >&  obj_map)
{
    // bind weak objects
    m_properties.BindWeak(obj_map);

    // recursivelly
    CEntityPtr obj = m_first;

    while( obj ){
        obj->BindWeakProperties(obj_map);
        obj = obj->m_sibling;
    }
}

// -------------------------------------------------------------------------

void CEntity::UpdateObjectMap(map< int, CEntityPtr >&  obj_map)
{
    CEntityPtr obj = m_first;

    while( obj ){
        obj_map[obj->GetId()] = obj;
        obj->UpdateObjectMap(obj_map);
        obj = obj->m_sibling;
    }

}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CEntityPtr CEntity::GetRoot(void)
{
    if( m_root == NULL ) return( CEntityPtr() );
    return( m_root->GetSelf() );
}

// -------------------------------------------------------------------------

CEntityPtr  CEntity::GetSelf(void)
{
    if( m_self == NULL ) return( CEntityPtr() );
    if( m_self == m_root ) return( m_root->m_first );
    return( m_self->m_sibling );
}

// -------------------------------------------------------------------------

CEntity* CEntity::GetThis(void)
{
    return( this );
}

// -------------------------------------------------------------------------

CEntityPtr  CEntity::GetPrev(void)
{
    if( m_self == NULL ) return( CEntityPtr() );
    if( m_self == m_root ) return( CEntityPtr() );
    return( m_self->GetSelf() );
}

// -------------------------------------------------------------------------

CEntityPtr  CEntity::GetNext(void)
{
    return( m_sibling );
}

// -------------------------------------------------------------------------
// #########################################################################
// -------------------------------------------------------------------------

const CKey& CEntity::GetType(void) const
{
    return(m_type);
}

// -------------------------------------------------------------------------

const string& CEntity::GetName(void) const
{
    return(m_name);
}

// -------------------------------------------------------------------------

const string CEntity::GetPathName(void) const
{
    // skip database and above objects
    if( GetType() == DATABASE ) return( string() );

    // get owner path and append this object name
    string path_name;
    if( m_root ){
        path_name = m_root->GetPathName();
        path_name += ".";
    }
    path_name += m_name;
    return(path_name);
}

// -------------------------------------------------------------------------

void CEntity::SetName(const string& name)
{
    m_name = name;
}

// -------------------------------------------------------------------------

int CEntity::GetId(void) const
{
    return(m_id);
}

// -------------------------------------------------------------------------

void CEntity::SetId(int id)
{
    m_id = id;
    if( m_name.empty() ){
        string stype = GetType().GetName();
        stringstream str;
        str << stype[0] << m_id;
        SetName( str.str() );
    }
}

// -------------------------------------------------------------------------

void CEntity::Desc(ostream& ofs)
{
    ofs << GetType().GetName() << endl;
    ofs << "   Name              : " << GetName() << endl;
    ofs << "   GID               : " << GetId() << endl;
    ofs << "   Total objects     : " << NumberOfChildren( true ) << endl;

    int naprops = m_properties.NumberOfProperties();
    int noprops = m_properties.NumberOfObjectProperties();
    int nchildren = NumberOfChildren();
    int nrel = m_related.size();

    ofs << "   Properties        : " << naprops - noprops << endl;
    ofs << "   Object properties : " << noprops << endl;
    ofs << "   Children objects  : " << nchildren << endl;
    ofs << "   Related objects   : " << nrel << endl;

    m_properties.Desc(ofs);

    // has any children?
    if( nchildren > 0 ) {
        ofs << endl;
        ofs << "   >>> CHILDREN                                                          " << endl;
        ofs << "   GID          Type       Name                                          " << endl;
        ofs << "   ------------ ---------- ----------------------------------------------" << endl;

        CEntityPtr obj = m_first;

        ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
        while( obj ) {
            ofs << "   ";
            ofs << setw(12) << obj->GetId() << " ";
            ofs << setw(10) << obj->GetType().GetName() << " ";
            ofs << obj->GetName() << " ";
            ofs << endl;

            obj = obj->m_sibling;
        }
    }

    // references?
    if( nrel > 0 ) {
        ofs << endl;
        ofs << "   >>> REFERENCED BY                                                     " << endl;
        ofs << "   GID          Type       Name                                          " << endl;
        ofs << "   ------------ ---------- ----------------------------------------------" << endl;

        list< CEntityWPtr >::iterator oit = m_related.begin();
        list< CEntityWPtr >::iterator oie = m_related.end();

        ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
        while( oit != oie ){
            ofs << "   ";
            CEntityPtr obj( *oit );
            if( ! obj ){
                ofs << setw(12) << obj->GetId() << " ";
                ofs << setw(10) << obj->GetType().GetName() << " ";
                ofs << obj->GetName() << " ";
                ofs << endl;
            } else {
                ofs << "DEAD OBJECT" << endl;
            }
            oit++;
        }

    }
}

// -------------------------------------------------------------------------

void CEntity::DescSubObj(const string& oname,ostream& ofs)
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

    if( obj_name.empty() ){
        throw runtime_error("subobject name is empty for contatiner '" + GetName() + "'" );
    }

    // find object
    CEntityPtr obj;

    if( CFactory::IsInteger(obj_name) ) {
        int gid = CFactory::GetIntegerNumber(obj_name);
        obj = FindChild(gid);
    } else {
        obj = FindChild(obj_name);
    }

    // is object valid?
    if( ! obj ){
        throw runtime_error("unable to find object '" + obj_name + "' in '" + GetName() + "'" );
    }

    // should we go further ...
    if( sub_obj_name.empty() ){
        obj->Desc(ofs);
    } else {
        return( obj->DescSubObj(sub_obj_name,ofs) );
    }
}

// -------------------------------------------------------------------------

CEntityPtr CEntity::GetSubObj(const string& oname)
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

    if( obj_name.empty() ){
        throw runtime_error("subobject name is empty for contatiner '" + GetName() + "'" );
    }

    // find object
    CEntityPtr obj;

    if( CFactory::IsInteger(obj_name) ) {
        int gid = CFactory::GetIntegerNumber(obj_name);
        obj = FindChild(gid);
    } else {
        obj = FindChild(obj_name);
    }

    // is object valid?
    if( ! obj ){
        throw runtime_error("unable to find object '" + obj_name + "' in '" + GetName() + "'" );
    }

    // should we go further ...
    if( sub_obj_name.empty() ){
        return(obj);
    } else {
        return( obj->GetSubObj(sub_obj_name) );
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CEntity::Set(const CKey& parmid, const int& value)
{
    m_properties.Set(parmid,value);
}

// -------------------------------------------------------------------------

void CEntity::Set(const CKey& parmid, const double& value)
{
    m_properties.Set(parmid,value);
}

// -------------------------------------------------------------------------

void CEntity::Set(const CKey& parmid, const string& value)
{
    m_properties.Set(parmid,value);
}

// -------------------------------------------------------------------------

void CEntity::Set(const CKey& parmid, const CEntityPtr& value)
{
    if( ! value ){
        throw runtime_error("value is illegal in CEntity::Set(const CKey& parmid, const CEntityPtr& value)");
    }

    // unregister previous value
    CEntityPtr prev;
    m_properties.Get(parmid,prev);
    if( prev ){
        prev->RemoveRelated(GetSelf());
    }

    // register new value
    m_properties.Set(parmid, value);
    value->m_related.push_back(GetSelf());
}

// -------------------------------------------------------------------------

void CEntity::Get(const CKey& parmid, int& value)
{
    m_properties.Get(parmid,value);
}

// -------------------------------------------------------------------------

void CEntity::Get(const CKey& parmid, double& value)
{
    m_properties.Get(parmid,value);
}

// -------------------------------------------------------------------------

void CEntity::Get(const CKey& parmid, string& value)
{
    m_properties.Get(parmid,value);
}

// -------------------------------------------------------------------------

void CEntity::Get(const CKey& parmid, CEntityPtr& value)
{
    m_properties.Get(parmid,value);
}

// -------------------------------------------------------------------------

void CEntity::RemoveRelated(CEntityPtr value)
{
    list< CEntityWPtr >::iterator oit = m_related.begin();
    list< CEntityWPtr >::iterator oie = m_related.end();

    while( oit != oie ){
        list< CEntityWPtr >::iterator old = oit;
        oit++;
        if( (*old).lock() == value ){
            m_related.erase(old,old);
        }
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CForwardIterator CEntity::BeginChildren(void)
{
    CForwardIterator it( GetSelf() );
    it.SetToBegin();
    return( it );
}

// -------------------------------------------------------------------------

CForwardIterator CEntity::EndChildren(void)
{
    CForwardIterator it( GetSelf() );
    it.SetToEnd();
    return( it );
}

// -------------------------------------------------------------------------

void CEntity::AddChild( CEntityPtr child )
{
    if( ! child ){
        throw runtime_error("child is not valid object - CEntity::AddChild");
    }

    if( child->m_root ){
        throw runtime_error("child is already owned - CEntity::AddChild");
    }

    if( m_last ){
        // put to the end of list
        child->m_self = m_last;
        child->m_sibling = CEntityPtr();
        child->m_root = this;

        m_last->m_sibling = child;
        m_last = child->GetThis();
    } else {
        // new item
        child->m_self = this;
        child->m_sibling = CEntityPtr();
        child->m_root = this;

        m_first = child;
        m_last = child->GetThis();
    }
}

// -------------------------------------------------------------------------

void CEntity::RemoveChild( CEntityPtr child )
{
    if( ! child ){
        throw runtime_error("child is not valid object - CEntity::RemoveChild");
    }
    // first object
    if( m_first == child ){
        RemoveFirstChild();
        return;
    }
    // last object
    if( m_last == child->GetThis() ){
        RemoveLastChild();
        return;
    }

    // middle object
    if( child->m_root != this ){
        // objcreateatomect is not owned by this container
        throw runtime_error("object is not owned by this entity - CEntity::RemoveChild");
    }

    // remove middle object
    CEntityPtr next = child->m_sibling;
    CEntityPtr prev = child->GetPrev();
    prev->m_sibling = next;
    next->m_self = prev->GetThis();

    // update object - before its (possible) destruction
    child->m_root = NULL;
    child->m_self = NULL;
}

// -------------------------------------------------------------------------

void CEntity::RemoveFirstChild(void)
{
    if( m_first ){
        CEntityPtr old_first = m_first;

        // set and update new first object
        m_first = m_first->m_sibling;
        if( m_first ){
            m_first->m_self = this;
        } else {
            // no item in the list - update last pointer
            m_last = NULL;
        }

        // update old first object - before its (possible) destruction
        old_first->m_root = NULL;
        old_first->m_self = NULL;
        old_first->m_sibling = CEntityPtr();
    }
}

// -------------------------------------------------------------------------

void CEntity::RemoveLastChild(void)
{
    if( m_last ){
        CEntityPtr old_last = m_last->GetSelf();

        if( ! old_last ){
            throw runtime_error("unable get self reference - CEntity::RemoveLastChild");
        }

        CEntityPtr prev = old_last->GetPrev();

        if( prev ){
            m_last = prev->GetThis();
            prev->m_sibling = CEntityPtr();
        } else {
            // no item in the list - update last and first pointer
            m_first = CEntityPtr();
            m_last = NULL;
        }

        // update old last object - before its (possible) destruction
        old_last->m_root = NULL;
        old_last->m_self = NULL;
        old_last->m_sibling = CEntityPtr();
    }
}

// -------------------------------------------------------------------------

void CEntity::RemoveAllChildren(void)
{
    while( NumberOfChildren() > 0 ){
        RemoveFirstChild();
    }
}

// -------------------------------------------------------------------------

CEntityPtr CEntity::FindChild(const string& name, bool recursive)
{
    CEntityPtr obj = m_first;

    while( obj ){
        if( obj->GetName() == name ) return( obj );
        if( recursive ){
            CEntityPtr sub = FindChild( name, recursive );
            if( sub ) return( sub );
        }
        obj = obj->m_sibling;
    }

    return( CEntityPtr() );
}

// -------------------------------------------------------------------------

CEntityPtr CEntity::FindChild(int id, bool recursive)
{
    CEntityPtr obj = m_first;

    while( obj ){
        if( obj->GetId() == id ) return( obj );
        if( recursive ){
            CEntityPtr sub = obj->FindChild(id, recursive);
            if( sub ) return( sub );
        }
        obj = obj->m_sibling;
    }

    return( CEntityPtr() );
}

// -------------------------------------------------------------------------

bool CEntity::IsChild(CEntityPtr cobj, bool recursive)
{
    CEntityPtr obj = m_first;

    while( obj ){
        if( obj == cobj ) return( true );
        if( recursive ){
            if( obj->IsChild(cobj, recursive) ) return(true);
        }
        obj = obj->m_sibling;
    }

    return( false );
}

// -------------------------------------------------------------------------

size_t CEntity::NumberOfChildren(bool recursive)
{
    size_t      size = 0;
    CEntityPtr  obj = m_first;

    while( obj ){
        size++;
        if( recursive ){
            size += obj->NumberOfChildren(recursive);
        }
        obj = obj->m_sibling;
    }

    return( size );
}

// -------------------------------------------------------------------------

CEntityPtr CEntity::GetChild(size_t index)
{
    size_t      lindex = 0;
    CEntityPtr  obj = m_first;

    while( obj ){
        if( lindex == index ){
            return( obj );
        }
        lindex++;
        obj = obj->m_sibling;
    }

    return( CEntityPtr() );
}

// -------------------------------------------------------------------------

CEntityPtr CEntity::GetFirstChild( )
{
    return( m_first );
}

// -------------------------------------------------------------------------

CEntityPtr CEntity::GetLastChild( )
{
    if( m_last ){
        return( m_last->GetSelf() );
    } else {
        return( CEntityPtr() );
    }
}

// -------------------------------------------------------------------------
// #########################################################################
// -------------------------------------------------------------------------

bool CEntity::Load(CXMLElement* p_ele)
{
//        string type;
//        p_ele->GetAttribute("type",type);
//        if( hash(type) != m_type ){
//            CSmallString error;
//            error << "inconsistent node type, "
//            ES_ERROR()
//        }
    return(false);
}

// -------------------------------------------------------------------------

bool CEntity::Save(CXMLElement* p_ele)
{
//        // check element
//        if( p_ele == NULL ){
//            ES_ERROR("p_ele is NULL");
//            return(false);
//        }
//        if( p_ele->GetName() != "entity" ){
//            ES_ERROR("XMLElement is not 'entity'");
//            return(false);
//        }
//        // save object main attributes
//        bool result = true;
//        result &= p_ele->SetAttribute("type",GetName(m_type));
//        result &= p_ele->SetAttribute("id",m_id);

//        if( ! m_name.empty() ){
//            result &= p_ele->SetAttribute("name",m_name);
//        }
//        if( result == false ){
//            ES_ERROR("unable to save main entity attributtes");
//            return(false);
//        }

//        // save object int attributes ----------------------
//        int nprops = m_i_properties.size() + m_d_properties.size()
//                   + m_s_properties.size() + m_o_properties.size();
//        if( nprops > 0 ){
//            CXMLElement* p_pele = p_ele->CreateChildElement("props");
//            if( p_pele == NULL ){
//                ES_ERROR("unable to create properties element");
//                return(false);
//            }

//            map< CKey, int >::iterator iit = m_i_properties.begin();
//            map< CKey, int >::iterator iie = m_i_properties.end();
//            for(; iit != iie; iit++){
//                CXMLElement* p_prop = p_pele->CreateChildElement("prop");
//                if( p_prop == NULL ){
//                    ES_ERROR("unable to create prop element");
//                    return(false);
//                }
//                result &= p_prop->SetAttribute("name",GetName(iit->first));
//                result &= p_prop->SetAttribute("value",iit->second);
//                result &= p_prop->SetAttribute("type","int");

//                if( result == false ){
//                    ES_ERROR("unable to save property attributtes");
//                    return(false);
//                }
//            }

//            map< CKey, double >::iterator dit = m_d_properties.begin();
//            map< CKey, double >::iterator die = m_d_properties.end();
//            for(; dit != die; dit++){
//                CXMLElement* p_prop = p_pele->CreateChildElement("prop");
//                if( p_prop == NULL ){
//                    ES_ERROR("unable to create prop element");
//                    return(false);
//                }
//                result &= p_prop->SetAttribute("name",GetName(dit->first));
//                result &= p_prop->SetAttribute("value",dit->second);
//                result &= p_prop->SetAttribute("type","double");

//                if( result == false ){
//                    ES_ERROR("unable to save property attributtes");
//                    return(false);
//                }
//            }

//            map< CKey, string >::iterator sit = m_s_properties.begin();
//            map< CKey, string >::iterator sie = m_s_properties.end();
//            for(; sit != sie; sit++){
//                CXMLElement* p_prop = p_pele->CreateChildElement("prop");
//                if( p_prop == NULL ){
//                    ES_ERROR("unable to create prop element");
//                    return(false);
//                }
//                result &= p_prop->SetAttribute("name",GetName(sit->first));
//                result &= p_prop->SetAttribute("value",sit->second);
//                result &= p_prop->SetAttribute("type","string");

//                if( result == false ){
//                    ES_ERROR("unable to save property attributtes");
//                    return(false);
//                }
//            }

//            map< CKey, CEntityPtr >::iterator oit = m_o_properties.begin();
//            map< CKey, CEntityPtr >::iterator oie = m_o_properties.end();
//            for(; oit != oie; oit++){
//                CXMLElement* p_prop = p_pele->CreateChildElement("prop");
//                if( p_prop == NULL ){
//                    ES_ERROR("unable to create prop element");
//                    return(false);
//                }
//                result &= p_prop->SetAttribute("name",GetName(oit->first));
//                result &= p_prop->SetAttribute("value",oit->second->GetId());
//                result &= p_prop->SetAttribute("type","object");

//                if( result == false ){
//                    ES_ERROR("unable to save property attributtes");
//                    return(false);
//                }
//            }

//        }

//        // save children -----------------------------------
//        if( m_children.size() > 0 ){
//            CXMLElement* p_cele = p_ele->CreateChildElement("childs");
//            if( p_cele == NULL ){
//                ES_ERROR("unable to create childs element");
//                return(false);
//            }
//            CForwardIterator ct = m_children.begin();
//            CForwardIterator ce = m_children.end();
//            for(; ct != ce; ct++){
//                CXMLElement* p_entity = p_cele->CreateChildElement("entity");
//                if( p_entity == NULL ){
//                    ES_ERROR("unable to create entity element");
//                    return(false);
//                }
//                result &= (*ct)->save(p_entity);
//            }
//            if( result == false ){
//                ES_ERROR("unable to save child entity");
//                return(false);
//            }
//        }

    return(true);
}

// -------------------------------------------------------------------------

}


