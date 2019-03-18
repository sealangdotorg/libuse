//
//  Copyright (C) 2014-2019 CASM Organization <https://casm-lang.org>
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

#include "IPv4.h"

#include "Socket.h"

using namespace libstdhl;
using namespace Network;
using namespace UDP;

UDP::IPv4::IPv4( const std::string& name )
{
    auto socket = std::make_shared< IPv4Socket >( name );
    auto link = std::static_pointer_cast< Socket< IPv4Packet > >( socket );
    setSocket( link );
}

void UDP::IPv4::send( const IPv4Packet& data )
{
    const auto& link = static_cast< IPv4Socket& >( *socket() );
    link.send( data );
}

IPv4Packet UDP::IPv4::send( const std::vector< u8 >& data )
{
    const auto packet = IPv4Packet{ Network::IPv4::Protocol{}, Network::UDP::Protocol{}, data };
    send( packet );
    return packet;
}

void UDP::IPv4::receive( IPv4Packet& data )
{
    auto& link = static_cast< IPv4Socket& >( *socket() );
    link.receive( data );
}

IPv4Packet UDP::IPv4::receive( std::vector< u8 >& data )
{
    IPv4Packet packet{ data };
    receive( packet );
    return packet;
}

// void UDP::IPv4::send( const std::string& data, const Address& address, const Port& port )
// {
//     auto packet = IPv4Packet{
//         Network::IPv4::Protocol{ address, address }, Network::UDP::Protocol{ port, port }, data
//     };

//     send( packet );
// }

// void UDP::IPv4::send( const Network::Packet& data, const Address& address, const Port& port )
// {
//     const std::string str( (const char*)data.buffer() );

//     auto packet = IPv4Packet{
//         Network::IPv4::Protocol{ address, address }, Network::UDP::Protocol{ port, port }, str
//     };

//     send( packet );
// }

// void UDP::IPv4::send( const Network::Packet& data, const IPv4Packet& destination )
// {
//     const std::string str( (const char*)data.buffer() );

//     IPv4Packet packet{
//         Network::IPv4::Protocol{ destination.ip() },
//         Network::UDP::Protocol{ destination.udp() },
//         str,
//     };

//     send( packet );
// }

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
