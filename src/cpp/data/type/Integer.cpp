//
//  Copyright (C) 2014-2022 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber et al.
//                <https://github.com/casm-lang/libstdhl/graphs/contributors>
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

#include "Integer.h"

#include <libstdhl/Math>
#include <libstdhl/data/type/Natural>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <limits>

using namespace libstdhl;
using namespace Type;

static inline std::size_t hi( std::size_t x )
{
#if defined( LIBSTDHL_CPP_TYPE_64_BIT )
    static_assert( sizeof( std::size_t ) == ( 64 / 8 ) );
    return x >> 32;
#else  // LIBSTDHL_CPP_TYPE_32_BIT
    static_assert( sizeof( std::size_t ) == ( 32 / 8 ) );
    return x >> 16;
#endif
}

static inline std::size_t lo( std::size_t x )
{
#if defined( LIBSTDHL_CPP_TYPE_64_BIT )
    static_assert( sizeof( std::size_t ) == ( 64 / 8 ) );
    return ( ( ( (std::size_t)1 ) << 32 ) - 1 ) & x;
#else  // LIBSTDHL_CPP_TYPE_32_BIT
    static_assert( sizeof( std::size_t ) == ( 32 / 8 ) );
    return ( ( ( (std::size_t)1 ) << 16 ) - 1 ) & x;
#endif
}

static inline std::size_t umull_carry( std::size_t a, std::size_t b )
{
    std::size_t x = lo( a ) * lo( b );

    x = hi( a ) * lo( b ) + hi( x );
    std::size_t s1 = lo( x );
    std::size_t s2 = hi( x );

    x = s1 + lo( a ) * hi( b );
    s1 = lo( x );

    x = s2 + hi( a ) * hi( b ) + hi( x );
    s2 = lo( x );
    std::size_t s3 = hi( x );

#if defined( LIBSTDHL_CPP_TYPE_64_BIT )
    static_assert( sizeof( std::size_t ) == ( 64 / 8 ) );
    return ( s3 << 32 ) | s2;
#else  // LIBSTDHL_CPP_TYPE_32_BIT
    static_assert( sizeof( std::size_t ) == ( 32 / 8 ) );
    return ( s3 << 16 ) | s2;
#endif
}

static inline u1 uaddl_overflow( std::size_t a, std::size_t b, std::size_t* res )
{
#if( defined( __GNUG__ ) or defined( __clang__ ) ) and not defined( __EMSCRIPTEN__ )
    return __builtin_uaddl_overflow( a, b, res );
#else
    *res = a + b;
    return ( a + b ) < a;
#endif
}

static inline bool umull_overflow( std::size_t a, std::size_t b, std::size_t* res )
{
#if( defined( __GNUG__ ) or defined( __clang__ ) ) and not defined( __EMSCRIPTEN__ )
    return __builtin_umull_overflow( a, b, res );
#else
    *res = a * b;
    return b > 0 && a > ( std::numeric_limits< std::size_t >::max() / b );
#endif
}

//
// Type::create*
//

Integer Type::createInteger( const std::string& value, const Radix radix )
{
    return Integer::fromString( value, radix );
}

Integer Type::createInteger( const unsigned int value )
{
    Integer tmp( value, false );
    return tmp;
}

Integer Type::createInteger( const int value )
{
    Integer tmp( ( value >= 0 ? (std::size_t)value : ( std::size_t )( -value ) ), ( value < 0 ) );
    return tmp;
}

Integer Type::createInteger( const Natural& value, const u1 sign )
{
    Integer tmp( value );

    if( sign )
    {
        return -tmp;
    }
    else
    {
        return tmp;
    }
}

//
// Integer
//

