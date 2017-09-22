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
using namespace Standard;
using namespace RFC3986;

TEST( libstdhl_cpp_standard_rfc3986, https_link )
{
    const auto uri = UniformResourceIdentifier::fromString(
        "https://code.visualstudio.com/docs/extensions/overview#frag" );

    EXPECT_STREQ( uri.scheme().c_str(), "https" );
    EXPECT_STREQ( uri.authority().c_str(), "code.visualstudio.com" );
    EXPECT_STREQ( uri.path().c_str(), "/docs/extensions/overview" );
    EXPECT_STREQ( uri.query().c_str(), "" );
    EXPECT_STREQ( uri.fragment().c_str(), "frag" );
    EXPECT_STREQ( uri.toString().c_str(),
        "https://code.visualstudio.com/docs/extensions/overview#frag" );
}

TEST( libstdhl_cpp_standard_rfc3986, file_path_ascii_only )
{
    const auto uri = UniformResourceIdentifier::fromString(
        "file:///users/me/c-projects/" );

    EXPECT_STREQ( uri.scheme().c_str(), "file" );
    EXPECT_STREQ( uri.authority().c_str(), "" );
    EXPECT_STREQ( uri.path().c_str(), "/users/me/c-projects/" );
    EXPECT_STREQ( uri.query().c_str(), "" );
    EXPECT_STREQ( uri.fragment().c_str(), "" );
    EXPECT_STREQ( uri.toString().c_str(), "file:///users/me/c-projects/" );
}

TEST( libstdhl_cpp_standard_rfc3986, invalid_uri_path )
{
    EXPECT_THROW(
        UniformResourceIdentifier::fromString( "this is not a valid uri path" );
        , std::invalid_argument );
}

TEST( DISABLED_libstdhl_cpp_standard_rfc3986, file_path_with_sharp )
{
    const auto uri = UniformResourceIdentifier::fromString(
        "file:///users/me/c#-projects/" );

    EXPECT_STREQ( uri.scheme().c_str(), "file" );
    EXPECT_STREQ( uri.authority().c_str(), "" );
    EXPECT_STREQ( uri.path().c_str(), "/users/me/c#-projects/" );
    EXPECT_STREQ( uri.query().c_str(), "" );
    EXPECT_STREQ( uri.fragment().c_str(), "" );
    EXPECT_STREQ( uri.toString().c_str(), "file:///users/me/c%23-projects/" );
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
