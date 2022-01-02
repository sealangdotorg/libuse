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

#include <libstdhl/data/type/Integer>
#include <libstdhl/data/type/Natural>

#include <cmath>

using namespace libstdhl;
using namespace Type;

TEST( libstdhl_cpp_type_integer, u64_valid )
{
    u64 number = 1234;

    auto i = createInteger( number );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), true );
    EXPECT_EQ( i.value(), number );
}

TEST( libstdhl_cpp_type_integer, i64_positive )
{
    i64 number = 123456789;

    auto i = createInteger( number );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), true );
    EXPECT_EQ( i.value(), number * ( i.sign() ? -1 : 1 ) );
}

TEST( libstdhl_cpp_type_integer, i64_negative )
{
    i64 number = -123456789;

    auto i = createInteger( number );

    EXPECT_EQ( i.sign(), true );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), true );
    EXPECT_EQ( i.value(), number * ( i.sign() ? -1 : 1 ) );
}

TEST( libstdhl_cpp_type_integer, str_binary4 )
{
    auto i = createInteger( "0111", Type::Radix::BINARY );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), true );
    EXPECT_EQ( i.value(), 7 );
}

TEST( libstdhl_cpp_type_integer, str_binary64 )
{
    auto i = createInteger(
        "01111011"  // 7b
        "11110010"  // f2
        "00000100"  // 04
        "11101110"  // ee
        "01111011"  // 7b
        "11110010"  // f2
        "00000100"  // 04
        "11101101"  // ed
        ,
        Type::Radix::BINARY );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), true );
    EXPECT_EQ( i.value(), 0x7bf204ee7bf204ed );
}

TEST( libstdhl_cpp_type_integer, str_binary64_max )
{
    auto i = createInteger(
        "11111111"  // ff
        "11111111"  // ff
        "11111111"  // ff
        "11111111"  // ff
        "11111111"  // ff
        "11111111"  // ff
        "11111111"  // ff
        "11111111"  // ff
        ,
        Type::Radix::BINARY );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), true );
    EXPECT_EQ( i.value(), UINT64_MAX );
}

TEST( libstdhl_cpp_type_integer, str_binary70 )
{
    auto i = createInteger(
        "101010"    // 2a
        "01111011"  // 7b
        "11110010"  // f2
        "00000100"  // 04
        "11101110"  // ee
        "01111011"  // 7b
        "11110010"  // f2
        "00000100"  // 04
        "11101101"  // ed
        ,
        Type::Radix::BINARY );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), false );
    EXPECT_EQ( i[ 1 ], 0x2a );
    EXPECT_EQ( i[ 0 ], 0x7bf204ee7bf204ed );
}

TEST( libstdhl_cpp_type_integer, str_binary522 )
{
    auto i = createInteger(
        "11"        // 03
        "11110010"  // f2
        "01111011"  // 7b // 512
        "11110010"  // f2
        "00000100"  // 04
        "11101110"  // ee
        "01111011"  // 7b // -
        "11110010"  // f2
        "00000100"  // 04
        "11101101"  // ed
        "01111011"  // 7b // -
        "11110010"  // f2
        "00000100"  // 04
        "11101110"  // ee
        "01111011"  // 7b // -
        "11110010"  // f2
        "00000100"  // 04
        "11101101"  // ed
        "01111011"  // 7b // -
        "11110010"  // f2
        "00000100"  // 04
        "11101110"  // ee
        "01111011"  // 7b // -
        "11110010"  // f2
        "00000100"  // 04
        "11101101"  // ed
        "01111011"  // 7b // -
        "11110010"  // f2
        "00000100"  // 04
        "11101110"  // ee
        "01111011"  // 7b // -
        "11110010"  // f2
        "00000100"  // 04
        "11101101"  // ed
        "01111011"  // 7b // 256
        "11110010"  // f2
        "00000100"  // 04
        "11101110"  // ee
        "01111011"  // 7b // -
        "11110010"  // f2
        "00000100"  // 04
        "11101101"  // ed
        "01111011"  // 7b // -
        "11110010"  // f2
        "00000100"  // 04
        "11101110"  // ee
        "01111011"  // 7b // -
        "11110010"  // f2
        "00000100"  // 04
        "11101101"  // ed
        "01111011"  // 7b // 128
        "11110010"  // f2
        "00000100"  // 04
        "11101110"  // ee
        "01111011"  // 7b // -
        "11110010"  // f2
        "00000100"  // 04
        "11101101"  // ed
        "01111011"  // 7b // 64
        "11110010"  // f2
        "00000100"  // 04
        "11101110"  // ee
        "01111011"  // 7b // -
        "11110010"  // f2
        "00000100"  // 04
        "11101101"  // ed // 8
        ,
        Type::Radix::BINARY );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), false );
    EXPECT_EQ( i[ 8 ], 0x03f2 );
    EXPECT_EQ( i[ 7 ], 0x7bf204ee7bf204ed );
    EXPECT_EQ( i[ 6 ], 0x7bf204ee7bf204ed );
    EXPECT_EQ( i[ 5 ], 0x7bf204ee7bf204ed );
    EXPECT_EQ( i[ 4 ], 0x7bf204ee7bf204ed );
    EXPECT_EQ( i[ 3 ], 0x7bf204ee7bf204ed );
    EXPECT_EQ( i[ 2 ], 0x7bf204ee7bf204ed );
    EXPECT_EQ( i[ 1 ], 0x7bf204ee7bf204ed );
    EXPECT_EQ( i[ 0 ], 0x7bf204ee7bf204ed );
}

