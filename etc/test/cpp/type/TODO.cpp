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

// #define TEST_LIBSTDHL_CPP_TYPE__TO( RADIX, VALUE, STRING )                     \
//     TEST( libstdhl_cpp_type, to_##RADIX )                                      \
//     {                                                                          \
//         EXPECT_STREQ( Type( VALUE ).to< Type::RADIX >().c_str(), STRING );     \
//     }

// TEST_LIBSTDHL_CPP_TYPE__TO( BINARY, 123, "0b1111011" );
// TEST_LIBSTDHL_CPP_TYPE__TO( OCTAL, 123, "0c173" );
// TEST_LIBSTDHL_CPP_TYPE__TO( DECIMAL, 123, "123" );
// TEST_LIBSTDHL_CPP_TYPE__TO( HEXADECIMAL, 123, "0x7b" );

// TEST( libstdhl_cpp_type, to_string )
// {
//     EXPECT_STREQ( Type( 123 ).to_string( Type::BINARY ).c_str(), "1111011" );
//     EXPECT_STREQ( Type( 123 ).to_string( Type::OCTAL ).c_str(), "173" );
//     EXPECT_STREQ( Type( 123 ).to_string( Type::DECIMAL ).c_str(), "123" );
//     EXPECT_STREQ( Type( 123 ).to_string( Type::HEXADECIMAL ).c_str(), "7b" );
//     EXPECT_STREQ( Type( 123 ).to_string( Type::SEXAGESIMAL ).c_str(), "23" );
//     EXPECT_STREQ( Type( 123 ).to_string( Type::RADIX64 ).c_str(), "1X" );
// }

// TEST( libstdhl_cpp_type, check_division )
// {
//     Type example( std::initializer_list< u64 >{ 0x0000000044448888, 0x2 } );

//     example /= 2;

//     const std::vector< u64 > result2 = { 0x0000000022224444, 0x1 };
//     EXPECT_TRUE( example == result2 );

//     example /= 3;

//     const std::vector< u64 > result3 = { 0x5555555560b616c1, 0x0 };
//     EXPECT_TRUE( example == result3 );
// }

// TEST( libstdhl_cpp_type, divide_by_2_and_print_until_zero )
// {
//     Type example( std::initializer_list< u64 >{ 0x0000000044448888, 0x2 } );

//     printf( "%s\n", example.to_string( Type::HEXADECIMAL ).c_str() );

//     while( example > 0 )
//     {
//         example /= 2;
//         printf( "%s\n", example.to_string( Type::HEXADECIMAL ).c_str() );
//     }
// }

// TEST( libstdhl_cpp_type, divide_by_3_and_print_until_zero )
// {
//     Type example( std::initializer_list< u64 >{ 0x0000000044448888, 0x2 } );

//     printf( "%s\n", example.to_string( Type::HEXADECIMAL ).c_str() );

//     while( example > 0 )
//     {
//         example /= 3;
//         printf( "%s\n", example.to_string( Type::HEXADECIMAL ).c_str() );
//     }
// }

// TEST( libstdhl_cpp_type, check_to_string_of_big_value )
// {
//     EXPECT_STREQ( Type( std::initializer_list< u64 >{ 0x123, 0x456, 0x789 } )
//                       .to< Type::HEXADECIMAL >()
//                       .c_str(),
//         "0x"
//         "789"
//         "0000000000000456"
//         "0000000000000123" );
// }

// TEST( libstdhl_cpp_type, operator_div )
// {
//     for( u64 v = 1; v < UINT64_MAX / 2; v *= 123 )
//     {
//         Type t( std::initializer_list< u64 >{ v, 0 } );

//         Type a;
//         Type b;

//         a = t / 2;
//         a = a / 2;

//         b = t / 4;

//         EXPECT_TRUE( a == b );
//     }
// }

// TEST( libstdhl_cpp_type, hash_value_example )
// {
//     Type a( std::initializer_list< u64 >{ 0x0000000044448888, 0x2 } );
//     Type b( std::initializer_list< u64 >{ 0x0000000044448888, 0x2 } );
//     Type c( std::initializer_list< u64 >{ 0x0012340556788888, 0x4 } );

//     EXPECT_EQ( Hash::value( a ), Hash::value( b ) );

//     EXPECT_NE( Hash::value( a ), Hash::value( c ) );
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
