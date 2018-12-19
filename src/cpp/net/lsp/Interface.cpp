//
//  Copyright (C) 2014-2019 CASM Organization <https://casm-lang.org>
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

#include "Interface.h"

#include "Identifier.h"

using namespace libstdhl;
using namespace Network;
using namespace LSP;

ServerInterface::ServerInterface( void )
: m_responseBuffer()
, m_responseBufferSlot( 0 )
, m_responseBufferLock()
, m_notificationBuffer()
, m_notificationBufferSlot( 0 )
, m_notificationBufferLock()
, m_serverFlushLock()
, m_requestBuffer()
, m_requestBufferSlot( 0 )
, m_requestBufferLock()
, m_requestCallback()
{
}

void ServerInterface::respond( const ResponseMessage& message )
{
    std::lock_guard< std::mutex > guard( m_responseBufferLock );
    m_responseBuffer[ m_responseBufferSlot ].emplace_back( message );
}

void ServerInterface::notify( const NotificationMessage& message )
{
    std::lock_guard< std::mutex > guard( m_notificationBufferLock );
    m_notificationBuffer[ m_notificationBufferSlot ].emplace_back( message );
}

void ServerInterface::request(
    const RequestMessage& message, const std::function< void( const ResponseMessage& ) >& callback )
{
    std::lock_guard< std::mutex > guard( m_requestBufferLock );
    m_requestBuffer[ m_requestBufferSlot ].emplace_back( message );

    // TODO: @ppaulweber: check for unique value (msg id) condition @Clasc
    m_requestCallback[ message.id() ] = callback;
}

void ServerInterface::handle( const ResponseMessage& message )
{
    const auto result = m_requestCallback.find( message.id() );
    if( result != m_requestCallback.end() )
    {
        const std::function< void( const ResponseMessage& ) >& callback = result->second;
        callback( message );
    }
}

void ServerInterface::flush( const std::function< void( const Message& ) >& callback )
{
    std::lock_guard< std::mutex > guard( m_serverFlushLock );

    std::size_t pos = -1;

    {
        std::lock_guard< std::mutex > guard( m_responseBufferLock );
        pos = m_responseBufferSlot;
        m_responseBufferSlot = ( pos + 1 ) % 2;
    }

    for( auto msg : m_responseBuffer[ pos ] )
    {
        callback( msg );
    }

    m_responseBuffer[ pos ].clear();

    {
        std::lock_guard< std::mutex > guard( m_notificationBufferLock );
        pos = m_notificationBufferSlot;
        m_notificationBufferSlot = ( pos + 1 ) % 2;
    }

    for( auto msg : m_notificationBuffer[ pos ] )
    {
        callback( msg );
    }

    m_notificationBuffer[ pos ].clear();

    {
        std::lock_guard< std::mutex > guard( m_requestBufferLock );
        pos = m_requestBufferSlot;
        m_requestBufferSlot = ( pos + 1 ) % 2;
    }

    for( auto msg : m_requestBuffer[ pos ] )
    {
        callback( msg );
    }

    m_requestBuffer[ pos ].clear();
}
void ServerInterface::server_cancel( const CancelParams& params ) noexcept
{
    NotificationMessage msg( std::string{ Identifier::cancelRequest } );
    msg.setParams( params );
    notify( msg );
}

void ServerInterface::window_showMessage( const ShowMessageParams& params ) noexcept
{
    NotificationMessage msg( std::string{ Identifier::window_showMessage } );
    msg.setParams( params );
    notify( msg );
}

void ServerInterface::window_showMessageRequest(
    const ShowMessageRequestParams& params,
    const std::function< void( const ShowMessageRequestResult& ) >& callback )
{
    RequestMessage msg( request_id++, std::string{ Identifier::window_showMessageRequest } );
    msg.setParams( params );

    const auto responseCallback = [&]( const ResponseMessage& response ) {
        auto result = static_cast< const ShowMessageRequestResult >( response.result() );
        callback( result );
        // TODO: @ppaulweber: error handling has to be defined
    };

    request( msg, responseCallback );
}

void ServerInterface::window_logMessage( const LogMessageParams& params ) noexcept
{
    NotificationMessage msg( std::string{ Identifier::window_logMessage } );
    msg.setParams( params );
    notify( msg );
}

void ServerInterface::telemetry_event( const TelemetryEventParams& params ) noexcept
{
    NotificationMessage msg( std::string{ Identifier::telemetry_event } );
    msg.setParams( params );
    notify( msg );
}

void ServerInterface::client_registerCapability(
    const RegistrationParams& params, const std::function< void( void ) >& callback )
{
    RequestMessage msg( request_id++, std::string{ Identifier::client_registerCapability } );
    msg.setParams( params );

    const auto responseCallback = [&]( const ResponseMessage& response ) {
        callback();
        // TODO: @ppaulweber: error handling has to be defined
    };
    request( msg, responseCallback );
}

void ServerInterface::client_unregisterCapability(
    const UnregistrationParams& params, const std::function< void( void ) >& callback )
{
    RequestMessage msg( request_id++, std::string{ Identifier::client_unregisterCapability } );
    msg.setParams( params );
    const auto responseCallback = [&]( const ResponseMessage& response ) {
        callback();
        // TODO: @ppaulweber: error handling has to be defined
    };
    request( msg, responseCallback );
}

void ServerInterface::workspace_workspaceFolders(
    const std::function< void( WorkspaceFoldersResult ) >& callback )
{
    RequestMessage msg( request_id++, std::string{ Identifier::workspace_workspaceFolders } );
    msg.setParams( Data() );
    const auto responseCallback = [&]( const ResponseMessage& response ) {
        auto result = static_cast< const WorkspaceFoldersResult >( response.result() );
        callback( result );
        // TODO: @ppaulweber: error handling has to be defined
    };
    request( msg, responseCallback );
}

Data ServerInterface::workspace_configuration( const ConfigurationParams& params )
{
    RequestMessage msg( 0 /* TODO */, std::string{ Identifier::workspace_configuration } );
    // request( msg );   // TODO: FIXME: @Clasc
    // TODO: FIXME: @Clasc: handle response
    return Data( Data::object() );
}

ApplyWorkspaceEditResult ServerInterface::workspace_applyEdit(
    const ApplyWorkspaceEditParams& params )
{
    RequestMessage msg( 0 /* TODO */, std::string{ Identifier::workspace_applyEdit } );
    // request( msg );   // TODO: FIXME: @Clasc
    // TODO: FIXME: @Clasc: handle response
    return ApplyWorkspaceEditResult( true );
}

void ServerInterface::textDocument_publishDiagnostics(
    const PublishDiagnosticsParams& params ) noexcept
{
    NotificationMessage msg( std::string{ Identifier::textDocument_publishDiagnostics } );
    msg.setParams( params );
    notify( msg );
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
