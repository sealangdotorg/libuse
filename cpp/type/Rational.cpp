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

using namespace libstdhl;
using namespace Type;

//
// Type::create*
//

Rational Type::createRational( const std::string& value, const Radix radix )
{
    return Rational::fromString( value, radix );
}

Rational Type::createRational(
    const Integer& numerator, const Integer& denominator )
{
    if( denominator == 0 )
    {
        throw std::domain_error( "denominator of rational cannot be zero!" );
    }

    return Rational( new RationalLayout( numerator, denominator ) );
}

//
// Rational
//

Rational Rational::fromString(
    const std::string& value, const Type::Radix radix )
{
    std::vector< std::string > parts;
    libstdhl::String::split( value, "/", parts );

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

    Rational tmp( nullptr );
    tmp.m_sign = false;
    tmp.m_trivial = false;

    const auto numerator = Integer::fromString( parts[ 0 ], radix );

    if( parts.size() > 1 )
    {
        tmp.m_data.ptr = new RationalLayout(
            numerator, Integer::fromString( parts[ 1 ], radix ) );
    }
    else
    {
        tmp.m_data.ptr
            = new RationalLayout( numerator, createInteger( (u64)1 ) );
    }

    return tmp;
}

const Integer& Rational::numerator( void ) const
{
    assert( m_data.ptr );
    const auto data = static_cast< RationalLayout* >( m_data.ptr );
    return data->numerator();
}

const Integer& Rational::denominator( void ) const
{
    assert( m_data.ptr );
    const auto data = static_cast< RationalLayout* >( m_data.ptr );
    return data->denominator();
}

u1 Rational::operator==( const u64 rhs ) const
{
    if( not defined() )
    {
        return false;
    }

    assert( m_data.ptr );
    const auto data = static_cast< RationalLayout* >( m_data.ptr );

    if( *data == rhs )
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

u1 Rational::operator==( const Rational& rhs ) const
{
    if( defined() and rhs.defined() )
    {
        assert( m_data.ptr );
        assert( rhs.m_data.ptr );

        const auto lval = static_cast< RationalLayout* >( m_data.ptr );
        const auto rval = static_cast< RationalLayout* >( rhs.m_data.ptr );

        if( *lval == *rval )
        {
            if( m_sign != rhs.m_sign )
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
    else if( defined() xor rhs.defined() )
    {
        return false;
    }
    else
    {
        return true;
    }
}

//
// RationalLayout
//

RationalLayout::RationalLayout(
    const Integer& numerator, const Integer& denominator )
: m_numerator( numerator )
, m_denominator( denominator )
{
}

Layout* RationalLayout::clone( void ) const
{
    return new RationalLayout( m_numerator, m_denominator );
}

const Integer& RationalLayout::numerator( void ) const
{
    return m_numerator;
}

const Integer& RationalLayout::denominator( void ) const
{
    return m_denominator;
}

u1 RationalLayout::operator==( const u64 rhs ) const
{
    return m_numerator == rhs and m_denominator == 1;
}

u1 RationalLayout::operator==( const RationalLayout& rhs ) const
{
    return m_numerator == rhs.m_numerator
           and m_denominator == rhs.m_denominator;
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