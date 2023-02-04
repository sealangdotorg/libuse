//
//  Copyright (C) 2014-2023 CASM Organization <https://casm-lang.org>
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
