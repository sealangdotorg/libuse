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

#include "Socket.h"

using namespace libstdhl;
using namespace Network;
using namespace TCP;

IPv4PosixSocket::IPv4PosixSocket(
    const IPv4::Address& address, const Port& port )
: PosixSocket< IPv4Packet >(
      "IPv4", PF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP )
, m_address( address )
, m_port( port )
{
}

IPv4PosixSocket::IPv4PosixSocket( const std::string& name )
: PosixSocket< IPv4Packet >(
      name, PF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP )
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

IPv4PosixSocket::IPv4PosixSocket( const PosixSocket< IPv4Packet >& socket )
: PosixSocket< IPv4Packet >( socket )
, m_address( { { 0 } } )
, m_port( { { 0 } } )
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

void IPv4PosixSocket::send( const IPv4Packet& data ) const
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

void IPv4PosixSocket::receive( IPv4Packet& data ) const
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

    i32 connection = ::accept4(
        id(), (struct sockaddr*)&configuration, &len, SOCK_NONBLOCK );

    return IPv4PosixSocket( PosixSocket< IPv4Packet >( *this, connection ) );
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
