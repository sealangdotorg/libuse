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

#include "uts/main.h"

using namespace libstdhl;

TEST( libstdhl_cpp_rational, str_decimal1 )
{
    auto i = Rational( "5", Type::Radix::DECIMAL );

    EXPECT_EQ( i.words().size(), 1 );
    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.word( 0 ), 5 );
}

TEST( libstdhl_cpp_rational, str_decimal4 )
{
    auto i = Rational( "2/31", Type::Radix::DECIMAL );

    EXPECT_EQ( i.words().size(), 2 );
    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.word( 0 ), 2 );
    EXPECT_EQ( i.word( 1 ), 31 );
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