Integer Integer::fromString( const std::string& value, const Radix radix )
{
    const std::size_t shift =
        ( radix == BINARY
              ? 1
              : ( radix == OCTAL ? 3
                                 : ( radix == HEXADECIMAL ? 4 : ( radix == RADIX64 ? 6 : 0 ) ) ) );

    std::string data = value;
    data.erase( std::remove( data.begin(), data.end(), '\'' ), data.end() );

    i64 length = data.size();
    u1 sign = false;

    if( data[ 0 ] == '-' )
    {
        length--;
        sign = true;
    }
    else if( data[ 0 ] == '+' )
    {
        length--;
    }

    if( length == 0 )
    {
        throw std::domain_error( "unable to convert string '" + data + "' to a valid Integer" );
    }

    const i64 bound = (i64)( sign );

    Integer tmp = Type::createInteger( 0 );
    assert( tmp.trivial() );
    assert( tmp.value() == 0 );

    for( i64 c = bound; c < data.size(); c++ )
    {
        if( shift )
        {
            tmp <<= shift;
        }
        else
        {
            tmp *= radix;
        }

        tmp += Data::to_digit( data[ c ], radix );
    }

    if( sign )
    {
        return -tmp;
    }
    else
    {
        return tmp;
    }
}

const std::size_t Integer::operator[]( std::size_t idx ) const
{
    if( m_trivial )
    {
        assert( idx == 0 );
        return m_data.value;
    }
    else
    {
        auto data = static_cast< IntegerLayout* >( m_data.ptr );
        return data->operator[]( idx );
    }
}

//
// IntegerLayout
//

IntegerLayout::IntegerLayout( const std::size_t low, const std::size_t high )
: m_word( { low, high } )
{
}

Layout* IntegerLayout::clone( void ) const
{
    return new IntegerLayout( *this );
}

std::size_t IntegerLayout::hash( void ) const
{
    auto h = std::hash< std::size_t >()( m_word[ 0 ] );

    for( std::size_t c = 1; c < m_word.size(); c++ )
    {
        h = libstdhl::Hash::combine( h, m_word[ c ] );
    }

    return h;
}

const std::size_t IntegerLayout::operator[]( std::size_t idx ) const
{
    assert( m_word.size() > 0 and idx < m_word.size() );
    return m_word[ idx ];
}

const std::vector< std::size_t >& IntegerLayout::word( void ) const
{
    return m_word;
}

//
// Integer and IntegerLayout Operators
//

//
// operator '==' and '!='
//

u1 Integer::operator==( const std::size_t rhs ) const
{
    if( trivial() )
    {
        if( m_data.value == rhs )
        {
            if( m_sign )
            {
                if( rhs == 0 )
                {
                    return true;
                }

                return false;
            }

            return true;
        }

        return false;
    }
    else
    {
        auto data = static_cast< IntegerLayout* >( m_data.ptr );
        return data->operator==( rhs );
    }
}

u1 Integer::operator==( const Integer& rhs ) const
{
    if( defined() and rhs.defined() )
    {
        if( trivial() and rhs.trivial() )
        {
            if( value() == rhs.value() )
            {
                if( rhs.sign() != sign() )
                {
                    if( value() == 0 )
                    {
                        return true;
                    }
                    return false;
                }
                return true;
            }
            return false;
        }
        else if( trivial() )
        {
            assert( 0 );
            return rhs == value() and rhs.sign() == sign();
        }
        else if( rhs.trivial() )
        {
            assert( 0 );
            return this->operator==( rhs.value() ) and rhs.sign() == sign();
        }
        else
        {
            auto a = static_cast< const IntegerLayout* >( ptr() );
            auto b = static_cast< const IntegerLayout* >( rhs.ptr() );

            if( *a == *b )
            {
                if( rhs.sign() != sign() )
                {
                    if( rhs == 0 )
                    {
                        return true;
                    }
                    return false;
                }
                return true;
            }
            return false;
        }
    }
    else if( defined() xor rhs.defined() )
    {
        return false;
    }
    else
    {
        return true;
    }
}

u1 IntegerLayout::operator==( const std::size_t rhs ) const
{
    assert( m_word.size() > 0 );

    for( std::size_t c = 1; c < m_word.size(); c++ )
    {
        if( m_word[ c ] != 0 )
        {
            return false;
        }
    }

    if( m_word[ 0 ] == rhs )
    {
        return true;
    }

    return false;
}

u1 IntegerLayout::operator==( const IntegerLayout& rhs ) const
{
    assert( m_word.size() > 0 );
    assert( rhs.m_word.size() > 0 );

    if( m_word.size() != rhs.m_word.size() )
    {
        return false;
    }

    for( std::size_t c = 0; c < m_word.size(); c++ )
    {
        if( m_word[ c ] != rhs.m_word[ c ] )
        {
            return false;
        }
    }

    return true;
}

