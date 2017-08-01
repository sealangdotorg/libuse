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
    EXPECT_NE( b.ptr(), layout ); // should clone layout
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
    EXPECT_NE( b.ptr(), layout ); // should clone layout
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
