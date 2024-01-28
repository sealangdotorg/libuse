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

//
//
// File
//

TEST( libstdhl_cpp_File, create_exists_remove )
{
    // GIVEN
    std::string filename = TEST_NAME + ".txt";
    EXPECT_FALSE( libstdhl::File::exists( filename ) );

    // WHEN
    auto file = libstdhl::File::open( filename, std::ios::out | std::ios::trunc );

    // THEN
    EXPECT_TRUE( libstdhl::File::exists( filename ) );
    EXPECT_TRUE( libstdhl::File::exists( file ) );

    // CLEANUP
    file.close();
    libstdhl::File::remove( filename );
    EXPECT_FALSE( libstdhl::File::exists( filename ) );
}

TEST( libstdhl_cpp_File, does_not_exist_during_open_triggers_exception )
{
    // WHEN
    std::string filename = TEST_NAME + ".txt";

    // THEN
    EXPECT_FALSE( libstdhl::File::exists( filename ) );
    EXPECT_THROW( libstdhl::File::open( filename ), std::invalid_argument );
}

TEST( libstdhl_cpp_File, does_not_exist_during_remove_triggers_exception )
{
    // GIVEN
    std::string filename = TEST_NAME + ".txt";

    // WHEN
    EXPECT_FALSE( libstdhl::File::exists( filename ) );

    // THEN
    EXPECT_THROW( libstdhl::File::remove( filename ), std::invalid_argument );
}

TEST( libstdhl_cpp_File, readLines )
{
    // GIVEN
    static const auto source = R"***(
foo
bar foo
qux bar foo
eof)***";
    const std::string filename = TEST_NAME + ".txt";
    EXPECT_FALSE( libstdhl::File::exists( filename ) );
    auto file = libstdhl::File::open( filename, std::fstream::out );
    file << source;
    file.close();

    // WHEN
    EXPECT_TRUE( libstdhl::File::exists( filename ) );
    const std::vector< std::string > sourceLineStrings = {
        "", "foo", "bar foo", "qux bar foo", "eof"
    };

    // THEN
    auto sourceLineCounter = 0;
    const auto result = libstdhl::File::readLines(
        filename, [&]( u32 readLineCounter, const std::string& readLineString ) {
            EXPECT_EQ( readLineCounter, sourceLineCounter );
            EXPECT_STREQ( readLineString.c_str(), sourceLineStrings[ readLineCounter ].c_str() );
            sourceLineCounter++;
        } );
    EXPECT_EQ( result, 0 );

    // CLEANUP
    libstdhl::File::remove( filename );
    EXPECT_FALSE( libstdhl::File::exists( filename ) );
}

TEST( libstdhl_cpp_File, readLine_at_specific_line_number )
{
    // GIVEN
    static const auto source = R"***(
foo
bar foo
qux bar foo
eof)***";
    const std::string filename = TEST_NAME + ".txt";
    EXPECT_FALSE( libstdhl::File::exists( filename ) );
    auto file = libstdhl::File::open( filename, std::fstream::out );
    file << source;
    file.close();

    // WHEN
    EXPECT_TRUE( libstdhl::File::exists( filename ) );

    // THEN
    EXPECT_STREQ( libstdhl::File::readLine( filename, 1 ).c_str(), "" );
    EXPECT_STREQ( libstdhl::File::readLine( filename, 2 ).c_str(), "foo" );
    EXPECT_STREQ( libstdhl::File::readLine( filename, 3 ).c_str(), "bar foo" );
    EXPECT_STREQ( libstdhl::File::readLine( filename, 4 ).c_str(), "qux bar foo" );
    EXPECT_STREQ( libstdhl::File::readLine( filename, 5 ).c_str(), "eof" );

    // CLEANUP
    libstdhl::File::remove( filename );
    EXPECT_FALSE( libstdhl::File::exists( filename ) );
}

TEST( libstdhl_cpp_File, readLine_at_invalid_line_number_triggers_exception )
{
    // GIVEN
    static const auto source = R"***(
foo
bar foo
qux bar foo
eof)***";
    const std::string filename = TEST_NAME + ".txt";
    EXPECT_FALSE( libstdhl::File::exists( filename ) );
    auto file = libstdhl::File::open( filename, std::fstream::out );
    file << source;
    file.close();

    // WHEN
    EXPECT_TRUE( libstdhl::File::exists( filename ) );

    // THEN
    EXPECT_THROW( libstdhl::File::readLine( filename, -4321 ), FileNumberOutOfRangeException );
    EXPECT_THROW( libstdhl::File::readLine( filename, -1 ), FileNumberOutOfRangeException );
    EXPECT_THROW( libstdhl::File::readLine( filename, 0 ), FileNumberOutOfRangeException );
    EXPECT_THROW( libstdhl::File::readLine( filename, 6 ), FileNumberOutOfRangeException );
    EXPECT_THROW( libstdhl::File::readLine( filename, 1234 ), FileNumberOutOfRangeException );

    // CLEANUP
    libstdhl::File::remove( filename );
    EXPECT_FALSE( libstdhl::File::exists( filename ) );
}

//
//
// File::Path
//

TEST( libstdhl_cpp_File_Path, create_exists_remove )
{
    // GIVEN
    std::string path = TEST_NAME;
    EXPECT_FALSE( libstdhl::File::Path::exists( path ) );

    // WHEN
    libstdhl::File::Path::create( path );

    // THEN
    EXPECT_TRUE( libstdhl::File::Path::exists( path ) );
    EXPECT_FALSE( libstdhl::File::exists( path ) );

    // CLEANUP
    libstdhl::File::Path::remove( path );
    EXPECT_FALSE( libstdhl::File::Path::exists( path ) );
}

TEST( libstdhl_cpp_File_Path, does_not_exist_during_remove_triggers_exception )
{
    // GIVEN
    std::string path = TEST_NAME;

    // WHEN
    EXPECT_FALSE( libstdhl::File::Path::exists( path ) );

    // THEN
    EXPECT_THROW( libstdhl::File::Path::remove( path ), std::domain_error );
}

TEST( libstdhl_cpp_File_Path, redundant_creation_triggers_exception )
{
    // GIVEN
    std::string path = TEST_NAME;

    // WHEN
    EXPECT_FALSE( libstdhl::File::Path::exists( path ) );
    libstdhl::File::Path::create( path );
    EXPECT_TRUE( libstdhl::File::Path::exists( path ) );

    // THEN
    EXPECT_THROW( libstdhl::File::Path::create( path ), std::domain_error );

    // CLEANUP
    libstdhl::File::Path::remove( path );
    EXPECT_FALSE( libstdhl::File::Path::exists( path ) );
}

TEST( libstdhl_cpp_File_Path, temporary )
{
    // GIVEN
#if defined( __WIN32__ ) or defined( __WIN32 ) or defined( _WIN32 )
    std::string path = "C:/Windows/TEMP";
#else
    std::string path = "/tmp";
#endif

    // WHEN & THEN
    EXPECT_STREQ( libstdhl::File::Path::temporary().c_str(), path.c_str() );
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
