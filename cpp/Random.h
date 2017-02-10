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

#ifndef _LIB_STDHL_CPP_RANDOM_H_
#define _LIB_STDHL_CPP_RANDOM_H_

#include "Default.h"
#include "Math.h"
#include "Type.h"

#include <random>

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    class Random : public Stdhl
    {
      public:
        template < typename T >
        static inline T uniform( T from = std::numeric_limits< T >::min(),
            T to = std::numeric_limits< T >::max() )
        {
            std::random_device device;
            std::default_random_engine engine( device() );

            // std::seed_seq seed{ device(), device(), device(), device(),
            //     device(), device(), device(), device() };
            // std::mt19937_64 engine( seed );

            std::uniform_int_distribution< T > distribution( from, to );
            return distribution( engine );
        }
    };
}

#endif // _LIB_STDHL_CPP_RANDOM_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
