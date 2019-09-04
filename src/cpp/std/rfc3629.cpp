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

/**
   @brief    TBD

   TBD
*/

using namespace libstdhl;
using namespace Standard;
using namespace RFC3629;

UTF8::UTF8( const u32 code )
: m_code( code )
{
}

const u32 UTF8::code( void ) const
{
    return m_code;
}

std::string UTF8::description( void ) const
{
    const auto value = Type::createInteger( (u64)code() );
    return value.to< Type::Radix::HEXADECIMAL, Type::Literal::NONE >();
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

UTF8 UTF8::fromString( const std::string& byteSequence )
{
    const auto size = byteSequence.size();
    if( size > 4 )
    {
        throw std::domain_error( "UTF-8 character byte size bigger than 4 is not supported" );
    }

    u32 code = 0;
    for( const auto byteElement : byteSequence )
    {
        code = ( code << 8 ) | ( u8 )( byteElement );
    }

    // | Bytes | Size | From    | To       |   Byte 1 |   Byte 2 |   Byte 3 |   Byte 4 |
    // |-------+------+---------+----------+----------+----------+----------+----------|
    // |     1 |    7 | U+0000  | U+007F   | 0xxxxxxx |          |          |          |
    // |     2 |   11 | U+0080  | U+07FF   | 110xxxxx | 10xxxxxx |          |          |
    // |     3 |   16 | U+0800  | U+FFFF   | 1110xxxx | 10xxxxxx | 10xxxxxx |          |
    // |     4 |   21 | U+10000 | U+10FFFF | 11110xxx | 10xxxxxx | 10xxxxxx | 10xxxxxx |

    u1 valid = false;
    switch( size )
    {
        case 1:
        {
            if( code >= 0x00 and code <= 0x7f )
            {
                valid = true;
            }
            break;
        }
        case 2:
        {
            if( code >= 0xc080 and code <= 0xdfbf )
            {
                valid = true;
            }
            break;
        }
        case 3:
        {
            if( code >= 0xe08080 and code <= 0xefbfbf )
            {
                valid = true;
            }
            break;
        }
        case 4:  // [[fallthrough]]
        default:
        {
            if( code >= 0xf0808080 and code <= 0xf7bfbfbf )
            {
                valid = true;
            }
            break;
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

    return UTF8( code );
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
