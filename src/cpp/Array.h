//
//  Copyright (C) 2014-2022 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber et al.
//                <https://github.com/casm-lang/libstdhl/graphs/contributors>
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

        template < std::size_t N1, std::size_t... I1, std::size_t N2, std::size_t... I2 >
        constexpr std::array< u8, N1 + N2 > concat(
            const std::array< u8, N1 >& a1,
            const std::array< u8, N2 >& a2,
            seq< I1... >,
            seq< I2... > )
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
        return Array::concat( a1, a2, Array::gen_seq< N1 >{}, Array::gen_seq< N2 >{} );
    }
}

#endif  // _LIBSTDHL_CPP_ARRAY_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
