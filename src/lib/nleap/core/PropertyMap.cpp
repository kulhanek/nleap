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
#include <core/PropertyMap.hpp>
#include <core/Property.hpp>
#include <core/PredefinedKeys.hpp>
#include <core/Entity.hpp>

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

size_t CPropertyMap::m_default_size = 2;

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CPropertyMap::CPropertyMap(void)
{
    m_first_block = NULL;
}

// -------------------------------------------------------------------------

CPropertyMap::CPropertyMap(const CPropertyMap& src)
{
    // do not copy
    m_first_block = NULL;
}

// -------------------------------------------------------------------------

CPropertyMap::~CPropertyMap(void)
{
    CProperty*    p_block = m_first_block;

    // deallocate block sequence
    while( p_block != NULL ){
        int i = 0;
        while( p_block[i].m_key != NEXT_PROP ){
            i++;
        }
        CProperty* p_next;
        p_block[i].Get(p_next);

        delete[] p_block;
        p_block = p_next;
    }
}

// -------------------------------------------------------------------------

void CPropertyMap::SetInitialBlockSize(size_t block_size)
{
    if( m_first_block == NULL ){
        AllocateBlock( block_size );
    }
}

// -------------------------------------------------------------------------

void CPropertyMap::CloneWeakly(const CPropertyMap& src, int base_id)
{
    size_t size = src.NumberOfProperties();
    AllocateBlock(size);

    // copy individual properties
    for(size_t i=0; i < size; i++){
        CProperty& srcnode = src.GetNode(i);
        CProperty& dstnode = GetNode(i);
        dstnode.CloneWeakly(srcnode, base_id);
    }
}

// -------------------------------------------------------------------------

void CPropertyMap::BindWeak(map< int, CEntityPtr >&  obj_map)
{
    size_t size = NumberOfProperties();

    // bind_weak
    for(size_t i=0; i < size; i++){
        CProperty& node = GetNode(i);
        node.BindWeak(obj_map);
    }
}

// -------------------------------------------------------------------------

size_t  CPropertyMap::NumberOfProperties(void) const
{
    CProperty*    p_block = m_first_block;
    size_t size = 0;

    while( p_block != NULL ){
        int i = 0;
        while( p_block[i].m_key != NEXT_PROP ){
            if( p_block[i].m_key == NULL_PROP ) break;
            i++;
            size++;
        }
        p_block[i].Get(p_block);
    }

    return(size);
}

// -------------------------------------------------------------------------

size_t  CPropertyMap::NumberOfObjectProperties(void) const
{
    CProperty*    p_block = m_first_block;
    size_t size = 0;

    while( p_block != NULL ){
        int i = 0;
        while( p_block[i].m_key != NEXT_PROP ){
            if( p_block[i].m_key == NULL_PROP ) break;
            if( p_block[i].m_type == PTR__PROP ) size++;
            i++;
        }
        p_block[i].Get(p_block);
    }

    return(size);
}

// -------------------------------------------------------------------------

void CPropertyMap::Desc(ostream& ofs)
{
    size_t naprops = NumberOfProperties();
    size_t noprops = NumberOfObjectProperties();

    // has any property?
    if( naprops - noprops  > 0 ) {
        ofs << endl;
        ofs << "   >>> PROPERTIES                                                        " << endl;
        ofs << "   Property      Type    Value                                           " << endl;
        ofs << "   ------------- ------- ------------------------------------------------" << endl;

        ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
        for(size_t i=0; i < naprops; i++){
            CProperty& node = GetNode(i);
            if( node.m_type == PTR__PROP ) continue;
            ofs << "   ";
            ofs << setw(13) << node.m_key.GetName() << " ";
            ofs << setw(7)  << node.m_type.GetName();
            ofs << node << endl;
        }

    }

    if( noprops > 0 ) {
        ofs << endl;
        ofs << "   >>> OBJECT PROPERTIES                                                 " << endl;
        ofs << "   Property      GID          Type       Name                            " << endl;
        ofs << "   ------------- ------------ ---------- --------------------------------" << endl;

        ofs << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
        for(size_t i=0; i < naprops; i++){
            CProperty& node = GetNode(i);
            if( node.m_type != PTR__PROP ) continue;
            CEntityPtr obj;
            node.Get(obj);
            if( obj ){
                ofs << "   ";
                ofs << setw(13) << node.m_key.GetName() << " ";
                ofs << setw(12) << obj->GetId() << " ";
                ofs << setw(10) << obj->GetType().GetName() << " ";
                ofs << obj->GetName() << " ";
                ofs << endl;
            } else {
                ofs << "   DEAD OBJECT" << endl;
            }
        }

    }
}

