//
//  Copyright (C) 2014-2020 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
//                Christian Lascsak
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

#include <cassert>
#include <cstring>

#if defined( __linux__ )
#include <net/if.h>
#include <netpacket/packet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif

using namespace libstdhl;
using namespace Network;

Ethernet::Socket::Socket( const std::string& name )
: Network::Socket< Frame >( name )
, m_address( { { 0 } } )
, m_ifridx( 0 )
{
}

const Ethernet::Address& Ethernet::Socket::address( void ) const
{
    return m_address;
}

void Ethernet::Socket::connect( void )
{
#if defined( __linux__ )
    const i32 fd = socket( AF_PACKET, SOCK_RAW, 0 );

    if( fd <= 0 )
    {
        throw std::domain_error( "unable to open socket '" + name() + "'" );
    }

    struct ifreq info = { { { 0 } } };

    strncpy( info.ifr_name, name().c_str(), IFNAMSIZ - 1 );
    if( ioctl( fd, SIOCGIFINDEX, &info ) < 0 )
    {
        throw std::domain_error( "unable to connect to socket '" + name() + "'" );
    }

    m_ifridx = info.ifr_ifindex;
    if( ioctl( fd, SIOCGIFHWADDR, &info ) < 0 )
    {
        throw std::domain_error( "unable to read socket '" + name() + "' hardware address" );
    }

    m_address = { {
        static_cast< u8 >( info.ifr_hwaddr.sa_data[ 0 ] ),
        static_cast< u8 >( info.ifr_hwaddr.sa_data[ 1 ] ),
        static_cast< u8 >( info.ifr_hwaddr.sa_data[ 2 ] ),
        static_cast< u8 >( info.ifr_hwaddr.sa_data[ 3 ] ),
        static_cast< u8 >( info.ifr_hwaddr.sa_data[ 4 ] ),
        static_cast< u8 >( info.ifr_hwaddr.sa_data[ 5 ] ),
    } };

    setId( fd );
#endif
}

void Ethernet::Socket::disconnect( void )
{
#if defined( __linux__ )
    if( close( id() ) )
    {
        throw std::domain_error( "unable to close socket '" + name() + "'" );
    }

    setId( 0 );
#endif
}

std::size_t Ethernet::Socket::send( const Frame& data ) const
{
    if( not connected() )
    {
        throw std::logic_error( "unable to send, socket '" + name() + "'is not connected" );
    }

#if defined( __linux__ )
    const auto& destination = data.header().destination();

    struct sockaddr_ll configuration = { 0 };

    configuration.sll_ifindex = m_ifridx;
    configuration.sll_halen = 6;
    configuration.sll_addr[ 0 ] = destination[ 0 ];
    configuration.sll_addr[ 1 ] = destination[ 1 ];
    configuration.sll_addr[ 2 ] = destination[ 2 ];
    configuration.sll_addr[ 3 ] = destination[ 3 ];
    configuration.sll_addr[ 4 ] = destination[ 4 ];
    configuration.sll_addr[ 5 ] = destination[ 5 ];

    auto result = sendto(
        id(),
        data.buffer(),
        data.size(),
        0,
        (struct sockaddr*)&configuration,
        sizeof( struct sockaddr_ll ) );

    if( result < 0 )
    {
        throw std::domain_error( "unable to send, failed with '" + std::to_string( result ) );
    }

    return result;
#else
    return -1;
#endif
}

std::size_t Ethernet::Socket::receive( Frame& data ) const
{
    // recvfrom
    assert( !" TODO! " );
    return 0;
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
