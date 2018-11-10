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

#include "../../String.h"

/**
   @brief    TBD

   TBD
x*/

using namespace libstdhl;
using namespace Network;
using namespace LSP;

//
//
// Message
//

Message::Message( const ID id, const Data& data )
: Data( data )
, m_id( id )
{
    assert( id != ID::UNKNOWN );
    operator[]( Identifier::jsonrpc ) = Identifier::jsonrpc_version;
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
            const auto msg = reinterpret_cast< const NotificationMessage& >( *this );
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
            auto msg = ResponseMessage();
            msg.setError( ErrorCode::InternalError, "invalid message" );
            interface.respond( msg );
            break;
        }
    };
}

void Message::validate( const Data& data )
{
    static const auto context = "LSP: Message:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::jsonrpc, true );
}

Message Message::parse( const std::string& data )
{
    ID id = ID::UNKNOWN;
    Data payload;

    // parse JSON data from string
    payload = Json::Object::parse( data );
    Message::validate( payload );

    const auto& jsonrpc = payload.at( Identifier::jsonrpc ).get< std::string >();
    if( jsonrpc.compare( Identifier::jsonrpc_version ) != 0 )
    {
        throw std::invalid_argument(
            "LSP: unsupported version '" + jsonrpc + "', only version '" +
            Identifier::jsonrpc_version + "' is supported" );
    }

    if( payload.find( Identifier::id ) != payload.end() )
    {
        if( payload.find( Identifier::method ) != payload.end() )
        {
            // contains 'id' and 'method'
            // ==> request message
            id = ID::REQUEST_MESSAGE;
        }
        else
        {
            // contains 'id' and NOT 'method'
            // ==> response message
            id = ID::RESPONSE_MESSAGE;
        }
    }
    else
    {
        if( payload.find( Identifier::method ) != payload.end() )
        {
            // contains NOT 'id' and 'method'
            // ==> notification message
            id = ID::NOTIFICATION_MESSAGE;
        }
        else
        {
            // contains NOT 'id' and NOT 'method'
            // ==> invalid message, cannot be parsed
            id = ID::UNKNOWN;
        }
    }

    switch( id )
    {
        case ID::REQUEST_MESSAGE:
        {
            RequestMessage::validate( payload );
            return RequestMessage( payload );
        }
        case ID::RESPONSE_MESSAGE:
        {
            ResponseMessage::validate( payload );
            return ResponseMessage( data );
        }
        case ID::NOTIFICATION_MESSAGE:
        {
            NotificationMessage::validate( payload );
            return NotificationMessage( payload );
        }
        case ID::UNKNOWN:  // [[fallthrough]]
        case ID::_SIZE_:
        {
            break;
        }
    }
    throw std::invalid_argument( "LSP: unknown JSON-RPC message payload" );
    return Message();
}

//
//
// Request Message
//

RequestMessage::RequestMessage( const Data& data )
: Message( ID::REQUEST_MESSAGE, data )
{
}

RequestMessage::RequestMessage( const std::size_t id, const std::string& method )
: Message( ID::REQUEST_MESSAGE )
{
    operator[]( Identifier::id ) = id;
    operator[]( Identifier::method ) = method;
    operator[]( Identifier::params ) = Data::object();
}

RequestMessage::RequestMessage( const std::string& id, const std::string& method )
: Message( ID::REQUEST_MESSAGE )
{
    operator[]( Identifier::id ) = id;
    operator[]( Identifier::method ) = method;
    operator[]( Identifier::params ) = Data::object();
}

std::string RequestMessage::id( void ) const
{
    const auto& data_id = operator[]( Identifier::id );
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
    return at( Identifier::method ).get< std::string >();
}

u1 RequestMessage::hasParams( void ) const
{
    return find( Identifier::params ) != end();
}

Data RequestMessage::params( void ) const
{
    return at( Identifier::params );
}

void RequestMessage::setParams( const Data& data )
{
    operator[]( Identifier::params ) = Data::from_cbor( Data::to_cbor( data ) );
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
            case String::value( Identifier::initialize ):
            {
                const auto& parameters = InitializeParams( params() );
                const auto& result = interface.initialize( parameters );
                response.setResult( result );
                break;
            }
            case String::value( Identifier::shutdown ):
            {
                interface.shutdown();
                response.setResult( nullptr );
                break;
            }
            // workspace
            case String::value( Identifier::workspace_executeCommand ):
            {
                const auto& parameters = ExecuteCommandParams( params() );
                const auto& result = interface.workspace_executeCommand( parameters );
                response.setResult( result );
                break;
            }
            // document
            case String::value( Identifier::textDocument_hover ):
            {
                const auto& parameters = HoverParams( params() );
                const auto& result = interface.textDocument_hover( parameters );
                response.setResult( result );
                break;
            }
            case String::value( Identifier::textDocument_codeAction ):
            {
                const auto& parameters = CodeActionParams( params() );
                const auto& result = interface.textDocument_codeAction( parameters );
                response.setResult( result );
                break;
            }
            case String::value( Identifier::textDocument_codeLens ):
            {
                const auto& parameters = CodeLensParams( params() );
                const auto& result = interface.textDocument_codeLens( parameters );
                response.setResult( result );
                break;
            }
            default:
            {
                response.setError(
                    ErrorCode::MethodNotFound,
                    "request method '" + m + "' not specified in interface implementation" );
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
        response.setError(
            ErrorCode::ParseError, "parse error: '" + std::string( e.what() ) + "'" );
    }

    interface.respond( response );
}

