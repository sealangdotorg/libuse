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

#include "Decimal.h"

#include <libstdhl/data/type/Natural>

#include <cassert>
#include <cmath>

using namespace libstdhl;
using namespace Type;

//
// Type::create*
//

Decimal Type::createDecimal( const std::string& value, const Radix radix )
{
    return Decimal::fromString( value, radix );
}

Decimal Type::createDecimal( const double value )
{
    // IEEE 754 double-precision binary decimal-point format
    static_assert( sizeof( double ) == 8, " double shall be a byte-size of 8 " );

    Decimal tmp( ( value >= 0 ? (u64)value : ( u64 )( -value ) ), ( value < 0 ) );

    return tmp;
}

Decimal Type::createDecimal( const Integer& value )
{
    assert( value.trivial() );
    Decimal tmp( value.value(), value.sign() );
    return tmp;
}

Decimal Type::createDecimal( const Natural& value )
{
    return createDecimal( static_cast< const Integer& >( value ) );
}

//
// Decimal
//

Decimal Decimal::fromString( const std::string& value, const Radix radix )
{
    try
    {
        double val = std::stod( value );
        return createDecimal( val );
    }
    catch( const std::invalid_argument& e )
    {
        // TODO: PPA: FIXME: if no conversion could be performed
    }
    catch( const std::out_of_range& e )
    {
        // TODO: PPA: FIXME: if the converted value would fall out
        // of the range of the result type or if the underlying function
    }

    Decimal tmp( 0, false );
    return tmp;
}

Integer Decimal::toInteger( void ) const
{
    assert( trivial() );

    const auto fval = static_cast< double >( m_data.value );
    const auto ival = static_cast< u64 >( fval );

    Integer tmp( ival, this->sign() );
    return tmp;
}

//
// DecimalLayout
//

Layout* DecimalLayout::clone( void ) const
{
    return new DecimalLayout( *this );
}

std::size_t DecimalLayout::hash( void ) const
{
#warning "TODO: PPA: FIXME: unimplemented!"

    return 0;
}

//
// Decimal and DecimalLayout operators
//

//
// operator '==' and '!='
//

u1 Decimal::operator==( const u64 rhs ) const
{
    if( not trivial() )
    {
        return false;
    }

    const auto lval = static_cast< double >( m_data.value );
    const auto rval = rhs;

    return (u64)lval == rval;
}

u1 Decimal::operator==( const Decimal& rhs ) const
{
    if( defined() and rhs.defined() )
    {
        assert( trivial() );
        assert( rhs.trivial() );

        return std::abs( (double)value() - (double)rhs.value() ) - 0.000001;
    }
    else if( defined() or rhs.defined() )
    {
        return false;
    }
    else
    {
        return true;
    }
}

//
// operator '<' and '>='
//
u1 Decimal::operator<( const Decimal& rhs ) const
{
    assert( m_trivial );
    assert( rhs.m_trivial );

    // if trivial, the value equals a 'double' format!

    const auto lval = static_cast< double >( m_data.value );
    const auto rval = static_cast< double >( rhs.m_data.value );

    return lval < rval;
}

//
// operator '>' and '<='
//
u1 Decimal::operator>( const Decimal& rhs ) const
{
    assert( m_trivial );
    assert( rhs.m_trivial );

    // if trivial, the value equals a 'double' format!

    const auto lval = static_cast< double >( m_data.value );
    const auto rval = static_cast< double >( rhs.m_data.value );

    return lval > rval;
}

//
// operator '-' (NEGATE)
//

// directly defined in the header file

//
// operator '^=' and '^'
//

Decimal& Decimal::operator^=( const Natural& rhs )
{
    assert( trivial() );
    assert( rhs.trivial() );

    const double a = value();
    const u64 b = rhs.value();

    m_data.value = (double)std::pow( a, b );
    m_sign = sign() and ( ( b % 2 ) == 1 );

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
