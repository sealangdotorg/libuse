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

#include "main.h"

using namespace libstdhl;

TEST( libstdhl_args_cpp, no_files_default_processing )
{
    const char* argv[] = { "program" };
    Args options( 1, argv );

    Log::Stream s;
    Logger log( s );

    options.parse( log );

    s.dump();
}

TEST( libstdhl_args_cpp, no_files_pass )
{
    u32 cnt = 0;

    const char* argv[] = { "program" };
    Args options( 1, argv, [&options, &cnt]( const char* arg ) {
        EXPECT_TRUE( false );
        return 0;
    } );

    EXPECT_EQ( cnt, 0 );

    Log::Stream s;
    Logger log( s );

    options.parse( log );

    s.dump();

    ASSERT_EQ( cnt, 0 );
}

TEST( libstdhl_args_cpp, no_files_fail_one )
{
    u32 cnt = 0;
    const char* argv[] = { "program", "file.txt" };
    Args options( 2, argv, [&options, &cnt]( const char* arg ) {
        EXPECT_TRUE( true );
        cnt++;
        return 0;
    } );

    EXPECT_EQ( cnt, 0 );

    Log::Stream s;
    Logger log( s );

    options.parse( log );

    s.dump();

    ASSERT_EQ( cnt, 1 );
}

TEST( libstdhl_args_cpp, no_files_fail_multiple )
{
    u32 cnt = 0;
    const char* argv[] = { "program", "file.txt", "file.txt", "file.txt" };
    Args options( 4, argv, [&options, &cnt]( const char* arg ) {
        EXPECT_TRUE( true );
        cnt++;
        return 0;
    } );

    EXPECT_EQ( cnt, 0 );

    Log::Stream s;
    Logger log( s );

    options.parse( log );

    s.dump();

    ASSERT_EQ( cnt, 3 );
}

TEST( libstdhl_args_cpp, create_only_one_file )
{
    const char* argument = "";
    u32 cnt = 0;

    const char* argv[] = { "program", "input.txt" };
    Args options( 2, argv, [&options, &argument, &cnt]( const char* arg ) {
        cnt++;
        EXPECT_LE( cnt, 1 );
        argument = arg;
        return 0;
    } );

    EXPECT_EQ( cnt, 0 );
    EXPECT_STRNE( "input.txt", argument );

    Log::Stream s;
    Logger log( s );

    options.parse( log );

    s.dump();

    EXPECT_EQ( cnt, 1 );
    ASSERT_STREQ( "input.txt", argument );
}

TEST( libstdhl_args_cpp, argument_once_required_parameter_short_no_metatag )
{
    const char* argument = "";
    u32 cnt = 0;

    const char* argv[] = { "program", "file.txt", "-a", "data", "file.txt" };
    Args options( 5, argv );

    options.add( 'a', Args::REQUIRED, __FUNCTION__,
        [&options, &argument, &cnt]( const char* arg ) {
            cnt++;
            EXPECT_LE( cnt, 1 );
            argument = arg;
            return 0;
        } );

    EXPECT_STRNE( "data", argument );

    Log::Stream s;
    Logger log( s );

    options.parse( log );

    s.dump();

    ASSERT_STREQ( "data", argument );
}

TEST( libstdhl_args_cpp, argument_once_required_parameter_short_with_metatag )
{
    const char* argument = "";
    u32 cnt = 0;

    const char* argv[] = { "program", "file.txt", "-a", "data", "file.txt" };
    Args options( 5, argv );

    options.add( 'a', Args::REQUIRED, __FUNCTION__,
        [&options, &argument, &cnt]( const char* arg ) {
            cnt++;
            EXPECT_LE( cnt, 1 );
            argument = arg;
            return 0;
        } );

    EXPECT_STRNE( "data", argument );

    Log::Stream s;
    Logger log( s );

    options.parse( log );

    s.dump();

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

    Args options( argc, (const char**)argv,
        [&param, &options, &argument, &cnt_files]( const char* arg ) {
            cnt_files++;
            EXPECT_LE( cnt_files, param.cnt_files );
            argument = arg;
            return 0;
        } );

    std::unordered_map< char, u32 > cnt_options_chr;
    std::unordered_map< std::string, u32 > cnt_options_str;

    for( auto param_opt : param.options )
    {
        cnt_options_chr[ param_opt.chr ] = 0;
        cnt_options_str[ std::string( param_opt.str ? param_opt.str : "" ) ]
            = 0;

        std::function< i32( const char* ) > hook = [param_opt, &cnt_options_chr,
            &cnt_options_str]( const char* arg ) {

            if( param_opt.chr )
            {
                cnt_options_chr[ param_opt.chr ] += 1;
            }
            else
            {
                cnt_options_str[ param_opt.str ? param_opt.str : "" ] += 1;
            }
            return 0;
        };

        ASSERT_GE( param_opt.req, 0 );
        ASSERT_LE( param_opt.req, 2 );
        Args::Kind kind = ( Args::Kind )( param_opt.req );

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

    Log::Stream s;
    Logger log( s );

    u32 res = options.parse( log );

    s.dump();

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

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
