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

#include "Math.h"

using namespace libstdhl;

Integer::Integer( u64 value )
: Type( value )
{
}

Integer::Integer( i64 value )
: Type( ( value >= 0 ? (u64)value : ( u64 )( -value ) ), ( value < 0 ) )
{
}

Integer::Integer( const std::string& value, const Radix radix )
: Type()
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

    std::size_t precision
        = ( std::size_t )( length * std::log2( (double)radix ) );

    Type tmp( std::vector< u64 >(
        ( precision / 64 ) + ( precision % 64 ? 1 : 0 ), 0 ) );

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

        tmp += Type::to_digit( data[ c ], radix );
    }

    tmp.shrink();

    tmp.foreach( [&]( const std::size_t index, const u64 value ) {
        if( index < 2 )
        {
            m_words[ index ] = value;
        }
        else
        {
            m_words_ext.push_back( value );
        }
    } );

    m_sign = sign;
}

Integer& Integer::operator+=( const Integer& rhs )
{
    assert( size() == rhs.size() );
    assert( size() == 1 );

#warning "HACK!"

    m_words[ 0 ] += rhs.word( 0 );

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
