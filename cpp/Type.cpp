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

#include "Type.h"

#include "Math.h"

using namespace libstdhl;

Type::Type( const std::vector< u64 >& words, const u1 sign )
: m_words( words )
, m_carry( 0 )
, m_sign( sign )
{
}

Type::Type( void )
: Type( std::vector< u64 >() )
{
}

Type::Type( u64 word, u64 precision, const u1 sign )
: Type(
      std::vector< u64 >( ( precision / 64 ) + ( precision % 64 ? 1 : 0 ), 0 ),
      sign )
{
    if( precision == 0 )
    {
        throw std::domain_error( "Integer precision cannot be zero" );
    }

    if( precision < 64 )
    {
        u64 mask = ( ( u64 )( UINT64_MAX ) ) << precision;

        if( ( word & mask ) != 0 )
        {
            throw std::domain_error( "word '" + std::to_string( word )
                                     + "' does not fit into a precision of '"
                                     + std::to_string( precision )
                                     + "'" );
        }

        word &= ~mask;
    }

    m_words[ 0 ] = word;
}

void Type::add( u64 word )
{
    m_words.push_back( word );
}

void Type::set( std::size_t index, u64 word )
{
    this->word( index );
    m_words[ index ] = word;
}

u64 Type::word( std::size_t index ) const
{
    if( index >= m_words.size() )
    {
        throw std::domain_error( "invalid index '" + std::to_string( index )
                                 + "' of 'libstdhl::Type' with word size of '"
                                 + std::to_string( m_words.size() )
                                 + "'" );
    }

    return m_words[ index ];
}

const std::vector< u64 >& Type::words( void ) const
{
    return m_words;
}

u64 Type::carry( void ) const
{
    return m_carry;
}

u1 Type::sign( void ) const
{
    return m_sign;
}

u1 Type::operator>( const u64 rhs ) const
{
    const std::size_t size = m_words.size();

    switch( size )
    {
        case 0:
        {
            return false;
        }
        default:
        {
            for( auto c = size - 1; c > 0; c-- )
            {
                if( m_words[ c ] > 0 )
                {
                    return true;
                }
            }

            if( m_words[ 0 ] > rhs )
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

Type& Type::operator-=( const u64 rhs )
{
    u64 overflow = rhs;

    for( std::size_t c = 0; c < m_words.size(); c++ )
    {
        overflow = __builtin_usubl_overflow(
            m_words.data()[ c ], overflow, (u64*)&m_words.data()[ c ] );
    }

    m_sign = overflow;

    return *this;
}

Type& Type::operator*=( const u64 rhs )
{
    assert( !" TODO! " ); // TODO: PPA:
    return *this;
}

Type& Type::operator/=( const u64 rhs )
{
    const std::size_t size = m_words.size();

    if( size == 0 )
    {
        throw std::domain_error( "no data to divide" );
    }

    if( rhs == 0 )
    {
        throw std::domain_error( "division by zero" );
    }
    else if( rhs > 1 )
    {
        if( size == 1 )
        {
            // case: 1 word / 1 word

            m_words[ 0 ] = m_words[ 0 ] / rhs;
            m_carry = m_words[ 0 ] % rhs;
        }
        else
        {
            // case: n words / 1 word
            //
            // using an adapted version of the 'Single Limb Dision'
            // https://gmplib.org/manual/Single-Limb-Division.html#Single-Limb-Division

            using precision = long double;
            constexpr auto base = Math::pow< precision >( 2, 64 );
            constexpr u64 dmask = 0xC000000000000000;

            u64 s = 0;
            u64 d = rhs;

            while( ( d & dmask ) == 0 )
            {
                d <<= 1;
                s++;
            }

            precision inv_d = 1.0 / (precision)d;
            precision inv_rhs = 1.0 / (precision)rhs;

            precision q = 0;
            u64 u = 0;
            u64 r = 0;
            u64 a = 0;

            for( i64 c = size - 1; c >= 0; c-- )
            {
                a = m_words[ c ];
                q = (precision)r * base + (precision)a;
                u = ( u64 )( std::floor( q * inv_d ) );

                r = a - u * d;

                q = std::floor( (precision)r * inv_rhs );
                u = u << s;
                u = u + (u64)q;

                m_words[ c ] = u;
            }

            m_carry = r % rhs;
        }
    }

    return *this;
}

Type& Type::operator%=( const u64 rhs )
{
    *this /= rhs;
    auto carry = m_carry;

    this->setToZero();
    m_words[ 0 ] = carry;

    return *this;
}

std::string Type::to_string(
    const Type::Radix radix, const Literal literal ) const
{

#define NUMBER "0123456789"
#define UPPER_CASE "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWER_CASE "abcdefghijklmnopqrstuvwxyz"

    static const char* digits_definitions[] = {

        NUMBER LOWER_CASE UPPER_CASE "@$", // general digit encoding

        UPPER_CASE LOWER_CASE NUMBER "+/", // base64 encoding

        "./" NUMBER UPPER_CASE LOWER_CASE, // unix radix 64 encoding
    };

    std::string prefix;
    std::string postfix;

    auto size = m_words.size();

    if( size == 0 )
    {
        throw std::domain_error( "no data to format" );
    }

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

    if( size == 1 )
    {
        u64 tmp = data()[ 0 ];

        do
        {
            n = tmp % radix;
            format += digits[ n ];
            tmp /= radix;

        } while( tmp > 0 );
    }
    else
    {
        Type tmp( m_words );

        do
        {
            tmp /= radix;
            n = tmp.carry();
            format += digits[ n ];

        } while( tmp != 0 );
    }

    std::reverse( format.begin(), format.end() );

    return prefix + format + postfix;
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
