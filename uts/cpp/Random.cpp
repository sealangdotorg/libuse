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

template < typename T >
void test_random( T from, T to, u32 iteration = 256 )
{
    for( u32 i = 0; i < iteration; i++ )
    {
        // create random number with API default range
        T number = Random::uniform< T >();

        ASSERT_GE( number, std::numeric_limits< T >::min() );
        ASSERT_LE( number, std::numeric_limits< T >::max() );

        // create random number in test range
        number = Random::uniform< T >( from, to );

        ASSERT_GE( number, from );
        ASSERT_LE( number, to );
    }
}

#define TEST_RANDOM( T, MIN, MAX )                                             \
    TEST( libstdhl_cpp_Random, uniform_##T )                                   \
    {                                                                          \
        test_random< T >( MIN, MAX );                                          \
    }

TEST_RANDOM( u8, 12, 98 );
TEST_RANDOM( u16, 1234, 9876 );
TEST_RANDOM( u32, 123456, 987654 );
TEST_RANDOM( u64, 123456789, 987654321 );

TEST_RANDOM( i8, -12, 34 );
TEST_RANDOM( i16, -1234, 5678 );
TEST_RANDOM( i32, -123456, 789012 );
TEST_RANDOM( i64, -123456789, 123456789 );

// TEST( libstdhl_cpp_Random, uniform_i64 )
// {
//     i64 number = Random::uniform< i64 >();

//     ASSERT_GE( number, std::numeric_limits< i64 >::min() );
//     ASSERT_LE( number, std::numeric_limits< i64 >::max() );
// }

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
