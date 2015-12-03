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

#include <Add.hpp>
#include <iostream>
#include <engine/Context.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CAddCommand::CAddCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CAddCommand::CAddCommand( const string& cmd_name, CEntityPtr& dst_obj, CEntityPtr& src_obj )
    : CCommand( cmd_name, cmd_name ), m_dst( dst_obj ), m_src( src_obj )
{
}

//------------------------------------------------------------------------------

const char* CAddCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("control");
    }

    if( type == help_short )
    {
        return("add an object into another one");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>add</b> - add an object into another one\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>add</b> <u>dstobject</u> <u>srcobject</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Add the object <u>srcobject</u> to the object <u>dstobject</u>. This command is used "
    "to place ATOMs within RESIDUEs, and RESIDUEs within UNITs. This command will work "
    "only if <u>srcobject</u> is not contained by any other object.\n"
    );
}

//------------------------------------------------------------------------------

void CAddCommand::Exec( CContext* p_ctx )
{
    // who is the owner of m_src?
    if( m_src->GetRoot() ){
        if( m_src->GetRoot()->GetName() != "_objects" ){
            stringstream str;
            str << "source object " << m_src->GetPathName() << " is already added in ";
            str << m_src->GetRoot()->GetPathName() << "";
            throw runtime_error( str.str() );
        }
        // remove it from _objects
        m_src->GetRoot()->RemoveChild(m_src);
    }

    // insert ATOM to RESIDUE object
    if( (m_src->GetType() == ATOM) && (m_dst->GetType() == RESIDUE) ){
        CResiduePtr res = dynamic_pointer_cast<CResidue>(m_dst);
        CAtomPtr    atm = dynamic_pointer_cast<CAtom>(m_src);
        res->AddAtom( atm );
        CUnitPtr unit = res->GetUnit();
        if( unit ){
            unit->FixCounters();
        }
        return;
    }

    // insert RESIDUE to UNIT object
    if( (m_src->GetType() == RESIDUE) && (m_dst->GetType() == UNIT) ){
        CUnitPtr    unit = dynamic_pointer_cast<CUnit>(m_dst);
        CResiduePtr res = dynamic_pointer_cast<CResidue>(m_src);
        unit->AddResidue( res );
        unit->FixCounters();
        return;
    }

    // unsupported object types
    stringstream str;
    str << "unable to add the object of type " << m_src->GetType().GetName();
    str << " to the object of type " << m_dst->GetType().GetName();
    throw runtime_error( str.str() );
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CAddCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 2, 2 );

    CEntityPtr dst,src;
    ExpandArgument( p_ctx, cmdline, 0, dst, ANY );
    ExpandArgument( p_ctx, cmdline, 1, src, ANY );

    return shared_ptr< CCommand >( new CAddCommand( m_action, dst, src ) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}

