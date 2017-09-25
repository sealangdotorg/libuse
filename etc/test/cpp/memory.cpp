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

#include <libstdhl/Test>

using namespace libstdhl;
using namespace Memory;

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

    inline u1 operator==( const TestClass& rhs ) const
    {
        return this->value() == rhs.value();
    }

    std::unordered_map< std::string, TestClass::Ptr >& make_cache( void )
    {
        static std::unordered_map< std::string, TestClass::Ptr > cache;
        return cache;
    }

    const char* make_hash( void )
    {
        return Allocator::string( std::to_string( value() ) );
    }

  private:
    u32 m_value;
};

TEST( libstdhl_cpp_Default, make_unique )
{
    auto a = make_unique< TestClass >( 123 );
    auto b = make_unique< TestClass >( 123 );

    EXPECT_NE( a, b );
    EXPECT_NE( a.get(), b.get() );
    EXPECT_EQ( a->value(), b->value() );
}

TEST( libstdhl_cpp_Default, make )
{
    auto a = make< TestClass >( 123 );
    auto b = make< TestClass >( 123 );

    EXPECT_NE( a, b );
    EXPECT_NE( a.get(), b.get() );
    EXPECT_EQ( a->value(), b->value() );
}

TEST( libstdhl_cpp_Default, get )
{
    auto a = get< TestClass >( 321 );
    auto b = get< TestClass >( 321 );

    EXPECT_TRUE( a != nullptr );
    EXPECT_TRUE( b != nullptr );

    EXPECT_EQ( a, b );
    EXPECT_EQ( a.get(), b.get() );
    EXPECT_EQ( *a, *b );
    EXPECT_EQ( a->value(), b->value() );
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
