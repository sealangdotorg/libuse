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

String::String( const std::string& value )
: Layout()
{
    const auto size = value.size() + 1;
    m_data.ptr = malloc( sizeof( char ) * size );
    memcpy( m_data.ptr, value.data(), size );
    ( (char*)m_data.ptr )[ size - 1 ] = '\0';
}

String::~String( void )
{
    // if( m_data.ptr ) // PPA: FIXME:
    // {
    //     free( m_data.ptr );
    // }
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
