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

#include <libstdhl/Test>

using namespace libstdhl;
using namespace Unicode;

TEST( libstdhl_cpp_unicode, inside_block_ranges )
{
    // GIVEN
    const auto byteSequence = "\xf0\x9f\x8d\x8e";  // RED APPLE

    // WHEN
    const auto utf8 = UTF8::fromString( byteSequence );

    // THEN
    EXPECT_TRUE( inside( utf8, { Block::MISCELLANEOUS_SYMBOLS_AND_PICTOGRAPHS } ) );
    EXPECT_TRUE( inside(
        utf8,
        { Block::EMOTICONS, Block::MISCELLANEOUS_SYMBOLS_AND_PICTOGRAPHS, Block::DINGBATS } ) );

    EXPECT_FALSE( inside( utf8, { Block::MISCELLANEOUS_SYMBOLS } ) );
    EXPECT_FALSE( inside( utf8, { Block::DINGBATS } ) );
    EXPECT_FALSE( inside( utf8, { Block::EMOTICONS, Block::ORNAMENTAL_DINGBATS } ) );
}

TEST( libstdhl_cpp_unicode_range, of_blocks )
{
    // GIVEN
    const std::vector< Block > blocks = {
        // 0 BMP
        Block::MISCELLANEOUS_SYMBOLS,
        Block::DINGBATS,

        // 1 SMP
        Block::MISCELLANEOUS_SYMBOLS_AND_PICTOGRAPHS,
        Block::EMOTICONS,
        Block::ORNAMENTAL_DINGBATS,
    };

    // WHEN & THEN
    for( auto block : blocks )
    {
        switch( block )
        {
            // Plane 0, the Basic Multilingual Plane (BMP)
            case Block::MISCELLANEOUS_SYMBOLS:
            {
                // U+2600..U+26FF | Miscellaneous Symbols | 256 | 256 | Common
                const auto range = Range::of( block );
                EXPECT_EQ( range.start(), 0x2600 );
                EXPECT_EQ( range.end(), 0x26ff );
                EXPECT_EQ( range.plane(), Plane::BASIC_MULTILINGUAL );
                break;
            }
            case Block::DINGBATS:
            {
                // U+2700..U+27BF | Dingbats | 192 | 192 | Common
                const auto range = Range::of( block );
                EXPECT_EQ( range.start(), 0x2700 );
                EXPECT_EQ( range.end(), 0x27bf );
                EXPECT_EQ( range.plane(), Plane::BASIC_MULTILINGUAL );
                break;
            }

            // Plane 1, the Supplementary Multilingual Plane (SMP)
            case Block::MISCELLANEOUS_SYMBOLS_AND_PICTOGRAPHS:
            {
                // U+1F300..U+1F5FF | Miscellaneous Symbols and Pictographs | 768 | 768 | Common
                const auto range = Range::of( block );
                EXPECT_EQ( range.start(), 0x1f300 );
                EXPECT_EQ( range.end(), 0x1f5ff );
                EXPECT_EQ( range.plane(), Plane::SUPPLEMENTARY_MULTILINGUAL );
                break;
            }
            case Block::EMOTICONS:
            {
                // U+1F600..U+1F64F | Emoticons | 80 | 80 | Common
                const auto range = Range::of( block );
                EXPECT_EQ( range.start(), 0x1f600 );
                EXPECT_EQ( range.end(), 0x1f64f );
                EXPECT_EQ( range.plane(), Plane::SUPPLEMENTARY_MULTILINGUAL );
                break;
            }
            case Block::ORNAMENTAL_DINGBATS:
            {
                // U+1F650..U+1F67F | Ornamental Dingbats | 48 | 48 | Common
                const auto range = Range::of( block );
                EXPECT_EQ( range.start(), 0x1f650 );
                EXPECT_EQ( range.end(), 0x1f67f );
                EXPECT_EQ( range.plane(), Plane::SUPPLEMENTARY_MULTILINGUAL );
                break;
            }
            case Block::TRANSPORT_AND_MAP_SYMBOLS:
            {
                // U+1F680..U+1F6FF | Transport and Map Symbols | 128 | 110 | Common
                const auto range = Range::of( block );
                EXPECT_EQ( range.start(), 0x1f680 );
                EXPECT_EQ( range.end(), 0x1f6ff );
                EXPECT_EQ( range.plane(), Plane::SUPPLEMENTARY_MULTILINGUAL );
                break;
            }
                // Plane 2, the Supplementary Ideographic Plane (SIP)
                // case ...
        }
    }
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
