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

#include "gtest/gtest.h"

#include "libstdhl.h"

using namespace libstdhl;

enum class Color
{
    Red,
    Green,
    Blue
};

using Colors = Enum::Flags< Color >;

TEST( libstdhl_cpp_Enum_Flags, initially_no_color_should_be_set )
{
    // WHEN
    Colors colors;

    // THEN
    EXPECT_TRUE( colors.empty() );
    EXPECT_FALSE( colors.isSet( Color::Red ) );
    EXPECT_FALSE( colors.isSet( Color::Green ) );
    EXPECT_FALSE( colors.isSet( Color::Blue ) );
}

TEST( libstdhl_cpp_Enum_Flags, set_should_persist )
{
    // WHEN
    Colors colors;
    colors.set( Color::Green );

    // THEN
    EXPECT_TRUE( colors.isSet( Color::Green ) );
    EXPECT_TRUE( colors & Color::Green );
}

TEST( libstdhl_cpp_Enum_Flags, or_assign_operator_set_should_persist )
{
    // WHEN
    Colors colors;
    colors |= Color::Green;

    // THEN
    EXPECT_TRUE( colors.isSet( Color::Green ) );
    EXPECT_TRUE( colors & Color::Green );
}

TEST( libstdhl_cpp_Enum_Flags, or_operator_set_should_persist )
{
    // WHEN
    Colors colors;
    colors = colors | Color::Green;

    // THEN
    EXPECT_TRUE( colors.isSet( Color::Green ) );
    EXPECT_TRUE( colors & Color::Green );
}

TEST( libstdhl_cpp_Enum_Flags, empty_should_be_false_if_some_value_is_set )
{
    // WHEN
    Colors colors;
    colors.set( Color::Green );

    // THEN
    EXPECT_FALSE( colors.empty() );
}

TEST( libstdhl_cpp_Enum_Flags, set_should_use_one_bit_for_each_enum_value )
{
    // WHEN
    Colors colors;
    colors.set( Color::Green );
    colors.set( Color::Blue );

    // THEN
    EXPECT_FALSE( colors.isSet( Color::Red ) );
    EXPECT_TRUE( colors.isSet( Color::Green ) );
    EXPECT_TRUE( colors.isSet( Color::Blue ) );
}

TEST( libstdhl_cpp_Enum_Flags, unset_should_persist )
{
    // PREPARE
    Colors colors;
    colors.set( Color::Green );
    EXPECT_TRUE( colors.isSet( Color::Green ) );

    // WHEN
    colors.unset( Color::Green );

    // THEN
    EXPECT_FALSE( colors.isSet( Color::Green ) );
    EXPECT_FALSE( colors & Color::Green );
}

TEST( libstdhl_cpp_Enum_Flags, flip_should_set_if_currently_unset )
{
    // WHEN
    Colors colors;
    colors.flip( Color::Red );

    // THEN
    EXPECT_TRUE( colors.isSet( Color::Red ) );
}

TEST( libstdhl_cpp_Enum_Flags, caret_assign_operator_should_set_if_currently_unset )
{
    // WHEN
    Colors colors;
    colors ^= Color::Red;

    // THEN
    EXPECT_TRUE( colors.isSet( Color::Red ) );
}

TEST( libstdhl_cpp_Enum_Flags, caret_operator_should_set_if_currently_unset )
{
    // WHEN
    Colors colors;
    colors = colors ^ Color::Red;

    // THEN
    EXPECT_TRUE( colors.isSet( Color::Red ) );
}

TEST( libstdhl_cpp_Enum_Flags, flip_should_unset_if_currently_set )
{
    // PREPARE
    Colors colors;
    colors.set( Color::Red );
    EXPECT_TRUE( colors.isSet( Color::Red ) );

    // WHEN
    colors.flip( Color::Red );

    // THEN
    EXPECT_FALSE( colors.isSet( Color::Red ) );
}

TEST( libstdhl_cpp_Enum_Flags, caret_assign_operator_flip_should_unset_if_currently_set )
{
    // PREPARE
    Colors colors;
    colors.set( Color::Red );
    EXPECT_TRUE( colors.isSet( Color::Red ) );

    // WHEN
    colors ^= Color::Red;

    // THEN
    EXPECT_FALSE( colors.isSet( Color::Red ) );
}

TEST( libstdhl_cpp_Enum_Flags, caret_operator_flip_should_unset_if_currently_set )
{
    // PREPARE
    Colors colors;
    colors.set( Color::Red );
    EXPECT_TRUE( colors.isSet( Color::Red ) );

    // WHEN
    colors = colors ^ Color::Red;

    // THEN
    EXPECT_FALSE( colors.isSet( Color::Red ) );
}

TEST( libstdhl_cpp_Enum_Flags, or_operator_merge_should_persist )
{
    // PREPARE
    Colors colors1;
    colors1.set( Color::Red );

    Colors colors2;
    colors2.set( Color::Blue );

    // WHEN
    const Colors mergedColors = colors1 | colors2;

    // THEN
    EXPECT_TRUE( mergedColors.isSet( Color::Red ) );
    EXPECT_TRUE( mergedColors.isSet( Color::Blue ) );
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
