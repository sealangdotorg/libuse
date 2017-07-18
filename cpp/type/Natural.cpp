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

#include "Natural.h"

#include "../String.h"

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
        throw std::domain_error(
            "Natural type cannot be initialized with negative Integer value" );
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
        throw std::domain_error(
            "invalid negative '" + value + "' Natural literal" );
    }

    return createNatural( tmp );
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
