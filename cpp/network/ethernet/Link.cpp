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

#include "Link.h"

#include "Socket.h"

using namespace libstdhl;
using namespace Network;
using namespace Ethernet;

static std::unordered_map< std::string, RawPosixSocket > phy;

Link::Link( const std::string& name )
{
    auto result = phy.emplace( name, RawPosixSocket( name ) );
    setSocket( result.first->second );
}

void Link::send( const std::string& data )
{
    send( std::vector< u8 >( data.begin(), data.end() ) );
}

void Link::send( const std::vector< u8 >& data )
{
    const auto size = data.size();
    assert( size <= 1500 );

    const Type type = { { ( u8 )( size >> 8 ), (u8)size } };

    auto& link = static_cast< RawPosixSocket& >( socket() );

    const auto frame = Packet( BROADCAST, link.address(), type, data );

    link.send( frame );
}

void Link::receive( std::string& data )
{
    assert( !" TODO! " );
}

void Link::receive( std::vector< u8 >& data )
{
    assert( !" TODO! " );
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
