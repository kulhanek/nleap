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

#include <LoadPDB.hpp>
#include <fstream>
#include <engine/Context.hpp>
#include <types/Factory.hpp>
#include <format/FormatPDB.hpp>

namespace nleapcmds {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CLoadPDBCommand::CLoadPDBCommand( const string& cmd_name )
    : CCommand( cmd_name )
{
}

//------------------------------------------------------------------------------

CLoadPDBCommand::CLoadPDBCommand( const string& cmd_name, const string& var, const string& file )
    : CCommand( cmd_name, cmd_name ), m_var( var ), m_file( file )
{
}

//------------------------------------------------------------------------------

const char* CLoadPDBCommand::Info(  EHelp type ) const
{
    if( type == help_group )
    {
        return("input");
    }

    if( type == help_short )
    {
        return("load a Protein Data Bank (PDB) format file");
    }
    return(
    "<b>NAME:</b>\n"
    "       <b>loadPdb</b> - load a Protein Data Bank (PDB) format file\n"
    "\n"
    "<b>SYNOPSIS:</b>\n"
    "       <u>variable</u> = <b>loadPdb</b> <u>filename</u>\n"
    "\n"
    "<b>DESCRIPTION:</b>\n"
    "Load a Protein Data Bank (PDB) format file with the file name <u>filename</u> and store it into the UNIT referenced by <u>variable</u>. "
    "The sequence numbers of the "
    "RESIDUEs will be determined from the order of residues within the PDB file ATOM records. "
    "This function will search the variables currently defined within LEaP for variable names that "
    "map to residue names within the ATOM records of the PDB file. If a matching variable name "
    "is found then the contents of the variable are added to the UNIT that will contain the structure "
    "being loaded from the PDB file. Adding the contents of the matching UNIT into the UNIT "
    "being constructed means that the contents of the matching UNIT are copied into the UNIT "
    "being built and that a bond is created between the connect0 ATOM of the matching UNIT and "
    "the connect1 ATOM of the UNIT being built. The UNITs are combined in the same way "
    "UNITs are combined using the <b>sequence</b> command. As atoms are read from the ATOM "
    "records their coordinates are written into the correspondingly named ATOMs within the UNIT "
    "being built. If the entire residue is read and it is found that ATOM coordinates are missing, "
    "then external coordinates are built from the internal coordinates that were defined in the "
    "matching UNIT. This allows LEaP to build coordinates for hydrogens and lone-pairs which are "
    "not specified in PDB files. "
    "The file <u>filename</u> is not searched in the PATH (see <b>nleap</b> command)."
    );
}

//------------------------------------------------------------------------------

void CLoadPDBCommand::Exec( CContext* p_ctx )
{
    CDatabasePtr db = p_ctx->database();

    // open file
    ifstream is( m_file.c_str() );

    if( ! is ) {
        throw std::runtime_error( "Cannot open file '" + m_file + "'' for reading." );
    }

    // create unit
    int top_id = p_ctx->m_index_counter.GetTopIndex();
    CUnitPtr unit = db->CreateUnit( top_id );

    CFormatPDB   reader( p_ctx->out() );
    reader.Read( is, unit, top_id );

    // set variable
    db->SetVariable( top_id, m_var, unit );
    p_ctx->m_index_counter.SetTopIndex( top_id );
}

//------------------------------------------------------------------------------

shared_ptr< CCommand > CLoadPDBCommand::Clone( CContext* p_ctx, const CParser& cmdline ) const
{
    AssigmentRequired( cmdline );
    CheckNumberOfArguments( cmdline, 1, 1);

    string var;
    string file;

    ExpandLHS( p_ctx , cmdline, var );
    ExpandArgument( p_ctx, cmdline, 0, file );

    return shared_ptr< CCommand >( new CLoadPDBCommand(m_action, var, file) );
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}



