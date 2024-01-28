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
using namespace Standard;
using namespace RFC3986;

TEST( libstdhl_cpp_standard_rfc3986, https_link )
{
    const auto uri = UniformResourceIdentifier::fromString(
        "https://code.visualstudio.com/docs/extensions/overview?test=true#frag" );

    EXPECT_STREQ( uri.scheme().c_str(), "https" );
    EXPECT_STREQ( uri.authority().c_str(), "code.visualstudio.com" );
    EXPECT_STREQ( uri.path().c_str(), "/docs/extensions/overview" );
    EXPECT_STREQ( uri.query().c_str(), "test=true" );
    EXPECT_STREQ( uri.fragment().c_str(), "frag" );
    EXPECT_STREQ(
        uri.toString().c_str(),
        "https://code.visualstudio.com/docs/extensions/overview?test=true#frag" );
}

TEST( libstdhl_cpp_standard_rfc3986, file_path_absolute_ascii_only )
{
    const auto uri = UniformResourceIdentifier::fromString( "file:///users/me/c-projects/" );

    EXPECT_STREQ( uri.scheme().c_str(), "file" );
    EXPECT_STREQ( uri.authority().c_str(), "" );
    EXPECT_STREQ( uri.path().c_str(), "/users/me/c-projects/" );
    EXPECT_STREQ( uri.query().c_str(), "" );
    EXPECT_STREQ( uri.fragment().c_str(), "" );
    EXPECT_STREQ( uri.toString().c_str(), "file:///users/me/c-projects/" );
}

TEST( libstdhl_cpp_standard_rfc3986, file_path_relative_ascii_only )
{
    const auto uri = UniformResourceIdentifier::fromString( "file:///./users/me/c-projects/" );

    EXPECT_STREQ( uri.scheme().c_str(), "file" );
    EXPECT_STREQ( uri.authority().c_str(), "" );
    EXPECT_STREQ( uri.path().c_str(), "/./users/me/c-projects/" );
    EXPECT_STREQ( uri.query().c_str(), "" );
    EXPECT_STREQ( uri.fragment().c_str(), "" );
    EXPECT_STREQ( uri.toString().c_str(), "file:///./users/me/c-projects/" );
}

TEST( libstdhl_cpp_standard_rfc3986, invalid_uri_path )
{
    EXPECT_THROW( UniformResourceIdentifier::fromString( "this is not a valid uri path" );
                  , std::invalid_argument );
}

TEST( DISABLED_libstdhl_cpp_standard_rfc3986, file_path_with_sharp )
{
    const auto uri = UniformResourceIdentifier::fromString( "file:///users/me/c#-projects/" );

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
