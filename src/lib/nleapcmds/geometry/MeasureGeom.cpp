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

#include <geometry/MeasureGeom.hpp>
#include <engine/Context.hpp>
#include <misc/Geometry.hpp>
#include <iomanip>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CMeasureGeomCommand::CMeasureGeomCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

// -------------------------------------------------------------------------

CMeasureGeomCommand::CMeasureGeomCommand( const string& cmd_name, const CEntityPtr& at1, const CEntityPtr& at2, const CEntityPtr& at3, const CEntityPtr& at4 )
    : CCommand( cmd_name, cmd_name ), m_at1( at1 ), m_at2( at2 ), m_at3( at3 ), m_at4( at4 )
{
}

// -------------------------------------------------------------------------

const char* CMeasureGeomCommand::Info(  EHelp type ) const
{
    if( type == help_group )
    {
        return("geometry");
    }

    if( type == help_short )
    {
        return("measure geometry");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>measureGeom</b> - measure geometry\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>measureGeom</b> <u>atom1</u> <u>atom2</u> [<u>atom3</u> <u>atom4</u>]\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Measure the distance, angle or torsion between two, three or four ATOMs,respectively."
    );
}

// -------------------------------------------------------------------------

void CMeasureGeomCommand::Exec( CContext* p_ctx )
{
    CAtomPtr at1 = dynamic_pointer_cast<CAtom>(m_at1);
    CAtomPtr at2 = dynamic_pointer_cast<CAtom>(m_at2);
    CAtomPtr at3 = dynamic_pointer_cast<CAtom>(m_at3);
    CAtomPtr at4 = dynamic_pointer_cast<CAtom>(m_at4);

    if( at4 ){
        double angle = GetTorsion(at1->GetPos(),at2->GetPos(),at3->GetPos(),at4->GetPos());
        angle = angle * 180.0 / M_PI;
        p_ctx->out() << "Torsion = " << fixed << setw(9) << setprecision(4) << angle << endl;
        return;
    }

    if( at3 ){
        double angle = GetAngle(at1->GetPos(),at2->GetPos(),at3->GetPos());
        angle = angle * 180.0 / M_PI;
        p_ctx->out() << "Angle = " << fixed << setw(9) << setprecision(4) << angle << endl;
        return;
    }

    double dist = GetDistance(at1->GetPos(),at2->GetPos());
    p_ctx->out() << "Distance = " << fixed << setw(9) << setprecision(4) << dist << endl;
}

// -------------------------------------------------------------------------

shared_ptr< CCommand > CMeasureGeomCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 2 , 4);

    CEntityPtr at1;
    CEntityPtr at2;
    CEntityPtr at3;
    CEntityPtr at4;

    ExpandArgument( p_ctx , cmdline.GetArgs() , 0 , at1, ATOM );
    ExpandArgument( p_ctx , cmdline.GetArgs() , 1 , at2, ATOM );
    ExpandArgument( p_ctx , cmdline.GetArgs() , 2 , at3, ATOM );
    ExpandArgument( p_ctx , cmdline.GetArgs() , 3 , at4, ATOM );

    return shared_ptr< CCommand >( new CMeasureGeomCommand(m_action, at1, at2, at3, at4) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}



