//
//  Copyright (c) 2014-2017 CASM Organization https://casm-lang.org
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
//                https://github.com/casm-lang/libstdhl
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

#include "Data.h"

#include "Formatter.h"

using namespace libstdhl;
using namespace Log;

//
// Data
//

Data::Data(
    Level::ID level, const Source::Ptr& source, const Category::Ptr& category, const Items& items )
: Item( Item::ID::DATA )
, m_timestamp()
, m_level( level )
, m_source( source )
, m_category( category )
, m_items( items )
{
}

Data::Data( const Level::ID level, const Source::Ptr& source, const Category::Ptr& category )
: Data( level, source, category, Items() )
{
}

Data::Data(
    const Level::ID level,
    const Source::Ptr& source,
    const Category::Ptr& category,
    const std::string& text )
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
