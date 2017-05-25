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

#include "Socket.h"

using namespace libstdhl;
using namespace Network;
using namespace TCP;

IPv4PosixSocket::IPv4PosixSocket(
    const IPv4::Address& address, const Port& port )
: PosixSocket< Network::Packet >( "IPv4", PF_INET, SOCK_STREAM, IPPROTO_TCP )
, m_address( address )
, m_port( port )
, m_server( false )
{
}

IPv4PosixSocket::IPv4PosixSocket( const std::string& name )
: PosixSocket< Network::Packet >( name, PF_INET, SOCK_STREAM, IPPROTO_TCP )
, m_address( { { 0 } } )
, m_port( { { 0 } } )
, m_server( false )
{
    const auto address = String::split( name, "." );

    if( address.size() != 4 )
    {
        throw std::invalid_argument( "invalid IPv4 address '" + name + "'" );
    }

    const auto addrPort = String::split( address[ 3 ], ":" );

    if( addrPort.size() != 2 )
    {
        throw std::invalid_argument( "invalid IPv4 port '" + name + "'" );
    }

    m_address = { {
        (u8)std::stoi( address[ 0 ] ), (u8)std::stoi( address[ 1 ] ),
        (u8)std::stoi( address[ 2 ] ), (u8)std::stoi( addrPort[ 0 ] ),
    } };

    u16 port = std::stoi( addrPort[ 1 ] );

    m_port = { { ( u8 )( port >> 8 ), (u8)port } };
}

IPv4PosixSocket::IPv4PosixSocket( const PosixSocket< Network::Packet >& socket )
: PosixSocket< Network::Packet >( socket )
, m_address( { { 0 } } )
, m_port( { { 0 } } )
, m_server( false )
{
}

void IPv4PosixSocket::connect( void )
{
    if( connected() )
    {
        return;
    }

    struct sockaddr_in configuration = { 0 };

    configuration.sin_family = AF_INET;

    configuration.sin_addr.s_addr
        = ( (u32)m_address[ 3 ] << 24 ) | ( (u32)m_address[ 2 ] << 16 )
          | ( (u32)m_address[ 1 ] << 8 ) | (u32)m_address[ 0 ];

    configuration.sin_port = ( (u16)m_port[ 1 ] << 8 ) | (u16)m_port[ 0 ];

    if( not server() )
    {
        if(::connect(
               id(), (struct sockaddr*)&configuration, sizeof( configuration ) )
            < 0 )
        {
            throw std::domain_error(
                "unable to connect to TCP address '" + name() + "'" );
        }
    }
    else
    {
        if(::bind(
               id(), (struct sockaddr*)&configuration, sizeof( configuration ) )
            < 0 )
        {
            throw std::domain_error(
                "unable to bind to TCP address '" + name() + "'" );
        }
        ::listen( id(),
            5 ); // TODO: PPA: listening queue limit as configurable parameter
    }

    setConnected( true );
}

void IPv4PosixSocket::send( const Network::Packet& data ) const
{
    if( not server() )
    {
        PosixSocket::send( data );
    }
    else
    {
        auto client = accept();
        client.send( data );
        client.disconnect();
    }
}

void IPv4PosixSocket::receive( Network::Packet& data ) const
{
    if( not server() )
    {
        PosixSocket::receive( data );
    }
    else
    {
        auto client = accept();
        client.receive( data );
        client.disconnect();
    }
}

void IPv4PosixSocket::setServer( const u1 enable )
{
    m_server = enable;
}

u1 IPv4PosixSocket::server( void ) const
{
    return m_server;
}

/**
   blocking call!
 */

IPv4PosixSocket IPv4PosixSocket::accept( void ) const
{
    if( not connected() )
    {
        throw std::domain_error(
            "cannot accept new connection, because socket is not connected to "
            "TCP address '"
            + name()
            + "'" );
    }

    if( not server() )
    {
        throw std::domain_error(
            "client cannot use accept functionality for TCP address '" + name()
            + "'" );
    }

    struct sockaddr_in configuration = { 0 };

    socklen_t len = sizeof( configuration );

    i32 connection = ::accept( id(), (struct sockaddr*)&configuration, &len );

    return IPv4PosixSocket(
        PosixSocket< Network::Packet >( *this, connection ) );
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
