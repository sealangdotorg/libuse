//
//  Copyright (c) 2014-2017 Philipp Paulweber
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
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

#include "c/type.h"

#define TEST_NAME                                                              \
    ( std::string(::testing::UnitTest::GetInstance()                           \
                      ->current_test_info()                                    \
                      ->test_case_name() )                                     \
        + "." + std::string(::testing::UnitTest::GetInstance()                 \
                                ->current_test_info()                          \
                                ->name() ) )                                   \
        .c_str()

TEST( libstdhl_type_c, sizeof_u1 )
{
    ASSERT_EQ( sizeof( u1 ), 1 );
}

TEST( libstdhl_type_c, sizeof_u8 )
{
    ASSERT_EQ( sizeof( u8 ), 1 );
}

TEST( libstdhl_type_c, sizeof_u16 )
{
    ASSERT_EQ( sizeof( u16 ), 2 );
}

TEST( libstdhl_type_c, sizeof_u32 )
{
    ASSERT_EQ( sizeof( u32 ), 4 );
}

TEST( libstdhl_type_c, sizeof_u64 )
{
    ASSERT_EQ( sizeof( u64 ), 8 );
}

TEST( libstdhl_type_c, sizeof_i1 )
{
    ASSERT_EQ( sizeof( i1 ), 1 );
}

TEST( libstdhl_type_c, sizeof_i8 )
{
    ASSERT_EQ( sizeof( i8 ), 1 );
}

TEST( libstdhl_type_c, sizeof_i16 )
{
    ASSERT_EQ( sizeof( i16 ), 2 );
}

TEST( libstdhl_type_c, sizeof_i32 )
{
    ASSERT_EQ( sizeof( i32 ), 4 );
}

TEST( libstdhl_type_c, sizeof_i64 )
{
    ASSERT_EQ( sizeof( i64 ), 8 );
}
