//
//  Copyright (C) 2014-2020 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
//                Christian Lascsak
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

TEST( libstdhl_cpp_String, split2 )
{
    std::string str = "foo.bar";

    const auto splitted = libstdhl::String::split( str, "." );

    EXPECT_EQ( splitted.size(), 2 );

    u32 cnt = 0;

    for( auto s : splitted )
    {
        std::cerr << s << "\n";

        if( cnt == 0 )
        {
            EXPECT_STREQ( s.c_str(), "foo" );
        }
        else
        {
            EXPECT_STREQ( s.c_str(), "bar" );
        }

        cnt++;
    }
}

TEST( libstdhl_cpp_String, split3 )
{
    std::string str = "foo.bar.baz";

    const auto splitted = libstdhl::String::split( str, "." );

    u32 cnt = 0;

    for( auto s : splitted )
    {
        std::cerr << s << "\n";

        if( cnt == 0 )
        {
            EXPECT_STREQ( s.c_str(), "foo" );
        }
        else if( cnt == 1 )
        {
            EXPECT_STREQ( s.c_str(), "bar" );
        }
        else
        {
            EXPECT_STREQ( s.c_str(), "baz" );
        }

        cnt++;
    }

    EXPECT_EQ( splitted.size(), 3 );
}

TEST( libstdhl_cpp_String, split_double_delimitor )
{
    std::string str = "foo\r\nbar\r\n\r\nbaz";

    const auto splitted = libstdhl::String::split( str, "\r\n\r\n" );

    u32 cnt = 0;

    for( auto s : splitted )
    {
        std::cerr << s << "\n";

        if( cnt == 0 )
        {
            EXPECT_STREQ( s.c_str(), "foo\r\nbar" );
        }
        else
        {
            EXPECT_STREQ( s.c_str(), "baz" );
        }

        cnt++;
    }

    EXPECT_EQ( splitted.size(), 2 );
}

TEST( libstdhl_cpp_String, join2 )
{
    std::vector< std::string > parts = { "foo", "bar" };

    const auto joined = libstdhl::String::join( parts, "." );

    EXPECT_STREQ( joined.c_str(), "foo.bar" );
}

TEST( libstdhl_cpp_String, join3 )
{
    std::vector< std::string > parts = { "foo", "bar", "qux" };

    const auto joined = libstdhl::String::join( parts, "." );

    EXPECT_STREQ( joined.c_str(), "foo.bar.qux" );
}

TEST( libstdhl_cpp_String, value_over_switch_case )
{
    const auto v = String::value( "foo" );

    switch( v )
    {
        case String::value( "bar" ):
        {
            ASSERT_TRUE( false );
            break;
        }
        case String::value( "foo" ):
        {
            ASSERT_TRUE( true );
            break;
        }
        case String::value( "baz" ):
        {
            ASSERT_TRUE( false );
            break;
        }
        default:
        {
            ASSERT_TRUE( false );
            break;
        }
    }
}

TEST( libstdhl_cpp_String, startsWith_true )
{
    EXPECT_EQ( true, String::startsWith( "foobarbaz", "foo" ) );
    EXPECT_EQ( true, String::startsWith( "foobarbaz", "foobar" ) );
    EXPECT_EQ( true, String::startsWith( "foobarbaz", "foobarbaz" ) );
}

TEST( libstdhl_cpp_String, startsWith_false )
{
    EXPECT_EQ( false, String::startsWith( "foobarbaz", "123" ) );
    EXPECT_EQ( false, String::startsWith( "foobarbaz", "123bar" ) );
    EXPECT_EQ( false, String::startsWith( "foobarbaz", "123barbaz" ) );
}

TEST( libstdhl_cpp_String, endsWith_true )
{
    EXPECT_EQ( true, String::endsWith( "foobarbaz", "baz" ) );
    EXPECT_EQ( true, String::endsWith( "foobarbaz", "barbaz" ) );
    EXPECT_EQ( true, String::endsWith( "foobarbaz", "foobarbaz" ) );
}

TEST( libstdhl_cpp_String, endsWith_false )
{
    EXPECT_EQ( false, String::endsWith( "foobarbaz", "123" ) );
    EXPECT_EQ( false, String::endsWith( "foobarbaz", "asdf" ) );
    EXPECT_EQ( false, String::endsWith( "foobarbaz", "fffffffffffffffff" ) );
}

TEST( libstdhl_cpp_String, replaceAll )
{
    EXPECT_STREQ( "fbazbarfbazbaz", String::replaceAll( "foobarfoobaz", "oo", "baz" ).c_str() );
}

TEST( libstdhl_cpp_String, expansion_ascii )
{
    //                 0-------------1-----------2-
    //                 0123456 7 8 9 0 1 2345678901
    std::string str = "abcdef\n\r\t\t\r\n0123456789";
    EXPECT_STREQ( String::expansion( str, 0, 3, 8, '.' ).c_str(), "..." );
    EXPECT_STREQ( String::expansion( str, 5, 6, 8, '.' ).c_str(), "...................." );
    EXPECT_STREQ( String::expansion( str, 7, 6, 4, '+' ).c_str(), "++++++++++++" );
}

TEST( libstdhl_cpp_String, expansion_utf8 )
{
    std::string str = "/* foo 😆😆 */ bar 😆 = baz";
    EXPECT_STREQ(
        String::expansion( str, 0, 24, 4, '.', ":-" ).c_str(), ".......:-:-........:-......" );
    EXPECT_STREQ(
        String::expansion( str, 2, 20, 4, '.', ":-" ).c_str(), ".....:-:-........:-...." );
    EXPECT_STREQ( String::expansion( str, 0, 12, 4, '.', ":-" ).c_str(), ".......:-:-..." );
    EXPECT_STREQ( String::expansion( str, 22, 2, 4, '.', ":-" ).c_str(), ".." );
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
