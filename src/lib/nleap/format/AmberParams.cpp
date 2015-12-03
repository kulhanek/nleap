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

#include <format/AmberParams.hpp>
#include <vector>
#include <sstream>
#include <types/Factory.hpp>
#include <boost/algorithm/string.hpp>
#include <core/PredefinedKeys.hpp>

using namespace boost;

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CAmberParams::CAmberParams(CVerboseStr& debug)
    : CCommonIO( debug )
{
}

// -------------------------------------------------------------------------

void CAmberParams::Read(istream& is, CAmberFFPtr& ff, int& top_id)
{
    if( ! ff ){
        // invalid ff
        throw runtime_error( "ff is NULL in CAmberParams::Read" );
    }

    // read title ----------------------------
    m_debug << high;
    m_debug << "> Reading title ..." << endl;
    getline( is, m_line );
    if( is ){
        ff->Set(TITLE, m_line);
        m_line_no++;
    }
    m_debug << low;
    m_debug << "  Title: " << m_line << endl;
    m_debug << high;

    // process file --------------------------
    getline( is, m_line );
    bool first_time = true;
    while( is ) {
        m_line_no++;

        if( empty(m_line) ){
            getline( is, m_line );
            continue;
        }

        string key;
        if( m_line.size() >= 4 ) key = m_line.substr(0,4);

        if( key == "MASS" ) {
            getline( is, m_line );
            m_line_no++;
            ReadTypes( is, ff, top_id );
        } else if ( key == "BOND" ) {
            getline( is, m_line );
            m_line_no++;
            ReadBonds( is, ff, top_id );
        } else if ( key == "ANGL" ) {
            getline( is, m_line );
            m_line_no++;
            ReadAngles( is, ff, top_id );
        } else if ( key == "DIHE" ) {
            getline( is, m_line );
            m_line_no++;
            ReadTorsions( is, ff, top_id );
        } else if ( key == "IMPR" ) {
            getline( is, m_line );
            m_line_no++;
            ReadImpropers( is, ff, top_id );
        } else if ( key == "NONB" ) {
            getline( is, m_line );
            m_line_no++;
            ReadvdWParams( is, ff );
        } else {
            if( first_time ){
                ReadNoKeys( is, ff, top_id );
                return;
            } else {
                EmitRWError("unknown section");
            }
        }
        getline( is, m_line );
    }
}

// -------------------------------------------------------------------------

void CAmberParams::ReadNoKeys( istream& is, CAmberFFPtr& ff, int& top_id )
{
        ReadTypes( is, ff, top_id );
    getline( is, m_line );
    m_line_no++;
    SkipHydr( is );
        ReadBonds( is, ff, top_id );
    getline( is, m_line );
    m_line_no++;
        ReadAngles( is, ff, top_id );
    getline( is, m_line );
    m_line_no++;
        ReadTorsions( is, ff, top_id );
    getline( is, m_line );
    m_line_no++;
        ReadImpropers( is, ff, top_id );
    getline( is, m_line );
    m_line_no++;
        SkipHBnd( is );
    getline( is, m_line );
    m_line_no++;
        ReadvdWMap( is );
    getline( is, m_line );
    m_line_no++;
        SkipvdWHeader( is );
        ReadvdWParams( is, ff );
        AssignvdW( ff );
}

// -------------------------------------------------------------------------

void CAmberParams::ReadTypes(istream& is, CAmberFFPtr& ff, int& top_id)
{
    m_debug << "> Reading types ..." << endl;
    CEntityPtr list = ff->FindChild( "types" );

    while( is && (! empty(m_line)) ) {

        // skip comments ---------------------
        if( m_line[0]=='#' ) {
            continue;
        }

        // split line ------------------------
        vector<string>  ts;
        string          line = trim_left_copy(m_line); // it has to be here, split does not remove trailing space
        if( empty(line) ) break;

        split( ts, line, is_any_of(" \t"), token_compress_on );

        if( ts.size() < 2 ){
            EmitRWError( "Atom type and its mass expected." );
        }

        // create type -----------------------
        CEntityPtr obj = CFactory::CreateNode( top_id );
        list->AddChild(obj);
        top_id++;

        // read data -------------------------
        CheckItemAsType(0, ts);
        string tn1 = ts[0];

        CheckItemAsNumber(1, ts);
        double mass = CFactory::GetNumber( ts[1] );
        double polar = 0.0;

        string title;
        if( (ts.size() > 2) && CFactory::IsNumber(ts[2]) ) {
            polar = CFactory::GetNumber( ts[2] );
            title = GetComment(3, ts);
        } else {
            title = GetComment(2, ts);
        }

        obj->SetName( tn1 );
        obj->Set( MASS, mass );
        obj->Set( POLAR, polar );
        obj->Set( TITLE, title );

        getline( is, m_line );
        m_line_no++;
    }

}

