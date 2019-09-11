//
//  Copyright (C) 2014-2019 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
//                Christian Lascsak
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

#include "Unicode.h"

#include <cassert>

/**
   @brief    TODO

   TODO
*/

using namespace libstdhl;
using namespace Unicode;

//
//
// Unicode
//

u1 Unicode::inside( const UTF8& character, const std::vector< Unicode::Block >& blocks )
{
    const auto utf8UnicodePoint = character.point();
    for( auto block : blocks )
    {
        const auto range = Unicode::Range::of( block );
        if( utf8UnicodePoint >= range.start() and utf8UnicodePoint <= range.end() )
        {
            return true;
        }
    }

    return false;
}

//
//
// Range
//

Range::Range( const u32 start, const u32 end, const Plane plane )
: m_start( start )
, m_end( end )
, m_plane( plane )
{
    assert( start < end and "invalid Unicode range" );
}

const u32 Range::start( void ) const
{
    return m_start;
}

const u32 Range::end( void ) const
{
    return m_end;
}

const Plane Range::plane( void ) const
{
    return m_plane;
}

Range Range::of( const Block block )
{
    const u32 start = static_cast< const u32 >( block );

    // Block Range | Block Name | Code Points | Assigned Characters | Scripts
    switch( block )
    {
        // Plane 0, the Basic Multilingual Plane (BMP)
        case Block::MISCELLANEOUS_SYMBOLS:
        {
            // U+2600..U+26FF | Miscellaneous Symbols | 256 | 256 | Common
            return Range{ start, 0x26ff, Plane::BASIC_MULTILINGUAL };
        }
        case Block::DINGBATS:
        {
            // U+2700..U+27BF | Dingbats | 192 | 192 | Common
            return Range{ start, 0x27bf, Plane::BASIC_MULTILINGUAL };
        }

        // Plane 1, the Supplementary Multilingual Plane (SMP)
        case Block::MISCELLANEOUS_SYMBOLS_AND_PICTOGRAPHS:
        {
            // U+1F300..U+1F5FF | Miscellaneous Symbols and Pictographs | 768 | 768 | Common
            return Range{ start, 0x1f5ff, Plane::SUPPLEMENTARY_MULTILINGUAL };
        }
        case Block::EMOTICONS:
        {
            // U+1F600..U+1F64F | Emoticons | 80 | 80 | Common
            return Range{ start, 0x1f64f, Plane::SUPPLEMENTARY_MULTILINGUAL };
        }
        case Block::ORNAMENTAL_DINGBATS:
        {
            // U+1F650..U+1F67F | Ornamental Dingbats | 48 | 48 | Common
            return Range{ start, 0x1f67f, Plane::SUPPLEMENTARY_MULTILINGUAL };
        }
        case Block::TRANSPORT_AND_MAP_SYMBOLS:
        {
            // U+1F680..U+1F6FF | Transport and Map Symbols | 128 | 110 | Common
            return Range{ start, 0x1f6ff, Plane::SUPPLEMENTARY_MULTILINGUAL };
        }

            // Plane 2, the Supplementary Ideographic Plane (SIP)
            // case ...
    }

    assert( !"unreachable" );
    return Range{ 0, 0, Plane::BASIC_MULTILINGUAL };
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
