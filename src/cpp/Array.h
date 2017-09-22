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

#ifndef _LIBSTDHL_CPP_ARRAY_H_
#define _LIBSTDHL_CPP_ARRAY_H_

#include <libstdhl/Type>

#include <array>

/**
   @brief    TBD

   TBD
*/

namespace libstdhl
{
    /**
       @extends Stdhl
    */
    namespace Array
    {

        template < std::size_t... Is >
        struct seq
        {
        };
        template < std::size_t N, std::size_t... Is >
        struct gen_seq : gen_seq< N - 1, N - 1, Is... >
        {
        };
        template < std::size_t... Is >
        struct gen_seq< 0, Is... > : seq< Is... >
        {
        };

        template < std::size_t N1, std::size_t... I1, std::size_t N2,
            std::size_t... I2 >
        constexpr std::array< u8, N1 + N2 > concat(
            const std::array< u8, N1 >& a1, const std::array< u8, N2 >& a2,
            seq< I1... >, seq< I2... > )
        {
            return { { a1[ I1 ]..., a2[ I2 ]... } };
        }

        template < std::size_t N1, std::size_t N2 >
        constexpr std::array< u8, N1 + N2 > concat(
            const std::array< u8, N1 >& a1, const std::array< u8, N2 >& a2 )
        {
            return concat( a1, a2, gen_seq< N1 >{}, gen_seq< N2 >{} );
        }
    };

    template < std::size_t N1, std::size_t N2 >
    constexpr std::array< u8, N1 + N2 > operator+(
        const std::array< u8, N1 >& a1, const std::array< u8, N2 >& a2 )
    {
        return Array::concat(
            a1, a2, Array::gen_seq< N1 >{}, Array::gen_seq< N2 >{} );
    }
}

#endif // _LIBSTDHL_CPP_ARRAY_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
