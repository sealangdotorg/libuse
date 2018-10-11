//
//  Copyright (C) 2014-2018 CASM Organization <https://casm-lang.org>
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

#include <libstdhl/Test>

using namespace libstdhl;
using namespace Type;

TEST( libstdhl_cpp_type_data, undef_ctor )
{
    Data a;

    EXPECT_EQ( a.value(), 0 );
    EXPECT_EQ( a.trivial(), false );
    EXPECT_EQ( a.defined(), false );
    EXPECT_EQ( a.sign(), false );
}

TEST( libstdhl_cpp_type_data, trivial_ctor )
{
    Data a( 1234, false );

    EXPECT_EQ( a.value(), 1234 );
    EXPECT_EQ( a.trivial(), true );
    EXPECT_EQ( a.defined(), true );
    EXPECT_EQ( a.sign(), false );
}

TEST( libstdhl_cpp_type_data, trivial_ctor_neg )
{
    Data a( 1234, true );

    EXPECT_EQ( a.value(), 1234 );
    EXPECT_EQ( a.trivial(), true );
    EXPECT_EQ( a.defined(), true );
    EXPECT_EQ( a.sign(), true );
}

TEST( libstdhl_cpp_type_data, trivial_ctor_copy )
{
    Data a( 1234, false );
    Data b( a );

    EXPECT_EQ( b.value(), 1234 );
    EXPECT_EQ( b.trivial(), true );
    EXPECT_EQ( b.defined(), true );
    EXPECT_EQ( b.sign(), false );
}

TEST( libstdhl_cpp_type_data, trivial_oper_copy )
{
    Data a( 1234, false );
    Data b = a;

    EXPECT_EQ( b.value(), 1234 );
    EXPECT_EQ( b.trivial(), true );
    EXPECT_EQ( b.defined(), true );
    EXPECT_EQ( b.sign(), false );
}

TEST( libstdhl_cpp_type_data, trivial_ctor_move )
{
    Data a( 1234, false );
    Data b( std::move( a ) );

    EXPECT_EQ( b.value(), 1234 );
    EXPECT_EQ( b.trivial(), true );
    EXPECT_EQ( b.defined(), true );
    EXPECT_EQ( b.sign(), false );

    EXPECT_EQ( a.value(), 0 );
    EXPECT_EQ( a.trivial(), false );
    EXPECT_EQ( a.defined(), false );
    EXPECT_EQ( a.sign(), false );
}

TEST( libstdhl_cpp_type_data, trivial_oper_move )
{
    Data a( 1234, false );
    Data b = std::move( a );

    EXPECT_EQ( b.value(), 1234 );
    EXPECT_EQ( b.trivial(), true );
    EXPECT_EQ( b.defined(), true );
    EXPECT_EQ( b.sign(), false );

    EXPECT_EQ( a.value(), 0 );
    EXPECT_EQ( a.trivial(), false );
    EXPECT_EQ( a.defined(), false );
    EXPECT_EQ( a.sign(), false );
}

// ---

TEST( libstdhl_cpp_type_data, pointer_ctor )
{
    auto layout = new IntegerLayout( 10, 20 );
    Data a( layout );

    EXPECT_EQ( a.ptr(), layout );
    EXPECT_EQ( a.trivial(), false );
    EXPECT_EQ( a.defined(), true );
    EXPECT_EQ( a.sign(), false );
}

TEST( libstdhl_cpp_type_data, pointer_ctor_copy )
{
    auto layout = new IntegerLayout( 10, 20 );
    Data a( layout );
    Data b( a );

    EXPECT_NE( b.ptr(), nullptr );
    EXPECT_NE( b.ptr(), layout );  // should clone layout
    EXPECT_EQ( b.trivial(), false );
    EXPECT_EQ( b.defined(), true );
    EXPECT_EQ( b.sign(), false );
}

TEST( libstdhl_cpp_type_data, pointer_oper_copy )
{
    auto layout = new IntegerLayout( 10, 20 );
    Data a( layout );
    Data b = a;

    EXPECT_NE( b.ptr(), nullptr );
    EXPECT_NE( b.ptr(), layout );  // should clone layout
    EXPECT_EQ( b.trivial(), false );
    EXPECT_EQ( b.defined(), true );
    EXPECT_EQ( b.sign(), false );
}

TEST( libstdhl_cpp_type_data, pointer_ctor_move )
{
    auto layout = new IntegerLayout( 10, 20 );
    Data a( layout );
    Data b( std::move( a ) );

    EXPECT_EQ( b.ptr(), layout );
    EXPECT_EQ( b.trivial(), false );
    EXPECT_EQ( b.defined(), true );
    EXPECT_EQ( b.sign(), false );

    EXPECT_EQ( a.ptr(), nullptr );
    EXPECT_EQ( a.trivial(), false );
    EXPECT_EQ( a.defined(), false );
    EXPECT_EQ( a.sign(), false );
}

TEST( libstdhl_cpp_type_data, pointer_oper_move )
{
    auto layout = new IntegerLayout( 10, 20 );
    Data a( layout );
    Data b = std::move( a );

    EXPECT_EQ( b.ptr(), layout );
    EXPECT_EQ( b.trivial(), false );
    EXPECT_EQ( b.defined(), true );
    EXPECT_EQ( b.sign(), false );

    EXPECT_EQ( a.ptr(), nullptr );
    EXPECT_EQ( a.trivial(), false );
    EXPECT_EQ( a.defined(), false );
    EXPECT_EQ( a.sign(), false );
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
