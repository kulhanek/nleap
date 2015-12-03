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

#include <LoadState.hpp>
#include <iostream>
#include <engine/Context.hpp>
#include <XMLDocument.hpp>
#include <XMLPrinter.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CLoadStateCommand::CLoadStateCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CLoadStateCommand::CLoadStateCommand( const string& cmd_name, const string& file_name )
    : CCommand( cmd_name ), m_file_name( file_name )
{
}

//------------------------------------------------------------------------------

const char* CLoadStateCommand::Info( EHelp type ) const
{
    if( type == help_group )
    {
        return("context");
    }

    if( type == help_short )
    {
        return("load nLEaP state");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>loadState</b> - load nLEaP state\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <b>loadState</b> <u>filename</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "TODO\n"
    );
}

//------------------------------------------------------------------------------

void CLoadStateCommand::Exec( CContext* p_ctx )
{
    CXMLDocument xml_document;
    xml_document.CreateChildDeclaration();
    xml_document.CreateChildComment(" nLEaP state file ");
    CXMLElement* p_master = xml_document.CreateChildElement("entity");
    if( p_master != NULL ){
        p_ctx->Save(p_master);
    }

    CXMLPrinter xml_printer;
    xml_printer.SetPrintedXMLNode(&xml_document);
    xml_printer.Print(m_file_name.c_str());

}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CLoadStateCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    NoAssigmentPossible( cmdline );
    CheckNumberOfArguments( cmdline, 1 , 1);

    string      name;
    ExpandArgument( p_ctx , cmdline, 0, name );

    return shared_ptr< CCommand >( new CLoadStateCommand( m_action, name ) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}

