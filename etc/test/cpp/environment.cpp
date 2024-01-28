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

TEST( libstdhl_cpp_Environment_Variable, get_positive )
{
    // get variable 'PATH' exists on all major platforms
    // --> compare it with zero length string, because they definitely have some paths in it ;-)
    EXPECT_STRNE( Environment::Variable::get( "PATH" ).c_str(), "" );
}

TEST( libstdhl_cpp_Environment_Variable, get_negative )
{
    // get variable TEST_NAME does not exist
    // --> shall throw an Exception
    EXPECT_THROW( Environment::Variable::get( TEST_NAME ), Exception );
}

TEST( libstdhl_cpp_Environment_Variable, set_positive )
{
    // set variable field with the same key and value of '701af7499ddf0b416a4693977ade282a'
    const auto field = "701af7499ddf0b416a4693977ade282a";
    Environment::Variable::set( field, field );

    // check if the variable is set and its key equals the value
    EXPECT_STREQ( Environment::Variable::get( field ).c_str(), field );

    // clean-up, by deleting (del) the variable field
    Environment::Variable::del( field );
}

TEST( libstdhl_cpp_Environment_Variable, set_negative )
{
    // set variable "" with value TEST_NAME
    // --> shall throw an Exception, because "" is an invalid variable name
    EXPECT_THROW( Environment::Variable::set( "", TEST_NAME ), Exception );
}

TEST( libstdhl_cpp_Environment_Variable, has_positive )
{
    EXPECT_TRUE( Environment::Variable::has( "PATH" ) );
}

TEST( libstdhl_cpp_Environment_Variable, has_negative )
{
    EXPECT_FALSE( Environment::Variable::has( "701af7499ddf0b416a4693977ade282a" ) );
}

TEST( libstdhl_cpp_Environment_Variable, del_positive )
{
    // set variable field with the same key and value of '701af7499ddf0b416a4693977ade282a'
    const auto field = "701af7499ddf0b416a4693977ade282a";
    Environment::Variable::set( field, field );

    // clean-up, by deleting (del) the variable field
    Environment::Variable::del( field );
}

TEST( libstdhl_cpp_Environment_Variable, del_negative )
{
    // del variable field
    // --> shall throw an Exception, because field does not exist!
    const auto field = "701af7499ddf0b416a4693977ade282a";
    EXPECT_THROW( Environment::Variable::del( field ), Exception );
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
