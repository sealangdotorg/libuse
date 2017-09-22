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

#include <cassert>

using namespace libstdhl;
using namespace Network;
using namespace Ethernet;

RawPosixSocket::RawPosixSocket( const std::string& name )
: PosixSocket< Packet >( name, AF_PACKET, SOCK_RAW, 0 )
, m_address( { { 0 } } )
{
}

const Address& RawPosixSocket::address( void ) const
{
    return m_address;
}

void RawPosixSocket::connect( void )
{
    struct ifreq info = { { { 0 } } };

    strncpy( info.ifr_name, name().c_str(), IFNAMSIZ - 1 );

    if( ioctl( id(), SIOCGIFINDEX, &info ) < 0 )
    {
        throw std::domain_error(
            "unable to connect to socket '" + name() + "'" );
    }

    m_ifridx = info.ifr_ifindex;

    if( ioctl( id(), SIOCGIFHWADDR, &info ) < 0 )
    {
        throw std::domain_error(
            "unable to read socket '" + name() + "' hardware address" );
    }

    m_address = { {
        static_cast< u8 >( info.ifr_hwaddr.sa_data[ 0 ] ),
        static_cast< u8 >( info.ifr_hwaddr.sa_data[ 1 ] ),
        static_cast< u8 >( info.ifr_hwaddr.sa_data[ 2 ] ),
        static_cast< u8 >( info.ifr_hwaddr.sa_data[ 3 ] ),
        static_cast< u8 >( info.ifr_hwaddr.sa_data[ 4 ] ),
        static_cast< u8 >( info.ifr_hwaddr.sa_data[ 5 ] ),
    } };

    setConnected( true );
}

void RawPosixSocket::send( const Packet& data ) const
{
    if( not connected() )
    {
        throw std::logic_error(
            "unable to send, socket '" + name() + "'is not connected" );
    }

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

    auto result = sendto( id(),
        data.buffer(),
        data.size(),
        0,
        (struct sockaddr*)&configuration,
        sizeof( struct sockaddr_ll ) );

    if( result < 0 )
    {
        throw std::domain_error(
            "unable to send, failed with '" + std::to_string( result ) );
    }
}

void RawPosixSocket::receive( Packet& data ) const
{
    // recvfrom
    assert( 0 );
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
