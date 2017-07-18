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

#include "String.h"

using namespace libstdhl;
using namespace Type;

//
// Type::create*
//

String Type::createString( const std::string& value )
{
    return String( new StringLayout( value ) );
}

//
// String
//

std::string String::toString( void ) const
{
    const auto data = static_cast< StringLayout* >( m_data.ptr );
    return data->str();
}

String& String::operator+=( const String& rhs )
{
    auto lval = static_cast< StringLayout* >( m_data.ptr );
    const auto rval = static_cast< StringLayout* >( rhs.m_data.ptr );

    lval->operator+=( *rval );

    return *this;
}

//
// StringLayout
//

StringLayout::StringLayout( const std::string& value )
: m_str( value )
{
}

Layout* StringLayout::clone( void ) const
{
    return new StringLayout( m_str );
}

const std::string& StringLayout::str( void ) const
{
    return m_str;
}

StringLayout& StringLayout::operator+=( const StringLayout& rhs )
{
    m_str += rhs.m_str;
    return *this;
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
