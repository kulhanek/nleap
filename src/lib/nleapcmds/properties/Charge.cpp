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

#include <Charge.hpp>
#include <engine/Context.hpp>
#include <iomanip>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
    
CChargeCommand::CChargeCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
    m_change_state = false;
}

//------------------------------------------------------------------------------

CChargeCommand::CChargeCommand(const string& cmd_name, CEntityPtr& obj)
    : CCommand( cmd_name, cmd_name ), m_obj( obj )
{
    m_change_state = false;
}

//------------------------------------------------------------------------------

const char* CChargeCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("properties");
    }

    if( type == help_short )
    {
        return("print a charge of object");
    }

    return(
    "<b>NAME:</b>\n"
    "       <b>charge</b> - print a charge of object\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>charge</b> <u>object</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "This command calculates the total charge of the ATOMs within the <u>object</u>. "
    "The object can be either an UNIT, RESIDUE or ATOM.\n"
    );
}

//------------------------------------------------------------------------------

void CChargeCommand::Exec( CContext* p_ctx )
{
    double charge = 0.0;
    if( m_obj->GetType() == ATOM ){
        charge = m_obj->Get<double>(CHARGE);
    } else {
        CRecursiveIterator it = CRecursiveIterator(m_obj);
        it.SetFilter(ATOM);
        it.SetToBegin();
        CRecursiveIterator ie = it;
        ie.SetToEnd();

        while( it != ie ){
            charge += it->Get<double>(CHARGE);
            it++;
        }
    }

    p_ctx->out() << "Total charge = " << fixed << setw(9) << setprecision(4) << charge << endl;
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CChargeCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 1, 1 );

    CEntityPtr  obj;
    ExpandArgument( p_ctx, cmdline, 0, obj, ANY );

    if( (obj->GetType() != ATOM) &&
        (obj->GetType() != RESIDUE) &&
        (obj->GetType() != UNIT) ){
        stringstream str;
        str << "ATOM, RESIDUE, or UNIT expected, but " << obj->GetType().GetName() << " provided";
        throw runtime_error( str.str() );
    }

    return shared_ptr< CCommand >( new CChargeCommand( m_action, obj) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}

 
