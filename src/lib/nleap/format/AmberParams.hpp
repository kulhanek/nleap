#ifndef NLEAP_FORMAT_AMBER_PARAMS_HPP
#define NLEAP_FORMAT_AMBER_PARAMS_HPP
// =============================================================================
// nLEaP - prepare input for the AMBER molecular mechanics programs
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

#include <NLEaPMainHeader.hpp>
#include <iosfwd>
#include <vector>
#include <map>
#include <string>
#include <VerboseStr.hpp>
#include <format/CommonIO.hpp>
#include <types/AmberFF.hpp>

namespace nleap {
//------------------------------------------------------------------------------

using namespace std;

//------------------------------------------------------------------------------

class NLEAP_PACKAGE CAmberParams : public CCommonIO {
public:

    CAmberParams(CVerboseStr& debug);

    /// read amber parameter file
    void Read(istream& is, CAmberFFPtr& ff, int& top_id);

    /// write amber parameter file
    void Write(ostream& os, CAmberFFPtr& ff);

// private section -------------------------------------------------------------
private:
    map< string, string >   m_vdwmap;

    void ReadNoKeys( istream& is, CAmberFFPtr& ff, int& top_id );
    void ReadTypes( istream& is, CAmberFFPtr& ff, int& top_id );
    void SkipHydr( istream& is );
    void ReadBonds( istream& is, CAmberFFPtr& ff, int& top_id );
    void ReadAngles( istream& is, CAmberFFPtr& ff, int& top_id );
    void ReadTorsions( istream& is, CAmberFFPtr& ff, int& top_id );
    void ReadImpropers( istream& is, CAmberFFPtr& ff, int& top_id );
    void SkipHBnd( istream& is );
    void ReadvdWMap( istream& is );
    void SkipvdWHeader( istream& is );
    void ReadvdWParams( istream& is, CAmberFFPtr& ff );
    void AssignvdW( CAmberFFPtr& ff );

    void SplitTypes(string& line, int number );
    string GetComment(int from, const vector<string>& ts );

    void CheckItemAsType( size_t item, const vector<string>& ts );
    void CheckItemAsNumber( size_t item, const vector<string>& ts );
    void ReadError( const string& reason );
};

//------------------------------------------------------------------------------
}

#endif