// -------------------------------------------------------------------------

void CAmberParams::SkipHydr(istream& is)
{
    getline( is, m_line );
    m_line_no++;
}

// -------------------------------------------------------------------------

void CAmberParams::ReadBonds(istream& is, CAmberFFPtr& ff, int& top_id)
{
    m_debug << "> Reading bonds ..." << endl;
    CEntityPtr list = ff->FindChild( "bonds" );

    while( is && (! empty(m_line)) ) {

        // skip comments ---------------------
        if( m_line[0]=='#' ) {
            continue;
        }

        // split line ------------------------
        vector<string>  ts;
        string          line = trim_left_copy(m_line); // it has to be here, split does not remove trailing space
        if( empty(line) ) break;

        SplitTypes( line, 2);
        split( ts, line, is_any_of(" \t"), token_compress_on );

        if( ts.size() < 4 ){
            EmitRWError( "Two atom types, force constant, and equilibrium distance are expected." );
        }

        // create bond -----------------------
        CEntityPtr obj = CFactory::CreateNode( top_id );
        list->AddChild(obj);
        top_id++;

        // read data -------------------------
        CheckItemAsType(0, ts);
        CheckItemAsType(1, ts);
        CheckItemAsNumber(2, ts);
        CheckItemAsNumber(3, ts);

        string tn1 = ts[0];
        string tn2 = ts[1];
        double force = CFactory::GetNumber( ts[2] );
        double equil = CFactory::GetNumber( ts[3] );
        string title = GetComment(4, ts);

        obj->Set( ATOM1, tn1 );
        obj->Set( ATOM2, tn2 );
        obj->Set( FORCE, force );
        obj->Set( EQUIL, equil );
        obj->Set( TITLE, title );

        getline( is, m_line );
        m_line_no++;
    }

}

// -------------------------------------------------------------------------

void CAmberParams::ReadAngles(istream& is, CAmberFFPtr& ff, int& top_id)
{
    m_debug << "> Reading angles ..." << endl;
    CEntityPtr list = ff->FindChild( "angles" );

    while( is && (! empty(m_line)) ) {

        // skip comments ---------------------
        if( m_line[0]=='#' ) {
            continue;
        }

        // split line ------------------------
        vector<string>  ts;
        string          line = trim_left_copy(m_line); // it has to be here, split does not remove trailing space
        if( empty(line) ) break;

        SplitTypes( line, 3);
        split( ts, line, is_any_of(" \t"), token_compress_on );

        if( ts.size() < 5 ){
            EmitRWError( "Three atom types, force constant, and equilibrium angle are expected." );
        }

        // create angle -----------------------
        CEntityPtr angle = CFactory::CreateNode( top_id );
        list->AddChild(angle);
        top_id++;

        // read data -------------------------
        CheckItemAsType(0, ts);
        CheckItemAsType(1, ts);
        CheckItemAsType(2, ts);
        CheckItemAsNumber(3, ts);
        CheckItemAsNumber(4, ts);

        string tn1 = ts[0];
        string tn2 = ts[1];
        string tn3 = ts[2];
        double force = CFactory::GetNumber( ts[3] );
        double equil = CFactory::GetNumber( ts[4] );
        string title = GetComment(5, ts);

        angle->Set( ATOM1, tn1 );
        angle->Set( ATOM2, tn2 );
        angle->Set( ATOM3, tn3 );
        angle->Set( FORCE, force );
        angle->Set( EQUIL, equil );
        angle->Set( TITLE, title );

        getline( is, m_line );
        m_line_no++;
    }

}

