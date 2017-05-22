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
using namespace UDP;

IPv4PosixSocket::IPv4PosixSocket(
    const IPv4::Address& address, const Port& port )
: PosixSocket< Network::Packet >( "IPv4", AF_INET, SOCK_DGRAM, IPPROTO_UDP )
, m_address( address )
, m_port( port )
{
}

IPv4PosixSocket::IPv4PosixSocket( const std::string& name )
: PosixSocket< Network::Packet >( name, AF_INET, SOCK_DGRAM, IPPROTO_UDP )
, m_address( { { 0 } } )
, m_port( { { 0 } } )
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

    if(::bind( id(), (struct sockaddr*)&configuration, sizeof( configuration ) )
        < 0 )
    {
        throw std::domain_error(
            "unable to bind UDP address '" + name() + "'" );
    }

    setConnected( true );
}

void IPv4PosixSocket::send( const Network::Packet& data ) const
{
    if( not connected() )
    {
        throw std::logic_error(
            "unable to send, socket '" + name() + "' is not connected" );
    }

    const auto result
        = ::send( id(), data.buffer(), data.size(), MSG_DONTWAIT );

    if( result < 0 )
    {
        throw std::domain_error(
            "unable to send, failed with '" + std::to_string( result ) );
    }
}

void IPv4PosixSocket::receive( Network::Packet& data ) const
{
    if( not connected() )
    {
        throw std::logic_error(
            "unable to receive, socket '" + name() + "' is not connected" );
    }

    const auto result
        = ::recv( id(), (void*)( data.buffer() ), data.size(), MSG_DONTWAIT );

    if( result < 0 )
    {
        throw std::domain_error(
            "unable to receive, failed with '" + std::to_string( result ) );
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
