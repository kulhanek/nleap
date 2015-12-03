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

#include <stdexcept>
#include <sstream>
#include <assert.h>
#include <stdio.h>
#include <engine/Parser.hpp>
#include <engine/Command.hpp>

namespace nleap {
//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

bool closed( const string& list, char bgn, char end )
{
    int ncurly=0;

    for( int i=0; i < (int)list.length(); ++i ) {
        if( list[i] == bgn ) {
            ncurly++;
        } else if( list[i] == end ) {
            ncurly--;
        }

        if( ncurly < 0 ) {
            throw runtime_error( "Error: bad list " + list );
        }
    }

    return ( ncurly == 0 );
}

// -------------------------------------------------------------------------

bool curly_closed( const string& list )
{
    return closed( list, '{', '}' );
}

// -------------------------------------------------------------------------

bool quota_closed( const string& list )
{
    int ncurly=0;

    for( int i=0; i < (int)list.length(); ++i ) {
        if( list[i] == '"' ) {
            ncurly++;
        }
    }

    return ( ncurly % 2 == 0 );
}

// -------------------------------------------------------------------------

bool isblank_tab_newline_carriagereturn( char n )
{
    return n==' ' || n=='\t' || n=='\n' || n=='\r';
}

// -------------------------------------------------------------------------

string parse_curly( istream& is )
{
    char c = is.get();
    assert( c == '{' );

    int ncurly = 1;
    string str = "{";

    while( ncurly > 0 ) {
        c = is.get();

        if( !is ) throw std::runtime_error( "unpaired {" );

        if( c == '{' ) ++ncurly;
        if( c == '}' ) --ncurly;
        str.append( 1, c );
    }

    return str;
}

// -------------------------------------------------------------------------

string parse_quota( istream& is )
{
    char c;
    is >> c;
    assert( c == '"' );

    string str;
    while( is >> std::noskipws >> c && c !='"' ) {
        if(c !='"' ) str.append( 1, c );
    }

    if( c != '"' ) {
        throw runtime_error( "unpaired \"" );
    }

    str = "\"" + str + "\"";

    return str;
}

// -------------------------------------------------------------------------

string parse_line( istream& is )
{
    char n = is.peek();
    while( is && isblank_tab_newline_carriagereturn(n) ) {
        is.ignore();
        n = is.peek();
    }

    if( !is ) return "";

    if( n == '#' ) {
        is.ignore();
        return "#";
    }

    if( n == '=' ) {
        is.ignore();
        return "=";
    }

    if( n == '{' ) {
        return parse_curly( is );
    }

    if( n == '"' ) {
        return parse_quota( is );
    }

    if( n == '}' ) {
        throw runtime_error( "unpaired }" );
    }

    string reserve = "={}\" \n\t\r";
    string word;

    while( reserve.find(n)==string::npos ) {
        if(is >> n) {
            word.append(1, n);
        } else {
            break;
        }

        n = is.peek();
    }

    return word;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

const vector<string>& CParser::GetArgs(void) const
{
    return(m_args);
}

// -------------------------------------------------------------------------

const string& CParser::GetLHS(void) const
{
    return(m_lhs);
}

// -------------------------------------------------------------------------

const string& CParser::GetCmd(void) const
{
    return(m_cmd);
}

// -------------------------------------------------------------------------

CParser::ESyntaxStatus CParser::CheckSyntax(const string& line)
{
    if( (!curly_closed(line)) || (!quota_closed(line)) ) {
        return syntax_intermediate;
    }

    return syntax_valid;
}

// -------------------------------------------------------------------------

void CParser::Parse(const string& line)
{
    istringstream is( line );

    bool assign = false;

    while( is ) {
        string arg =  parse_line( is );

        if( arg == "#" ) {
            break;
        }

        if( arg == "=" ) {
            if( assign ) {
                throw runtime_error( "operator '=' appeared more than one time" );
            }
            if( m_args.size() != 1 ) {
                throw runtime_error( "none or more than one assignee" );
            }

            assign = true;
            continue;
        }

        if( ! arg.empty() ) {
            m_args.push_back(arg);
            continue;
        }
    }

    if( assign ) {
        m_lhs = m_args[0];
        m_args.erase(m_args.begin());

        if( m_args.size() < 1 ) {
            throw runtime_error( "no arguments for assignment" );
        }

        if( m_args.size() == 1 ) {
            m_cmd = "assign";
        } else {
            m_cmd = m_args[0];
            m_args.erase(m_args.begin());
        }
    } else {
        if( m_args.size() >= 1 ) {
            m_cmd = m_args[0];
            m_args.erase(m_args.begin());
        }
    }
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================
}

