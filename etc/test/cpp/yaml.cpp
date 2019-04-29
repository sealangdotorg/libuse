//
//  Copyright (C) 2014-2019 CASM Organization <https://casm-lang.org>
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

TEST( libstdhl_cpp_Yaml, example )
{
    static const auto source = R"***(key: 
  - text
  - 1234
  - "-4321"
  - true
  - false
  - yes
  - no
  - 1
  - 0
)***";

    auto yaml = libstdhl::Yaml::Content::fromString( source );

    EXPECT_STREQ( yaml.dump().c_str(), source );

    auto map = yaml;
    EXPECT_STREQ( map.description().c_str(), "Map" );
    EXPECT_EQ( map.type(), libstdhl::Yaml::Type::MAP );
    EXPECT_EQ( map.size(), 1 );

    EXPECT_FALSE( map.has( "value" ) );
    ASSERT_TRUE( map.has( "key" ) );
    auto sequence = map[ "key" ];
    EXPECT_STREQ( sequence.description().c_str(), "Sequence" );
    EXPECT_EQ( sequence.type(), libstdhl::Yaml::Type::SEQUENCE );
    EXPECT_EQ( sequence.size(), 9 );

    EXPECT_FALSE( map.has( 123 ) );
    ASSERT_TRUE( sequence.has( 0 ) );
    {
        auto scalar = sequence[ 0 ];
        EXPECT_STREQ( scalar.description().c_str(), "String" );
        EXPECT_EQ( scalar.type(), libstdhl::Yaml::Type::STRING );
        EXPECT_STREQ( scalar.as< std::string >().c_str(), "text" );
        EXPECT_EQ( scalar.size(), 0 );
    }

    ASSERT_TRUE( sequence.has( 1 ) );
    {
        auto scalar = sequence[ 1 ];
        EXPECT_STREQ( scalar.description().c_str(), "Integer" );
        EXPECT_EQ( scalar.type(), libstdhl::Yaml::Type::INTEGER );
        EXPECT_EQ( scalar.as< int >(), 1234 );
        EXPECT_EQ( scalar.size(), 0 );
    }

    ASSERT_TRUE( sequence.has( 2 ) );
    {
        auto scalar = sequence[ 2 ];
        EXPECT_STREQ( scalar.description().c_str(), "Integer" );
        EXPECT_EQ( scalar.type(), libstdhl::Yaml::Type::INTEGER );
        EXPECT_EQ( scalar.as< int >(), -4321 );
        EXPECT_EQ( scalar.size(), 0 );
    }

    ASSERT_TRUE( sequence.has( 3 ) );
    {
        auto scalar = sequence[ 3 ];
        EXPECT_STREQ( scalar.description().c_str(), "Boolean" );
        EXPECT_EQ( scalar.type(), libstdhl::Yaml::Type::BOOLEAN );
        EXPECT_EQ( scalar.as< bool >(), true );
        EXPECT_EQ( scalar.size(), 0 );
    }

    ASSERT_TRUE( sequence.has( 4 ) );
    {
        auto scalar = sequence[ 4 ];
        EXPECT_STREQ( scalar.description().c_str(), "Boolean" );
        EXPECT_EQ( scalar.type(), libstdhl::Yaml::Type::BOOLEAN );
        EXPECT_EQ( scalar.as< bool >(), false );
        EXPECT_EQ( scalar.size(), 0 );
    }

    ASSERT_TRUE( sequence.has( 5 ) );
    {
        auto scalar = sequence[ 5 ];
        EXPECT_STREQ( scalar.description().c_str(), "Boolean" );
        EXPECT_EQ( scalar.type(), libstdhl::Yaml::Type::BOOLEAN );
        EXPECT_EQ( scalar.as< bool >(), true );
        EXPECT_EQ( scalar.size(), 0 );
    }

    ASSERT_TRUE( sequence.has( 6 ) );
    {
        auto scalar = sequence[ 6 ];
        EXPECT_STREQ( scalar.description().c_str(), "Boolean" );
        EXPECT_EQ( scalar.type(), libstdhl::Yaml::Type::BOOLEAN );
        EXPECT_EQ( scalar.as< bool >(), false );
        EXPECT_EQ( scalar.size(), 0 );
    }

    ASSERT_TRUE( sequence.has( 7 ) );
    {
        auto scalar = sequence[ 7 ];
        EXPECT_STREQ( scalar.description().c_str(), "Boolean" );
        EXPECT_EQ( scalar.type(), libstdhl::Yaml::Type::BOOLEAN );
        EXPECT_EQ( scalar.as< bool >(), true );
        EXPECT_EQ( scalar.size(), 0 );
    }

    ASSERT_TRUE( sequence.has( 8 ) );
    {
        auto scalar = sequence[ 8 ];
        EXPECT_STREQ( scalar.description().c_str(), "Boolean" );
        EXPECT_EQ( scalar.type(), libstdhl::Yaml::Type::BOOLEAN );
        EXPECT_EQ( scalar.as< bool >(), false );
        EXPECT_EQ( scalar.size(), 0 );
    }
}

TEST( libstdhl_cpp_Yaml, invalid_file_triggers_exception )
{
    static const auto source = R"***(

- : - : -

)***";

    EXPECT_THROW( libstdhl::Yaml::Content::fromString( source ), ::Yaml::Exception );
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
