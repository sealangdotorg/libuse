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

#include "Filter.h"
#include "Stream.h"

#include <cassert>

using namespace libstdhl;
using namespace Log;

//
// Filter
//

Filter::Filter( void )
: m_channel( nullptr )
, m_inverse( false )
{
}

Channel::Ptr Filter::channel( void ) const
{
    return m_channel;
}

void Filter::setChannel( const Channel::Ptr& channel )
{
    assert( channel );
    m_channel = channel;
}

void Filter::clear( void )
{
    m_sources.clear();
    m_categories.clear();
    m_levels.clear();
}

void Filter::setInverse( u1 inverse )
{
    m_inverse = inverse;
}

u1 Filter::inverse( void ) const
{
    return m_inverse;
}

void Filter::setSource( const Source::Ptr& source )
{
    m_sources.add( source );
}

void Filter::setCategory( const Category::Ptr& category )
{
    m_categories.add( category );
}

void Filter::setLevel( Level level )
{
    m_levels.emplace_back( level );
}

void Filter::setLevel( Level::ID level )
{
    m_levels.emplace_back( level );
}

void Filter::process( Stream& stream )
{
    Stream filtered;

    if( not m_channel )
    {
        return;
    }

    if( not m_inverse )
    {
        // normal filter case

        for( auto data : stream.data() )
        {
            for( auto source : m_sources )
            {
                if( *source == *data.source() )
                {
                    filtered.add( std::move( data ) );
                }
            }

            for( auto category : m_categories )
            {
                if( *category == *data.category() )
                {
                    filtered.add( std::move( data ) );
                }
            }

            for( auto level : m_levels )
            {
                if( level == data.level() )
                {
                    filtered.add( std::move( data ) );
                }
            }
        }
    }
    else
    {
        // inverse filter case

        for( auto data : stream.data() )
        {
            if( m_sources.size() != 0 )
            {
                for( auto source : m_sources )
                {
                    if( *source != *data.source() )
                    {
                        filtered.add( std::move( data ) );
                    }
                }
            }
            else
            {
                filtered.add( std::move( data ) );
            }

            if( m_categories.size() != 0 )
            {
                for( auto category : m_categories )
                {
                    if( *category != *data.category() )
                    {
                        filtered.add( std::move( data ) );
                    }
                }
            }
            else
            {
                filtered.add( std::move( data ) );
            }

            if( m_levels.size() != 0 )
            {
                for( auto level : m_levels )
                {
                    if( level != data.level() )
                    {
                        filtered.add( std::move( data ) );
                    }
                }
            }
            else
            {
                filtered.add( std::move( data ) );
            }
        }
    }

    m_channel->process( filtered );
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