// -------------------------------------------------------------------------

void CAmberParams::ReadTorsions(istream& is, CAmberFFPtr& ff, int& top_id)
{
    m_debug << "> Reading torsions ..." << endl;
    CEntityPtr list = ff->FindChild( "torsions" );

    while( is && (! empty(m_line)) ) {

        // skip comments ---------------------
        if( m_line[0]=='#' ) {
            continue;
        }

        // split line ------------------------
        vector<string>  ts;
        string          line = trim_left_copy(m_line); // it has to be here, split does not remove trailing space
        if( empty(line) ) break;

        SplitTypes( line, 4);
        split( ts, line, is_any_of(" \t"), token_compress_on );

        if( ts.size() < 7 ){
            EmitRWError( "Four atom types and dihedral angle specification (3 items at least) are expected." );
        }

        // create bond -----------------------
        CEntityPtr obj = CFactory::CreateNode( top_id );
        list->AddChild(obj);
        top_id++;

        // read data -------------------------
        CheckItemAsType(0, ts);
        CheckItemAsType(1, ts);
        CheckItemAsType(2, ts);
        CheckItemAsType(3, ts);

        string tn1 = ts[0];
        string tn2 = ts[1];
        string tn3 = ts[2];
        string tn4 = ts[3];

        double divide;
        double force;
        double equil;
        double period;
        string title;

        if( (ts.size() >= 8) && CFactory::IsNumber(ts[7]) ) {
            CheckItemAsNumber(4, ts);
            CheckItemAsNumber(5, ts);
            CheckItemAsNumber(6, ts);
            CheckItemAsNumber(7, ts);
            divide = CFactory::GetNumber( ts[4] );
            force  = CFactory::GetNumber( ts[5] );
            equil  = CFactory::GetNumber( ts[6] );
            period = CFactory::GetNumber( ts[7] );
            title = GetComment(8, ts);
        } else {
            CheckItemAsNumber(4, ts);
            CheckItemAsNumber(5, ts);
            CheckItemAsNumber(6, ts);
            divide = 1.0;
            force  = CFactory::GetNumber( ts[4] );
            equil  = CFactory::GetNumber( ts[5] );
            period = CFactory::GetNumber( ts[6] );
            title = GetComment(7, ts);
        }

        if( divide == 0.0 ) {
            divide = 1.0;
        }

        obj->Set( ATOM1, tn1 );
        obj->Set( ATOM2, tn2 );
        obj->Set( ATOM3, tn3 );
        obj->Set( ATOM4, tn4 );

        obj->Set( DIVIDE, divide );
        obj->Set( FORCE, force );
        obj->Set( EQUIL, equil );
        obj->Set( PERIOD, period );
        obj->Set( TITLE, title );

        getline( is, m_line );
        m_line_no++;
    }

}

// -------------------------------------------------------------------------

void CAmberParams::ReadImpropers(istream& is, CAmberFFPtr& ff, int& top_id)
{
    m_debug << "> Reading impropers ..." << endl;
    CEntityPtr list = ff->FindChild( "impropers" );

    while( is && (! empty(m_line)) ) {

        // skip comments ---------------------
        if( m_line[0]=='#' ) {
            continue;
        }

        // split line ------------------------
        vector<string>  ts;
        string          line = trim_left_copy(m_line); // it has to be here, split does not remove trailing space
        if( empty(line) ) break;

        SplitTypes( line, 4);
        split( ts, line, is_any_of(" \t"), token_compress_on );

        if( ts.size() < 7 ){
            EmitRWError( "Four atom types and dihedral angle specification (3 items at least) are expected." );
        }

        // create bond -----------------------
        CEntityPtr obj = CFactory::CreateNode( top_id );
        list->AddChild(obj);
        top_id++;

        // read data -------------------------
        CheckItemAsType(0, ts);
        CheckItemAsType(1, ts);
        CheckItemAsType(2, ts);
        CheckItemAsType(3, ts);

        string tn1 = ts[0];
        string tn2 = ts[1];
        string tn3 = ts[2];
        string tn4 = ts[3];

        double divide;
        double force;
        double equil;
        double period;
        string title;

        CheckItemAsNumber(4, ts);
        CheckItemAsNumber(5, ts);
        CheckItemAsNumber(6, ts);
        divide = 1.0;
        force  = CFactory::GetNumber( ts[4] );
        equil  = CFactory::GetNumber( ts[5] );
        period = CFactory::GetNumber( ts[6] );
        title = GetComment(7, ts);

        obj->Set( ATOM1, tn1 );
        obj->Set( ATOM2, tn2 );
        obj->Set( ATOM3, tn3 );
        obj->Set( ATOM4, tn4 );

        obj->Set( DIVIDE, divide );
        obj->Set( FORCE, force );
        obj->Set( EQUIL, equil );
        obj->Set( PERIOD, period );
        obj->Set( TITLE, title );

        getline( is, m_line );
        m_line_no++;
    }

}

