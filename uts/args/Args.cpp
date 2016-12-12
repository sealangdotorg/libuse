//
//  Copyright (c) 2014-2016 Philipp Paulweber
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

#include "cpp/Args.h"

#define TEST_NAME                                                              \
    ( std::string(::testing::UnitTest::GetInstance()                           \
                      ->current_test_info()                                    \
                      ->test_case_name() )                                     \
        + "." + std::string(::testing::UnitTest::GetInstance()                 \
                                ->current_test_info()                          \
                                ->name() ) )                                   \
        .c_str()

TEST( libstdhl_args_cpp, no_files_default_processing )
{
    const char* argv[] = { "program" };
    libstdhl::Args options( 1, argv );

    options.parse();
}

TEST( libstdhl_args_cpp, no_files_pass )
{
    u32 cnt = 0;

    const char* argv[] = { "program" };
    libstdhl::Args options( 1, argv,
        [&options, &cnt]( const char* arg ) { EXPECT_TRUE( false ); } );

    EXPECT_EQ( cnt, 0 );

    options.parse();

    ASSERT_EQ( cnt, 0 );
}

TEST( libstdhl_args_cpp, no_files_fail_one )
{
    u32 cnt = 0;
    const char* argv[] = { "program", "file.txt" };
    libstdhl::Args options( 2, argv, [&options, &cnt]( const char* arg ) {
        EXPECT_TRUE( true );
        cnt++;
    } );

    EXPECT_EQ( cnt, 0 );

    options.parse();

    ASSERT_EQ( cnt, 1 );
}

TEST( libstdhl_args_cpp, no_files_fail_multiple )
{
    u32 cnt = 0;
    const char* argv[] = { "program", "file.txt", "file.txt", "file.txt" };
    libstdhl::Args options( 4, argv, [&options, &cnt]( const char* arg ) {
        EXPECT_TRUE( true );
        cnt++;
    } );

    EXPECT_EQ( cnt, 0 );

    options.parse();

    ASSERT_EQ( cnt, 3 );
}

TEST( libstdhl_args_cpp, create_only_one_file )
{
    const char* argument = "";
    u32 cnt = 0;

    const char* argv[] = { "program", "input.txt" };
    libstdhl::Args options(
        2, argv, [&options, &argument, &cnt]( const char* arg ) {
            cnt++;
            EXPECT_LE( cnt, 1 );
            argument = arg;
        } );

    EXPECT_EQ( cnt, 0 );
    EXPECT_STRNE( "input.txt", argument );

    options.parse();

    EXPECT_EQ( cnt, 1 );
    ASSERT_STREQ( "input.txt", argument );
}

TEST( libstdhl_args_cpp, argument_once_required_parameter_short_no_metatag )
{
    const char* argument = "";
    u32 cnt = 0;

    const char* argv[] = { "program", "file.txt", "-a", "data", "file.txt" };
    libstdhl::Args options( 5, argv );

    options.add( 'a', libstdhl::Args::REQUIRED, __FUNCTION__,
        [&options, &argument, &cnt]( const char* arg ) {
            cnt++;
            EXPECT_LE( cnt, 1 );
            argument = arg;
        } );

    EXPECT_STRNE( "data", argument );

    options.parse();

    ASSERT_STREQ( "data", argument );
}

TEST( libstdhl_args_cpp, argument_once_required_parameter_short_with_metatag )
{
    const char* argument = "";
    u32 cnt = 0;

    const char* argv[] = { "program", "file.txt", "-a", "data", "file.txt" };
    libstdhl::Args options( 5, argv );

    options.add( 'a', libstdhl::Args::REQUIRED, __FUNCTION__,
        [&options, &argument, &cnt]( const char* arg ) {
            cnt++;
            EXPECT_LE( cnt, 1 );
            argument = arg;
        } );

    EXPECT_STRNE( "data", argument );

    options.parse();

    ASSERT_STREQ( "data", argument );
}

struct ParamOpt
{
    char chr;
    const char* str;
    u8 req;
    u8 cnt;
};

struct Param
{
    std::vector< std::string > arguments;
    u32 cnt_files;
    ;
    std::vector< ParamOpt > options;
    ;
    u32 result;
    ;
};

class cpp_Args : public ::testing::TestWithParam< Param >
{
};

