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

class TestClass
{
  public:
    using Ptr = std::shared_ptr< TestClass >;

    TestClass( u32 value )
    : m_value( value )
    {
    }

    const u32 value() const
    {
        return m_value;
    }

    std::unordered_map< std::string, Ptr >& make_cache( void )
    {
        static std::unordered_map< std::string, Ptr > cache;
        return cache;
    }

    const char* make_hash( void )
    {
        return Allocator::string( std::to_string( value() ) );
    }

  private:
    u32 m_value;
};

TEST( libstdhl_cpp_Default, make )
{
    auto a = make< TestClass >( 123 );
    auto b = make< TestClass >( 123 );

    ASSERT_NE( a, b );
    ASSERT_EQ( a->value(), b->value() );
}

TEST( libstdhl_cpp_Default, get )
{
    auto a = get< TestClass >( 321 );
    auto b = get< TestClass >( 321 );

    ASSERT_EQ( a, b );
    ASSERT_EQ( a->value(), b->value() );
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
