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

#include "Data.h"

#include "Formatter.h"

using namespace libstdhl;
using namespace Log;

//
// Data
//

Data::Data( Level::ID level, const Source::Ptr& source,
    const Category::Ptr& category, const Items& items )
: m_timestamp()
, m_level( level )
, m_source( source )
, m_category( category )
, m_items( items )
{
}

Data::Data( const Level::ID level, const Source::Ptr& source,
    const Category::Ptr& category )
: Data( level, source, category, Items() )
{
}

Data::Data( const Level::ID level, const Source::Ptr& source,
    const Category::Ptr& category, const std::string& text )
: Data( level, source, category )
{
    add< TextItem >( text );
}

Data::Data( const Level::ID level, const std::string& text )
: Data( level, Source::defaultSource(), Category::defaultCategory(), text )
{
}

Data::Data( const Level::ID level )
: Data( level, Source::defaultSource(), Category::defaultCategory() )
{
}

Timestamp Data::timestamp( void ) const
{
    return m_timestamp;
}

Level Data::level( void ) const
{
    return m_level;
}

void Data::setSource( const Source::Ptr& source )
{
    m_source = source;
}

Source::Ptr Data::source( void ) const
{
    return m_source;
}

void Data::setCategory( const Category::Ptr& category )
{
    m_category = category;
}

Category::Ptr Data::category( void ) const
{
    return m_category;
}

void Data::setItems( const Items& items )
{
    m_items = items;
}

Items Data::items( void ) const
{
    return m_items;
}

std::string Data::accept( Formatter& formatter )
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
