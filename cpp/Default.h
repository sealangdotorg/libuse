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

#ifndef _LIB_STDHL_CPP_DEFAULT_H_
#define _LIB_STDHL_CPP_DEFAULT_H_

#include "c/default.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <utility>

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    class Stdhl
    {
    };

    //
    // shared object creation utility
    //

    template < typename T, typename... Args >
    typename T::Ptr make( Args&&... args )
    {
        return std::make_shared< T >( std::forward< Args >( args )... );
    }

    //
    // shared object creation utility which allocates only new objects
    // if they are not already cached in the 'make_cache()' facility
    // through 'make_hash()'
    //
    template < typename T, typename... Args >
    static inline typename T::Ptr get( Args&&... args )
    {
        T tmp = T( std::forward< Args >( args )... );

        auto cache = tmp.make_cache().find( tmp.make_hash() );
        if( cache != tmp.make_cache().end() )
        {
            return std::static_pointer_cast< T >( cache->second );
        }

        auto ptr = make< T >( tmp );

        return std::static_pointer_cast< T >(
            tmp.make_cache().emplace( tmp.make_hash(), ptr ).first->second );
    }
}

#endif /* _LIB_STDHL_CPP_DEFAULT_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
