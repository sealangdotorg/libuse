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

#include "Floating.h"

using namespace libstdhl;
using namespace Type;

Floating::Floating( double value )
: Layout( ( value >= 0 ? (u64)value : ( u64 )( -value ) ), ( value < 0 ) )
{
    // IEEE 754 double-precision binary floating-point format
    static_assert(
        sizeof( double ) == 8, " double shall be a byte-size of 8 " );
}

Floating::Floating( const std::string& value )
: Layout()
{
#warning "TODO"    
}

u1 Floating::operator==( const Floating& rhs ) const
{
    if( defined() and rhs.defined() )
    {
        assert( trivial() );
        assert( rhs.trivial() );

        return value() == rhs.value();
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
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
