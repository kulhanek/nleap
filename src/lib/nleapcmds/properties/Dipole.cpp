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

#include <Dipole.hpp>
#include <engine/Context.hpp>
#include <Point.hpp>
#include <iomanip>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
    
CDipoleCommand::CDipoleCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CDipoleCommand::CDipoleCommand(const string& cmd_name, CEntityPtr& obj)
    : CCommand( cmd_name, cmd_name ), m_obj( obj )
{
}

//------------------------------------------------------------------------------

const char* CDipoleCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("properties");
    }

    if( type == help_short )
    {
        return("print a dipole moment of object");
    }

    return(
    "<b>NAME:</b>\n"
    "       <b>dipole</b> - print a dipole moment of object\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>dipole</b> <u>object</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "This command calculates the dipole moment of the ATOMs within the <u>object</u>. "
    "The object can be either an UNIT or RESIDUE. If the object does not have the total "
    "charge equals to zero then the center of mass of the obeject is used as a reference point.\n"
    );
}

//------------------------------------------------------------------------------

void CDipoleCommand::Exec( CContext* p_ctx )
{
    CRecursiveIterator it = CRecursiveIterator(m_obj);
    it.SetFilter(ATOM);
    it.SetToBegin();
    CRecursiveIterator ie = it;
    ie.SetToEnd();

    // calculate COM -----------------------
    CPoint com;
    double tmass = 0.0;
    while( it != ie ){
        CPoint pos;
        double mass;
        pos.x = it->Get<double>(POSX);
        pos.y = it->Get<double>(POSY);
        pos.z = it->Get<double>(POSZ);
        mass  = CAtomTypes::GetMass( p_ctx, it->Get<string>(TYPE) );
        com   += pos*mass;
        tmass += mass;
        it++;
    }

    if( tmass == 0 ){
        throw runtime_error("unable to calculate COM of the object, the object has zero mass");
    }

    com /= tmass;

    // calculate dipole moment -------------
    it.SetToBegin();
    CPoint dip;

    while( it != ie ){
        CPoint pos;
        double charge;
        pos.x  = it->Get<double>(POSX);
        pos.y  = it->Get<double>(POSY);
        pos.z  = it->Get<double>(POSZ);
        charge = it->Get<double>(CHARGE);

        dip += (pos - com)*charge;

        it++;
    }

    // convert to Debye
    double dipole = Size(dip) * 4.803;

    p_ctx->out() << "Dipole moment = " << fixed << setw(9) << setprecision(4) << dipole << " D" << endl;
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CDipoleCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 1, 1 );

    CEntityPtr  obj;
    ExpandArgument( p_ctx, cmdline, 0, obj, ANY );

    if( (obj->GetType() != RESIDUE) &&
        (obj->GetType() != UNIT) ){
        stringstream str;
        str << "RESIDUE or UNIT expected, but " << obj->GetType().GetName() << " provided";
        throw runtime_error( str.str() );
    }

    return shared_ptr< CCommand >( new CDipoleCommand( m_action, obj) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}

 
