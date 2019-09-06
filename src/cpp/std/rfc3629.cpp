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

#include "rfc3629.h"

#include <libstdhl/Type>
#include <libstdhl/data/type/Integer>

#include <algorithm>

/**
   @brief    TBD

   TBD
*/

using namespace libstdhl;
using namespace Standard;
using namespace RFC3629;

UTF8::UTF8( const u32 code, const u32 point )
: m_code( code )
, m_point( point )
{
}

const u32 UTF8::code( void ) const
{
    return m_code;
}

const u32 UTF8::point( void ) const
{
    return m_point;
}

std::string UTF8::description( void ) const
{
    const auto value = Type::createInteger( (u64)code() );
    return value.to< Type::Radix::HEXADECIMAL, Type::Literal::NONE >();
}

std::string UTF8::unicode( void ) const
{
    const auto value = Type::createInteger( (u64)point() );
    auto hexPoint = value.to< Type::Radix::HEXADECIMAL, Type::Literal::NONE >();

    for_each( hexPoint.begin(), hexPoint.end(), []( char& character ) {
        character = ::toupper( static_cast< unsigned char >( character ) );
    } );

    return "U+" + hexPoint;
}

std::string UTF8::toString( void ) const
{
    std::string byteSequence = "";
    u32 byteValue = code();
    while( byteValue != 0x00 )
    {
        byteSequence = std::string( 1, ( u8 )( byteValue & 0x000000ff ) ) + byteSequence;
        byteValue = byteValue >> 8;
    }
    return byteSequence;
}

std::size_t UTF8::byteSequenceLengthIndication( const u8 byte )
{
    if( byte >= 0x00 and byte <= 0x7f )
    {
        return 1;
    }
    else if( byte >= 0xc0 and byte <= 0xdf )
    {
        return 2;
    }
    else if( byte >= 0xe0 and byte <= 0xef )
    {
        return 3;
    }
    else if( byte >= 0xf0 and byte <= 0xf7 )
    {
        return 4;
    }

    return 0;
}

// | Bytes | Size | From    | To       |   Byte 1 |   Byte 2 |   Byte 3 |   Byte 4 |
// |-------+------+---------+----------+----------+----------+----------+----------|
// |     1 |    7 | U+0000  | U+007F   | 0xxxxxxx |          |          |          |
// |     2 |   11 | U+0080  | U+07FF   | 110xxxxx | 10xxxxxx |          |          |
// |     3 |   16 | U+0800  | U+FFFF   | 1110xxxx | 10xxxxxx | 10xxxxxx |          |
// |     4 |   21 | U+10000 | U+10FFFF | 11110xxx | 10xxxxxx | 10xxxxxx | 10xxxxxx |

UTF8 UTF8::fromString( const std::string& byteSequence )
{
    const auto size = byteSequence.size();
    if( size > 4 )
    {
        throw std::domain_error( "invalid UTF-8 character byte sequence length is larger than 4" );
    }

    u1 valid = true;
    u8 startByte = byteSequence[ 0 ];
    u32 code = startByte;
    u32 point = startByte;
    const auto length = byteSequenceLengthIndication( startByte );

    if( size != length or length == 0 )
    {
        valid = false;
    }
    else
    {
        if( size == 2 )
        {
            point = ( point & 0x1f );
        }
        else if( size == 3 )
        {
            point = ( point & 0x0f );
        }
        else if( size == 4 )
        {
            point = ( point & 0x07 );
        }

        for( auto position = 1; position < size; position++ )
        {
            const auto byteElement = ( u8 )( byteSequence[ position ] );
            if( byteElement < 0x80 or byteElement > 0xbf )
            {
                valid = false;
                break;
            }
            code = ( code << 8 ) | byteElement;
            point = ( point << 6 ) | ( byteElement & 0x3f );
        }
    }

    if( not valid )
    {
        std::stringstream stream;
        for( const auto byteElement : byteSequence )
        {
            stream << std::hex << ( u8 )( byteElement );
        }
        throw std::domain_error( "invalid UTF-8 character '" + stream.str() + "'" );
    }

    return UTF8( code, point );
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
