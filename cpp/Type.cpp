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

#define NUMBER "0123456789"
#define UPPER_CASE "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWER_CASE "abcdefghijklmnopqrstuvwxyz"

static constexpr const char* digits_definitions[] = {

    NUMBER LOWER_CASE UPPER_CASE "@$", // general digit encoding

    UPPER_CASE LOWER_CASE NUMBER "+/", // base64 encoding

    "./" NUMBER UPPER_CASE LOWER_CASE, // unix radix 64 encoding
};

Type::Type( const std::string& data, const u1 sign, const Radix radix )
: m_words( { { 0 } } )
, m_carry( 0 )
, m_sign( sign )
, m_meta( 0 )
{
    if( radix != BINARY and radix != HEXADECIMAL and radix != RADIX64 )
    {
        throw std::domain_error( "unsupported radix '" + std::to_string( radix )
                                 + "' to create Type data" );
    }

    std::string tmp = data;
    tmp.erase( std::remove( tmp.begin(), tmp.end(), '\'' ), tmp.end() );

    const i64 offset
        = ( radix == BINARY ? 64 : ( radix == HEXADECIMAL ? 16 : 1 ) );

    assert( tmp.size() <= offset );
    m_words[ 0 ] = std::stoull( tmp, nullptr, radix );
    // TODO: PPA: exception handling
}

Type::Type( const std::vector< u64 >& words, const u1 sign )
: m_words( { { 0 } } )
, m_words_ext()
, m_carry( 0 )
, m_sign( sign )
, m_meta( 0 )
{
    const auto size = words.size();

    if( size > 0 )
    {
        m_words[ 0 ] = words[ 0 ];

        if( size > 1 )
        {
            m_words[ 1 ] = words[ 1 ];

            for( std::size_t c = 2; c < size; c++ )
            {
                m_words_ext.push_back( words[ c ] );
            }
        }
    }
}

Type::Type( void )
: m_words( { { 0 } } )
, m_words_ext()
, m_carry( 0 )
, m_sign( false )
, m_meta( 0 )
{
}

Type::Type( u64 word, const u1 sign )
: m_words( { { word, 0 } } )
, m_words_ext()
, m_carry( 0 )
, m_sign( sign )
, m_meta( 0 )
{
}

void Type::set( std::size_t index, u64 word )
{
    this->word( index );
    m_words[ index ] = word;
}

u64 Type::carry( void ) const
{
    return m_carry;
}

u1 Type::sign( void ) const
{
    return m_sign;
}

void Type::shrink( void )
{
    const std::size_t size = m_words_ext.size();

    for( i64 c = size - 1; c > 0; c-- )
    {
        if( m_words_ext[ c ] == 0 )
        {
            m_words_ext.pop_back();
        }
    }
}

u1 Type::operator>( const u64 rhs ) const
{
    switch( size() )
    {
        case 0:
        {
            return false;
        }
        default:
        {
            for( i64 c = m_words_ext.size() - 1; c > 0; c-- )
            {
                if( m_words_ext[ c ] > 0 )
                {
                    return true;
                }
            }

            if( m_words[ 1 ] > 0 )
            {
                return true;
            }

            return m_words[ 0 ] > rhs;
        }
    }
}

u1 Type::operator>( const Type& rhs ) const
{
    assert( size() == rhs.size() );
    assert( size() == 1 );

    return this->operator>( rhs.word( 0 ) );
}

u1 Type::operator<( const u64 rhs ) const
{
    switch( size() )
    {
        case 0:
        {
            return false;
        }
        default:
        {
            for( i64 c = m_words_ext.size() - 1; c > 0; c-- )
            {
                if( m_words_ext[ c ] > 0 )
                {
                    return false;
                }
            }

            if( m_words[ 1 ] > 0 )
            {
                return false;
            }

            return m_words[ 0 ] < rhs;
        }
    }
}

u1 Type::operator<( const Type& rhs ) const
{
    assert( size() == rhs.size() );
    assert( size() == 1 );

    return this->operator<( rhs.word( 0 ) );
}

Type& Type::operator+=( const u64 rhs )
{
    u64 overflow = rhs;

    overflow = __builtin_uaddl_overflow(
        m_words[ 0 ], overflow, (u64*)&m_words.data()[ 0 ] );

    if( size() > 1 )
    {
        overflow = __builtin_uaddl_overflow(
            m_words[ 1 ], overflow, (u64*)&m_words.data()[ 1 ] );

        for( std::size_t c = 0; c < m_words_ext.size(); c++ )
        {
            overflow = __builtin_uaddl_overflow(
                m_words_ext[ c ], overflow, (u64*)&m_words_ext.data()[ c ] );
        }
    }

    m_carry = overflow;

    return *this;
}

