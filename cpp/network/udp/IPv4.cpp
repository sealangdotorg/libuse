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

#include "IPv4.h"

#include "Socket.h"

using namespace libstdhl;
using namespace Network;
using namespace UDP;

UDP::IPv4::IPv4( const std::string& name )
{
    auto socket = libstdhl::make< IPv4PosixSocket >( name );
    auto link = std::static_pointer_cast< Socket< IPv4Packet > >( socket );
    setSocket( link );
}

void UDP::IPv4::send( const IPv4Packet& data )
{
    auto& link = static_cast< IPv4PosixSocket& >( *socket() );
    link.send( data );
}

void UDP::IPv4::receive( IPv4Packet& data )
{
    auto& link = static_cast< IPv4PosixSocket& >( *socket() );
    link.receive( data );
}

void UDP::IPv4::send(
    const std::string& data, const Address& address, const Port& port )
{
    auto packet = IPv4Packet{ Network::IPv4::Protocol{ address, address },
        Network::UDP::Protocol{ port, port }, data };

    send( packet );
}

void UDP::IPv4::send(
    const Network::Packet& data, const Address& address, const Port& port )
{
    const std::string str( (const char*)data.buffer() );

    auto packet = IPv4Packet{ Network::IPv4::Protocol{ address, address },
        Network::UDP::Protocol{ port, port }, str };

    send( packet );
}

void UDP::IPv4::send(
    const Network::Packet& data, const IPv4Packet& destination )
{
    const std::string str( (const char*)data.buffer() );

    auto packet = IPv4Packet{
        Network::IPv4::Protocol{ destination.ip() },
        Network::UDP::Protocol{ destination.udp() }, str,
    };

    send( packet );
}

IPv4Packet UDP::IPv4::receive( std::string& data )
{
    data.resize( 2048 ); // TODO: PPA: FIXME: should be configured etc.
    auto packet = IPv4Packet{ data };
    receive( packet );
    return packet;
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