TEST( libstdhl_cpp_type_integer, str_octal5 )
{
    auto i = createInteger( "54321", Type::Radix::OCTAL );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), true );
    EXPECT_EQ( i.value(), 054321 );
}

TEST( libstdhl_cpp_type_integer, str_octal21 )
{
    auto i = createInteger( "543217060554321706053", Type::Radix::OCTAL );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), true );
    EXPECT_EQ( i.value(), 0543217060554321706053 );
}

TEST( libstdhl_cpp_type_integer, str_octal22_u64max )
{
    auto i = createInteger( "1777777777777777777777", Type::Radix::OCTAL );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), true );
    EXPECT_EQ( i.value(), UINT64_MAX );
}

TEST( libstdhl_cpp_type_integer, str_octal22_no_overlap )
{
    auto i = createInteger( "1543217060554321706053", Type::Radix::OCTAL );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), true );
    EXPECT_EQ( i.value(), 01543217060554321706053 );
}

TEST( libstdhl_cpp_type_integer, str_octal22_with_overlap )
{
    auto i = createInteger( "7543217060554321706053", Type::Radix::OCTAL );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), false );
    EXPECT_EQ( i[ 1 ], 03 );
    EXPECT_EQ( i[ 0 ], 01543217060554321706053 );
}

TEST( libstdhl_cpp_type_integer, str_octal23 )
{
    auto i = createInteger( "337543217060554321706053", Type::Radix::OCTAL );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), false );
    EXPECT_EQ( i[ 1 ], 0157 );
    EXPECT_EQ( i[ 0 ], 01543217060554321706053 );
}

TEST( libstdhl_cpp_type_integer, str_hexadecimal2 )
{
    auto i = createInteger( "e1", Type::Radix::HEXADECIMAL );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), true );
    EXPECT_EQ( i.value(), 0xe1 );
}

TEST( libstdhl_cpp_type_integer, str_hexadecimal16_max )
{
    auto i = createInteger( "ffffffffffffffff", Type::Radix::HEXADECIMAL );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), true );
    EXPECT_EQ( i.value(), UINT64_MAX );
}

TEST( libstdhl_cpp_type_integer, str_hexadecimal17 )
{
    auto i = createInteger(
        "f"
        "eedbee0123456789",
        Type::Radix::HEXADECIMAL );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), false );
    EXPECT_EQ( i[ 1 ], 0xf );
    EXPECT_EQ( i[ 0 ], 0xeedbee0123456789 );
}

