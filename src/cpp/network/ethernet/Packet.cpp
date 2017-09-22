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

#include "Packet.h"

using namespace libstdhl;
using namespace Network;
using namespace Ethernet;

Ethernet::Packet::Packet( const Address& destination, const Address& source,
    const Type& type, const std::vector< u8 >& payload )
: m_header( destination, source,
      ( payload.size() < 64 ) ? RUNT : type ) // runt frame prevention
, m_payload{}
, m_size( ( payload.size() < 64 ) // runt frame prevention
              ? 64
              : payload.size() )
{
    std::copy( payload.begin(), payload.end(), m_payload.begin() );
}

const Ethernet::Protocol& Ethernet::Packet::header( void ) const
{
    return m_header;
}

const Data& Ethernet::Packet::payload( void ) const
{
    return m_payload;
}

const u8* Ethernet::Packet::buffer( void ) const
{
    return m_header.buffer();
}

std::size_t Ethernet::Packet::size( void ) const
{
    return m_header.size() + m_size;
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
