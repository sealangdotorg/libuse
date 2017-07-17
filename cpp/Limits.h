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

#ifndef _LIB_STDHL_CPP_LIMITS_H_
#define _LIB_STDHL_CPP_LIMITS_H_

#include <limits>

#include "Integer.h"
#include "FloatingPoint.h"

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    template < typename T >
    class Limits
    {
      public:
        static T min() noexcept
        {
            return std::numeric_limits< T >::min();
        }

        static T max() noexcept
        {
            return std::numeric_limits< T >::max();
        }
    };

    template <>
    class Limits< Integer >
    {
      public:
        static Integer min() noexcept
        {
            return Integer( std::numeric_limits< i64 >::min() );
        }

        static Integer max() noexcept
        {
            return Integer( std::numeric_limits< i64 >::max() );
        }
    };

    template <>
    class Limits< FloatingPoint >
    {
      public:
        static FloatingPoint min() noexcept
        {
            return FloatingPoint( std::numeric_limits< double >::min() );
        }

        static FloatingPoint max() noexcept
        {
            return FloatingPoint( std::numeric_limits< double >::max() );
        }
    };
}

#endif // _LIB_STDHL_CPP_LIMITS_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
