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

#include <Bond.hpp>
#include <iostream>
#include <engine/Context.hpp>
#include <types/Atom.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CBondCommand::CBondCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CBondCommand::CBondCommand( const string& cmd_name, CEntityPtr& atom1, CEntityPtr& atom2, int order )
    : CCommand( cmd_name, cmd_name ), m_atom1( atom1 ), m_atom2( atom2 ), m_order( order )
{
}

//------------------------------------------------------------------------------

const char* CBondCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("control");
    }

    if( type == help_short )
    {
        return("create a bond between two atoms");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>bond</b> - create a bond between two atoms\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>bond</b> <u>atom1</u> <u>atom2</u> [<u>order</u>] \n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Create a bond between <u>atom1</u> and <u>atom2</u>. Both of these ATOMs must be contained by the "
    "same UNIT. By default, the bond will be a single bond. By specifying \"-\"|\"S\", \"=\"|\"D\", \"#\"|\"T\", or \":\"|\"A\" as "
    "the optional argument, <u>order</u>, the user can specify a single, double, triple, or aromatic bond, "
    "respectively.\n"
    );
}

//------------------------------------------------------------------------------

void CBondCommand::Exec( CContext* p_ctx )
{
    CAtomPtr at1 = dynamic_pointer_cast<CAtom>(m_atom1);
    CAtomPtr at2 = dynamic_pointer_cast<CAtom>(m_atom2);

    if( ! at1->GetUnit() ){
        stringstream str;
        str << "the first atom " << at1->GetPathName() << " has to be the part of unit";
        throw runtime_error( str.str() );
    }

    if( ! at2->GetUnit() ){
        stringstream str;
        str << "the second atom " << at2->GetPathName() << " has to be the part of unit";
        throw runtime_error( str.str() );
    }

    if( at1->GetUnit() != at2->GetUnit() ){
        stringstream str;
        str << "both atoms must be the part of the same unit; ";
        str << "the first atom unit is " << at1->GetUnit()->GetPathName() << ", ";
        str << "the second atom unit is " << at2->GetUnit()->GetPathName();
        throw runtime_error( str.str() );
    }

    CUnitPtr unit = dynamic_pointer_cast<CUnit>(at1->GetUnit());
    if( unit->AreBonded(at1,at2) ){
        stringstream str;
        str << "the atom " << at1->GetPathName() << " and atom " <<  at1->GetPathName();
        str << " are already bonded";
        throw runtime_error( str.str() );
    }

    int top_id = p_ctx->m_index_counter.GetTopIndex();
    unit->CreateBond(at1,at2,m_order,top_id);
    p_ctx->m_index_counter.SetTopIndex( top_id );
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CBondCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 2 , 3 );

    CEntityPtr at1,at2;
    ExpandArgument( p_ctx, cmdline, 0, at1, ATOM );
    ExpandArgument( p_ctx, cmdline, 1, at2, ATOM );

    int order = 1;
    if( cmdline.GetArgs().size() == 3 ){
        string str;
        ExpandArgument( p_ctx, cmdline, 2, str );
        if( (str == "S") || (str == "-") ){
            order = 1;
        } else if( (str == "D") || (str == "=") ){
            order = 2;
        } else if( (str == "T") || (str == "#") ){
            order = 3;
        } else if( (str == "A") || (str == ":") ){
            order = 4;
        } else {
            stringstream sstr;
            sstr << "unrecognized bond order '" << str << "'";
            throw runtime_error(sstr.str());
        }
    }

    return shared_ptr< CCommand >( new CBondCommand(m_action, at1, at2, order) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}

