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

#include "libstdhl.h"

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

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
