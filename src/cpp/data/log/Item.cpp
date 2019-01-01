//
//  Copyright (C) 2014-2019 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
//                <https://github.com/casm-lang/libstdhl>
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

#include "Item.h"
#include "Formatter.h"

using namespace libstdhl;
using namespace Log;

Item::Item( const ID id )
: m_id( id )
{
}

Item::ID Item::id( void ) const
{
    return m_id;
}

//
// TextItem
//

TextItem::TextItem( const std::string& text )
: Item( Item::ID::TEXT )
, m_text( text )
{
}

std::string TextItem::text( void ) const
{
    return m_text;
}

std::string TextItem::accept( Formatter& formatter )
{
    return formatter.visit( *this );
}

//
// PositionItem
//

PositionItem::PositionItem( const u64 line, const u64 column )
: Item( Item::ID::POSITION )
, m_line( line )
, m_column( column )
{
}

u64 PositionItem::line( void ) const
{
    return m_line;
}

u64 PositionItem::column( void ) const
{
    return m_column;
}

std::string PositionItem::accept( Formatter& formatter )
{
    return formatter.visit( *this );
}

//
// RangeItem
//

RangeItem::RangeItem( const PositionItem& begin, const PositionItem& end )
: Item( Item::ID::RANGE )
, m_begin( begin )
, m_end( end )
{
}

PositionItem RangeItem::begin( void ) const
{
    return m_begin;
}

PositionItem RangeItem::end( void ) const
{
    return m_end;
}

std::string RangeItem::accept( Formatter& formatter )
{
    return formatter.visit( *this );
}

//
// LocationItem
//

LocationItem::LocationItem( const TextItem& filename, const RangeItem& range )
: Item( Item::ID::LOCATION )
, m_filename( filename )
, m_range( range )
{
}

LocationItem::LocationItem(
    const std::string& filename,
    const u64 beginLine,
    const u64 beginColumn,
    const u64 endLine,
    const u64 endColumn )
: LocationItem(
      TextItem( filename ),
      RangeItem( PositionItem( beginLine, beginColumn ), PositionItem( endLine, endColumn ) ) )
{
}

TextItem LocationItem::filename( void ) const
{
    return m_filename;
}

RangeItem LocationItem::range( void ) const
{
    return m_range;
}

std::string LocationItem::accept( Formatter& formatter )
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
