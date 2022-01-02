//
//  Copyright (C) 2014-2022 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber et al.
//                <https://github.com/casm-lang/libstdhl/graphs/contributors>
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

TEST( libstdhl_cpp_Yaml, parsing_and_emitting )
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

    EXPECT_THROW( map.has( 123 ), libstdhl::Yaml::Exception );
    EXPECT_FALSE( map.has( "value" ) );
    ASSERT_TRUE( map.has( "key" ) );
    auto sequence = map[ "key" ];
    EXPECT_STREQ( sequence.description().c_str(), "Sequence" );
    EXPECT_EQ( sequence.type(), libstdhl::Yaml::Type::SEQUENCE );
    EXPECT_EQ( sequence.size(), 9 );

    EXPECT_THROW( sequence.has( "key" ), libstdhl::Yaml::Exception );
    EXPECT_FALSE( sequence.has( 123 ) );
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

TEST( libstdhl_cpp_Yaml, parsing_invalid_file_triggers_exception )
{
    static const auto source = R"***(

- : - : -

)***";

    EXPECT_THROW( libstdhl::Yaml::Content::fromString( source ), libstdhl::Yaml::Exception );
}

//
//
// Yaml::Sequence
//

TEST( libstdhl_cpp_Yaml, sequence_find_has_with_no_elements )
{
    // GIVEN
    auto yaml = libstdhl::Yaml::Content();
    EXPECT_EQ( yaml.size(), 0 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::NONE );

    // WHEN
    auto hasValue = yaml.has( 123 );
    auto findValue = yaml.find( 123 );

    // THEN
    EXPECT_EQ( yaml.size(), 0 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::NONE );
    EXPECT_EQ( yaml.description(), "None" );

    EXPECT_THROW( yaml[ 123 ], libstdhl::Yaml::Exception );

    EXPECT_FALSE( hasValue );
    EXPECT_FALSE( findValue.has_value() );
}

TEST( libstdhl_cpp_Yaml, sequence_find_element_at_existing_index )
{
    // GIVEN
    auto yaml = libstdhl::Yaml::Content();
    EXPECT_EQ( yaml.size(), 0 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::NONE );
    yaml.emplace_back();
    yaml.emplace_back();
    yaml.emplace_back();
    EXPECT_EQ( yaml.size(), 3 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::SEQUENCE );

    // WHEN
    auto value = yaml.find( 2 );

    // THEN
    EXPECT_EQ( yaml.size(), 3 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::SEQUENCE );

    ASSERT_TRUE( value.has_value() );
}

TEST( libstdhl_cpp_Yaml, sequence_find_element_at_non_existing_index )
{
    // GIVEN
    auto yaml = libstdhl::Yaml::Content();
    EXPECT_EQ( yaml.size(), 0 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::NONE );
    yaml.emplace_back();
    yaml.emplace_back();
    yaml.emplace_back();
    EXPECT_EQ( yaml.size(), 3 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::SEQUENCE );

    // WHEN
    auto value = yaml.find( 9 );

    // THEN
    EXPECT_EQ( yaml.size(), 3 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::SEQUENCE );

    ASSERT_FALSE( value.has_value() );
}

TEST( libstdhl_cpp_Yaml, sequence_emplace_back )
{
    // GIVEN
    auto yaml = libstdhl::Yaml::Content();
    EXPECT_EQ( yaml.size(), 0 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::NONE );

    // WHEN
    yaml.emplace_back();  // 0
    yaml.emplace_back();  // 1
    yaml.emplace_back();  // 2

    // THEN
    EXPECT_EQ( yaml.size(), 3 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::SEQUENCE );
}

TEST( libstdhl_cpp_Yaml, sequence_emplace_in_empty_index )
{
    // GIVEN
    auto yaml = libstdhl::Yaml::Content();
    EXPECT_EQ( yaml.size(), 0 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::NONE );

    auto sequenceValue = libstdhl::Yaml::Content();
    sequenceValue.emplace();
    EXPECT_EQ( sequenceValue.size(), 1 );
    EXPECT_EQ( sequenceValue.type(), libstdhl::Yaml::Type::SEQUENCE );

    // WHEN
    const auto sequenceIndex = 123;
    auto value = yaml.emplace( sequenceValue, sequenceIndex );
    // since the yaml is empty a new sequence gets created and placed at index 0

    // THEN
    EXPECT_EQ( yaml.size(), 1 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::SEQUENCE );

    ASSERT_FALSE( yaml.has( sequenceIndex ) );
    ASSERT_FALSE( yaml.has( 1 ) );
    ASSERT_TRUE( yaml.has( 0 ) );
    EXPECT_EQ( yaml[ 0 ].size(), sequenceValue.size() );
    EXPECT_EQ( yaml[ 0 ].type(), sequenceValue.type() );
    EXPECT_STREQ( yaml[ 0 ].dump().c_str(), sequenceValue.dump().c_str() );

    ASSERT_FALSE( value.has_value() );
}

