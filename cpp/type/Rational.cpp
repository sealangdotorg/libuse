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

#include "Rational.h"

#include "../String.h"
#include "Integer.h"

using namespace libstdhl;
using namespace Type;

Rational::Rational( const std::string& value, const Type::Radix radix )
: Layout()
{
    std::vector< std::string > parts;

    String::split( value, "/", parts );

    if( parts.size() == 0 )
    {
        throw std::invalid_argument(
            "value '" + value + "' seems to be an invalid Rational literal" );
    }
    else if( parts.size() > 2 )
    {
        throw std::domain_error(
            "value '" + value
            + "' too many Rational '/' characters found in literal" );
    }

    m_sign = false;
    m_trivial = false;

    const auto numerator = Integer( parts[ 0 ], radix );

    if( parts.size() > 1 )
    {
        m_data.ptr = new RationalLayout(
            { { numerator, Integer( parts[ 1 ], radix ) } } );
    }
    else
    {
        m_data.ptr = new RationalLayout( { { numerator, Integer( (u64)1 ) } } );
    }
}

Rational::Rational( const Integer& numerator, const Integer& denominator )
: Layout()
{
    if( denominator == 0 )
    {
        throw std::domain_error( "denominator of Rational is zero" );
    }

    m_sign = false;
    m_trivial = false;

    m_data.ptr = new RationalLayout( { { numerator, denominator } } );
}

u1 Rational::operator==( const Rational& rhs ) const
{
    return static_cast< RationalLayout* >( m_data.ptr )->at( 0 )
               == static_cast< RationalLayout* >( rhs.ptr() )->at( 0 )
           and static_cast< RationalLayout* >( m_data.ptr )->at( 1 )
                   == static_cast< RationalLayout* >( rhs.ptr() )->at( 1 );
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
