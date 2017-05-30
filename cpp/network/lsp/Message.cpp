//
//  Copyright (c) 2014-2017 Philipp Paulweber
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
//                https://github.com/ppaulweber/libstdhl
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

#include "Message.h"

#include "Content.h"
#include "Exception.h"
#include "Identifier.h"
#include "Interface.h"

#include "../../String.h"

/**
   @brief    TBD

   TBD
*/

using namespace libstdhl;
using namespace Network;
using namespace LSP;

//
//
// Message
//

Message::Message( const ID id )
: Data( Data::object() )
, m_id( id )
{
    operator[]( JSONRPC ) = JSONRPC_VERSION;
}

Message::Message( const Data& data )
: Data( data )
, m_id( ID::UNKNOWN )
{
    if( not Message::isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data found, does not contain JSON-RPC field of type "
            "'string'" );
    }

    const auto& jsonrpc = operator[]( JSONRPC ).get< std::string >();
    if( jsonrpc.compare( JSONRPC_VERSION ) != 0 )
    {
        throw std::invalid_argument(
            "invalid data found, unsupported JSON-RPC version '" + jsonrpc
            + "', only version '"
            + JSONRPC_VERSION
            + "' is supported" );
    }

    if( find( LSP::ID ) != end() )
    {
        // can be a request or a response message
        if( find( METHOD ) != end() )
        {
            m_id = ID::REQUEST_MESSAGE;
            if( not RequestMessage::isValid( data ) )
            {
                throw std::invalid_argument(
                    "invalid data provided for 'RequestMessage'" );
            }
        }
        else
        {
            m_id = ID::RESPONSE_MESSAGE;
            if( not ResponseMessage::isValid( data ) )
            {
                throw std::invalid_argument(
                    "invalid data provided for 'ResponseMessage'" );
            }
        }
    }
    else
    {
        // can be a notification message
        if( find( METHOD ) != end() )
        {
            m_id = ID::NOTIFICATION_MESSAGE;
            if( not NotificationMessage::isValid( data ) )
            {
                throw std::invalid_argument(
                    "invalid data provided for 'NotificationMessage'" );
            }
        }
    }

    if( m_id == ID::UNKNOWN )
    {
        throw std::invalid_argument( "invalid data provided" );
    }
}

Message::ID Message::id( void ) const
{
    return m_id;
}

const Data& Message::obj( void ) const
{
    return static_cast< const Data& >( *this );
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
        case ID::NOTIFICATION_MESSAGE:
        {
            const auto msg
                = reinterpret_cast< const NotificationMessage& >( *this );
            msg.process( interface );
            break;
        }
        case ID::RESPONSE_MESSAGE:
        {
            const auto msg
                = reinterpret_cast< const ResponseMessage& >( *this );
            msg.process( interface );
            break;
        }
        case ID::UNKNOWN: // [[fallthrough]]
        case ID::_SIZE_:
        {
            auto msg = ResponseMessage();
            msg.setError( ErrorCode::InternalError, "invalid message" );
            interface.respond( msg );
            break;
        }
    };
}

u1 Message::isValid( const Data& data )
{
    if( data.is_object() and data.find( JSONRPC ) != data.end()
        and data[ JSONRPC ].is_string() )
    {
        return true;
    }
    else
    {
        return false;
    }
}

//
//
// Request Message
//

RequestMessage::RequestMessage( const Data& data )
: Message( data )
{
}

RequestMessage::RequestMessage(
    const std::size_t id, const std::string& method )
: Message( ID::REQUEST_MESSAGE )
{
    operator[]( LSP::ID ) = id;
    operator[]( METHOD ) = method;
    operator[]( PARAMS ) = Data::object();
}

RequestMessage::RequestMessage(
    const std::string& id, const std::string& method )
: Message( ID::REQUEST_MESSAGE )
{
    operator[]( LSP::ID ) = id;
    operator[]( METHOD ) = method;
    operator[]( PARAMS ) = Data::object();
}

std::string RequestMessage::id( void ) const
{
    const auto& data_id = operator[]( LSP::ID );
    if( data_id.is_number() )
    {
        return std::to_string( data_id.get< std::size_t >() );
    }
    else
    {
        return data_id.get< std::string >();
    }
}

std::string RequestMessage::method( void ) const
{
    return operator[]( METHOD ).get< std::string >();
}

u1 RequestMessage::hasParams( void ) const
{
    return find( PARAMS ) != end();
}

Data RequestMessage::params( void ) const
{
    return operator[]( PARAMS );
}

void RequestMessage::setParams( const Data& data )
{
    operator[]( PARAMS ) = data;
}

void RequestMessage::process( ServerInterface& interface ) const
{
    auto response = ResponseMessage( id() );

    const auto& m = method();
    const auto mv = String::value( m );

    try
    {
        switch( mv )
        {
            // default
            case String::value( INITIALIZE ):
            {
                const auto& parameters = InitializeParams( params() );
                const auto& result = interface.initialize( parameters );
                response.setResult( result );
                break;
            }
            case String::value( SHUTDOWN ):
            {
                interface.shutdown();
                response.setResult( nullptr );
                break;
            }
            // document
            case String::value( Identifier::textDocument_codeAction ):
            {
                const auto& parameters = CodeActionParams( params() );
                const auto& result
                    = interface.textDocument_codeAction( parameters );
                response.setResult( result );
                break;
            }
            default:
            {
                response.setError( ErrorCode::MethodNotFound,
                    "request method '" + m
                        + "' not specified in interface implementation" );
                break;
            }
        };
    }
    catch( const Exception& e )
    {
        response.setError( e.code(), e.message(), e.data() );
    }
    catch( const std::invalid_argument& e )
    {
        response.setError( ErrorCode::ParseError,
            "parse error: '" + std::string( e.what() ) + "'" );
    }

    interface.respond( response );
}

