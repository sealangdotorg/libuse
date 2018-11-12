//
//  Copyright (C) 2014-2018 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
//                <https://github.com/casm-lang/libstdhl>
//
//  This file is part of libstdhl.
//
//  libstdhl is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  libstdhl is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with libstdhl. If not, see <http://www.gnu.org/licenses/>.
//
//  Additional permission under GNU GPL version 3 section 7
//
//  libstdhl is distributed under the terms of the GNU General Public License
//  with the following clarification and special exception: Linking libstdhl
//  statically or dynamically with other modules is making a combined work
//  based on libstdhl. Thus, the terms and conditions of the GNU General
//  Public License cover the whole combination. As a special exception,
//  the copyright holders of libstdhl give you permission to link libstdhl
//  with independent modules to produce an executable, regardless of the
//  license terms of these independent modules, and to copy and distribute
//  the resulting executable under terms of your choice, provided that you
//  also meet, for each linked independent module, the terms and conditions
//  of the license of that module. An independent module is a module which
//  is not derived from or based on libstdhl. If you modify libstdhl, you
//  may extend this exception to your version of the library, but you are
//  not obliged to do so. If you do not wish to do so, delete this exception
//  statement from your version.
//

#include "Message.h"

#include "Content.h"
#include "Exception.h"
#include "Identifier.h"
#include "Interface.h"

#include <libstdhl/String>

using namespace libstdhl;
using namespace Network;
using namespace DAP;

//
//
// Message
//

Message::Message( const ID id, const std::size_t seq, const Content& data )
: Content( data )
, m_id( id )
{
    operator[]( Identifier::seq ) = seq;

    switch( m_id )
    {
        case ID::REQUEST_MESSAGE:
        {
            operator[]( Identifier::type ) = Identifier::request;
            break;
        }
        case ID::EVENT_MESSAGE:
        {
            operator[]( Identifier::type ) = Identifier::event;
            break;
        }
        case ID::RESPONSE_MESSAGE:
        {
            operator[]( Identifier::type ) = Identifier::response;
            break;
        }
        case ID::UNKNOWN:  // [[fallthrough]]
        case ID::_SIZE_:
        {
            assert( !"internal error!" );
            break;
        }
    };
}

Message::ID Message::id( void ) const
{
    return m_id;
}

const Content& Message::obj( void ) const
{
    return static_cast< const Content& >( *this );
}

std::size_t Message::seq( void ) const
{
    return at( Identifier::seq ).get< std::size_t >();
}

std::string Message::type( void ) const
{
    return at( Identifier::type ).get< std::string >();
}

void Message::process( ServerInterface& interface ) const
{
    switch( id() )
    {
        case ID::REQUEST_MESSAGE:
        {
            const auto msg = reinterpret_cast< const RequestMessage& >( *this );
            msg.process( interface );
            break;
        }
        case ID::EVENT_MESSAGE:
        {
            const auto msg = reinterpret_cast< const EventMessage& >( *this );
            msg.process( interface );
            break;
        }
        case ID::RESPONSE_MESSAGE:
        {
            const auto msg = reinterpret_cast< const ResponseMessage& >( *this );
            msg.process( interface );
            break;
        }
        case ID::UNKNOWN:  // [[fallthrough]]
        case ID::_SIZE_:
        {
            // TODO: FIXME: @ppaulweber
            // auto msg = ResponseMessage();
            // msg.setError( ErrorCode::InternalError, "invalid message" );
            // interface.respond( msg );
            break;
        }
    };
}

void Message::validate( const Content& data )
{
    static const std::string context = "DAP: Message:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsNumber( context, data, Identifier::seq, true );
    Json::validatePropertyIsString( context, data, Identifier::type, true );

    const auto& type = data.at( Identifier::type ).get< std::string >();

    static const std::vector< std::string > messageTypes = { Identifier::request,
                                                             Identifier::event,
                                                             Identifier::response };

    u1 valid = false;
    for( auto& messageType : messageTypes )
    {
        if( type == messageType )
        {
            valid = true;
            break;
        }
    }

    if( not valid )
    {
        throw std::invalid_argument( context + " invalid type '" + type + "'" );
    }
}

