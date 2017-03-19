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

#include "Timestamp.h"

#include "Formatter.h"

using namespace libstdhl;
using namespace Log;

//
// Timestamp
//

Timestamp::Timestamp( void )
: m_timestamp( std::chrono::system_clock::now() )
{
}

std::chrono::system_clock::time_point Timestamp::timestamp( void ) const
{
    return m_timestamp;
}

std::time_t Timestamp::c_timestamp( void ) const
{
    return std::chrono::system_clock::to_time_t( m_timestamp );
}

std::string Timestamp::local( const std::string& format ) const
{
    const auto t = c_timestamp();

    std::stringstream s;
    s << std::put_time( std::localtime( &t ), format.c_str() );

    return s.str();
}

std::string Timestamp::utc( const std::string& format ) const
{
    const auto t = c_timestamp();

    std::stringstream s;
    s << std::put_time( std::gmtime( &t ), format.c_str() );

    return s.str();
}

std::string Timestamp::accept( Formatter& formatter )
{
    return formatter.visit( *this );
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