u1 RequestMessage::isValid( const Data& data )
{
    if( data.find( LSP::ID ) != data.end()
        and ( data[ LSP::ID ].is_string() or data[ LSP::ID ].is_number() )
        and data.find( METHOD ) != data.end()
        and data[ METHOD ].is_string() )
    {
        if( data.find( PARAMS ) != data.end()
            and not data[ PARAMS ].is_object() )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// Notification Message
//

NotificationMessage::NotificationMessage( const Data& data )
: Message( data )
{
}

NotificationMessage::NotificationMessage( const std::string& method )
: Message( ID::NOTIFICATION_MESSAGE )
{
    operator[]( METHOD ) = method;
    operator[]( PARAMS ) = Data::object();
}

std::string NotificationMessage::method( void ) const
{
    return operator[]( METHOD ).get< std::string >();
}

u1 NotificationMessage::hasParams( void ) const
{
    return find( PARAMS ) != end();
}

Data NotificationMessage::params( void ) const
{
    return operator[]( PARAMS );
}

void NotificationMessage::setParams( const Data& data )
{
    operator[]( PARAMS ) = data;
}

void NotificationMessage::process( ServerInterface& interface ) const
{
    auto response = ResponseMessage();

    const auto& m = method();
    const auto mv = String::value( m );

    try
    {
        switch( mv )
        {
            // general
            case String::value( INITIALIZED ):
            {
                interface.initialized();
                break;
            }
            case String::value( EXIT ):
            {
                interface.exit();
                break;
            }
            // document
            case String::value( Identifier::textDocument_didOpen ):
            {
                const auto& parameters = DidOpenTextDocumentParams( params() );
                interface.textDocument_didOpen( parameters );
                break;
            }
            case String::value( Identifier::textDocument_didChange ):
            {
                const auto& parameters
                    = DidChangeTextDocumentParams( params() );
                interface.textDocument_didChange( parameters );
                break;
            }
            default:
            {
                response.setError( ErrorCode::MethodNotFound,
                    "notification method '" + m
                        + "' not specified in interface implementation" );
                break;
            }
        };
    }
    catch( const std::invalid_argument& e )
    {
        response.setError( ErrorCode::ParseError,
            "notification method '" + m + "': parse error: '"
                + std::string( e.what() )
                + "'" );
    }

    if( response.hasError() )
    {
        interface.respond( response );
    }
}

u1 NotificationMessage::isValid( const Data& data )
{
    if( data.find( METHOD ) != data.end() and data[ METHOD ].is_string() )
    {
        if( data.find( PARAMS ) != data.end()
            and not data[ PARAMS ].is_object() )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// Response Message
//

ResponseMessage::ResponseMessage( const Data& data )
: Message( data )
{
}

ResponseMessage::ResponseMessage( const std::size_t id )
: Message( ID::RESPONSE_MESSAGE )
{
    operator[]( LSP::ID ) = id;
}

ResponseMessage::ResponseMessage( const std::string id )
: Message( ID::RESPONSE_MESSAGE )
{
    operator[]( LSP::ID ) = id;
}

ResponseMessage::ResponseMessage( void )
: Message( ID::RESPONSE_MESSAGE )
{
    operator[]( LSP::ID ) = nullptr;
}

u1 ResponseMessage::hasResult( void ) const
{
    return find( RESULT ) != end();
}

Data ResponseMessage::result( void ) const
{
    if( not hasResult() )
    {
        throw std::domain_error(
            "response message has no response 'result' field" );
    }

    return operator[]( RESULT );
}

void ResponseMessage::setResult( const Data& result )
{
    operator[]( RESULT ) = result;
}

u1 ResponseMessage::hasError( void ) const
{
    return find( ERROR ) != end();
}

ResponseError ResponseMessage::error( void ) const
{
    if( not hasError() )
    {
        throw std::domain_error(
            "response message has no response 'error' field" );
    }

    return ResponseError{ operator[]( ERROR ) };
}

void ResponseMessage::setError( const ResponseError& error )
{
    operator[]( ERROR ) = error;
}

void ResponseMessage::setError( const ErrorCode code, const std::string& name )
{
    setError( ResponseError( code, name ) );
}

void ResponseMessage::setError(
    const ErrorCode code, const std::string& name, const Data& data )
{
    auto error = ResponseError( code, name );
    error.setData( data );
    setError( error );
}

void ResponseMessage::process( ServerInterface& interface ) const
{
    assert( !" TODO! " );
}

u1 ResponseMessage::isValid( const Data& data )
{
    if( data.find( LSP::ID ) != data.end()
        and ( data[ LSP::ID ].is_string() or data[ LSP::ID ].is_number() ) )
    {
        if( data.find( RESULT ) != data.end()
            and not data[ RESULT ].is_object() )
        {
            return false;
        }

        if( data.find( ERROR ) != data.end()
            and not ResponseError::isValid( data[ ERROR ] ) )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
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
