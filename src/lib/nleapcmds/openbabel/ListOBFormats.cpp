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

#include <ListOBFormats.hpp>
#include <fstream>
#include <engine/Context.hpp>
#include <types/Factory.hpp>
#include <iomanip>

#include <openbabel/obconversion.h>
#include <openbabel/plugin.h>

using namespace OpenBabel;

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CListOBFormatsCommand::CListOBFormatsCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CListOBFormatsCommand::CListOBFormatsCommand( const string& cmd_name,  const string& action )
    : CCommand( cmd_name, action )
{
}

//------------------------------------------------------------------------------

const char* CListOBFormatsCommand::Info(  EHelp type ) const
{
    if( type == help_group )
    {
        return("openbabel");
    }

    if( type == help_short )
    {
        return("list i/o formats supported by OpenBabel");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>listOBFormats</b> - list i/o formats supported by OpenBabel\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>listOBFormats</b>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "List all input/output formats supported by OpenBabel.\n"
    );
}

//------------------------------------------------------------------------------

void CListOBFormatsCommand::Exec( CContext* p_ctx )
{
    vector<string>  formats;
    OBPlugin::ListAsVector("formats","all",formats);

    p_ctx->out() << endl;
    p_ctx->out() << "OpenBabel" << endl;
    p_ctx->out() << "   Number of formats : " << formats.size() << endl;
    p_ctx->out() << endl;

    p_ctx->out() << "      Format       Description                                    " << endl;
    p_ctx->out() << "   ------------ --------------------------------------------------" << endl;

    p_ctx->out() << resetiosflags(ios_base::right) << setiosflags(ios_base::left);
    for(unsigned int i = 0; i < formats.size(); i++) {
        string format,desc;
        size_t pos = formats[i].find(" -- ");
        if( pos != string::npos ){
            format = formats[i].substr(0,pos);
            desc = formats[i].substr(pos + 4);
            p_ctx->out() << "   ";
            p_ctx->out() << setw(12) << format;
            p_ctx->out() << " ";
            p_ctx->out() << setw(50) << desc;
            p_ctx->out() << endl;
        }
    }
    p_ctx->out() << endl;

}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CListOBFormatsCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 0, 0);

    return shared_ptr< CCommand >( new CListOBFormatsCommand(m_action, m_action) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}