TEST( libstdhl_cpp_Yaml, sequence_emplace_at_non_existing_index )
{
    // GIVEN
    auto yaml = libstdhl::Yaml::Content();
    EXPECT_EQ( yaml.size(), 0 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::NONE );
    yaml.emplace_back();  // 0
    yaml.emplace_back();  // 1
    yaml.emplace_back();  // 2
    EXPECT_EQ( yaml.size(), 3 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::SEQUENCE );

    auto sequenceValue = libstdhl::Yaml::Content();
    sequenceValue.emplace();
    EXPECT_EQ( sequenceValue.size(), 1 );
    EXPECT_EQ( sequenceValue.type(), libstdhl::Yaml::Type::SEQUENCE );

    // WHEN
    const auto sequenceIndex = 123;
    auto value = yaml.emplace( sequenceValue, sequenceIndex );

    // THEN
    EXPECT_EQ( yaml.size(), 4 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::SEQUENCE );

    ASSERT_FALSE( yaml.has( sequenceIndex ) );
    ASSERT_FALSE( yaml.has( -1 ) );
    ASSERT_TRUE( yaml.has( 0 ) );
    ASSERT_TRUE( yaml.has( 1 ) );
    ASSERT_TRUE( yaml.has( 2 ) );
    ASSERT_TRUE( yaml.has( 3 ) );
    ASSERT_FALSE( yaml.has( 4 ) );

    EXPECT_EQ( yaml[ 3 ].size(), sequenceValue.size() );
    EXPECT_EQ( yaml[ 3 ].type(), sequenceValue.type() );
    EXPECT_STREQ( yaml[ 3 ].dump().c_str(), sequenceValue.dump().c_str() );

    ASSERT_FALSE( value.has_value() );
}

TEST( libstdhl_cpp_Yaml, sequence_emplace_at_existing_index )
{
    // GIVEN
    auto yaml = libstdhl::Yaml::Content();
    EXPECT_EQ( yaml.size(), 0 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::NONE );
    yaml.emplace_back();  // 0
    yaml.emplace_back();  // 1
    yaml.emplace_back();  // 2
    EXPECT_EQ( yaml.size(), 3 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::SEQUENCE );

    auto sequenceValue = libstdhl::Yaml::Content();
    sequenceValue.emplace();
    EXPECT_EQ( sequenceValue.size(), 1 );
    EXPECT_EQ( sequenceValue.type(), libstdhl::Yaml::Type::SEQUENCE );

    // WHEN
    const auto sequenceIndex = 1;
    auto value = yaml.emplace( sequenceValue, sequenceIndex );

    // THEN
    EXPECT_EQ( yaml.size(), 3 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::SEQUENCE );

    ASSERT_TRUE( yaml.has( sequenceIndex ) );
    EXPECT_EQ( yaml[ sequenceIndex ].size(), sequenceValue.size() );
    EXPECT_EQ( yaml[ sequenceIndex ].type(), sequenceValue.type() );
    EXPECT_STREQ( yaml[ sequenceIndex ].dump().c_str(), sequenceValue.dump().c_str() );

    ASSERT_TRUE( value.has_value() );
    EXPECT_EQ( ( *value ).size(), 0 );
    EXPECT_EQ( ( *value ).type(), libstdhl::Yaml::Type::NONE );
    EXPECT_STREQ( ( *value ).dump().c_str(), "" );
}

TEST( libstdhl_cpp_Yaml, sequence_erase_existing_index )
{
    // GIVEN
    auto yaml = libstdhl::Yaml::Content();
    EXPECT_EQ( yaml.size(), 0 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::NONE );
    yaml.emplace_back();  // 0
    yaml.emplace_back();  // 1
    yaml.emplace_back();  // 2
    EXPECT_EQ( yaml.size(), 3 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::SEQUENCE );

    // WHEN
    const auto sequenceIndex = 1;
    yaml.erase( sequenceIndex );

    // THEN
    EXPECT_EQ( yaml.size(), 2 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::SEQUENCE );

    ASSERT_FALSE( yaml.has( -1 ) );
    ASSERT_TRUE( yaml.has( 0 ) );
    ASSERT_TRUE( yaml.has( 1 ) );
    ASSERT_FALSE( yaml.has( 2 ) );
    ASSERT_FALSE( yaml.has( 3 ) );
}