// -------------------------------------------------------------------------

void CPropertyMap::UnsetObjectProperty(const CEntityPtr& obj)
{
    int num = NumberOfProperties();
    for(int i = 0; i < num; i++){
        CProperty& prop = GetNode(i);
        if( prop.GetType() == PTR__PROP ){
            CEntityPtr value;
            prop.Get(value);
            if( value == obj ){
                prop.Set( CEntityPtr() );
            }
        }
    }
}

// -------------------------------------------------------------------------

void CPropertyMap::Set(const CKey& parmid, const int& value)
{
    CProperty*    p_block = m_first_block;

    // overwrite previous value
    int i = 0;
    while( p_block != NULL ){
        i = 0;
        while( p_block[i].m_key != NEXT_PROP ){
            if( p_block[i].m_key == NULL_PROP ) break;
            if( p_block[i].m_key == parmid ){
                p_block[i].Set(value);
                return;
            }
            i++;
        }
        if( p_block[i].m_key == NULL_PROP ) break;
        p_block[i].Get(p_block);
    }

    // do we have space?
    if( p_block == NULL ){
        p_block = AllocateBlock(m_default_size);
        i = 0;
    }

    // set as new value
    p_block[i].m_key = parmid;
    p_block[i].Set(value);
}

// -------------------------------------------------------------------------

void CPropertyMap::Set(const CKey& parmid, const double& value)
{
    CProperty*    p_block = m_first_block;

    // overwrite previous value
    int i = 0;
    while( p_block != NULL ){
        i = 0;
        while( p_block[i].m_key != NEXT_PROP ){
            if( p_block[i].m_key == NULL_PROP ) break;
            if( p_block[i].m_key == parmid ){
                p_block[i].Set(value);
                return;
            }
            i++;
        }
        if( p_block[i].m_key == NULL_PROP ) break;
        p_block[i].Get(p_block);
    }

    // do we have space?
    if( p_block == NULL ){
        p_block = AllocateBlock(m_default_size);
        i = 0;
    }

    // set as new value
    p_block[i].m_key = parmid;
    p_block[i].Set(value);
}

// -------------------------------------------------------------------------

void CPropertyMap::Set(const CKey& parmid, const string& value)
{
    CProperty*    p_block = m_first_block;

    // overwrite previous value
    int i = 0;
    while( p_block != NULL ){
        i = 0;
        while( p_block[i].m_key != NEXT_PROP ){
            if( p_block[i].m_key == NULL_PROP ) break;
            if( p_block[i].m_key == parmid ){
                p_block[i].Set(value);
                return;
            }
            i++;
        }
        if( p_block[i].m_key == NULL_PROP ) break;
        p_block[i].Get(p_block);
    }

    // do we have space?
    if( p_block == NULL ){
        p_block = AllocateBlock(m_default_size);
        i = 0;
    }

    // set as new value
    p_block[i].m_key = parmid;
    p_block[i].Set(value);
}

// -------------------------------------------------------------------------

