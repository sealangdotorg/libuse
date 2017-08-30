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
using namespace TCP;

TCP::IPv4::IPv4( const std::string& name, const u1 server )
{
    auto socket = libstdhl::make< IPv4PosixSocket >( name );
    socket->setServer( server );
    auto link = std::static_pointer_cast< Socket< IPv4Packet > >( socket );
    setSocket( link );
}

TCP::IPv4::IPv4( void )
{
}

void TCP::IPv4::send( const IPv4Packet& data )
{
    auto& link = static_cast< IPv4PosixSocket& >( *socket() );

    if( not link.server() )
    {
        link.send( data );
    }
    else
    {
        auto client = link.accept();
        client.send( data );
        client.disconnect();
    }
}
void TCP::IPv4::send( const std::string& data )
{
    assert( !" TODO " );
    // const auto packet = StringData{ data };
    // send( packet );
}

void TCP::IPv4::receive( IPv4Packet& data )
{
    auto& link = static_cast< IPv4PosixSocket& >( *socket() );

    if( not link.server() )
    {
        link.receive( data );
    }
    else
    {
        auto client = link.accept();
        client.receive( data );
        client.disconnect();
    }
}

IPv4Packet TCP::IPv4::receive( std::string& data )
{
    data.resize( 2048 ); // TODO: PPA: FIXME: should be configured etc.

    IPv4Packet packet{ data };
    receive( packet );

    return packet;
}

TCP::IPv4 TCP::IPv4::session( void )
{
    auto& link = static_cast< IPv4PosixSocket& >( *socket() );

    if( not link.server() )
    {
        return *this;
    }
    else
    {
        auto client = link.accept();

        auto socket = libstdhl::make< IPv4PosixSocket >( client );
        socket->setServer( false );

        auto link = std::static_pointer_cast< Socket< IPv4Packet > >( socket );
        auto connection = TCP::IPv4();
        connection.setSocket( link );

        return connection;
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