//
// operator '<' and '>='
//

u1 Integer::operator<( const std::size_t rhs ) const
{
    if( not trivial() )
    {
        return false;
    }
    else if( sign() )
    {
        return true;
    }
    else
    {
        return value() < rhs;
    }
}

u1 Integer::operator<( const Integer& rhs ) const
{
    const auto lhs_neg = m_sign;
    const auto rhs_neg = rhs.sign();

    if( lhs_neg )
    {
        if( rhs_neg )
        {
            if( trivial() and rhs.trivial() )
            {
                return m_data.value > rhs.value();
            }
            else
            {
                throw std::domain_error( "unimplemented 'Integer::operator<' of -lhs and -rhs" );
            }
        }
        else  // rhs pos
        {
            return true;
        }
    }
    else  // lhs pos
    {
        if( rhs_neg )
        {
            return false;
        }
        else  // rhs pos
        {
            if( trivial() and rhs.trivial() )
            {
                return m_data.value < rhs.value();
            }
            else
            {
                throw std::domain_error( "unimplemented 'Integer::operator<' of +lhs and +rhs" );
            }
        }
    }
}

//
// operator '>' and '<='
//

u1 Integer::operator>( const std::size_t rhs ) const
{
    if( sign() )
    {
        return false;
    }
    else if( not trivial() )
    {
        return true;
    }
    else
    {
        return value() > rhs;
    }
}

u1 Integer::operator>( const Integer& rhs ) const
{
    const auto lhs_neg = m_sign;
    const auto rhs_neg = rhs.sign();

    if( lhs_neg )
    {
        if( rhs_neg )
        {
            if( trivial() and rhs.trivial() )
            {
                return m_data.value < rhs.value();
            }
            else
            {
                throw std::domain_error( "unimplemented 'Integer::operator>' of -lhs and -rhs" );
            }
        }
        else  // rhs pos
        {
            return false;
        }
    }
    else  // lhs pos
    {
        if( rhs_neg )
        {
            return true;
        }
        else  // rhs pos
        {
            if( trivial() and rhs.trivial() )
            {
                return m_data.value > rhs.value();
            }
            else
            {
                throw std::domain_error( "unimplemented 'Integer::operator>' of +lhs and +rhs" );
            }
        }
    }
}

//
// operator '+=' and '+'
//

Integer& Integer::operator++( void )
{
    return operator+=( 1 );
}

Integer Integer::operator++( int )
{
    auto tmp = *this;
    operator++();
    return tmp;
}

Integer& Integer::operator+=( const std::size_t rhs )
{
    if( trivial() )
    {
        const auto lhs = m_data.value;

        if( m_sign )
        {
            if( lhs > rhs )
            {
                m_data.value = lhs - rhs;
            }
            else
            {
                m_data.value = rhs - lhs;
                m_sign = false;
            }
        }
        else
        {
            const auto addof = uaddl_overflow( lhs, rhs, (std::size_t*)&m_data.value );

            if( addof )
            {
                m_data.ptr = new IntegerLayout( { m_data.value, 1 } );
                m_trivial = false;
            }
        }
    }
    else
    {
        auto data = static_cast< IntegerLayout* >( m_data.ptr );
        data->operator+=( rhs );
    }

    return *this;
}

Integer& Integer::operator+=( const Integer& rhs )
{
    assert( trivial() );
    assert( rhs.trivial() );

    const auto lhs_neg = m_sign;
    const auto rhs_neg = rhs.sign();

    const auto a = value();
    const auto b = rhs.value();

    if( lhs_neg == rhs_neg )
    {
        m_data.value = a + b;
    }
    else
    {
        if( a >= b )
        {
            m_data.value = a - b;
        }
        else  // a < b
        {
            m_data.value = b - a;
            m_sign = rhs_neg;
        }
    }

    return *this;
}

IntegerLayout& IntegerLayout::operator+=( const std::size_t rhs )
{
    assert( m_word.size() > 0 );

    std::size_t carry = rhs;

    for( std::size_t c = 0; c < m_word.size(); c++ )
    {
        const auto lhs = m_word[ c ];
        const auto addof = uaddl_overflow( lhs, carry, &m_word[ c ] );
        carry = addof;
    }

    if( carry != 0 )
    {
        m_word.emplace_back( carry );
    }

    return *this;
}