// -------------------------------------------------------------------------

void CAmberParams::SkipHBnd(istream& is)
{
    m_debug << "> Skipping H-Bonds ..." << endl;
    while( getline(is, m_line) ){
        m_line_no++;
        if ( empty(m_line) ) return;
    }
}

// -------------------------------------------------------------------------

void CAmberParams::ReadvdWMap(istream& is)
{
    //m_debug << "> Reading vdW map ..." << endl;
    m_vdwmap.clear();

    while( is && (! empty(m_line)) ) {

        // skip comments ---------------------
        if( m_line[0]=='#' ) {
            continue;
        }

        // read aliases
        istringstream ls( m_line );

        string name, alias;
        ls >> name;

        while( ls >> alias ) {
            m_vdwmap[ alias ] = name;
        }

        getline( is, m_line );
        m_line_no++;
    }
}

// -------------------------------------------------------------------------

void CAmberParams::SkipvdWHeader(istream& is)
{
    m_debug << "> Skipping vdW header ..." << endl;
    getline( is, m_line ); // skip MOD4      RE
    m_line_no++;
}

// -------------------------------------------------------------------------

void CAmberParams::ReadvdWParams(istream& is, CAmberFFPtr& ff)
{
    m_debug << "> Reading vdW parameters ..." << endl;

    CEntityPtr list = ff->FindChild("types");

    while( is && (! empty(m_line)) ) {

        // skip comments ---------------------
        if( m_line[0]=='#' ) {
            continue;
        }

        // split line ------------------------
        vector<string>  ts;
        string          line = trim_left_copy(m_line); // it has to be here, split does not remove trailing space
        if( empty(line) ) break;

        split( ts, line, is_any_of(" \t"), token_compress_on );

        if( ts.size() < 3 ){
            EmitRWError( "Atom type, radius, and epsilon are expected." );
        }

        // find type ------------------------
        CheckItemAsType(0, ts);
        string tn1 = ts[0];
        CEntityPtr obj = list->FindChild(tn1);
        if( ! obj ){
            EmitRWError( " Atom type <b>" + tn1 + "</b> not previously defined." );
        }

        // read data -------------------------
        double radius = CFactory::GetNumber( ts[1] );
        double depth = CFactory::GetNumber( ts[2] );
        string title = GetComment(3, ts);

        obj->Set( RSTAR, radius );
        obj->Set( DEPTH, depth );
        obj->Set( TITLE2, title );

        getline( is, m_line );
        m_line_no++;
    }
}

// -------------------------------------------------------------------------

