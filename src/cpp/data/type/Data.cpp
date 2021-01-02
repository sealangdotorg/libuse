//
//  Copyright (C) 2014-2021 CASM Organization <https://casm-lang.org>
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

#include "Data.h"

#include <algorithm>
#include <cassert>
#include <cstring>

using namespace libstdhl;
using namespace Type;

#define NUMBER "0123456789"
#define UPPER_CASE "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWER_CASE "abcdefghijklmnopqrstuvwxyz"

static constexpr const char* digits_definitions[] = {

    NUMBER LOWER_CASE UPPER_CASE "@$",  // general digit encoding

    UPPER_CASE LOWER_CASE NUMBER "+/",  // base64 encoding

    "./" NUMBER UPPER_CASE LOWER_CASE,  // unix radix 64 encoding
};

Data::Data( const u64 data, const u1 sign )
: m_sign( sign )
, m_trivial( true )
{
    m_data.value = data;
}

Data::Data( Layout* data )
: m_sign( false )
, m_trivial( false )
{
    m_data.ptr = data;
}

Data::Data( void )
: m_sign( false )
, m_trivial( false )
{
    m_data.ptr = nullptr;
}

Data::~Data( void )
{
    if( not m_trivial and m_data.ptr != nullptr )
    {
        delete m_data.ptr;
        m_data.ptr = nullptr;
    }
}

Data::Data( const Data& other )
: Data( nullptr )
{
    *this = other;
}

Data::Data( Data&& other ) noexcept
: Data( nullptr )
{
    *this = std::move( other );
}

Data& Data::operator=( const Data& other )
{
    if( this != &other )
    {
        if( other.m_trivial )
        {
            m_data.value = other.m_data.value;
        }
        else
        {
            if( other.m_data.ptr != nullptr )
            {
                m_data.ptr = other.m_data.ptr->clone();
            }
            else
            {
                m_data.ptr = nullptr;
            }
        }

        m_trivial = other.m_trivial;
        m_sign = other.m_sign;
    }

    return *this;
}

Data& Data::operator=( Data&& other ) noexcept
{
    if( this != &other )
    {
        if( other.m_trivial )
        {
            m_data.value = other.m_data.value;
        }
        else
        {
            if( not m_trivial and m_data.ptr != nullptr )
            {
                delete m_data.ptr;
            }

            m_data.ptr = other.m_data.ptr;
        }

        m_trivial = other.m_trivial;
        m_sign = other.m_sign;

        other.m_data.ptr = nullptr;
        other.m_trivial = false;
    }

    return *this;
}

u64 Data::value( void ) const
{
    return m_data.value;
}

Layout* Data::ptr( void ) const
{
    return m_data.ptr;
}

u1 Data::sign( void ) const
{
    return m_sign;
}

u1 Data::trivial( void ) const
{
    return m_trivial;
}

u1 Data::defined( void ) const
{
    return m_trivial or m_data.ptr != 0;
}

std::size_t Data::hash( void ) const
{
    const std::size_t seed = ( ( (std::size_t)104729 ) << 2 )  // at pos 2. 10'000st prime number
                             | ( (std::size_t)sign() << 1 )    // at 1. idx the sign
                             | ( (std::size_t)defined() );     // at 0. idx the defined

    std::size_t hash = 0;
    if( trivial() )
    {
        hash = Hash::value( value() );
    }
    else
    {
        if( ptr() )
        {
            hash = ptr()->hash();
        }
    }

    return Hash::combine( seed, hash );
}

u1 Data::operator==( const Data& rhs ) const
{
    if( trivial() and rhs.trivial() )
    {
        return m_data.value == rhs.m_data.value and m_sign == rhs.m_sign;
    }
    else if( trivial() or rhs.trivial() )
    {
        return false;
    }
    else  // both are non-trivial
    {
        return m_data.ptr == rhs.m_data.ptr;
    }
}

std::string Data::to_string( const Radix radix, const Literal literal ) const
{
    std::string prefix;
    std::string postfix;

    if( m_sign )
    {
        prefix += "-";
    }

    switch( literal )
    {
        case NONE:  // fall-through
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
                        throw std::domain_error( "binary literal format not specified" );
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
                    "base64 literal format not specified for radix '" + std::to_string( radix ) +
                    "'" );
            }
            break;
        }
        case UNIX:
        {
            if( radix != RADIX64 )
            {
                throw std::domain_error(
                    "unix literal format not specified for radix '" + std::to_string( radix ) +
                    "'" );
            }
            break;
        }
    }

    const char* digits = digits_definitions[ literal / 10 ];

    std::string format;
    u64 n = 0;

    assert( trivial() );
    u64 tmp = m_data.value;
    // Data tmp( *this ); // PPA: TODO: FIXME:

    do
    {
        n = tmp % radix;
        format += digits[ n ];
        tmp /= radix;

    } while( tmp > 0 );

    std::reverse( format.begin(), format.end() );

    const auto result = prefix + format + postfix;

    if( result == "-0" )
    {
        return "0";
    }
    else
    {
        return result;
    }
}

u64 Data::to_digit( const char character, const Radix radix, const Literal literal )
{
    const char* digits = digits_definitions[ literal / 10 ];

    const char* pos = strchr( digits, character );

    if( not pos )
    {
        throw std::domain_error(
            "invalid character '" + std::string( 1, character ) + "' to convert to a digit" );
    }

    u64 digit = pos - digits;

    if( digit >= radix )
    {
        throw std::domain_error(
            "digit '" + std::to_string( digit ) + "' must be smaller than radix '" +
            std::to_string( radix ) + "'" );
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