//
// operator '--', '-=' and '-'
//

Integer& Integer::operator--( void )
{
    return operator-=( 1 );
}

Integer Integer::operator--( int )
{
    auto tmp = *this;
    operator--();
    return tmp;
}

Integer& Integer::operator-=( const std::size_t rhs )
{
    if( trivial() )
    {
        if( m_sign )
        {
            return operator+=( rhs );
        }
        else
        {
            if( m_data.value >= rhs )
            {
                m_data.value -= rhs;
            }
            else
            {
                m_data.value = rhs - m_data.value;
                m_sign = true;
            }
        }
    }
    else
    {
        auto data = static_cast< IntegerLayout* >( m_data.ptr );
        data->operator-=( rhs );
    }

    return *this;
}

Integer& Integer::operator-=( const Integer& rhs )
{
    assert( trivial() );
    assert( rhs.trivial() );

    const auto lhs_neg = m_sign;
    const auto rhs_neg = rhs.sign();

    const auto a = value();
    const auto b = rhs.value();

    if( lhs_neg == rhs_neg )
    {
        if( a >= b )
        {
            m_data.value = a - b;
        }
        else  // a < b
        {
            m_data.value = b - a;
            m_sign = not rhs_neg;
        }
    }
    else
    {
        m_data.value = a + b;
    }

    return *this;
}

IntegerLayout& IntegerLayout::operator-=( const std::size_t rhs )
{
    assert( m_word.size() > 0 );

    throw std::domain_error( "unimplemented 'IntegerLayout::operator-='" );

    return *this;
}

//
// operator '-' (NEGATE)
//

// inline defined in header file

//
// operator '*=' and '*'
//

Integer& Integer::operator*=( const std::size_t rhs )
{
    if( *this == 0 )
    {
        return *this;
    }

    if( *this == 1 )
    {
        assert( trivial() );
        m_data.value = rhs;
        return *this;
    }

    if( rhs == 0 )
    {
        if( not trivial() )
        {
            delete m_data.ptr;
            m_trivial = true;
        }

        m_data.value = 0;

        return *this;
    }

    if( rhs == 1 )
    {
        return *this;
    }

    if( trivial() )
    {
        const auto lhs = m_data.value;
        const auto mulof = umull_overflow( lhs, rhs, &m_data.value );

        if( mulof )
        {
            m_data.ptr = new IntegerLayout( { m_data.value, umull_carry( lhs, rhs ) } );
            m_trivial = false;
        }
    }
    else
    {
        auto data = static_cast< IntegerLayout* >( m_data.ptr );
        data->operator*=( rhs );
    }

    return *this;
}

Integer& Integer::operator*=( const Integer& rhs )
{
    assert( trivial() );
    assert( rhs.trivial() );

    const auto lhs_neg = m_sign;
    const auto rhs_neg = rhs.sign();

    const auto a = value();
    const auto b = rhs.value();

    m_data.value = a * b;
    m_sign = lhs_neg != rhs_neg;

    return *this;
}

IntegerLayout& IntegerLayout::operator*=( const std::size_t rhs )
{
    assert( m_word.size() > 0 );

    std::size_t carry = 0;

    for( std::size_t c = 0; c < m_word.size(); c++ )
    {
        const auto lhs = m_word[ c ];
        const auto mulof = umull_overflow( lhs, rhs, &m_word[ c ] );

        const auto addof = uaddl_overflow( m_word[ c ], carry, &m_word[ c ] );

        assert( not addof );

        carry = ( mulof ? umull_carry( lhs, rhs ) : 0 );
    }

    if( carry != 0 )
    {
        m_word.emplace_back( carry );
    }

    return *this;
}

//
// operator '%=' and '%'
//

Integer& Integer::operator%=( const std::size_t rhs )
{
    assert( trivial() );

    const std::size_t a = value();
    m_data.value = a % rhs;

    return *this;
}

Integer& Integer::operator%=( const Integer& rhs )
{
    assert( trivial() );
    assert( rhs.trivial() );

    const auto a = value();
    const auto b = rhs.value();

    m_data.value = a % b;

    return *this;
}

//
// operator '/=' and '/'
//

