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
using namespace TCP;

TCP::IPv4::IPv4( const std::string& name, const u1 server )
{
    auto socket = libstdhl::make< IPv4PosixSocket >( name );
    socket->setServer( server );
    auto link = std::static_pointer_cast< Socket< Network::Packet > >( socket );
    setSocket( link );
}

TCP::IPv4::IPv4( void )
{
}

void TCP::IPv4::send( const Network::Packet& data )
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

void TCP::IPv4::receive( Network::Packet& data )
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

        auto link
            = std::static_pointer_cast< Socket< Network::Packet > >( socket );
        auto connection = TCP::IPv4();
        connection.setSocket( link );

        return connection;
    }
}

void TCP::IPv4::send( const std::string& data )
{
    const auto packet = StringData{ data };
    send( packet );
}

void TCP::IPv4::receive( std::string& data )
{
    data.resize( 1400 ); // TODO: PPA: FIXME: should be configured etc.
    StringReferenceData packet( data );
    receive( packet );
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
