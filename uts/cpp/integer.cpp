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

#include "cpp/Integer.h"

using namespace libstdhl;

TEST( libstdhl_cpp_integer, u64_valid )
{
    u64 number = 123456789;

    auto i = Integer( number );

    EXPECT_EQ( i.words().size(), 1 );
    EXPECT_EQ( i.word( 0 ), number );

    EXPECT_EQ( i.sign(), false );
}

TEST( libstdhl_cpp_integer, i64_positive )
{
    i64 number = 123456789;

    auto i = Integer( number );

    EXPECT_EQ( i.words().size(), 1 );
    EXPECT_EQ( i.word( 0 ), number * ( i.sign() ? -1 : 1 ) );

    EXPECT_EQ( i.sign(), false );
}

TEST( libstdhl_cpp_integer, i64_negative )
{
    i64 number = -123456789;

    auto i = Integer( number );

    EXPECT_EQ( i.words().size(), 1 );
    EXPECT_EQ( i.word( 0 ), number * ( i.sign() ? -1 : 1 ) );

    EXPECT_EQ( i.sign(), true );
}

TEST( libstdhl_cpp_integer, str_binary4 )
{
    auto i = Integer( "0111", Type::Radix::BINARY );

    EXPECT_EQ( i.words().size(), 1 );
    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.word( 0 ), 7 );
}

TEST( libstdhl_cpp_integer, str_binary64 )
{
    auto i = Integer(
        "01111011" // 7b
        "11110010" // f2
        "00000100" // 04
        "11101110" // ee
        "01111011" // 7b
        "11110010" // f2
        "00000100" // 04
        "11101101" // ed
        ,
        Type::Radix::BINARY );

    EXPECT_EQ( i.words().size(), 1 );
    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.word( 0 ), 0x7bf204ee7bf204ed );
}

TEST( libstdhl_cpp_integer, str_binary64_max )
{
    auto i = Integer(
        "11111111" // ff
        "11111111" // ff
        "11111111" // ff
        "11111111" // ff
        "11111111" // ff
        "11111111" // ff
        "11111111" // ff
        "11111111" // ff
        ,
        Type::Radix::BINARY );

    EXPECT_EQ( i.words().size(), 1 );
    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.word( 0 ), UINT64_MAX );
}

TEST( libstdhl_cpp_integer, str_binary70 )
{
    auto i = Integer(
        "101010"   // 2a
        "01111011" // 7b
        "11110010" // f2
        "00000100" // 04
        "11101110" // ee
        "01111011" // 7b
        "11110010" // f2
        "00000100" // 04
        "11101101" // ed
        ,
        Type::Radix::BINARY );

    EXPECT_EQ( i.words().size(), 2 );
    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.word( 1 ), 0x2a );
    EXPECT_EQ( i.word( 0 ), 0x7bf204ee7bf204ed );
}

TEST( libstdhl_cpp_integer, str_binary522 )
{
    auto i = Integer(
        "11"       // 03
        "11110010" // f2
        "01111011" // 7b // 512
        "11110010" // f2
        "00000100" // 04
        "11101110" // ee
        "01111011" // 7b // -
        "11110010" // f2
        "00000100" // 04
        "11101101" // ed
        "01111011" // 7b // -
        "11110010" // f2
        "00000100" // 04
        "11101110" // ee
        "01111011" // 7b // -
        "11110010" // f2
        "00000100" // 04
        "11101101" // ed
        "01111011" // 7b // -
        "11110010" // f2
        "00000100" // 04
        "11101110" // ee
        "01111011" // 7b // -
        "11110010" // f2
        "00000100" // 04
        "11101101" // ed
        "01111011" // 7b // -
        "11110010" // f2
        "00000100" // 04
        "11101110" // ee
        "01111011" // 7b // -
        "11110010" // f2
        "00000100" // 04
        "11101101" // ed
        "01111011" // 7b // 256
        "11110010" // f2
        "00000100" // 04
        "11101110" // ee
        "01111011" // 7b // -
        "11110010" // f2
        "00000100" // 04
        "11101101" // ed
        "01111011" // 7b // -
        "11110010" // f2
        "00000100" // 04
        "11101110" // ee
        "01111011" // 7b // -
        "11110010" // f2
        "00000100" // 04
        "11101101" // ed
        "01111011" // 7b // 128
        "11110010" // f2
        "00000100" // 04
        "11101110" // ee
        "01111011" // 7b // -
        "11110010" // f2
        "00000100" // 04
        "11101101" // ed
        "01111011" // 7b // 64
        "11110010" // f2
        "00000100" // 04
        "11101110" // ee
        "01111011" // 7b // -
        "11110010" // f2
        "00000100" // 04
        "11101101" // ed // 8
        ,
        Type::Radix::BINARY );

    EXPECT_EQ( i.words().size(), 9 );
    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.word( 8 ), 0x03f2 );
    EXPECT_EQ( i.word( 7 ), 0x7bf204ee7bf204ed );
    EXPECT_EQ( i.word( 6 ), 0x7bf204ee7bf204ed );
    EXPECT_EQ( i.word( 5 ), 0x7bf204ee7bf204ed );
    EXPECT_EQ( i.word( 4 ), 0x7bf204ee7bf204ed );
    EXPECT_EQ( i.word( 3 ), 0x7bf204ee7bf204ed );
    EXPECT_EQ( i.word( 2 ), 0x7bf204ee7bf204ed );
    EXPECT_EQ( i.word( 1 ), 0x7bf204ee7bf204ed );
    EXPECT_EQ( i.word( 0 ), 0x7bf204ee7bf204ed );
}

