//
//  Copyright (C) 2014-2024 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber et al.
//  <https://github.com/casm-lang/libstdhl/graphs/contributors>
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

TEST( libstdhl_cpp_SourceLocation, empty )
{
    SourceLocation location;

    EXPECT_EQ( location.begin.line, 1 );
    EXPECT_EQ( location.begin.column, 1 );
    EXPECT_EQ( location.begin.fileName, nullptr );
    EXPECT_EQ( location.begin.fileName, location.fileName() );

    EXPECT_EQ( location.end.line, location.begin.line );
    EXPECT_EQ( location.end.column, location.begin.column );
    EXPECT_EQ( location.end.fileName, location.begin.fileName );
    EXPECT_EQ( location.end.fileName, location.fileName() );

    EXPECT_FALSE( location != SourceLocation() );

    location = location + 10;
    location = location - 10;
    EXPECT_TRUE( location == SourceLocation() );

    SourceLocation tmp;
    location += tmp;
    EXPECT_TRUE( location == SourceLocation() + tmp );

    EXPECT_THROW( location.read(), std::domain_error );
}

TEST( libstdhl_cpp_SourceLocation, position )
{
    SourceLocation location(
        SourcePosition( std::make_shared< std::string >( "file.ext" ), 12, 34 ) );

    EXPECT_EQ( location.begin.line, 12 );
    EXPECT_EQ( location.begin.column, 34 );
    EXPECT_NE( location.begin.fileName, nullptr );
    EXPECT_EQ( location.begin.fileName, location.fileName() );
    EXPECT_STREQ( location.begin.fileName->c_str(), "file.ext" );

    EXPECT_EQ( location.end.line, location.begin.line );
    EXPECT_EQ( location.end.column, location.begin.column );
    EXPECT_EQ( location.end.fileName, location.begin.fileName );
    EXPECT_EQ( location.end.fileName, location.fileName() );
    EXPECT_STREQ( location.end.fileName->c_str(), location.begin.fileName->c_str() );

    EXPECT_TRUE( location != SourceLocation() );

    const auto filename = std::make_shared< std::string >( TEST_NAME + ".txt" );
    auto file = File::open( *filename, std::fstream::out );
    file << "\n";
    file << "0123456789\n";
    file << "\n";
    file.close();

    const auto begin = SourcePosition( filename, 2, 4 );
    const auto end = SourcePosition( filename, 2, 7 );
    location = SourceLocation( begin, end );

    const auto text = location.read();
    EXPECT_STREQ( text.c_str(), "345" );

    File::remove( *filename );
}

TEST( libstdhl_cpp_SourceLocation, range )
{
    SourceLocation location(
        SourcePosition( std::make_shared< std::string >( "file.ext" ), 12, 34 ),
        SourcePosition( std::make_shared< std::string >( "ext.file" ), 56, 78 ) );

    EXPECT_EQ( location.begin.line, 12 );
    EXPECT_EQ( location.begin.column, 34 );
    EXPECT_NE( location.begin.fileName, nullptr );
    EXPECT_EQ( location.begin.fileName, location.fileName() );
    EXPECT_STREQ( location.begin.fileName->c_str(), "file.ext" );

    EXPECT_EQ( location.end.line, 56 );
    EXPECT_EQ( location.end.column, 78 );
    EXPECT_NE( location.end.fileName, nullptr );
    EXPECT_NE( location.end.fileName, location.fileName() );
    EXPECT_STREQ( location.end.fileName->c_str(), "ext.file" );

    EXPECT_TRUE( location != SourceLocation() );

    const auto filename = std::make_shared< std::string >( TEST_NAME + ".txt" );
    auto file = File::open( *filename, std::fstream::out );
    file << "\n";
    file << "0123456789\n";
    file << "abcdefghij\n";
    file << "\n";
    file.close();

    const auto begin = SourcePosition( filename, 2, 4 );
    const auto end = SourcePosition( filename, 3, 6 );
    location = SourceLocation( begin, end );

    const auto text = location.read();
    EXPECT_STREQ( text.c_str(), "3456789\nabcde" );

    File::remove( *filename );
}

TEST( libstdhl_cpp_SourceLocation, read_utf8 )
{
    SourceLocation location;
    const auto filename = std::make_shared< std::string >( TEST_NAME + ".txt" );
    auto file = File::open( *filename, std::fstream::out );
    file << "\n";
    file << "01234😀😀789\n";
    file << "abc😀😀fghij\n";
    file << "\n";
    file.close();

    location = SourceLocation( SourcePosition( filename, 2, 4 ), SourcePosition( filename, 2, 9 ) );
    EXPECT_STREQ( location.read().c_str(), "34😀😀7" );

    location = SourceLocation( SourcePosition( filename, 2, 4 ), SourcePosition( filename, 3, 2 ) );
    EXPECT_STREQ( location.read().c_str(), "34😀😀789\na" );

    location = SourceLocation( SourcePosition( filename, 2, 4 ), SourcePosition( filename, 3, 9 ) );
    EXPECT_STREQ( location.read().c_str(), "34😀😀789\nabc😀😀fgh" );

    location =
        SourceLocation( SourcePosition( filename, 2, 1 ), SourcePosition( filename, 2, 11 ) );
    EXPECT_STREQ( location.read().c_str(), "01234😀😀789" );

    location =
        SourceLocation( SourcePosition( filename, 3, 8 ), SourcePosition( filename, 3, 10 ) );
    EXPECT_STREQ( location.read().c_str(), "hi" );

    File::remove( *filename );
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