TEST( libstdhl_cpp_type_integer, str_hexadecimal140 )
{
    auto i = createInteger(
        "affe"
        "feedbeef13579024"  //
        "9024feedbeef1357"  // 512, 128
        "13579024feedbeef"  //
        "beef13579024feed"  //
        "feedbeef13579024"  //
        "edbeef13579024fe"  // 256, 64
        "ef13579024feedbe"  //
        "579024feedbeef13"  // 128, 32
        "24feedbeef135790"  // 64,  16
        ,
        Type::Radix::HEXADECIMAL );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), false );
    EXPECT_EQ( i[ 9 ], 0xaffe );
    EXPECT_EQ( i[ 8 ], 0xfeedbeef13579024 );
    EXPECT_EQ( i[ 7 ], 0x9024feedbeef1357 );  // 512, 128
    EXPECT_EQ( i[ 6 ], 0x13579024feedbeef );  //
    EXPECT_EQ( i[ 5 ], 0xbeef13579024feed );  //
    EXPECT_EQ( i[ 4 ], 0xfeedbeef13579024 );  //
    EXPECT_EQ( i[ 3 ], 0xedbeef13579024fe );  // 256, 64
    EXPECT_EQ( i[ 2 ], 0xef13579024feedbe );  //
    EXPECT_EQ( i[ 1 ], 0x579024feedbeef13 );  // 128, 32
    EXPECT_EQ( i[ 0 ], 0x24feedbeef135790 );  // 64,  16
}

TEST( libstdhl_cpp_type_integer, str_decimal3 )
{
    auto i = createInteger( "123", Type::Radix::DECIMAL );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), true );
    EXPECT_EQ( i.value(), 123 );
}

TEST( libstdhl_cpp_type_integer, str_decimal3_sign )
{
    auto i = createInteger( "-123", Type::Radix::DECIMAL );

    EXPECT_EQ( i.sign(), true );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), true );
    EXPECT_EQ( i.value(), 123 );
}

TEST( libstdhl_cpp_type_integer, str_decimal8 )
{
    auto i = createInteger( "64242662", Type::Radix::DECIMAL );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), true );
    EXPECT_EQ( i.value(), 64242662 );
}

TEST( libstdhl_cpp_type_integer, str_decimal8_sign )
{
    auto i = createInteger( "-64242662", Type::Radix::DECIMAL );

    EXPECT_EQ( i.sign(), true );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), true );
    EXPECT_EQ( i.value(), 64242662 );
}

TEST( libstdhl_cpp_type_integer, str_decimal20_u64max )
{
    auto i = createInteger(
        "1844674407"  // 20
        "3709551615"  // 10
        ,
        Type::Radix::DECIMAL );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), true );
    EXPECT_EQ( i.value(), UINT64_MAX );
    EXPECT_EQ( i.value(), 0xffffffffffffffff );
}

TEST( libstdhl_cpp_type_integer, str_decimal20_u64max_plus_one )
{
    auto i = createInteger(
        "1844674407"  // 20
        "3709551616"  // 10
        ,
        Type::Radix::DECIMAL );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), false );
    EXPECT_EQ( i[ 1 ], 0x1 );
    EXPECT_EQ( i[ 0 ], 0x0 );
}

TEST( libstdhl_cpp_type_integer, str_decimal21 )
{
    auto i = createInteger(
        "3"
        "7384632100"  // 20
        "9187212372"  // 10
        ,
        Type::Radix::DECIMAL );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), false );
    EXPECT_EQ( i[ 1 ], 0x14 );  // 14 4428F03C3341CC54
    EXPECT_EQ( i[ 0 ], 0x4428f03c3341cc54 );
}

TEST( libstdhl_cpp_type_integer, str_decimal21_sign )
{
    auto i = createInteger(
        "-3"
        "7384632100"  // 20
        "9187212372"  // 10
        ,
        Type::Radix::DECIMAL );

    EXPECT_EQ( i.sign(), true );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), false );
    EXPECT_EQ( i[ 1 ], 0x14 );
    EXPECT_EQ( i[ 0 ], 0x4428f03c3341cc54 );
}

TEST( libstdhl_cpp_type_integer, str_decimal23 )
{
    auto i = createInteger(
        "123"
        "7384632100"  // 20
        "9187212372"  // 10
        ,
        Type::Radix::DECIMAL );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), false );
    EXPECT_EQ( i[ 1 ], 0x29e );
    EXPECT_EQ( i[ 0 ], 0xc99d1582a2c1cc54 );
}