Type& Type::operator-=( const u64 rhs )
{
    u64 overflow = rhs;

    overflow = __builtin_usubl_overflow(
        m_words[ 0 ], overflow, (u64*)&m_words.data()[ 0 ] );

    if( size() > 1 )
    {
        overflow = __builtin_usubl_overflow(
            m_words[ 1 ], overflow, (u64*)&m_words.data()[ 1 ] );

        for( std::size_t c = 0; c < m_words_ext.size(); c++ )
        {
            overflow = __builtin_usubl_overflow(
                m_words_ext[ c ], overflow, (u64*)&m_words_ext.data()[ c ] );
        }
    }

    m_carry = overflow;

    return *this;
}

Type& Type::operator*=( const u64 rhs )
{
    const std::size_t size = m_words.size();

    if( size == 0 )
    {
        throw std::domain_error( "no data to multiply" );
    }

    assert( m_sign == false ); // TODO: PPA:

    if( *this == 0 )
    {
        return *this;
    }

    if( *this == 1 )
    {
        setToZero();
        m_words[ 0 ] = rhs;
        return *this;
    }

    if( rhs == 0 )
    {
        setToZero();
        return *this;
    }

    if( rhs == 1 )
    {
        return *this;
    }

    if( size == 1 or ( size == 2 and m_words[ 1 ] == 0 ) )
    {
        m_carry = __builtin_umull_overflow( m_words[ 0 ], rhs, m_words.data() );
    }
    else
    {
        assert( !" TODO! " ); // TODO: PPA:

        // std::reverse( std::begin( m_words ), std::end( m_words ) );

        // Type tmp( std::vector< u64 >( m_words.size() + 1, 0 ) );

        // u32 v[ 2 ];
        // v[ 1 ] = ( u32 )( rhs >> 32 );
        // v[ 0 ] = ( u32 )( rhs );

        // u32* u = (u32*)&m_words.data()[ 0 ];

        // u32* w = (u32*)&tmp.data()[ 0 ];

        // for( u8 j = 0; j < 2; j++ )
        // {
        //     u64 k = 0;

        //     for( u32 i = 0; i < ( size * 2 ); i++ )
        //     {
        //         const u32 x = i + j;

        //         u64 t = u[ i ] * v[ j ] + w[ x ] + k;

        //         w[ x ] = t % UINT32_MAX;

        //         k = t / UINT32_MAX;

        //         printf(
        //             "i=%u, j=%u, u[i]= %u, v[j]= %u --> x=%u, w[x]=%u, k=%u,
        //             "
        //             "t=%lu\n",
        //             i, j, u[ i ], v[ j ], x, w[ x ], k, t );
        //     }

        //     w[ ( size * 2 ) - j ] = k;
        // }

        // m_words = tmp.words();

        // shrink();
    }

    return *this;
}

Type& Type::operator/=( const u64 rhs )
{
    const std::size_t size = this->size();

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
            // using an adapted version of the 'Single Limb Division'
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

            for( i64 c = m_words_ext.size() - 1; c >= 0; c-- )
            {
                a = m_words_ext[ c ];
                q = (precision)r * base + (precision)a;
                u = ( u64 )( std::floor( q * inv_d ) );

                r = a - u * d;

                q = std::floor( (precision)r * inv_rhs );
                u = u << s;
                u = u + (u64)q;

                m_words_ext[ c ] = u;
            }

            for( i64 c = m_words.size() - 1; c >= 0; c-- )
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

Type& Type::operator<<=( const u64 rhs )
{
    if( rhs == 0 )
    {
        return *this;
    }

    const u64 shift = rhs;
    const u64 shinv = 64 - rhs;

    u64 current = 0;
    u64 previous = 0;

    for( std::size_t c = 0; c < m_words.size(); c++ )
    {
        current = m_words[ c ] >> shinv;
        m_words[ c ] = ( m_words[ c ] << shift ) | previous;
        previous = current;
    }

    for( std::size_t c = 0; c < m_words_ext.size(); c++ )
    {
        current = m_words_ext[ c ] >> shinv;
        m_words_ext[ c ] = ( m_words_ext[ c ] << shift ) | previous;
        previous = current;
    }

    m_carry = current;

    return *this;
}

std::string Type::to_string(
    const Type::Radix radix, const Literal literal ) const
{
    std::string prefix;
    std::string postfix;

    auto size = this->size();

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
        u64 tmp = m_words[ 0 ];

        do
        {
            n = tmp % radix;
            format += digits[ n ];
            tmp /= radix;

        } while( tmp > 0 );
    }
    else
    {
        Type tmp( *this );

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

u64 Type::to_digit(
    char character, const Type::Radix radix, const Type::Literal literal )
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
