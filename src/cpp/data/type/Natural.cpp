//
//  Copyright (C) 2014-2019 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
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

#include "Natural.h"

#include <libstdhl/String>

#include <cassert>

using namespace libstdhl;
using namespace Type;

//
// Type::create*
//

Natural Type::createNatural( const std::string& value, const Radix radix )
{
    return Natural::fromString( value, radix );
}

Natural Type::createNatural( const Integer& value )
{
    if( value.sign() )
    {
        throw std::domain_error( "Natural type cannot be initialized with negative Integer value" );
    }

    if( value.trivial() )
    {
        return Natural( value.value(), false );
    }
    else
    {
        return Natural( value.ptr() );
    }
}

Natural Type::createNatural( const u64 value )
{
    return Natural( value, false );
}

//
// Natural
//

Natural Natural::fromString( const std::string& value, const Type::Radix radix )
{
    Integer tmp = Integer::fromString( value, radix );

    if( tmp.sign() )
    {
        throw std::domain_error( "invalid negative '" + value + "' Natural literal" );
    }

    return createNatural( tmp );
}

u1 Natural::isSet( const u64 bit ) const
{
    assert( bit > 0 );
    assert( bit <= 64 );
    assert( trivial() );

    return value() & ( 1 << ( bit - 1 ) );
}

//
// operator '^=' and '^'
//

Natural& Natural::operator^=( const Natural& rhs )
{
    assert( this->trivial() );
    assert( rhs.trivial() );

    const u64 a = value();
    const u64 b = rhs.value();

    m_data.value = a ^ b;

    return *this;
}

//
// operator '|=' and '|'
//

Natural& Natural::operator|=( const Natural& rhs )
{
    assert( this->trivial() );
    assert( rhs.trivial() );

    const u64 a = value();
    const u64 b = rhs.value();

    m_data.value = a | b;

    return *this;
}

//
// operator '&=' and '&'
//

Natural& Natural::operator&=( const Natural& rhs )
{
    assert( this->trivial() );
    assert( rhs.trivial() );

    const u64 a = value();
    const u64 b = rhs.value();

    m_data.value = a & b;

    return *this;
}

//
// operator '<<=' and '<<'
//

Natural& Natural::operator<<=( const u64 rhs )
{
    static_cast< Integer* >( this )->operator<<=( rhs );
    return *this;
}

Natural& Natural::operator<<=( const Natural& rhs )
{
    static_cast< Integer* >( this )->operator<<=( rhs );
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