TEST( libstdhl_cpp_type_integer, str_decimal40 )
{
    auto i = createInteger(
        "9238463426"  // 40
        "6284625235"
        "7384632100"
        "9187212372"  // 10
        ,
        Type::Radix::DECIMAL );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), false );
    EXPECT_EQ( i[ 2 ], 0x1b );
    EXPECT_EQ( i[ 1 ], 0x263f55d94d2aacd2 );
    EXPECT_EQ( i[ 0 ], 0x2893945af841cc54 );
}

TEST( libstdhl_cpp_type_integer, str_decimal40_sign )
{
    auto i = createInteger(
        "-331155"
        "9238463426"  // 40
        "6284625235"
        "7384632100"
        "9187212372"  // 10
        ,
        Type::Radix::DECIMAL );

    EXPECT_EQ( i.sign(), true );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), false );
    EXPECT_EQ( i[ 2 ], 0x947ed5 );
    EXPECT_EQ( i[ 1 ], 0xf16ce3f39bdb16aa );
    EXPECT_EQ( i[ 0 ], 0x4b67475af841cc54 );
}

TEST( libstdhl_cpp_type_integer, str_decimal77_sign )
{
    auto i = createInteger(
        "1234567"
        "1234567890"  // 70
        "1234567890"
        "1234567890"
        "1234567890"  // 40
        "1234567890"
        "1234567890"
        "1234567890"  // 10
    );

    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.defined(), true );
    EXPECT_EQ( i.trivial(), false );
    EXPECT_EQ( i[ 3 ], 0x1b4b66fcc6b7a850 );
    EXPECT_EQ( i[ 2 ], 0x3f98e610151e6e07 );
    EXPECT_EQ( i[ 1 ], 0x5f72b95e8f5f488b );
    EXPECT_EQ( i[ 0 ], 0xaccff196ce3f0ad2 );
}

TEST( libstdhl_cpp_type_integer, hash_equal )
{
    u64 number = 1234;

    auto a = createInteger( number );
    auto b = createInteger( number );

    EXPECT_EQ( a.hash(), b.hash() );
}

TEST( libstdhl_cpp_type_integer, hash_not_equal )
{
    u64 number = 1234;

    auto a = createInteger( number );
    auto b = createInteger( number * ( -1 ) );

    EXPECT_NE( a.hash(), b.hash() );
}