void CAmberParams::AssignvdW(CAmberFFPtr& ff)
{
    CEntityPtr list = ff->FindChild("types");

    map< string, string >::const_iterator it = m_vdwmap.begin();
    map< string, string >::const_iterator ie = m_vdwmap.end();

    while( it != ie ) {
        // find objects
        CEntityPtr src = list->FindChild(it->second);
        if( ! src ){
            throw runtime_error( "vdW alias atom type <b>" + it->second + "</b> not previously defined" );
        }
        CEntityPtr dst = list->FindChild(it->first);
        if( dst ){
            dst->Set( RSTAR, src->Get<double>(RSTAR) );
            dst->Set( DEPTH, src->Get<double>(DEPTH) );
            dst->Set( TITLE2, src->Get<string>(TITLE2) );
        } else {
           throw runtime_error( "vdW aliased atom type <b>" + it->first + "</b> not previously defined" );
        }
        it++;
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

void CAmberParams::Write(ostream& os, CAmberFFPtr& ff)
{
    if( ! ff ){
        // invalid ff
        throw runtime_error( "ff is NULL in CAmberParams::Read" );
    }

    m_debug << high;
    m_debug << "> Writing title ..." << endl;
    m_debug << low;
    m_debug << "  Title: " << ff->Get<string>(TITLE) << endl;
    m_debug << high;

    // write title
    os << ff->Get<string>(TITLE) << endl;
    if( ! os ){
        throw runtime_error("unable to write title");
    }

    // write individual sections


}

////------------------------------------------------------------------------------

//void CAmberParams::WriteTypes(istream& os, CAmberFFPtr& ff)
//{
//    m_debug << "> Writing types ..." << endl;

//    os << "MASS" << endl;
//    CEntityPtr list = ff->FindChild( "types" );

//    CForwardIterator it = list->BeginChildren();
//    CForwardIterator ie = list->EndChildren();

//    while( it != ie ) {

//        os << it->GetName() << " ";
//        os << it->Get<double>(MASS) << " ";
//        os << it->Get<double>(POLAR) << " ";
//        os << it->Get<string>(TITLE);
//        os << endl;

//        it++;
//    }

//}

//// -------------------------------------------------------------------------

//void CAmberParams::WriteBonds(istream& is, CAmberFFPtr& ff)
//{
//    m_debug << "> Writing bonds ..." << endl;
//    os << "BOND" << endl;

//    CEntityPtr list = ff->FindChild( "bonds" );

//    CForwardIterator it = list->BeginChildren();
//    CForwardIterator ie = list->EndChildren();

//    while( it != ie ) {

//        os << it->Get<string>(ATOM1) << " ";
//        os << it->Get<string>(ATOM2) << " ";
//        os << it->Get<double>(FORCE) << " ";
//        os << it->Get<double>(EQUIL) << " ";
//        os << it->Get<string>(TITLE);
//        os << endl;

//        it++;
//    }

//}

//// -------------------------------------------------------------------------

//void CAmberParams::ReadAngles(istream& is, CAmberFFPtr& ff)
//{
//    m_debug << "> Writing angles ..." << endl;
//    os << "ANGL" << endl;

//    CEntityPtr list = ff->FindChild( "angles" );

//    CForwardIterator it = list->BeginChildren();
//    CForwardIterator ie = list->EndChildren();

//    while( it != ie ) {

//        os << it->Get<string>(ATOM1) << " ";
//        os << it->Get<string>(ATOM2) << " ";
//        os << it->Get<string>(ATOM3) << " ";
//        os << it->Get<double>(FORCE) << " ";
//        os << it->Get<double>(EQUIL) << " ";
//        os << it->Get<string>(TITLE);
//        os << endl;

//        it++;
//    }

//}

//// -------------------------------------------------------------------------

//void CAmberParams::WriteTorsions(istream& os, CAmberFFPtr& ff)
//{
//    m_debug << "> Writing torsions ..." << endl;
//    os << "TORS" << endl;

//    CEntityPtr list = ff->FindChild( "torsions" );

//    CForwardIterator it = list->BeginChildren();
//    CForwardIterator ie = list->EndChildren();

//    while( it != ie ) {

//        os << it->Get<string>(ATOM1)  << "-";
//        os << it->Get<string>(ATOM2)  << "-";
//        os << it->Get<string>(ATOM3)  << "-";
//        os << it->Get<string>(ATOM4)  << " ";
//        os << it->Get<double>(DIVIDE) << " ";
//        os << it->Get<double>(FORCE)  << " ";
//        os << it->Get<double>(EQUIL)  << " ";
//        os << it->Get<double>(PERIOD) << " ";
//        os << it->Get<string>(TITLE);
//        os << endl;

//        it++;
//    }

//}

//// -------------------------------------------------------------------------

//void CAmberParams::WriteImpropers(istream& is, CAmberFFPtr& ff)
//{
//    m_debug << "> Writing impropers ..." << endl;
//    os << "IMPR" << endl;

//    CEntityPtr list = ff->FindChild( "impropers" );

//    CForwardIterator it = list->BeginChildren();
//    CForwardIterator ie = list->EndChildren();

//    while( it != ie ) {

//        os << it->Get<string>(ATOM1)  << "-";
//        os << it->Get<string>(ATOM2)  << "-";
//        os << it->Get<string>(ATOM3)  << "-";
//        os << it->Get<string>(ATOM4)  << " ";
//        os << it->Get<double>(FORCE)  << " ";
//        os << it->Get<double>(EQUIL)  << " ";
//        os << it->Get<double>(PERIOD) << " ";
//        os << it->Get<string>(TITLE);

//        it++;
//    }

//}

//// -------------------------------------------------------------------------

//void CAmberParams::ReadvdWParams(istream& is, CAmberFFPtr& ff)
//{
//    m_debug << "> Reading vdW parameters ..." << endl;

//    CEntityPtr list = ff->FindChild("types");

//    while( is && (! empty(m_line)) ) {

//        // skip comments ---------------------
//        if( m_line[0]=='#' ) {
//            continue;
//        }

//        // split line ------------------------
//        vector<string>  ts;
//        string          line = trim_left_copy(m_line); // it has to be here, split does not remove trailing space
//        if( empty(line) ) break;

//        split( ts, line, is_any_of(" \t"), token_compress_on );

//        if( ts.size() < 3 ){
//            EmitRWError( "Atom type, radius, and epsilon are expected." );
//        }

//        // find type ------------------------
//        CheckItemAsType(0, ts);
//        string tn1 = ts[0];
//        CEntityPtr obj = list->FindChild(tn1);
//        if( ! obj ){
//            EmitRWError( " Atom type <b>" + tn1 + "</b> not previously defined." );
//        }

//        // read data -------------------------
//        double radius = CFactory::GetNumber( ts[1] );
//        double depth = CFactory::GetNumber( ts[2] );
//        string title = GetComment(3, ts);

//        obj->Set( RSTAR, radius );
//        obj->Set( DEPTH, depth );
//        obj->Set( TITLE2, title );

//        getline( is, m_line );
//        m_line_no++;
//    }
//}

// -------------------------------------------------------------------------

void CAmberParams::SplitTypes(string& line, int number)
{
    number--;
    for(size_t i=0; i < line.size(); i++){
        if( line[i] == '-' ){
            number--;
            line[i] = ' ';
        }
        if( number <= 0 ) break;
    }
}

// -------------------------------------------------------------------------

string CAmberParams::GetComment(int from, const vector<string>& ts)
{
    string title;

    for(size_t i=from; i < ts.size(); i++ ){
        title += ts[i];
        title += " ";
    }
    return( title );
}

// -------------------------------------------------------------------------

void CAmberParams::CheckItemAsType(size_t item, const vector<string>& ts)
{
    if( item >= ts.size() ){
        stringstream str;
        str << "Missing type name at position " << item+1;
        EmitRWError( str.str() );
    }
    if( ts[item].size() > 2  ){
        stringstream str;
        str << "Type name <b>" << ts[item] << "</b> at position " << item+1;
        str << " can have only one or two characters, but " << ts[item].size() << " are provided.";
        EmitRWError( str.str() );
    }
    if( isdigit(ts[item][0]) ){
        stringstream str;
        str << "Type name <b>" << ts[item] << "</b> at position " << item+1;
        str << " cannot start with a digit character.";
        EmitRWError( str.str() );
    }
}

// -------------------------------------------------------------------------

void CAmberParams::CheckItemAsNumber(size_t item, const vector<string>& ts)
{
    if( item >= ts.size() ){
        stringstream str;
        str << "Missing number parameter at position " << item+1;
        EmitRWError( str.str() );
    }

    if( ! CFactory::IsNumber(ts[item]) ){
        stringstream str;
        str << "Parameter <b>" << ts[item] << "</b> at position " << item+1;
        str << " is not a number.";
        EmitRWError( str.str() );
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}