TEST( libstdhl_cpp_Yaml, sequence_erase_non_existing_index_triggers_exception )
{
    // GIVEN
    auto yaml = libstdhl::Yaml::Content();
    EXPECT_EQ( yaml.size(), 0 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::NONE );
    yaml.emplace_back();  // 0
    yaml.emplace_back();  // 1
    yaml.emplace_back();  // 2
    EXPECT_EQ( yaml.size(), 3 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::SEQUENCE );

    // WHEN
    const auto sequenceIndex = 123;

    // THEN
    EXPECT_THROW( yaml.erase( sequenceIndex ), libstdhl::Yaml::Exception );

    EXPECT_EQ( yaml.size(), 3 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::SEQUENCE );

    ASSERT_FALSE( yaml.has( -1 ) );
    ASSERT_TRUE( yaml.has( 0 ) );
    ASSERT_TRUE( yaml.has( 1 ) );
    ASSERT_TRUE( yaml.has( 2 ) );
    ASSERT_FALSE( yaml.has( 3 ) );
}

//
//
// Yaml::Map
//

TEST( libstdhl_cpp_Yaml, map_find_has_with_no_elements )
{
    // GIVEN
    auto yaml = libstdhl::Yaml::Content();
    EXPECT_EQ( yaml.size(), 0 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::NONE );

    // WHEN
    auto hasValue = yaml.has( "key" );
    auto findValue = yaml.find( "key" );

    // THEN
    EXPECT_EQ( yaml.size(), 0 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::NONE );
    EXPECT_EQ( yaml.description(), "None" );

    EXPECT_THROW( yaml[ "key" ], libstdhl::Yaml::Exception );

    EXPECT_FALSE( hasValue );
    EXPECT_FALSE( findValue.has_value() );
}

TEST( libstdhl_cpp_Yaml, map_find_element_at_existing_key )
{
    // GIVEN
    auto yaml = libstdhl::Yaml::Content();
    EXPECT_EQ( yaml.size(), 0 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::NONE );
    yaml.emplace( "foo" );
    yaml.emplace( "bar" );
    yaml.emplace( "qux" );
    EXPECT_EQ( yaml.size(), 3 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::MAP );

    // WHEN
    auto value = yaml.find( "foo" );

    // THEN
    EXPECT_EQ( yaml.size(), 3 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::MAP );

    ASSERT_TRUE( value.has_value() );
}

TEST( libstdhl_cpp_Yaml, map_find_element_at_non_existing_key )
{
    // GIVEN
    auto yaml = libstdhl::Yaml::Content();
    EXPECT_EQ( yaml.size(), 0 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::NONE );
    yaml.emplace( "foo" );
    yaml.emplace( "bar" );
    yaml.emplace( "qux" );
    EXPECT_EQ( yaml.size(), 3 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::MAP );

    // WHEN
    auto value = yaml.find( "foobarqux" );

    // THEN
    EXPECT_EQ( yaml.size(), 3 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::MAP );

    ASSERT_FALSE( value.has_value() );
}

TEST( libstdhl_cpp_Yaml, map_emplace_in_empty_map )
{
    // GIVEN
    auto yaml = libstdhl::Yaml::Content();
    EXPECT_EQ( yaml.size(), 0 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::NONE );

    // WHEN
    const auto mapKey = "myKey";
    auto mapValue = libstdhl::Yaml::Content();
    mapValue.emplace( "myValue" );
    EXPECT_EQ( mapValue.size(), 1 );
    EXPECT_EQ( mapValue.type(), libstdhl::Yaml::Type::MAP );
    auto value = yaml.emplace( mapKey, mapValue );

    // THEN
    EXPECT_EQ( yaml.size(), 1 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::MAP );

    ASSERT_TRUE( yaml.has( mapKey ) );
    EXPECT_EQ( yaml[ mapKey ].size(), mapValue.size() );
    EXPECT_EQ( yaml[ mapKey ].type(), mapValue.type() );
    EXPECT_STREQ( yaml[ mapKey ].dump().c_str(), mapValue.dump().c_str() );

    ASSERT_FALSE( value.has_value() );
}

