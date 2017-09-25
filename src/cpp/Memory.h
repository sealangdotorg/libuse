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

#ifndef _LIBSTDHL_CPP_MEMORY_H_
#define _LIBSTDHL_CPP_MEMORY_H_

#include <memory>

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    /**
       @extends Stdhl
    */
    namespace Memory
    {
        //
        // unique object creation utility
        //

        template < typename T, typename... Args >
        std::unique_ptr< T > make_unique( Args&&... args )
        {
            return std::unique_ptr< T >(
                new T( std::forward< Args >( args )... ) ); // TODO: PPA: change
                                                            // this when C++14
            // make_unique is ready
        }

        //
        // shared object creation utility
        //

        template < typename T, typename... Args >
        std::shared_ptr< T > make( Args&&... args )
        {
            return std::make_shared< T >( std::forward< Args >( args )... );
        }

        //
        // shared object creation utility which allocates only new objects
        // if they are not already cached in the 'make_cache()' facility
        // through 'make_hash()'
        //
        template < typename T, typename... Args >
        inline std::shared_ptr< T > get( Args&&... args )
        {
            T tmp = T( std::forward< Args >( args )... );

            auto cache = tmp.make_cache().find( tmp.make_hash() );
            if( cache != tmp.make_cache().end() )
            {
                return std::static_pointer_cast< T >( cache->second );
            }

            auto ptr = make< T >( tmp );
            return std::static_pointer_cast< T >(
                tmp.make_cache()
                    .emplace( tmp.make_hash(), ptr )
                    .first->second );
        }

        //
        // shared object creation of stack object
        //

        template < typename T >
        std::shared_ptr< T > wrap( T& stack_object )
        {
            static auto no_dtor = []( T* ) {};
            return std::shared_ptr< T >( &stack_object, no_dtor );
        }
    }
}

#endif // _LIBSTDHL_CPP_MEMORY_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//