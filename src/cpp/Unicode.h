//
//  Copyright (C) 2014-2024 CASM Organization <https://casm-lang.org>
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

#pragma once
#ifndef _LIBSTDHL_CPP_UNICODE_H_
#define _LIBSTDHL_CPP_UNICODE_H_

#include <libstdhl/Type>
#include <libstdhl/std/rfc3629>

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    /**
       @extends Stdhl
    */
    namespace Unicode
    {
        using UTF8 = Standard::RFC3629::UTF8;

        enum class Plane
        {
            BASIC_MULTILINGUAL = 0,
            SUPPLEMENTARY_MULTILINGUAL = 1,
            SUPPLEMENTARY_IDEOGRAPHIC = 2,
            // 3-13 unassigned planes
        };

        /**
           https://en.wikipedia.org/wiki/Unicode_block
         */
        enum class Block
        {
            // 0 BMP
            // U+2600..U+26FF | Miscellaneous Symbols | 256 | 256 | Common
            MISCELLANEOUS_SYMBOLS = 0x2600,
            // U+2700..U+27BF | Dingbats | 192 | 192 | Common
            DINGBATS = 0x2700,

            // 1 SMP
            // U+1F300..U+1F5FF | Miscellaneous Symbols and Pictographs | 768 | 768 | Common
            MISCELLANEOUS_SYMBOLS_AND_PICTOGRAPHS = 0x1f300,
            // U+1F600..U+1F64F | Emoticons | 80 | 80 | Common
            EMOTICONS = 0x1f600,
            // U+1F650..U+1F67F | Ornamental Dingbats | 48 | 48 | Common
            ORNAMENTAL_DINGBATS = 0x1f650,
            // U+1F680..U+1F6FF | Transport and Map Symbols | 128 | 110 | Common
            TRANSPORT_AND_MAP_SYMBOLS = 0x1f680,
        };

        u1 inside( const UTF8& character, const std::vector< Unicode::Block >& blocks );

        class Range final
        {
          public:
            Range( const u32 start, const u32 end, const Plane plane );

            const u32 start( void ) const;

            const u32 end( void ) const;

            const Plane plane( void ) const;

          private:
            const u32 m_start;
            const u32 m_end;
            const Plane m_plane;

          public:
            static Range of( const Block block );
        };

    };
}

#endif  // _LIBSTDHL_CPP_UNICODE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