void RequestMessage::validate( const Data& data )
{
    static const auto context = "LSP: RequestMessage:";
    Message::validate( data );
    Json::validatePropertyIsUuid( context, data, Identifier::jsonrpc, true );
    Json::validatePropertyIsString( context, data, Identifier::method, true );
    Json::validatePropertyIsObject( context, data, Identifier::params, false );
}

//
//
// Notification Message
//

NotificationMessage::NotificationMessage( const Data& data )
: Message( ID::NOTIFICATION_MESSAGE, data )
{
}

NotificationMessage::NotificationMessage( const std::string& method )
: Message( ID::NOTIFICATION_MESSAGE )
{
    operator[]( Identifier::method ) = method;
    operator[]( Identifier::params ) = Data::object();
}

std::string NotificationMessage::method( void ) const
{
    return at( Identifier::method ).get< std::string >();
}

u1 NotificationMessage::hasParams( void ) const
{
    return find( Identifier::params ) != end();
}

Data NotificationMessage::params( void ) const
{
    return at( Identifier::params );
}

void NotificationMessage::setParams( const Data& data )
{
    operator[]( Identifier::params ) = Data::from_cbor( Data::to_cbor( data ) );
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
            case String::value( Identifier::initialized ):
            {
                interface.initialized();
                break;
            }
            case String::value( Identifier::exit ):
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
                const auto& parameters = DidChangeTextDocumentParams( params() );
                interface.textDocument_didChange( parameters );
                break;
            }
            default:
            {
                response.setError(
                    ErrorCode::MethodNotFound,
                    "notification method '" + m + "' not specified in interface implementation" );
                break;
            }
        };
    }
    catch( const std::invalid_argument& e )
    {
        response.setError(
            ErrorCode::ParseError,
            "notification method '" + m + "': parse error: '" + std::string( e.what() ) + "'" );
    }

    if( response.hasError() )
    {
        interface.respond( response );
    }
}

void NotificationMessage::validate( const Data& data )
{
    static const auto context = "LSP: NotificationMessage:";
    Message::validate( data );
    Json::validatePropertyIsString( context, data, Identifier::method, true );
    Json::validatePropertyIsObject( context, data, Identifier::params, false );
}

//
//
// Response Message
//

ResponseMessage::ResponseMessage( const Data& data )
: Message( ID::RESPONSE_MESSAGE, data )
{
}

ResponseMessage::ResponseMessage( const std::size_t id )
: Message( ID::RESPONSE_MESSAGE )
{
    operator[]( Identifier::id ) = std::to_string( id );
}

ResponseMessage::ResponseMessage( const std::string& id )
: Message( ID::RESPONSE_MESSAGE )
{
    operator[]( Identifier::id ) = id;
}

ResponseMessage::ResponseMessage( void )
: Message( ID::RESPONSE_MESSAGE )
{
    operator[]( Identifier::id ) = nullptr;
}

std::string ResponseMessage::id( void ) const
{
    return at( Identifier::id ).get< std::string >();
}

u1 ResponseMessage::hasResult( void ) const
{
    return find( Identifier::result ) != end();
}

Data ResponseMessage::result( void ) const
{
    return at( Identifier::result );
}

void ResponseMessage::setResult( const Data& result )
{
    operator[]( Identifier::result ) = Data::from_cbor( Data::to_cbor( result ) );
}

u1 ResponseMessage::hasError( void ) const
{
    return find( Identifier::error ) != end();
}

ResponseError ResponseMessage::error( void ) const
{
    return at( Identifier::error );
}

void ResponseMessage::setError( const ResponseError& error )
{
    operator[]( Identifier::error ) = Data::from_cbor( Data::to_cbor( error ) );
}

void ResponseMessage::setError( const ErrorCode code, const std::string& name )
{
    setError( ResponseError( code, name ) );
}

void ResponseMessage::setError( const ErrorCode code, const std::string& name, const Data& data )
{
    auto error = ResponseError( code, name );
    error.setData( data );
    setError( error );
}

void ResponseMessage::process( ServerInterface& interface ) const
{
    interface.handle( *this );
}

void ResponseMessage::validate( const Data& data )
{
    static const auto context = "LSP: ResponseMessage:";
    Message::validate( data );
    Json::validatePropertyIsUuid( context, data, Identifier::id, true );
    Json::validatePropertyIsObject( context, data, Identifier::result, false );
    Json::validatePropertyIsObject( context, data, Identifier::error, false );
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
