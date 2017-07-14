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

#include "Layout.h"

using namespace libstdhl;
using namespace Type;

#define NUMBER "0123456789"
#define UPPER_CASE "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWER_CASE "abcdefghijklmnopqrstuvwxyz"

static constexpr const char* digits_definitions[] = {

    NUMBER LOWER_CASE UPPER_CASE "@$", // general digit encoding

    UPPER_CASE LOWER_CASE NUMBER "+/", // base64 encoding

    "./" NUMBER UPPER_CASE LOWER_CASE, // unix radix 64 encoding
};

Layout::Layout( const u64 data, const u1 sign )
: m_sign( sign )
, m_trivial( true )
{
    m_data.value = data;
}

Layout::Layout( void* data )
: m_sign( false )
, m_trivial( false )
{
    m_data.ptr = data;
}

Layout::Layout( void )
: m_sign( false )
, m_trivial( false )
{
    m_data.ptr = 0;
}

u64 Layout::value( void ) const
{
    assert( trivial() );
    return m_data.value;
}

void Layout::setValue( const u64 value )
{
    m_data.value = value;
    m_trivial = true;
}

void* Layout::ptr( void ) const
{
    assert( not trivial() );
    return m_data.ptr;
}

void Layout::setPtr( void* ptr )
{
    m_data.ptr = ptr;
    m_trivial = false;
}

u1 Layout::sign( void ) const
{
    return m_sign;
}

u1 Layout::defined( void ) const
{
    return m_trivial or m_data.ptr != 0;
}

u1 Layout::trivial( void ) const
{
    return m_trivial;
}

std::string Layout::to_string( const Radix radix, const Literal literal ) const
{
    std::string prefix;
    std::string postfix;

    switch( literal )
    {
        case NONE: // fall-through
        {
            break;
        }
        case STDHL:
        case C:
        case CPP14:
        {
            // http://en.cppreference.com/w/cpp/language/integer_literal
            switch( radix )
            {
                case BINARY:
                {
                    if( literal == STDHL or literal == CPP14 )
                    {
                        prefix += "0b";
                    }
                    else
                    {
                        throw std::domain_error(
                            "binary literal format not specified" );
                    }
                    break;
                }
                case OCTAL:
                {
                    if( literal == STDHL )
                    {
                        prefix += "0c";
                    }
                    else
                    {
                        prefix += "0";
                    }
                    break;
                }
                case DECIMAL:
                {
                    if( m_sign )
                    {
                        prefix += "-";
                    }
                    break;
                }
                case HEXADECIMAL:
                {
                    prefix += "0x";
                    break;
                }
                case SEXAGESIMAL:
                {
                    if( literal != STDHL )
                    {
                        prefix += "0s";
                    }
                    else
                    {
                        throw std::domain_error( "sexagesimal not specified" );
                    }
                    break;
                }
                case RADIX64:
                {
                    if( literal != STDHL )
                    {
                        throw std::domain_error( "radix 64 not specified" );
                    }
                    break;
                }
            }
            break;
        }
        case BASE64:
        {
            if( radix != RADIX64 )
            {
                throw std::domain_error(
                    "base64 literal format not specified for radix '"
                    + std::to_string( radix )
                    + "'" );
            }
            break;
        }
        case UNIX:
        {
            if( radix != RADIX64 )
            {
                throw std::domain_error(
                    "unix literal format not specified for radix '"
                    + std::to_string( radix )
                    + "'" );
            }
            break;
        }
    }

    const char* digits = digits_definitions[ literal / 10 ];

    std::string format;
    u64 n = 0;

    assert( trivial() );
    u64 tmp = m_data.value;
    // Layout tmp( *this ); // PPA: TODO: FIXME:

    do
    {
        n = tmp % radix;
        format += digits[ n ];
        tmp /= radix;

    } while( tmp > 0 );

    std::reverse( format.begin(), format.end() );

    return prefix + format + postfix;
}

u64 Layout::to_digit(
    const char character, const Radix radix, const Literal literal )
{
    const char* digits = digits_definitions[ literal / 10 ];

    const char* pos = strchr( digits, character );

    if( not pos )
    {
        throw std::domain_error( "invalid character '"
                                 + std::string( 1, character )
                                 + "' to convert to a digit" );
    }

    u64 digit = pos - digits;

    if( digit >= radix )
    {
        throw std::domain_error( "digit '" + std::to_string( digit )
                                 + "' must be smaller than radix '"
                                 + std::to_string( radix )
                                 + "'" );
    }

    return digit;
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
