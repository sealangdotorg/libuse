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

#include "Protocol.h"

#include "../../Array.h"

using namespace libstdhl;
using namespace Network;
using namespace Ethernet;

constexpr std::array< u8, HEADER > Ethernet::Protocol::data( void ) const
{
    return m_destination + m_source + m_type;
}

const Address& Ethernet::Protocol::destination( void ) const
{
    return m_destination;
}

const Address& Ethernet::Protocol::source( void ) const
{
    return m_source;
}

const Ethernet::Type& Ethernet::Protocol::type( void ) const
{
    return m_type;
}

const u8* Ethernet::Protocol::buffer( void ) const
{
    return m_destination.data();
}

std::size_t Ethernet::Protocol::size( void ) const
{
    return data().size();
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