TEST_P( cpp_Args, param )
{
    Param const& param = GetParam();

    ASSERT_LT( (u32)param.arguments.size(), (u32)100 );

    u32 argc = param.arguments.size() + 1;
    char* argv[ 100 ];
    argv[ 0 ] = (char*)"program";

    for( u32 i = 1; i < argc; i++ )
    {
        argv[ i ] = (char*)param.arguments[ i - 1 ].c_str();
    }

    u32 cnt_files = 0;
    const char* argument = "";

    libstdhl::Args options( argc, (const char**)argv,
        [&param, &options, &argument, &cnt_files]( const char* arg ) {
            cnt_files++;
            EXPECT_LE( cnt_files, param.cnt_files );
            argument = arg;
        } );

    u32 cnt_err = 0;

    options.error = [this, &options, &cnt_err](
        int error_code, const char* msg ) {
        // omit output
        fprintf( stderr, "%s: error: %s\n", options.getProgramName(), msg );
    };

    std::unordered_map< char, u32 > cnt_options_chr;
    std::unordered_map< std::string, u32 > cnt_options_str;

    for( auto param_opt : param.options )
    {
        cnt_options_chr[ param_opt.chr ] = 0;
        cnt_options_str[ std::string( param_opt.str ? param_opt.str : "" ) ]
            = 0;

        function< void( const char* ) > hook = [param_opt, &cnt_options_chr,
            &cnt_options_str]( const char* arg ) {
            // printf("%s: '%c' \"%s\" {%u}\n", TEST_NAME, param_opt.chr,
            // param_opt.str, param_opt.req);

            if( param_opt.chr )
            {
                cnt_options_chr[ param_opt.chr ] += 1;
            }
            else
            {
                cnt_options_str[ param_opt.str ? param_opt.str : "" ] += 1;
            }
        };

        ASSERT_GE( param_opt.req, 0 );
        ASSERT_LE( param_opt.req, 2 );
        libstdhl::Args::Kind kind = ( libstdhl::Args::Kind )( param_opt.req );

        if( param_opt.chr != 0 and param_opt.str == 0 )
        {
            options.add( param_opt.chr, kind, TEST_NAME, hook );
        }
        else if( param_opt.chr == 0 and param_opt.str != 0 )
        {
            options.add( param_opt.str, kind, TEST_NAME, hook );
        }
        else if( param_opt.chr != 0 and param_opt.str != 0 )
        {
            options.add( param_opt.chr, param_opt.str, kind, TEST_NAME, hook );
        }
        else
        {
            FAIL();
        }
    }

    EXPECT_EQ( cnt_files, 0 );

    for( auto param_opt : param.options )
    {
        EXPECT_EQ( cnt_options_chr[ param_opt.chr ], 0 );
        EXPECT_EQ( cnt_options_str[ param_opt.str ? param_opt.str : "" ], 0 );
    }

    u32 res = options.parse();

    EXPECT_EQ( cnt_files, param.cnt_files );

    for( auto param_opt : param.options )
    {
        EXPECT_EQ(
            ( cnt_options_chr[ param_opt.chr ]
                + cnt_options_str[ param_opt.str ? param_opt.str : "" ] ),
            param_opt.cnt );
    }

    ASSERT_EQ( res, param.result );
}

INSTANTIATE_TEST_CASE_P( libstdhl_cpp_Args_pass, cpp_Args,
    ::testing::Values( Param{ { "text.txt" }, 1 },
        Param{ { "very_long_file_name", "short_file" }, 2 },
        Param{ { "file0", "file1", "file2", "file3", "file4", "file5" }, 6 },
        Param{ { "-v" }, 0, { { 'v', 0, 0, 1 } } },
        Param{ { "-f", "-i", "-f", "-o" }, 0,
            { { 'f', 0, 0, 2 }, { 'i', 0, 0, 1 }, { 'o', 0, 0, 1 } } },
        Param{ { "--help" }, 0, { { 0, "help", 0, 1 } } },
        Param{ { "--version" }, 0, { { 'v', "version", 0, 1 } } },
        Param{ { "-v" }, 0, { { 'v', "version", 0, 1 } } } ) );

INSTANTIATE_TEST_CASE_P( libstdhl_cpp_Args_fail, cpp_Args,
    ::testing::Values( Param{ { "-v" }, 0, {}, 1 },
        Param{ { "-h" }, 0, { { 'v' } }, 1 },
        Param{ { "-t", "-s", "-e", "-t" }, 0, { { 'v' } }, 4 },
        Param{ { "--long-option-name" }, 0, {}, 1 },
        Param{ { "--missing-argument", "bla" }, 0,
            { { 'a', "missing-argument", 1, 1 } } } ) );