Message Message::parse( const std::string& data )
{
    ID id = ID::UNKNOWN;
    Content payload;

    // parse JSON data from string
    payload = Json::Object::parse( data );
    Message::validate( payload );

    if( payload.find( Identifier::request_seq ) != payload.end() )
    {
        // contains 'request_seq'
        // ==> response message
        id = ID::RESPONSE_MESSAGE;
    }
    else if( payload.find( Identifier::event ) != payload.end() )
    {
        // contains 'event'
        // ==> event message
        id = ID::EVENT_MESSAGE;
    }
    else if( payload.find( Identifier::command ) != payload.end() )
    {
        // contains 'command'
        // ==> request message
        id = ID::REQUEST_MESSAGE;
    }

    switch( id )
    {
        case ID::REQUEST_MESSAGE:
        {
            RequestMessage::validate( payload );
            return RequestMessage( payload );
        }
        case ID::EVENT_MESSAGE:
        {
            EventMessage::validate( payload );
            return EventMessage( payload );
        }
        case ID::RESPONSE_MESSAGE:
        {
            ResponseMessage::validate( payload );
            return ResponseMessage( data );
        }
        case ID::UNKNOWN:  // [[fallthrough]]
        case ID::_SIZE_:
        {
            throw std::invalid_argument( "DAP: unknown message payload" );
            return Message();
        }
    }
}

//
//
// Request Message
//

RequestMessage::RequestMessage( const Content& data )
: Message( ID::REQUEST_MESSAGE, 0, data )
{
    assert( data.find( Identifier::seq ) != data.end() );
    operator[]( Identifier::seq ) = data[ Identifier::seq ];
}

RequestMessage::RequestMessage( const std::size_t seq, const std::string& command )
: Message( ID::REQUEST_MESSAGE, seq )
{
    operator[]( Identifier::command ) = command;
}

std::string RequestMessage::command( void ) const
{
    return operator[]( Identifier::command ).get< std::string >();
}

u1 RequestMessage::hasArguments( void ) const
{
    return find( Identifier::arguments ) != end();
}

Content RequestMessage::arguments( void ) const
{
    return at( Identifier::arguments );
}

void RequestMessage::setArguments( const Content& data )
{
    operator[]( Identifier::arguments ) = Content::from_cbor( Content::to_cbor( data ) );
}

void RequestMessage::process( ServerInterface& interface ) const
{
    // auto response = ResponseMessage( id() )
    const auto& cmd = command();
    const auto cmdValue = String::value( cmd );

    try
    {
        switch( cmdValue )
        {
            default:
            {
                // response.setError(
                //     ErrorCode::MethodNotFound,
                //     "notification method '" + m + "' not specified in interface implementation"
                //     );
                break;
            }
        }
    }
    catch( const Exception& e )
    {
        // response.setError( e.code(), e.message(), e.data() );
    }
    catch( const std::invalid_argument& e )
    {
        // response.setError(
        //     ErrorCode::ParseError, "parse error: '" + std::string( e.what() ) + "'" );
    }

    // interface.respond( response );
}

void RequestMessage::validate( const Content& data )
{
    static const auto context = "DAP: RequestMessage:";
    Message::validate( data );
    Json::validatePropertyIsString( context, data, Identifier::command, true );
    Json::validatePropertyIsObject( context, data, Identifier::arguments, false );
}

//
//
// Event Message
//

EventMessage::EventMessage( const Content& data )
: Message( ID::EVENT_MESSAGE, 0, data )
{
    assert( data.find( Identifier::seq ) != data.end() );
    operator[]( Identifier::seq ) = data[ Identifier::seq ];
}

EventMessage::EventMessage( const std::size_t seq, const std::string& event )
: Message( ID::EVENT_MESSAGE, seq )
{
    operator[]( Identifier::event ) = event;
}

std::string EventMessage::event( void ) const
{
    return operator[]( Identifier::event ).get< std::string >();
}

u1 EventMessage::hasBody( void ) const
{
    return find( Identifier::body ) != end();
}