void CPropertyMap::Set(const CKey& parmid, const CEntityPtr& value)
{
    CProperty*    p_block = m_first_block;

    // overwrite previous value
    int i = 0;
    while( p_block != NULL ){
        i = 0;
        while( p_block[i].m_key != NEXT_PROP ){
            if( p_block[i].m_key == NULL_PROP ) break;
            if( p_block[i].m_key == parmid ){
                p_block[i].Set(value);
                return;
            }
            i++;
        }
        if( p_block[i].m_key == NULL_PROP ) break;
        p_block[i].Get(p_block);
    }

    // do we have space?
    if( p_block == NULL ){
        p_block = AllocateBlock(m_default_size);
        i = 0;
    }

    // set as new value
    p_block[i].m_key = parmid;
    p_block[i].Set(value);
}

// -------------------------------------------------------------------------

void CPropertyMap::Get(const CKey& parmid, int& value)
{
    CProperty*    p_block = m_first_block;

    // find key
    while( p_block != NULL ){
        int i = 0;
        while( p_block[i].m_key != NEXT_PROP ){
            if( p_block[i].m_key == NULL_PROP ) break;
            if( p_block[i].m_key == parmid ){
                p_block[i].Get(value);
                return;
            }
            i++;
        }

        p_block[i].Get(p_block);    // NULL_PROP return NULL
    }

    value = 0;
}

// -------------------------------------------------------------------------

void CPropertyMap::Get(const CKey& parmid, double& value)
{
    CProperty*    p_block = m_first_block;

    // find key
    while( p_block != NULL ){
        int i = 0;
        while( p_block[i].m_key != NEXT_PROP ){
            if( p_block[i].m_key == NULL_PROP ) break;
            if( p_block[i].m_key == parmid ){
                p_block[i].Get(value);
                return;
            }
            i++;
        }

        p_block[i].Get(p_block);    // NULL_PROP return NULL
    }

    value = 0.0;
}

// -------------------------------------------------------------------------

void CPropertyMap::Get(const CKey& parmid, string& value)
{
    CProperty*    p_block = m_first_block;

    // find key
    while( p_block != NULL ){
        int i = 0;
        while( p_block[i].m_key != NEXT_PROP ){
            if( p_block[i].m_key == NULL_PROP ) break;
            if( p_block[i].m_key == parmid ){
                p_block[i].Get(value);
                return;
            }
            i++;
        }

        p_block[i].Get(p_block);    // NULL_PROP return NULL
    }

    value = string();
}

// -------------------------------------------------------------------------

void CPropertyMap::Get(const CKey& parmid, CEntityPtr& value)
{
    CProperty*    p_block = m_first_block;

    // find key
    while( p_block != NULL ){
        int i = 0;
        while( p_block[i].m_key != NEXT_PROP ){
            if( p_block[i].m_key == NULL_PROP ) break;
            if( p_block[i].m_key == parmid ){
                p_block[i].Get(value);
                return;
            }
            i++;
        }

        p_block[i].Get(p_block);    // NULL_PROP return NULL
    }

    value = CEntityPtr();
}

// -------------------------------------------------------------------------

CProperty* CPropertyMap::AllocateBlock(size_t size)
{
    if( size == 0 ) return(NULL);

    CProperty*    p_block = m_first_block;

    CProperty*    p_new = new CProperty[ size +1 ];
    p_new[size].m_key = NEXT_PROP;
    p_new[size].Set((CProperty*)NULL);

    // find the end
    while( p_block != NULL ){
        int i = 0;
        while( p_block[i].m_key != NEXT_PROP ){
            i++;
        }
        CProperty*    p_end;
        p_block[i].Get(p_end);

        if( p_end == NULL ){
            p_block[i].Set(p_new);
            return(p_new);
        }
        p_block = p_end;
    }

    if( m_first_block == NULL ){
        m_first_block = p_new;
    }
    return( p_new );
}

// -------------------------------------------------------------------------

CProperty& CPropertyMap::GetNode(size_t index) const
{
    CProperty*    p_block = m_first_block;
    size_t size = 0;
    static CProperty zero;

    while( p_block != NULL ){
        int i = 0;
        while( p_block[i].m_key != NEXT_PROP ){
            if( size == index ){
                return( p_block[i] );
            }
            i++;
            size++;
        }
        p_block[i].Get(p_block);
    }

    return(zero);
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}


