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

#include "Integer.h"

#include "../Math.h"

using namespace libstdhl;
using namespace Type;

Integer::Integer( void )
: Layout( 0, false )
{
}

Integer::Integer( u64 value )
: Layout( value, false )
{
}

Integer::Integer( i64 value )
: Layout( ( value >= 0 ? (u64)value : ( u64 )( -value ) ), ( value < 0 ) )
{
}

Integer::Integer( const std::string& value, const Radix radix )
: Layout()
{
    const u64 shift
        = ( radix == BINARY
                ? 1
                : ( radix == OCTAL
                          ? 3
                          : ( radix == HEXADECIMAL
                                    ? 4
                                    : ( radix == RADIX64 ? 6 : 0 ) ) ) );

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
        throw std::domain_error(
            "unable to convert string '" + data + "' to a valid Integer" );
    }

    const i64 bound = ( i64 )( sign );

    m_sign = sign;
    m_trivial = true;
    m_data.value = 0;

    for( i64 c = bound; c < data.size(); c++ )
    {
        if( shift )
        {
            *this <<= shift;
        }
        else
        {
            *this *= radix;
        }

        *this += Layout::to_digit( data[ c ], radix );
    }
}

Integer::~Integer( void )
{
    if( not trivial() )
    {
        delete static_cast< IntegerLayout* >( m_data.ptr );
    }
}

// Integer to u64

Integer& Integer::operator<<=( const u64 rhs )
{
    if( rhs == 0 )
    {
        return *this;
    }

    const u64 shift = rhs;
    const u64 shinv = 64 - rhs;

    u64 current = 0;
    u64 previous = 0;

    if( trivial() )
    {
        current = m_data.value >> shinv;
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
        assert( data and data->size() > 0 );

        for( std::size_t c = 0; c < data->size(); c++ )
        {
            current = data->at( c ) >> shinv;
            data->operator[]( c ) = ( data->at( c ) << shift ) | previous;
            previous = current;
        }

        if( current != 0 )
        {
            data->emplace_back( current );
        }
    }

    return *this;
}

Integer& Integer::operator+=( const u64 rhs )
{
    u64 overflow = rhs;

    if( trivial() )
    {
        overflow = __builtin_uaddl_overflow(
            m_data.value, overflow, (u64*)&m_data.value );

        if( overflow != 0 )
        {
            m_data.ptr = new IntegerLayout( { m_data.value, overflow } );
            m_trivial = false;
        }
    }
    else
    {
        auto data = static_cast< IntegerLayout* >( m_data.ptr );
        assert( data and data->size() > 0 );

        for( std::size_t c = 0; c < data->size(); c++ )
        {
            overflow = __builtin_uaddl_overflow(
                data->data()[ c ], overflow, (u64*)&data->data()[ c ] );
        }

        if( overflow != 0 )
        {
            data->emplace_back( overflow );
        }
    }

    return *this;
}

Integer& Integer::operator*=( const u64 rhs )
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
        if( trivial() )
        {
            m_data.value = 0;
        }
        else
        {
            delete static_cast< IntegerLayout* >( m_data.ptr );
            m_data.value = rhs;
            m_trivial = true;
        }

        return *this;
    }

    if( rhs == 1 )
    {
        return *this;
    }

    if( trivial() )
    {
        u64 carry
            = __builtin_umull_overflow( m_data.value, rhs, &m_data.value );
        if( carry != 0 )
        {
            m_data.ptr = new IntegerLayout( { m_data.value, carry } );
            m_trivial = false;
        }
    }
    else
    {
        throw std::domain_error( "unimplemented" );
    }

    return *this;
}

u1 Integer::operator==( const u64& rhs ) const
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
        assert( data and data->size() > 0 );

        for( std::size_t c = 1; c < data->size(); c++ )
        {
            if( data->at( c ) != 0 )
            {
                return false;
            }
        }

        if( data->at( 0 ) == rhs )
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
}

// Integer to Integer

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
        else // a < b
        {
            m_data.value = b - a;
            m_sign = rhs_neg;
        }
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
        else // a < b
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

Integer& Integer::operator%=( const Integer& rhs )
{
    assert( trivial() );
    assert( rhs.trivial() );

    const auto a = value();
    const auto b = rhs.value();

    m_data.value = a % b;

    return *this;
}

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

            if( a->size() != b->size() )
            {
                return false;
            }

            for( std::size_t c = 0; c < a->size(); c++ )
            {
                if( a->at( c ) != b->at( c ) )
                {
                    return false;
                }
            }
        }

        return true;
    }
    else if( defined() and rhs.defined() )
    {
        return false;
    }
    else
    {
        return true;
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
                throw std::domain_error( "unimplemented" );
            }
        }
        else // rhs pos
        {
            return true;
        }
    }
    else // lhs pos
    {
        if( rhs_neg )
        {
            return false;
        }
        else // rhs pos
        {
            if( trivial() and rhs.trivial() )
            {
                return m_data.value < rhs.value();
            }
            else
            {
                throw std::domain_error( "unimplemented" );
            }
        }
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
                throw std::domain_error( "unimplemented" );
            }
        }
        else // rhs pos
        {
            return false;
        }
    }
    else // lhs pos
    {
        if( rhs_neg )
        {
            return true;
        }
        else // rhs pos
        {
            if( trivial() and rhs.trivial() )
            {
                return m_data.value > rhs.value();
            }
            else
            {
                throw std::domain_error( "unimplemented" );
            }
        }
    }
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
