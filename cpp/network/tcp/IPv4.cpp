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

static std::unordered_map< std::string, IPv4PosixSocket > phy;

TCP::IPv4::IPv4( const std::string& name, const u1 server )
{
    auto result = phy.emplace( name, IPv4PosixSocket( name ) );
    setSocket( result.first->second );

    auto& link = static_cast< IPv4PosixSocket& >( socket() );
    link.setServer( server );
}

void TCP::IPv4::send( const std::string& data )
{
    const auto packet = StringData{ data };

    auto& link = static_cast< IPv4PosixSocket& >( socket() );
    link.send( packet );
}

void TCP::IPv4::send( const std::vector< u8 >& data )
{
    const auto packet = BinaryData{ data };

    auto& link = static_cast< IPv4PosixSocket& >( socket() );
    link.send( packet );
}

void TCP::IPv4::receive( std::string& data )
{
    data.resize( 1400 ); // TODO: PPA: FIXME: should be configured etc.
    StringReferenceData packet( data );

    auto& link = static_cast< IPv4PosixSocket& >( socket() );
    link.receive( packet );
}

void TCP::IPv4::receive( std::vector< u8 >& data )
{
    BinaryData packet( data );

    auto& link = static_cast< IPv4PosixSocket& >( socket() );
    link.receive( packet );
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