Integer& Integer::operator/=( const Integer& rhs )
{
    assert( trivial() );
    assert( rhs.trivial() );

    const auto lhs_neg = m_sign;
    const auto rhs_neg = rhs.sign();

    const auto a = value();
    const auto b = rhs.value();

    m_data.value = a / b;
    m_sign = lhs_neg != rhs_neg;

    return *this;
}

//
// operator '^=' and '^'
//

Integer& Integer::operator^=( const Natural& rhs )
{
    assert( trivial() );
    assert( rhs.trivial() );

    const std::size_t a = value();
    const std::size_t b = rhs.value();

    m_data.value = (std::size_t)std::llround( std::pow( a, b ) );
    m_sign = sign() and ( rhs % 2 ) == 1;

    return *this;
}

//
// operator '~' (INVERSE)
//

Integer Integer::operator~( void ) const
{
    Integer tmp( *this );

    if( tmp.trivial() )
    {
        tmp.m_data.value = ~tmp.m_data.value;
    }
    else
    {
        static_cast< IntegerLayout* >( tmp.m_data.ptr )->operator~();
    }

    return tmp;
}

IntegerLayout& IntegerLayout::operator~( void )
{
    assert( m_word.size() > 0 );

    for( std::size_t c = 0; c < m_word.size(); c++ )
    {
        m_word[ c ] = ~m_word[ c ];
    }

    return *this;
}

//
// operator '<<=' and '<<'
//

Integer& Integer::operator<<=( const std::size_t rhs )
{
    if( rhs == 0 )
    {
        return *this;
    }

    if( trivial() )
    {
        const std::size_t shift = rhs;
        const std::size_t shinv = 64 - rhs;

        const std::size_t current = m_data.value >> shinv;
        m_data.value = m_data.value << shift;

        if( current != 0 )
        {
            m_data.ptr = new IntegerLayout( { m_data.value, current } );
            m_trivial = false;
        }
    }
    else
    {
        auto data = static_cast< IntegerLayout* >( m_data.ptr );
        data->operator<<=( rhs );
    }

    return *this;
}

Integer& Integer::operator<<=( const Integer& rhs )
{
    if( rhs == 0 )
    {
        return *this;
    }

    assert( rhs.trivial() );
    return this->operator<<=( rhs.value() );
}

IntegerLayout& IntegerLayout::operator<<=( const std::size_t rhs )
{
    assert( m_word.size() > 0 );

    const std::size_t shift = rhs;
    const std::size_t shinv = 64 - rhs;

    std::size_t current = 0;
    std::size_t previous = 0;

    for( std::size_t c = 0; c < m_word.size(); c++ )
    {
        current = m_word[ c ] >> shinv;
        m_word[ c ] = ( m_word[ c ] << shift ) | previous;
        previous = current;
    }

    if( current != 0 )
    {
        m_word.emplace_back( current );
    }

    return *this;
}

//
// operator '>>=' and '>>'
//

Integer& Integer::operator>>=( const std::size_t rhs )
{
    if( rhs == 0 )
    {
        return *this;
    }

    if( trivial() )
    {
        const std::size_t shift = rhs;
        m_data.value = m_data.value >> shift;
    }
    else
    {
        auto data = static_cast< IntegerLayout* >( m_data.ptr );
        data->operator>>=( rhs );

        if( data->word().size() <= 1 )
        {
            const auto value = data->word()[ 0 ];
            delete m_data.ptr;
            m_trivial = true;
            m_data.value = value;
        }
    }

    return *this;
}

Integer& Integer::operator>>=( const Integer& rhs )
{
    if( rhs == 0 )
    {
        return *this;
    }

    assert( rhs.trivial() );
    return this->operator>>=( rhs.value() );
}

IntegerLayout& IntegerLayout::operator>>=( const std::size_t rhs )
{
    assert( m_word.size() > 0 );

    const std::size_t shift = rhs;
    const std::size_t shinv = 64 - rhs;

    std::size_t next = 0;

    for( std::size_t c = 0; c < m_word.size() - 1; c++ )
    {
        next = m_word[ c + 1 ] << shinv;
        m_word[ c ] = next | ( m_word[ c ] >> shift );
    }

    if( m_word.back() == 0 )
    {
        m_word.pop_back();
    }

    return *this;
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