#define TEST_CPP_TYPE_INTEGER_OPERATOR_ARITHMETIC_( NAME, OP, A, B )         \
    TEST( libstdhl_cpp_type_integer, operator_##NAME )                       \
    {                                                                        \
        const auto a = createInteger( (i64)A );                              \
        const auto b = createInteger( (i64)B );                              \
                                                                             \
        const auto c = a OP b;                                               \
        EXPECT_EQ( c.trivial(), true );                                      \
        EXPECT_EQ( (i64)c.value() * ( c.sign() ? ( -1 ) : ( 1 ) ), A OP B ); \
    }

TEST( libstdhl_cpp_type_integer, operator_equ_zero_pn )
{
    auto a = createInteger( "0", Type::Radix::DECIMAL );
    auto b = createInteger( "-0", Type::Radix::DECIMAL );

    const auto c = a == b;
    EXPECT_TRUE( c );
}

TEST( libstdhl_cpp_type_integer, operator_equ_zero_np )
{
    auto a = createInteger( "-0", Type::Radix::DECIMAL );
    auto b = createInteger( "0", Type::Radix::DECIMAL );

    const auto c = a == b;
    EXPECT_TRUE( c );
}

TEST( libstdhl_cpp_type_integer, operator_equ_zero_nn )
{
    auto a = createInteger( "-0", Type::Radix::DECIMAL );
    auto b = createInteger( "-0", Type::Radix::DECIMAL );

    const auto c = a == b;
    EXPECT_TRUE( c );
}

#define TEST_CPP_TYPE_INTEGER_OPERATOR_COMPARE_( NAME, OP, A, B ) \
    TEST( libstdhl_cpp_type_integer, operator_##NAME )            \
    {                                                             \
        const auto a = createInteger( (i64)A );                   \
        const auto b = createInteger( (i64)B );                   \
                                                                  \
        const auto c = a OP b;                                    \
        EXPECT_EQ( c, A OP B );                                   \
    }

#define TEST_CPP_TYPE_INTEGER_OPERATOR_SETz( KIND, NAME, OP )           \
    TEST_CPP_TYPE_INTEGER_OPERATOR_##KIND##_( NAME##z0, OP, -1234, 0 ); \
    TEST_CPP_TYPE_INTEGER_OPERATOR_##KIND##_( NAME##z1, OP, -1, 0 );    \
    TEST_CPP_TYPE_INTEGER_OPERATOR_##KIND##_( NAME##z2, OP, 0, 0 );     \
    TEST_CPP_TYPE_INTEGER_OPERATOR_##KIND##_( NAME##z3, OP, 1, 0 );     \
    TEST_CPP_TYPE_INTEGER_OPERATOR_##KIND##_( NAME##z4, OP, 1234, 0 );

#define TEST_CPP_TYPE_INTEGER_OPERATOR_SET0( KIND, NAME, OP )           \
    TEST_CPP_TYPE_INTEGER_OPERATOR_##KIND##_( NAME##00, OP, -1234, 1 ); \
    TEST_CPP_TYPE_INTEGER_OPERATOR_##KIND##_( NAME##01, OP, -1, 1 );    \
    TEST_CPP_TYPE_INTEGER_OPERATOR_##KIND##_( NAME##02, OP, 0, 1 );     \
    TEST_CPP_TYPE_INTEGER_OPERATOR_##KIND##_( NAME##03, OP, 1, 1 );     \
    TEST_CPP_TYPE_INTEGER_OPERATOR_##KIND##_( NAME##04, OP, 2, 1 );     \
    TEST_CPP_TYPE_INTEGER_OPERATOR_##KIND##_( NAME##05, OP, 1234, 1 );  \
    TEST_CPP_TYPE_INTEGER_OPERATOR_##KIND##_( NAME##06, OP, 0, 1234 );

#define TEST_CPP_TYPE_INTEGER_OPERATOR_SET1( KIND, NAME, OP )             \
    TEST_CPP_TYPE_INTEGER_OPERATOR_##KIND##_( NAME##10, OP, -1234, -10 ); \
    TEST_CPP_TYPE_INTEGER_OPERATOR_##KIND##_( NAME##11, OP, -11, -10 );   \
    TEST_CPP_TYPE_INTEGER_OPERATOR_##KIND##_( NAME##12, OP, -10, -10 );   \
    TEST_CPP_TYPE_INTEGER_OPERATOR_##KIND##_( NAME##13, OP, -10, -9 );    \
    TEST_CPP_TYPE_INTEGER_OPERATOR_##KIND##_( NAME##14, OP, -10, 1234 );

#define TEST_CPP_TYPE_INTEGER_OPERATOR_SET2( KIND, NAME, OP )            \
    TEST_CPP_TYPE_INTEGER_OPERATOR_##KIND##_( NAME##20, OP, -1234, 48 ); \
    TEST_CPP_TYPE_INTEGER_OPERATOR_##KIND##_( NAME##21, OP, 47, 48 );    \
    TEST_CPP_TYPE_INTEGER_OPERATOR_##KIND##_( NAME##22, OP, 48, 48 );    \
    TEST_CPP_TYPE_INTEGER_OPERATOR_##KIND##_( NAME##23, OP, 48, 49 );    \
    TEST_CPP_TYPE_INTEGER_OPERATOR_##KIND##_( NAME##24, OP, 48, 1234 )

#define TEST_CPP_TYPE_INTEGER_OPERATOR_ARITHMETIC( NAME, OP )    \
    TEST_CPP_TYPE_INTEGER_OPERATOR_SET0( ARITHMETIC, NAME, OP ); \
    TEST_CPP_TYPE_INTEGER_OPERATOR_SET1( ARITHMETIC, NAME, OP ); \
    TEST_CPP_TYPE_INTEGER_OPERATOR_SET2( ARITHMETIC, NAME, OP );

#define TEST_CPP_TYPE_INTEGER_OPERATOR_COMPARE( NAME, OP )    \
    TEST_CPP_TYPE_INTEGER_OPERATOR_SETz( COMPARE, NAME, OP ); \
    TEST_CPP_TYPE_INTEGER_OPERATOR_SET0( COMPARE, NAME, OP ); \
    TEST_CPP_TYPE_INTEGER_OPERATOR_SET1( COMPARE, NAME, OP ); \
    TEST_CPP_TYPE_INTEGER_OPERATOR_SET2( COMPARE, NAME, OP );

#define TEST_CPP_TYPE_INTEGER_OPERATOR_ARITHMETIC_POW( NUMBER, A, B )                  \
    TEST( libstdhl_cpp_type_integer, operator_pow##NUMBER )                            \
    {                                                                                  \
        const auto a = createInteger( (i64)A );                                        \
        const auto b = createNatural( (i64)B );                                        \
                                                                                       \
        const auto c = a ^ b;                                                          \
        EXPECT_EQ( c.trivial(), true );                                                \
        EXPECT_EQ( (i64)c.value() * ( c.sign() ? ( -1 ) : ( 1 ) ), std::pow( A, B ) ); \
    }

#define TEST_CPP_TYPE_INTEGER_OPERATOR_ARITHMETIC__pow           \
    TEST_CPP_TYPE_INTEGER_OPERATOR_ARITHMETIC_POW( 00, 0, 0 );   \
    TEST_CPP_TYPE_INTEGER_OPERATOR_ARITHMETIC_POW( 01, 0, 1 );   \
    TEST_CPP_TYPE_INTEGER_OPERATOR_ARITHMETIC_POW( 02, 1, 0 );   \
    TEST_CPP_TYPE_INTEGER_OPERATOR_ARITHMETIC_POW( 03, 1, 1 );   \
    TEST_CPP_TYPE_INTEGER_OPERATOR_ARITHMETIC_POW( 04, -1, 0 );  \
    TEST_CPP_TYPE_INTEGER_OPERATOR_ARITHMETIC_POW( 05, -1, 1 );  \
    TEST_CPP_TYPE_INTEGER_OPERATOR_ARITHMETIC_POW( 06, 3, 3 );   \
    TEST_CPP_TYPE_INTEGER_OPERATOR_ARITHMETIC_POW( 07, -5, 4 );  \
    TEST_CPP_TYPE_INTEGER_OPERATOR_ARITHMETIC_POW( 08, -23, 3 ); \
    TEST_CPP_TYPE_INTEGER_OPERATOR_ARITHMETIC_POW( 09, 8, 2 );   \
    TEST_CPP_TYPE_INTEGER_OPERATOR_ARITHMETIC_POW( 10, 2, 16 );  \
    TEST_CPP_TYPE_INTEGER_OPERATOR_ARITHMETIC_POW( 11, 16, 3 );  \
    TEST_CPP_TYPE_INTEGER_OPERATOR_ARITHMETIC_POW( 12, 14, 5 );

TEST_CPP_TYPE_INTEGER_OPERATOR_ARITHMETIC( add, +);
TEST_CPP_TYPE_INTEGER_OPERATOR_ARITHMETIC( sub, -);
TEST_CPP_TYPE_INTEGER_OPERATOR_ARITHMETIC( mul, * );
TEST_CPP_TYPE_INTEGER_OPERATOR_ARITHMETIC( mod, % );
TEST_CPP_TYPE_INTEGER_OPERATOR_ARITHMETIC( div, / );
TEST_CPP_TYPE_INTEGER_OPERATOR_ARITHMETIC__pow;

TEST_CPP_TYPE_INTEGER_OPERATOR_COMPARE( equ, == );
TEST_CPP_TYPE_INTEGER_OPERATOR_COMPARE( neq, != );
TEST_CPP_TYPE_INTEGER_OPERATOR_COMPARE( leq, <= );
TEST_CPP_TYPE_INTEGER_OPERATOR_COMPARE( les, < );
TEST_CPP_TYPE_INTEGER_OPERATOR_COMPARE( geq, >= );
TEST_CPP_TYPE_INTEGER_OPERATOR_COMPARE( gre, > );

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
