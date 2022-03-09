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

#include <libstdhl/Test>

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

TEST( libstdhl_cpp_Enum_Flags, ctor_init_list )
{
    // PREPARE
    Colors colors( { Color::Red, Color::Blue } );

    EXPECT_FALSE( colors.isSet( Color::Green ) );
    EXPECT_TRUE( colors.isSet( Color::Red ) );
    EXPECT_TRUE( colors.isSet( Color::Blue ) );
}

TEST( libstdhl_cpp_Enum_Flags, star_operator_intersection )
{
    // PREPARE
    Colors colors1;
    colors1.set( Color::Red );

    Colors colors2;
    colors2.set( Color::Blue );

    Colors colors3;
    colors3.set( Color::Red );
    colors3.set( Color::Blue );

    // WHEN
    const Colors mergedColors1 = colors1 * colors2;

    // WHEN
    const Colors mergedColors2 = colors1 * colors3;

    // THEN
    EXPECT_FALSE( mergedColors1.isSet( Color::Red ) );
    EXPECT_FALSE( mergedColors1.isSet( Color::Blue ) );
    EXPECT_FALSE( mergedColors1.isSet( Color::Green ) );

    // THEN
    EXPECT_TRUE( mergedColors2.isSet( Color::Red ) );
    EXPECT_FALSE( mergedColors2.isSet( Color::Blue ) );
    EXPECT_FALSE( mergedColors2.isSet( Color::Green ) );
}

TEST( libstdhl_cpp_Enum_Flags, foreach_iterate_over_no_flags )
{
    // PREPARE
    Colors input;
    Colors output;
    EXPECT_TRUE( output.empty() );

    // WHEN
    input.foreach( [ &output ]( const Color c ) -> u1 {
        output.set( c );
        return true;
    } );

    // THEN
    EXPECT_TRUE( output.empty() );
}

TEST( libstdhl_cpp_Enum_Flags, foreach_iterate_over_partial_flags )
{
    // PREPARE
    Colors input;
    input.set( Color::Green );
    Colors output;
    EXPECT_TRUE( output.empty() );

    // WHEN
    input.foreach( [ &output ]( const Color c ) -> u1 {
        output.set( c );
        return true;
    } );

    // THEN
    EXPECT_TRUE( not output.empty() );
    EXPECT_FALSE( output.isSet( Color::Red ) );
    EXPECT_TRUE( output.isSet( Color::Green ) );
    EXPECT_FALSE( output.isSet( Color::Blue ) );
}

TEST( libstdhl_cpp_Enum_Flags, foreach_iterate_over_all_set_flags )
{
    // PREPARE
    Colors input;
    input.set( Color::Red );
    input.set( Color::Green );
    input.set( Color::Blue );
    Colors output;
    EXPECT_TRUE( output.empty() );

    // WHEN
    input.foreach( [ &output ]( const Color c ) -> u1 {
        output.set( c );
        return true;
    } );

    // THEN
    EXPECT_TRUE( not output.empty() );
    EXPECT_TRUE( output.isSet( Color::Red ) );
    EXPECT_TRUE( output.isSet( Color::Green ) );
    EXPECT_TRUE( output.isSet( Color::Blue ) );
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
