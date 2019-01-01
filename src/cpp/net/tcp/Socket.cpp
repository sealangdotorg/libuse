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

#include "Socket.h"

#include <libstdhl/String>

#if defined( __WIN32__ ) or defined( __WIN32 ) or defined( _WIN32 )
#include <WS2tcpip.h>
#include <winsock2.h>
#define close closesocket
using bufferType = char*;
#else
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
using bufferType = void*;
#endif

using namespace libstdhl;
using namespace Network;
using namespace TCP;

IPv4Socket::IPv4Socket( const IPv4::Address& address, const Port& port )
: Socket< IPv4Packet >( "IPv4" )
, m_address( address )
, m_port( port )
{
}

IPv4Socket::IPv4Socket( const std::string& name )
: Socket< IPv4Packet >( name )
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
        (u8)std::stoi( address[ 0 ] ),
        (u8)std::stoi( address[ 1 ] ),
        (u8)std::stoi( address[ 2 ] ),
        (u8)std::stoi( addrPort[ 0 ] ),
    } };

    u16 port = std::stoi( addrPort[ 1 ] );

    m_port = { { ( u8 )( port >> 8 ), (u8)port } };
}

IPv4Socket::IPv4Socket( const IPv4Socket& socket, i32 connection )
: Socket< IPv4Packet >( socket )
, m_address( { { 0 } } )
, m_port( { { 0 } } )
{
    setId( connection );
}

void IPv4Socket::connect( void )
{
    const i32 fd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    if( fd <= 0 )
    {
        throw std::domain_error( "unable to open socket '" + name() + "'" );
    }

    struct sockaddr_in configuration = { 0 };

    configuration.sin_family = AF_INET;

    configuration.sin_addr.s_addr = ( (u32)m_address[ 3 ] << 24 ) | ( (u32)m_address[ 2 ] << 16 ) |
                                    ( (u32)m_address[ 1 ] << 8 ) | (u32)m_address[ 0 ];

    configuration.sin_port = ( (u16)m_port[ 1 ] << 8 ) | (u16)m_port[ 0 ];

    if( not server() )
    {
        if( ::connect( id(), (struct sockaddr*)&configuration, sizeof( configuration ) ) < 0 )
        {
            throw std::domain_error( "unable to connect to TCP address '" + name() + "'" );
        }
    }
    else
    {
        if( ::bind( id(), (struct sockaddr*)&configuration, sizeof( configuration ) ) < 0 )
        {
            throw std::domain_error( "unable to bind to TCP address '" + name() + "'" );
        }
        ::listen( id(),
                  5 );  // TODO: PPA: listening queue limit as configurable parameter
    }

    setId( fd );
}

void IPv4Socket::disconnect( void )
{
    if( close( id() ) )
    {
        throw std::domain_error( "unable to close socket '" + name() + "'" );
    }

    setId( 0 );
}

std::size_t IPv4Socket::send( const IPv4Packet& data ) const
{
    if( not connected() )
    {
        throw std::domain_error( "unable to send, not connected" );
    }

    const auto result = ::send( id(), (bufferType)data.buffer(), data.size(), 0 );

    if( result < 0 )
    {
        throw std::domain_error( "unable to send, failed with '" + std::to_string( result ) );
    }

    return result;
}

std::size_t IPv4Socket::receive( IPv4Packet& data ) const
{
    if( not connected() )
    {
        throw std::domain_error( "unable to receive, not connected" );
    }

    const auto result = ::recv( id(), (bufferType)data.buffer(), data.size(), 0 );

    if( result < 0 )
    {
        throw std::domain_error(
            "unable to receive, failed with '" + std::to_string( result ) + "'" );
    }

    if( result >= data.size() )
    {
        throw std::domain_error( "received to many bytes '" + std::to_string( result ) + "'" );
    }

    ( (u8*)data.buffer() )[ result ] = '\0';
    return result;
}

const Address& IPv4Socket::address( void ) const
{
    return m_address;
}

const Port& IPv4Socket::port( void ) const
{
    return m_port;
}

/**
   blocking call!
 */

IPv4Socket IPv4Socket::accept( void ) const
{
    if( not connected() )
    {
        throw std::domain_error(
            "cannot accept new connection, because socket is not connected to "
            "TCP address '" +
            name() + "'" );
    }

    if( not server() )
    {
        throw std::domain_error(
            "client cannot use accept functionality for TCP address '" + name() + "'" );
    }

    struct sockaddr_in configuration = { 0 };
    socklen_t len = sizeof( configuration );
    i32 connection = ::accept( id(), (struct sockaddr*)&configuration, &len );
    // return IPv4Socket( Socket< IPv4Packet >( *this, connection ) );
    return IPv4Socket( *this, connection );
}

void IPv4Socket::setServer( const u1 enable )
{
    m_server = true;
}

u1 IPv4Socket::server( void ) const
{
    return m_server;
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
