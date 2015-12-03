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

#include <engine/Command.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <core/Entity.hpp>
#include <engine/Context.hpp>
#include <types/Factory.hpp>
#include <stdio.h>

namespace nleap {

using namespace boost::algorithm;
using namespace std;

std::map<string, CCommand*> CCommand::g_commands;

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CCommand::CCommand(const string& cmd_name)
: m_action(cmd_name)
{
    g_commands[cmd_name] = this;
    m_change_state = true;
}

// -------------------------------------------------------------------------

CCommand::CCommand(const string& cmd_name, const string& action)
: m_action(action)
{
    m_change_state = true;
}

// -------------------------------------------------------------------------

CCommand::~CCommand(void)
{
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

CCommand* CCommand::Find(const string& name)
{
    map<string, CCommand*>::iterator i;

    for(i = g_commands.begin(); i != g_commands.end(); i++)
    {
        if( to_lower_copy(i->first) == to_lower_copy(name) )
        {
            return(i->second);
        }
    }

    return(NULL);
}

// -------------------------------------------------------------------------

map< string, CCommand* >::iterator CCommand::Begin(void)
{
    return g_commands.begin();
}

// -------------------------------------------------------------------------

map< string, CCommand* >::iterator CCommand::End(void)
{
    return g_commands.end();
}

// -------------------------------------------------------------------------

map< string, CCommand* >::const_iterator CCommand::Begin(void) const
{
    return g_commands.begin();
}

// -------------------------------------------------------------------------

map< string, CCommand* >::const_iterator CCommand::End(void) const
{
    return g_commands.end();
}

// -------------------------------------------------------------------------

bool CCommand::IsChangingState(void)
{
    return( m_change_state );
}

// -------------------------------------------------------------------------

void CCommand::InternalError(void) const
{
    throw std::runtime_error( "internal error in CCommand" );
}

// -------------------------------------------------------------------------

void CCommand::NoAssigmentPossible( const CParser& cmdline ) const
{
    if( cmdline.GetLHS().size() == 0 ) return; // no left side -> return

    // left side is there - throw an error
    stringstream str;
    str << "Command <b>" << m_action << "</b> does not return any object thus assigment is not possible." << endl;
    str << "       Type <b>help " << m_action << "</b> to get a clue what can be wrong.";

    throw std::runtime_error( str.str() );
}

// -------------------------------------------------------------------------

void CCommand::AssigmentRequired( const CParser& cmdline ) const
{
    if( cmdline.GetLHS().size() != 0 ) return;

    // no left side is there - throw an error
    stringstream str;
    str << "Assigment operator (=) is required." << endl;
    str << "       Type <b>help " << m_action << "</b> to get a clue what can be wrong.";

    throw std::runtime_error( str.str() );
}

// -------------------------------------------------------------------------

void CCommand::CheckNumberOfArguments( const CParser& cmdline, unsigned int min, unsigned int max ) const
{
    CheckNumberOfArguments( cmdline.GetArgs(), min, max );
}

// -------------------------------------------------------------------------

void CCommand::CheckNumberOfArguments( const vector<string>& args, unsigned int min, unsigned int max ) const
{
    if( max < min ){
        InternalError();
    }

    if( (args.size() >= min) && (args.size() <= max) ){
        return;
    }

    // incorrect number of arguments - throw an error
    stringstream str;
    if( min != max ){
        str << "Incorrect number (" << args.size() << ") of arguments provided but <<" << min << "," << max << "> expected." << endl;
    } else {
        str << "Incorrect number (" << args.size() << ") of arguments provided but " << min << " is expected." << endl;
    }
    str << "       Type <b>help " << m_action << "</b> to get a clue what can be wrong.";

    throw std::runtime_error( str.str() );
}

// -------------------------------------------------------------------------

void CCommand::WrongArgument( const CParser& cmdline, unsigned int pos, const string& reason ) const
{
    WrongArgument( cmdline.GetArgs(), pos, reason );
}

// -------------------------------------------------------------------------

void CCommand::WrongArgument( const vector<string>& args, unsigned int pos, const string& reason ) const
{
    if( pos >= args.size() ){
        InternalError();
    }

    // incorrect number of arguments - throw an error
    stringstream str;
    str << "Wrong argument '" << args[pos] << "' at position " << pos + 1;
    if( ! reason.empty() ){
        str << " (" << reason << ")";
    }
    str << "." << endl;
    str << "       Type <b>help " << m_action << "</b> to get a clue what can be wrong.";

    throw std::runtime_error( str.str() );
}

// -------------------------------------------------------------------------

void CCommand::ExpandArgument( CContext* p_ctx, const CParser& cmdline, unsigned int pos, string& output ) const
{
    ExpandArgument( p_ctx, cmdline.GetArgs(), pos, output );
}

// -------------------------------------------------------------------------

void CCommand::ExpandArgument( CContext* p_ctx, const vector<string>& args, unsigned int pos, string& output ) const
{
    output.clear();

    if( pos >= args.size() ){
        InternalError();
    }

    string value = args[pos];

    // no input
    if( value.empty() ) return;

    // is it variable?
    if( value[0] != '$' ) {
        // no variable
        if( value[0] == '"' ){
            if( (value.size() < 2) || ( value[value.size()-1] != '"' ) ){
                WrongArgument( args, pos, "unbalanced quotation");
            }
            // strip quotation
            output = value.substr(1,value.size()-2);
        } else {
            output = value;
        }
        return;
    }

    // it is variable - expand it
    string vname = value.substr(1);
    CEntityPtr obj = p_ctx->database()->GetVariableObject( vname );

    if( ! obj ){
        stringstream str;
        str << "variable '" << vname << "' does not exist";
        WrongArgument( args, pos, str.str() );
    }

    // can we convert variable?
    if( obj->GetType() != STRING ){
        stringstream str;
        str << "incorrect type of variable '" << vname << "', STRING expected but " << obj->GetType().GetName() << " provided";
        WrongArgument( args, pos, str.str() );
    }

    // get value
    output = obj->Get<string>(VALUE);
}

// -------------------------------------------------------------------------

void CCommand::ExpandArgument( CContext* p_ctx, const CParser& cmdline, unsigned int pos, int& output ) const
{
    ExpandArgument( p_ctx, cmdline.GetArgs(), pos, output );
}

// -------------------------------------------------------------------------

void CCommand::ExpandArgument( CContext* p_ctx, const vector<string>& args, unsigned int pos, int& output ) const
{
    output = 0;

    if( pos >= args.size() ){
        InternalError();
    }

    string value = args[pos];

    // no input
    if( value.empty() ){
        WrongArgument( args, pos, "the argument is empty but a number is expected" );
    }

    // is it variable?
    if( value[0] != '$' ) {
        // no variable
        if( value[0] == '"' ){
            WrongArgument( args, pos, "number cannot contain quotation");
        } else {
            // convert to number
            if( CFactory::DetermineType(value) != NUMBER ){
                WrongArgument( args, pos, "not an integer number");
            }
            if( ! CFactory::IsInteger(value) ){
                WrongArgument( args, pos, "not an integer number");
            }
            output = CFactory::GetNumber(value);
        }
        return;
    }

    // it is variable - expand it
    string vname = value.substr(1);
    CEntityPtr obj = p_ctx->database()->GetVariableObject( vname );

    if( ! obj ){
        stringstream str;
        str << "variable '" << vname << "' does not exist";
        WrongArgument( args, pos, str.str() );
    }

    // can we convert variable?
    if( obj->GetType() != NUMBER ){
        stringstream str;
        str << "incorrect type of variable '" << vname << "', NUMBER expected but " << obj->GetType().GetName() << " provided";
        WrongArgument( args, pos, str.str() );
    }

    CNumberPtr num = dynamic_pointer_cast<CNumber>(obj);

    if( ! num ){
        InternalError();
    }

    if( ! num->IsInteger() ){
        stringstream str;
        str << "variable '" << vname << "' does not contain an integer number";
        WrongArgument( args, pos, str.str() );
    }

    // get value
    output = num->GetNumber();
}

// -------------------------------------------------------------------------

void CCommand::ExpandArgument( CContext* p_ctx, const CParser& cmdline, unsigned int pos, double& output ) const
{
    ExpandArgument( p_ctx, cmdline.GetArgs(), pos, output );
}

// -------------------------------------------------------------------------

void CCommand::ExpandArgument( CContext* p_ctx, const vector<string>& args, unsigned int pos, double& output ) const
{
    output = 0;

    if( pos >= args.size() ){
        InternalError();
    }

    string value = args[pos];

    // no input
    if( value.empty() ){
        WrongArgument( args, pos, "the argument is empty but a number is expected" );
    }

    // is it variable?
    if( value[0] != '$' ) {
        // no variable
        if( value[0] == '"' ){
            WrongArgument( args, pos, "number cannot contain quotation");
        } else {
            // convert to number
            if( CFactory::DetermineType(value) != NUMBER ){
                WrongArgument( args, pos, "not a number");
            }
            output = CFactory::GetNumber(value);
        }
        return;
    }

    // it is variable - expand it
    string vname = value.substr(1);
    CEntityPtr obj = p_ctx->database()->GetVariableObject( vname );

    if( ! obj ){
        stringstream str;
        str << "variable '" << vname << "' does not exist";
        WrongArgument( args, pos, str.str() );
    }

    // can we convert variable?
    if( obj->GetType() != NUMBER ){
        stringstream str;
        str << "incorrect type of variable '" << vname << "', STRING expected but " << obj->GetType().GetName() << " provided";
        WrongArgument( args, pos, str.str() );
    }

    CNumberPtr num = dynamic_pointer_cast<CNumber>(obj);

    if( ! num ){
        InternalError();
    }

    // get value
    output = num->GetValue();
}

// -------------------------------------------------------------------------

void CCommand::ExpandArgument( CContext* p_ctx, const CParser& cmdline, unsigned int pos, CListPtr& list ) const
{
    ExpandArgument( p_ctx, cmdline.GetArgs(), pos, list );
}

// -------------------------------------------------------------------------

void CCommand::ExpandArgument( CContext* p_ctx, const vector<string>& args, unsigned int pos, CListPtr& list ) const
{
    if( pos >= args.size() ){
        InternalError();
    }

    string value = args[pos];

    // no input
    if( value.empty() ) {
        WrongArgument( args, pos, "the argument is empty but a list is expected" );
    }

    int top_id = p_ctx->m_index_counter.GetTopIndex();

    list = CFactory::CreateList( top_id, value );

    p_ctx->m_index_counter.SetTopIndex( top_id );

}

// -------------------------------------------------------------------------

void CCommand::ExpandArgument( CContext* p_ctx, const CParser& cmdline, unsigned int pos, CEntityPtr& object, const CKey& type ) const
{
    ExpandArgument( p_ctx, cmdline.GetArgs(), pos, object, type );
}

// -------------------------------------------------------------------------

void CCommand::ExpandArgument( CContext* p_ctx, const vector<string>& args, unsigned int pos, CEntityPtr& object, const CKey& type ) const
{
    object = CEntityPtr();

    if( pos >= args.size() ){
        InternalError();
    }

    string value = args[pos];

    // no input
    if( value.empty() ) {
        WrongArgument( args, pos, "the argument is empty but an object name is expected" );
    }

    // is it variable?
    if( value[0] != '$' ) {
        // no variable
        if( value[0] == '"' ){
            WrongArgument( args, pos, "object name cannot contain quotations");
        }
    } else {
        string vname = value.substr(1);
        CEntityPtr obj = p_ctx->database()->GetVariableObject( value );

        if( ! obj ){
            stringstream str;
            str << "variable '" << vname << "' does not exist";
            WrongArgument( args, pos, str.str() );
        }

        // can we convert variable?
        if( obj->GetType() != STRING ){
            stringstream str;
            str << "incorrect type of variable '" << vname << "', STRING expected but " << obj->GetType().GetName() << " provided";
            WrongArgument( args, pos, str.str() );
        }

        // get value
        value = obj->Get<string>(VALUE);
    }

    // once again check if argument is not empty (because of variable expansion)
    if( value.empty() ) {
        WrongArgument( args, pos, "the argument is empty but an object name is expected" );
    }

   CEntityPtr subobj = p_ctx->database()->GetSubObj(value);

   if( ! subobj ){
       stringstream str;
       str << "unable to find object '" << value << "'";
       WrongArgument( args, pos, str.str() );
   }

   if( (type != subobj->GetType()) && (type != ANY) ) {
       stringstream str;
       str << "incorrect type of object '" << value << "', " <<  type.GetName() << " expected but " << subobj->GetType().GetName() << " provided";
       WrongArgument( args, pos, str.str() );
   }

   object = subobj;
}

// -------------------------------------------------------------------------

void CCommand::WrongLHS( const CParser& cmdline, const string& reason ) const
{
    // incorrect number of arguments - throw an error
    stringstream str;
    str << "Wrong left-hand side '" << cmdline.GetLHS() << "' of command";
    if( ! reason.empty() ){
        str << " (" << reason << ")";
    }
    str << "." << endl;
    str << "       Type <b>help " << m_action << "</b> to get a clue what can be wrong.";

    throw std::runtime_error( str.str() );
}

// -------------------------------------------------------------------------

void CCommand::ExpandLHS( CContext* p_ctx, const CParser& cmdline, string& output ) const
{
    output.clear();

    string value = cmdline.GetLHS();

    // no input
    if( value.empty() ){
        WrongLHS( cmdline, "variable name cannot be empty");
    }

    // is it variable?
    if( value[0] != '$' ) {
        if( ! CFactory::IsValidVariableName(value) ){
            WrongLHS( cmdline, "illegal character in variable name");
        }
        output = value;
        return;
    }

    // it is variable - expand it
    string vname = value.substr(1);
    CEntityPtr obj = p_ctx->database()->GetVariableObject( vname );

    if( ! obj ){
        stringstream str;
        str << "object '" << vname << "' does not exist";
        WrongLHS( cmdline, str.str() );
    }

    // can we convert variable?
    if( obj->GetType() != STRING ){
        stringstream str;
        str << "incorrect type of object '" << vname << "', STRING expected but " << obj->GetType().GetName() << " provided";
        WrongLHS( cmdline, str.str() );
    }

    // get value
    value = obj->Get<string>(VALUE);

    if( ! CFactory::IsValidVariableName(value) ){
        WrongLHS( cmdline, "illegal character in variable name");
    }

    output = value;
}

//==============================================================================
//------------------------------------------------------------------------------
//==============================================================================

}

