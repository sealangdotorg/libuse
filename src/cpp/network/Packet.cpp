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

//
//
// BinaryData
//

BinaryData::BinaryData( const std::vector< u8 >& data )
: m_data( std::move( data ) )
{
}

const u8* BinaryData::buffer( void ) const
{
    return m_data.data();
}

u64 BinaryData::size( void ) const
{
    return m_data.size();
}

const std::vector< u8 >& BinaryData::data( void ) const
{
    return m_data;
}

//
//
// StringData
//

StringData::StringData( const std::string& data )
: m_data( data )
, m_ref( m_data )
{
}

StringData::StringData( std::string& data )
: m_data()
, m_ref( data )
{
}

const u8* StringData::buffer( void ) const
{
    return reinterpret_cast< const u8* >( m_ref.data() );
}

u64 StringData::size( void ) const
{
    return m_ref.size();
}

std::string StringData::data( void ) const
{
    return m_ref;
}

std::string& StringData::data( void )
{
    return m_ref;
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
