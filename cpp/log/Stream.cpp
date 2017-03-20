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

#include "Stream.h"

#include "Channel.h"

using namespace libstdhl;
using namespace Log;

//
// Stream
//

Stream::Stream( void )
{
}

std::vector< Data > Stream::data( void ) const
{
    return m_data;
}

void Stream::flush( Channel& channel )
{
    channel.process( *this );
    m_data.clear();
}

void Stream::aggregate( const Stream& stream )
{
    const auto stream_data = stream.data();

    m_data.insert( std::end( m_data ), std::begin( stream_data ),
        std::end( stream_data ) );

    std::sort(
        m_data.begin(), m_data.end(), []( const Data& a, const Data& b ) {
            return b.timestamp() >= a.timestamp();
        } );
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
