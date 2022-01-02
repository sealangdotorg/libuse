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
#ifndef _LIBSTDHL_CPP_LIST_H_
#define _LIBSTDHL_CPP_LIST_H_

#include <libstdhl/Type>

#include <memory>
#include <vector>

namespace libstdhl
{
    /**
       @class List
       @extends Stdhl
    */
    template < typename T >
    class List
    {
      public:
        using Ptr = std::shared_ptr< List >;

        using iterator = typename std::vector< typename T::Ptr >::iterator;

        using reverse_iterator = typename std::vector< typename T::Ptr >::reverse_iterator;

        using const_iterator = typename std::vector< typename T::Ptr >::const_iterator;

        using const_reverse_iterator =
            typename std::vector< typename T::Ptr >::const_reverse_iterator;

        using reference = typename std::vector< typename T::Ptr >::reference;

        using const_reference = typename std::vector< typename T::Ptr >::const_reference;

        List( void )
        {
        }

        List( const std::vector< typename T::Ptr >& elements )
        : m_elements( elements )
        {
        }

        std::size_t size( void ) const
        {
            return m_elements.size();
        }

        typename T::Ptr operator[]( const std::size_t index ) const
        {
            return m_elements[ index ];
        }

        void add( const typename T::Ptr& node )
        {
            m_elements.emplace_back( node );
        }

        iterator add( const_iterator position, const typename T::Ptr& node )
        {
            return m_elements.insert( position, node );
        }

        void remove( const iterator& it )
        {
            m_elements.erase( it );
        }

        // http://en.cppreference.com/w/cpp/container/vector/begin

        iterator begin( void )
        {
            return m_elements.begin();
        }

        const_iterator begin( void ) const
        {
            return m_elements.begin();
        }

        const_iterator cbegin( void ) const
        {
            return m_elements.cbegin();
        }

        // http://en.cppreference.com/w/cpp/container/vector/end

        iterator end( void )
        {
            return m_elements.end();
        }

        const_iterator end( void ) const
        {
            return m_elements.end();
        }

        const_iterator cend( void ) const
        {
            return m_elements.cend();
        }

        // http://en.cppreference.com/w/cpp/container/vector/rbegin

        reverse_iterator rbegin( void )
        {
            return m_elements.rbegin();
        }

        const_reverse_iterator rbegin( void ) const
        {
            return m_elements.rbegin();
        }

        const_reverse_iterator crbegin( void ) const
        {
            return m_elements.crbegin();
        }

        // http://en.cppreference.com/w/cpp/container/vector/rend

        reverse_iterator rend( void )
        {
            return m_elements.rend();
        }

        const_reverse_iterator rend( void ) const
        {
            return m_elements.rend();
        }

        const_reverse_iterator crend( void ) const
        {
            return m_elements.crend();
        }

        // http://en.cppreference.com/w/cpp/container/vector/front

        reference front( void )
        {
            return m_elements.front();
        }

        const_reference front( void ) const
        {
            return m_elements.front();
        }

        // http://en.cppreference.com/w/cpp/container/vector/back

        reference back( void )
        {
            return m_elements.back();
        }

        const_reference back( void ) const
        {
            return m_elements.back();
        }

        // http://en.cppreference.com/w/cpp/container/vector/at

        reference at( const std::size_t index )
        {
            return m_elements.at( index );
        }

        const_reference at( const std::size_t index ) const
        {
            return m_elements.at( index );
        }

        // http://en.cppreference.com/w/cpp/container/vector/clear

        void clear( void )
        {
            m_elements.clear();
        }

        // http://en.cppreference.com/w/cpp/container/vector/empty

        u1 empty( void ) const
        {
            return m_elements.empty();
        }

        const std::vector< typename T::Ptr >& data( void ) const
        {
            return m_elements;
        }

      private:
        std::vector< typename T::Ptr > m_elements;
    };
}

#endif  // _LIBSTDHL_CPP_LIST_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