TEST( libstdhl_cpp_integer, str_octal5 )
{
    auto i = Integer( "54321", Type::Radix::OCTAL );

    EXPECT_EQ( i.words().size(), 1 );
    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.word( 0 ), 054321 );
}

TEST( libstdhl_cpp_integer, str_octal21 )
{
    auto i = Integer( "543217060554321706053", Type::Radix::OCTAL );

    EXPECT_EQ( i.words().size(), 1 );
    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.word( 0 ), 0543217060554321706053 );
}

TEST( libstdhl_cpp_integer, str_octal22_u64max )
{
    auto i = Integer( "1777777777777777777777", Type::Radix::OCTAL );

    EXPECT_EQ( i.words().size(), 1 );
    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.word( 0 ), UINT64_MAX );
}

TEST( libstdhl_cpp_integer, str_octal22_no_overlap )
{
    auto i = Integer( "1543217060554321706053", Type::Radix::OCTAL );

    EXPECT_EQ( i.words().size(), 1 );
    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.word( 0 ), 01543217060554321706053 );
}

TEST( libstdhl_cpp_integer, str_octal22_with_overlap )
{
    auto i = Integer( "7543217060554321706053", Type::Radix::OCTAL );

    EXPECT_EQ( i.words().size(), 2 );
    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.word( 1 ), 03 );
    EXPECT_EQ( i.word( 0 ), 01543217060554321706053 );
}

TEST( libstdhl_cpp_integer, str_octal23 )
{
    auto i = Integer( "337543217060554321706053", Type::Radix::OCTAL );

    EXPECT_EQ( i.words().size(), 2 );
    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.word( 1 ), 0157 );
    EXPECT_EQ( i.word( 0 ), 01543217060554321706053 );
}

TEST( libstdhl_cpp_integer, str_hexadecimal2 )
{
    auto i = Integer( "e1", Type::Radix::HEXADECIMAL );

    EXPECT_EQ( i.words().size(), 1 );
    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.word( 0 ), 0xe1 );
}

TEST( libstdhl_cpp_integer, str_hexadecimal16_max )
{
    auto i = Integer( "ffffffffffffffff", Type::Radix::HEXADECIMAL );

    EXPECT_EQ( i.words().size(), 1 );
    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.word( 0 ), UINT64_MAX );
}

TEST( libstdhl_cpp_integer, str_hexadecimal17 )
{
    auto i = Integer(
        "f"
        "eedbee0123456789",
        Type::Radix::HEXADECIMAL );

    EXPECT_EQ( i.words().size(), 2 );
    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.word( 1 ), 0xf );
    EXPECT_EQ( i.word( 0 ), 0xeedbee0123456789 );
}