TEST( libstdhl_cpp_Yaml, map_emplace_at_non_existing_key )
{
    // GIVEN
    auto yaml = libstdhl::Yaml::Content();
    EXPECT_EQ( yaml.size(), 0 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::NONE );
    yaml.emplace( "foo" );
    yaml.emplace( "bar" );
    yaml.emplace( "qux" );
    EXPECT_EQ( yaml.size(), 3 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::MAP );

    // WHEN
    const auto mapKey = "myKey";
    auto mapValue = libstdhl::Yaml::Content();
    mapValue.emplace( "myValue" );
    EXPECT_EQ( mapValue.size(), 1 );
    EXPECT_EQ( mapValue.type(), libstdhl::Yaml::Type::MAP );
    auto value = yaml.emplace( mapKey, mapValue );

    // THEN
    EXPECT_EQ( yaml.size(), 4 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::MAP );

    ASSERT_TRUE( yaml.has( mapKey ) );
    EXPECT_EQ( yaml[ mapKey ].size(), mapValue.size() );
    EXPECT_EQ( yaml[ mapKey ].type(), mapValue.type() );
    EXPECT_STREQ( yaml[ mapKey ].dump().c_str(), mapValue.dump().c_str() );

    ASSERT_FALSE( value.has_value() );
}

TEST( libstdhl_cpp_Yaml, map_emplace_at_existing_key )
{
    // GIVEN
    auto yaml = libstdhl::Yaml::Content();
    EXPECT_EQ( yaml.size(), 0 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::NONE );
    yaml.emplace( "foo" );
    yaml.emplace( "bar" );
    yaml.emplace( "qux" );
    EXPECT_EQ( yaml.size(), 3 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::MAP );

    // WHEN
    const auto mapKey = "bar";
    auto mapValue = libstdhl::Yaml::Content();
    mapValue.emplace( "myValue" );
    EXPECT_EQ( mapValue.size(), 1 );
    EXPECT_EQ( mapValue.type(), libstdhl::Yaml::Type::MAP );
    auto value = yaml.emplace( mapKey, mapValue );

    // THEN
    EXPECT_EQ( yaml.size(), 3 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::MAP );

    ASSERT_TRUE( yaml.has( mapKey ) );
    EXPECT_EQ( yaml[ mapKey ].size(), mapValue.size() );
    EXPECT_EQ( yaml[ mapKey ].type(), mapValue.type() );
    EXPECT_STREQ( yaml[ mapKey ].dump().c_str(), mapValue.dump().c_str() );

    ASSERT_TRUE( value.has_value() );
    EXPECT_EQ( value.value().size(), 0 );
    EXPECT_EQ( value.value().type(), libstdhl::Yaml::Type::NONE );
    EXPECT_STREQ( value.value().dump().c_str(), "" );
}

TEST( libstdhl_cpp_Yaml, map_erase_existing_key )
{
    // GIVEN
    auto yaml = libstdhl::Yaml::Content();
    EXPECT_EQ( yaml.size(), 0 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::NONE );
    yaml.emplace( "foo" );
    yaml.emplace( "bar" );
    yaml.emplace( "qux" );
    EXPECT_EQ( yaml.size(), 3 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::MAP );

    // WHEN
    const auto mapKey = "bar";
    yaml.erase( mapKey );

    // THEN
    EXPECT_EQ( yaml.size(), 2 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::MAP );

    EXPECT_TRUE( yaml.has( "foo" ) );
    EXPECT_FALSE( yaml.has( "bar" ) );
    EXPECT_TRUE( yaml.has( "qux" ) );
}

TEST( libstdhl_cpp_Yaml, map_erase_non_existing_key_triggers_exception )
{
    // GIVEN
    auto yaml = libstdhl::Yaml::Content();
    EXPECT_EQ( yaml.size(), 0 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::NONE );
    yaml.emplace( "foo" );
    yaml.emplace( "bar" );
    yaml.emplace( "qux" );
    EXPECT_EQ( yaml.size(), 3 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::MAP );

    // WHEN
    const auto mapKey = "foobarqux";

    // THEN
    EXPECT_THROW( yaml.erase( mapKey ), libstdhl::Yaml::Exception );

    EXPECT_EQ( yaml.size(), 3 );
    EXPECT_EQ( yaml.type(), libstdhl::Yaml::Type::MAP );

    EXPECT_TRUE( yaml.has( "foo" ) );
    EXPECT_TRUE( yaml.has( "bar" ) );
    EXPECT_TRUE( yaml.has( "qux" ) );
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
