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

#include "main.h"

using namespace libstdhl;

TEST( libstdhl_cpp_Hash, combine_example )
{
    EXPECT_EQ( libstdhl::Hash::combine( 123, 456 ),
        libstdhl::Hash::combine( 123, 456 ) );

    EXPECT_NE( libstdhl::Hash::combine( 123, 456 ),
        libstdhl::Hash::combine( 456, 123 ) );
}

TEST( libstdhl_cpp_Hash, value_u64_example )
{
    EXPECT_EQ( Hash::value( 1234 ), Hash::value( 1234 ) );

    EXPECT_NE( Hash::value( 1234 ), Hash::value( 4321 ) );
}

TEST( libstdhl_cpp_Hash, value_string_example )
{
    EXPECT_EQ( Hash::value( "foobar" ), Hash::value( "foobar" ) );

    EXPECT_NE( Hash::value( "foobar" ), Hash::value( "FOOBAR" ) );
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