TEST( libstdhl_cpp_integer, str_hexadecimal140 )
{
    auto i = Integer(
        "affe"
        "feedbeef13579024" //
        "9024feedbeef1357" // 512, 128
        "13579024feedbeef" //
        "beef13579024feed" //
        "feedbeef13579024" //
        "edbeef13579024fe" // 256, 64
        "ef13579024feedbe" //
        "579024feedbeef13" // 128, 32
        "24feedbeef135790" // 64,  16
        ,
        Type::Radix::HEXADECIMAL );

    EXPECT_EQ( i.words().size(), 10 );
    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.word( 9 ), 0xaffe );
    EXPECT_EQ( i.word( 8 ), 0xfeedbeef13579024 ); //
    EXPECT_EQ( i.word( 7 ), 0x9024feedbeef1357 ); // 512, 128
    EXPECT_EQ( i.word( 6 ), 0x13579024feedbeef ); //
    EXPECT_EQ( i.word( 5 ), 0xbeef13579024feed ); //
    EXPECT_EQ( i.word( 4 ), 0xfeedbeef13579024 ); //
    EXPECT_EQ( i.word( 3 ), 0xedbeef13579024fe ); // 256, 64
    EXPECT_EQ( i.word( 2 ), 0xef13579024feedbe ); //
    EXPECT_EQ( i.word( 1 ), 0x579024feedbeef13 ); // 128, 32
    EXPECT_EQ( i.word( 0 ), 0x24feedbeef135790 ); // 64,  16
}

TEST( libstdhl_cpp_integer, str_decimal3 )
{
    auto i = Integer( "123", Type::Radix::DECIMAL );

    EXPECT_EQ( i.words().size(), 1 );
    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.word( 0 ), 123 );
}

TEST( libstdhl_cpp_integer, str_decimal3_sign )
{
    auto i = Integer( "-123", Type::Radix::DECIMAL );

    EXPECT_EQ( i.words().size(), 1 );
    EXPECT_EQ( i.sign(), true );
    EXPECT_EQ( i.word( 0 ), 123 );
}

TEST( libstdhl_cpp_integer, str_decimal8 )
{
    auto i = Integer( "64242662", Type::Radix::DECIMAL );

    EXPECT_EQ( i.words().size(), 1 );
    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.word( 0 ), 64242662 );
}

TEST( libstdhl_cpp_integer, str_decimal8_sign )
{
    auto i = Integer( "-64242662", Type::Radix::DECIMAL );

    EXPECT_EQ( i.words().size(), 1 );
    EXPECT_EQ( i.sign(), true );
    EXPECT_EQ( i.word( 0 ), 64242662 );
}

TEST( libstdhl_cpp_integer, str_decimal20_u64max )
{
    auto i = Integer(
        "1844674407" // 20
        "3709551615" // 10
        ,
        Type::Radix::DECIMAL );

    EXPECT_EQ( i.words().size(), 1 );
    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.word( 0 ), UINT64_MAX );
}

TEST( DISABLED_libstdhl_cpp_integer, str_decimal21 )
{
    auto i = Integer(
        "123"
        "7384632100" // 20
        "9187212372" // 10
        ,
        Type::Radix::DECIMAL );

    EXPECT_EQ( i.words().size(), 2 );
    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.word( 1 ), UINT64_MAX ); // TODO: fix result checks!
    EXPECT_EQ( i.word( 0 ), UINT64_MAX );
}

TEST( DISABLED_libstdhl_cpp_integer, str_decimal21_sign )
{
    auto i = Integer(
        "-3"
        "7384632100" // 20
        "9187212372" // 10
        ,
        Type::Radix::DECIMAL );

    EXPECT_EQ( i.words().size(), 2 );
    EXPECT_EQ( i.sign(), true );
    EXPECT_EQ( i.word( 1 ), UINT64_MAX ); // TODO: fix result checks!
    EXPECT_EQ( i.word( 0 ), UINT64_MAX );
}

TEST( DISABLED_libstdhl_cpp_integer, str_decimal40 )
{
    auto i = Integer(
        "9238463426" // 40
        "6284625235"
        "7384632100"
        "9187212372" // 10
        ,
        Type::Radix::DECIMAL );

    EXPECT_EQ( i.words().size(), 3 );
    EXPECT_EQ( i.sign(), false );
    EXPECT_EQ( i.word( 2 ), 0 ); // TODO: fix result checks!
    EXPECT_EQ( i.word( 1 ), 0 );
    EXPECT_EQ( i.word( 0 ), 0 );
}

TEST( DISABLED_libstdhl_cpp_integer, str_decimal40_sign )
{
    auto i = Integer(
        "-"
        "9238463426" // 40
        "6284625235"
        "7384632100"
        "9187212372" // 10
        ,
        Type::Radix::DECIMAL );

    EXPECT_EQ( i.words().size(), 3 );
    EXPECT_EQ( i.sign(), true );
    EXPECT_EQ( i.word( 2 ), 0 ); // TODO: fix result checks!
    EXPECT_EQ( i.word( 1 ), 0 );
    EXPECT_EQ( i.word( 0 ), 0 );
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
