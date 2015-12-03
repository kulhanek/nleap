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

#include <format/FormatPDB.hpp>
#include <vector>
#include <sstream>
#include <types/Factory.hpp>
#include <boost/algorithm/string.hpp>

using namespace boost;

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CFormatPDB::CFormatPDB( CVerboseStr& debug )
    : m_debug( debug )
{
    m_line_no = 0;
}

// -------------------------------------------------------------------------

void CFormatPDB::Read( istream& is, CUnitPtr& unit, int& top_id )
{
    if( ! unit ){
        // invalid unit
        throw runtime_error( "unit is NULL in CFormatPDB::Read" );
    }


}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}








//#include <sstream>
//#include <common.hpp>
//#include <object.hpp>

//namespace mort
//{
//    int get_nunfill(const string& chain)
//    {
//        static const char IDS[] = "ESB3456";

//        const char* ptr = std::find(IDS, IDS+sizeof(IDS)/sizeof(char), chain[0]);

//        if( ptr != IDS+sizeof(IDS)/sizeof(char) )
//        {
//            return ptr - IDS;
//        }
        
//        if( chain[0] == 'M' )
//        {
//            return 10000;
//        }
        
//        throw std::runtime_error("Error: unknown chain identifier " + chain);
//    }
    
//    void read_pdb(istream& is, molecule_t& mol)
//    {
//        molecule_t m;

//        // ignore first line
//	is.ignore(MAX_LINE_WIDTH, '\n');

//        // name and coordinate type (INT/XYZ, 0/1)
//	string name, crdtype, crdtypeid;
//	is >> name >> crdtype >> crdtypeid;
//	is.ignore(MAX_LINE_WIDTH, '\n');
//        m.set_s(NAME, name);

//        resd_t r = resd_t::create( m );
//        r.set_s(NAME, name);
//        r.set_s(TYPE, name.length()<3?name:name.substr(0,3));

//        // CORR/CHANGE, if CHANGE then sort line according
//	// the first column, no supported any more.
//	string use_first_column;
//	is >> use_first_column;
//	//if(use_first_column.find("CORR") == string::npos)
//        //{
//	//    throw std::runtime_error("Error: prep format, sort by first column no longer supported");
//	//}
//	is.ignore(MAX_LINE_WIDTH, '\n');

//        // cutoff: if > 0.0, make bond according to distance
//	double cutoff;
//	is >> cutoff;
//	is.ignore(MAX_LINE_WIDTH, '\n');

//        vector<CAtom> atoms;
//        vector<numvec> crds;
//        vector<int> nunfills;

//        int head = 0;
//        int tail = 0;
//	char line[MAX_LINE_WIDTH];
//	while( is.getline(line, MAX_LINE_WIDTH) && !empty(line) )
//	{
//            std::istringstream ls(line);
//	    int id;
//            ls >> id;

//	    string name, type, chain;
//            ls >> name >> type >> chain;


//            string curt;
//	    vector<string> terms;
//	    while(ls >> curt)
//	    {
//	        terms.push_back(curt);
//	    }

//            numvec pos(3);
//	    double pchg=0.0;
//	    if( terms.size()==3 || terms.size()==4 )
//	    {
//	        double x = atof( terms[0].c_str() );
//		double y = atof( terms[1].c_str() );
//		double z = atof( terms[2].c_str() );
//		pchg = (terms.size()==4) ? atof( terms[3].c_str() ) : 0.0;
//		pos  = makevec(x,y,z);
//	    }
//	    else
//	    {
//	        assert( terms.size()==6 || terms.size()==7 );
//		double dist = atof( terms[3].c_str() );
//		double angl = atof( terms[4].c_str() );
//		double tors = atof( terms[5].c_str() );
//		pchg = (terms.size()==7) ? atof( terms[6].c_str() ) : 0.0;
//		int len = crds.size();
//		if(len==0)
//		{
//		    pos = makevec(0.0, 0.0, 0.0);
//		}
//		else if(len==1)
//		{
//		    pos = zmatrix(crds[len-1], dist);
//		}
//		else if(len==2)
//		{
//		    pos = zmatrix(crds[len-1], dist, crds[len-2], angl);
//		}
//		else
//		{
//                    pos = zmatrix(crds[len-1], dist, crds[len-2], angl, crds[len-3], tors);
//                }
//            }

//            CAtom atom(m, -1);
//            if( type != "DU" )
//	    {
//	        atom = r.CreateAtom(name);
//	        atom.set_s(NAME, name);
//                atom.set_s(TYPE, type);
//	        atom.set_s(CHAIN, chain);
//	        atom.set_v(POSITION, pos);
//	        atom.set_d(PCHG, pchg);

//                if(atoms.size() > 0)
//		{
//		    assert( nunfills.size()==crds.size() );
//	            CBond::create(atom, atoms.back() ).set_i(ORDER, 1);
//                    nunfills.back() -= 1;
//                }

//                if( chain=="M" )
//                {
//                    // head is the first M atom, tail is the last M atom
//                    if( head==0 )
//                        head = atom.absGetId()+1;
//                    tail = atom.absGetId() + 1;
//                }
//	    }

            

//	    if( nunfills.size()>0 && nunfills.back()==0 )
//	    {
//	        crds.pop_back();
//	        atoms.pop_back();
//		nunfills.pop_back();
//	    }

//	    int nunfill = get_nunfill(chain);
//	    if(nunfill > 0)
//            {
//	        crds.push_back(pos);
//		if(type != "DU")
//	            atoms.push_back(atom);
//                nunfills.push_back(nunfill);
//	    }
            
//        }

//        m.set_i( HEAD, head );
//        m.set_i( TAIL, tail );
//        r.set_i( HEAD, head );
//        r.set_i( TAIL, tail );

//        string keyw;
//	while(is >> keyw && keyw != "DONE")
//	{
//            is.ignore(MAX_LINE_WIDTH, '\n');
            
//            if(keyw=="LOOP")
//	    {
//	        char line[MAX_LINE_WIDTH];
//		while( is.getline(line, MAX_LINE_WIDTH) && !empty(line) )
//		{
//                    std::istringstream ls(line);
//	            string atm1, atm2;
//		    ls >> atm1 >> atm2;

//                    CAtom a1 = CAtom::get( m, atm1);
//                    CAtom a2 = CAtom::get( m, atm2);
//	            CBond b = CBond::create(a1, a2);
//                    b.set_i(ORDER, 1);
//	        }
//	    }
//	    else if(keyw=="CHARGE")
//	    {
//                char line[MAX_LINE_WIDTH];
//		while( is.getline(line, MAX_LINE_WIDTH) && !empty(line) )
//		{
//                }
//	    }
//	    else if(keyw=="IMPROPER")
//	    {
//	        char line[MAX_LINE_WIDTH];
//		while( is.getline(line, MAX_LINE_WIDTH) && !empty(line) )
//		{
//                }
//            }
//	    else
//	    {
//	        throw std::logic_error("Error: unknown prep keyword " + keyw);
//	    }
//	}

//        mol.swap(m);
//    }

//    void read_pdb(istream& is, CDatabase& mdb)
//    {
//        // ignore first two lines
//	is.ignore(MAX_LINE_WIDTH, '\n');
//	is.ignore(MAX_LINE_WIDTH, '\n');

//        string keyw;
//	while(is>>keyw && keyw!="STOP")
//	{
//            is.ignore(MAX_LINE_WIDTH, '\n');
//	    molecule_ptr pm( new molecule_t() );
//	    read_pdb(is, *pm);
//	    mdb.add( pm->get_s(NAME), pm);
//	}
//    }

//} // namespace mort