Content EventMessage::body( void ) const
{
    return at( Identifier::body );
}

void EventMessage::setBody( const Content& data )
{
    operator[]( Identifier::body ) = Content::from_cbor( Content::to_cbor( data ) );
}

void EventMessage::process( ServerInterface& interface ) const
{
    // auto response = ResponseMessage();

    const auto& evt = event();
    const auto evtValue = String::value( evt );

    try
    {
        switch( evtValue )
        {
            default:
            {
                // response.setError(
                //     ErrorCode::MethodNotFound,
                //     "event method '" + m + "' not specified in interface implementation" );
                break;
            }
        };
    }
    catch( const std::invalid_argument& e )
    {
        // response.setError(
        //     ErrorCode::ParseError,
        //     "event method '" + m + "': parse error: '" + std::string( e.what() ) + "'" );
    }

    // if( response.hasError() )
    // {
    //     interface.respond( response );
    // }
}

void EventMessage::validate( const Content& data )
{
    static const auto context = "DAP: EventMessage:";
    Message::validate( data );
    Json::validatePropertyIsString( context, data, Identifier::event, true );
    Json::validatePropertyIsObject( context, data, Identifier::body, false );
}

//
//
// Response Message
//

ResponseMessage::ResponseMessage( const Content& data )
: Message( ID::RESPONSE_MESSAGE, 0, data )
{
    assert( data.find( Identifier::seq ) != data.end() );
    operator[]( Identifier::seq ) = data[ Identifier::seq ];
}

ResponseMessage::ResponseMessage(
    const std::size_t seq,
    const std::size_t request_seq,
    const u1 success,
    const std::string& command )
: Message( ID::RESPONSE_MESSAGE, seq )
{
    operator[]( Identifier::request_seq ) = request_seq;
    operator[]( Identifier::success ) = success;
    operator[]( Identifier::command ) = command;
}

std::size_t ResponseMessage::request_seq( void ) const
{
    return operator[]( Identifier::request_seq ).get< std::size_t >();
}

u1 ResponseMessage::success( void ) const
{
    return operator[]( Identifier::success ).get< u1 >();
}

std::string ResponseMessage::command( void ) const
{
    return operator[]( Identifier::command ).get< std::string >();
}

u1 ResponseMessage::hasMessage( void ) const
{
    return find( Identifier::message ) != end();
}

std::string ResponseMessage::message( void ) const
{
    return at( Identifier::message );
}

void ResponseMessage::setMessage( const std::string& message )
{
    operator[]( Identifier::message ) = message;
}

u1 ResponseMessage::hasBody( void ) const
{
    return find( Identifier::body ) != end();
}

Content ResponseMessage::body( void ) const
{
    return at( Identifier::body );
}

void ResponseMessage::setBody( const Content& body )
{
    operator[]( Identifier::body ) = Content::from_cbor( Content::to_cbor( body ) );
}

void ResponseMessage::process( ServerInterface& interface ) const
{
    // auto response = ResponseMessage();

    const auto& cmd = command();
    const auto cmdValue = String::value( cmd );

    try
    {
        switch( cmdValue )
        {
            default:
            {
                // response.setError(
                //     ErrorCode::MethodNotFound,
                //     "event method '" + m + "' not specified in interface implementation" );
                break;
            }
        };
    }
    catch( const std::invalid_argument& e )
    {
        // response.setError(
        //     ErrorCode::ParseError,
        //     "event method '" + m + "': parse error: '" + std::string( e.what() ) + "'" );
    }

    // if( response.hasError() )
    // {
    //     interface.respond( response );
    // }
}

void ResponseMessage::validate( const Content& data )
{
    static const auto context = "DAP: ReponseMessage:";
    Message::validate( data );
    Json::validatePropertyIsNumber( context, data, Identifier::request_seq, true );
    Json::validatePropertyIsBoolean( context, data, Identifier::success, true );
    Json::validatePropertyIsString( context, data, Identifier::command, true );
    Json::validatePropertyIsString( context, data, Identifier::message, false );
    Json::validatePropertyIsObject( context, data, Identifier::body, false );
}

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
