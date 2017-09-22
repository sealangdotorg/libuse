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

#ifndef _LIBSTDHL_CPP_VARIADIC_H_
#define _LIBSTDHL_CPP_VARIADIC_H_

#include <libstdhl/Type>

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    template < typename T, typename... Args >
    struct Variadic
    {
        using next = Variadic< Args... >;
        static const constexpr std::size_t size = 1 + next::size;

        // template < typename C >
        // inline constexpr static C iterate( C cbk, T&& arg, Args&&... args )
        // {
        //     cbk( size - 1, std::forward< T >( arg ) );
        //     next::iterate( cbk, std::forward< Args >( args )... );
        //     return cbk;
        // }

        // template < typename C >
        // inline constexpr C operator()( C cbk, T&& arg, Args&&... args ) const
        // {
        //     iterate( cbk,
        //         std::forward< T >( arg ),
        //         std::forward< Args >( args )... );
        //     return cbk;
        // }
    };

    template < typename T >
    struct Variadic< T >
    {
        static const constexpr std::size_t size = 1;

        // template < typename C >
        // inline constexpr static C iterate( C cbk, T&& arg )
        // {
        //     return cbk( 0, std::forward< T >( arg ) );
        // }

        // template < typename C >
        // inline constexpr C operator()( C cbk, T&& arg ) const
        // {
        //     return iterate( cbk, std::forward< T >( arg ) );
        // }
    };
}

#endif // _LIBSTDHL_CPP_VARIADIC_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
