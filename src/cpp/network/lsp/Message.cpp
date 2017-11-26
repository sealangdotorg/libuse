//
//  Copyright (c) 2014-2017 CASM Organization https://casm-lang.org
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
//                https://github.com/casm-lang/libstdhl
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
    operator[]( Identifier::jsonrpc ) = Identifier::jsonrpc_version;
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

    const auto& jsonrpc = at( Identifier::jsonrpc ).get< std::string >();
    if( jsonrpc.compare( Identifier::jsonrpc_version ) != 0 )
    {
        throw std::invalid_argument(
            "invalid data found, unsupported JSON-RPC version '" + jsonrpc + "', only version '" +
            Identifier::jsonrpc_version + "' is supported" );
    }

    if( find( Identifier::id ) != end() )
    {
        // can be a request or a response message
        if( find( Identifier::method ) != end() )
        {
            m_id = ID::REQUEST_MESSAGE;
            if( not RequestMessage::isValid( data ) )
            {
                throw std::invalid_argument( "invalid data provided for 'RequestMessage'" );
            }
        }
        else
        {
            m_id = ID::RESPONSE_MESSAGE;
            if( not ResponseMessage::isValid( data ) )
            {
                throw std::invalid_argument( "invalid data provided for 'ResponseMessage'" );
            }
        }
    }
    else
    {
        // can be a notification message
        if( find( Identifier::method ) != end() )
        {
            m_id = ID::NOTIFICATION_MESSAGE;
            if( not NotificationMessage::isValid( data ) )
            {
                throw std::invalid_argument( "invalid data provided for 'NotificationMessage'" );
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

u1 Message::isValid( const Data& data )
{
    if( data.is_object() and data.find( Identifier::jsonrpc ) != data.end() and
        data[ Identifier::jsonrpc ].is_string() )
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

u1 RequestMessage::isValid( const Data& data )
{
    if( data.find( Identifier::id ) != data.end() and
        ( data[ Identifier::id ].is_string() or data[ Identifier::id ].is_number() ) and
        data.find( Identifier::method ) != data.end() and data[ Identifier::method ].is_string() )
    {
        if( data.find( Identifier::params ) != data.end() and
            not data[ Identifier::params ].is_object() )
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

u1 NotificationMessage::isValid( const Data& data )
{
    if( data.find( Identifier::method ) != data.end() and data[ Identifier::method ].is_string() )
    {
        if( data.find( Identifier::params ) != data.end() and
            not data[ Identifier::params ].is_object() )
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
    operator[]( Identifier::id ) = id;
}

ResponseMessage::ResponseMessage( const std::string id )
: Message( ID::RESPONSE_MESSAGE )
{
    operator[]( Identifier::id ) = id;
}

ResponseMessage::ResponseMessage( void )
: Message( ID::RESPONSE_MESSAGE )
{
    operator[]( Identifier::id ) = nullptr;
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
    assert( !" TODO! " );
}

u1 ResponseMessage::isValid( const Data& data )
{
    if( data.find( Identifier::id ) != data.end() and
        ( data[ Identifier::id ].is_string() or data[ Identifier::id ].is_number() ) )
    {
        if( data.find( Identifier::result ) != data.end() and
            not data[ Identifier::result ].is_object() )
        {
            return false;
        }

        if( data.find( Identifier::error ) != data.end() and
            not ResponseError::isValid( data[ Identifier::error ] ) )
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
