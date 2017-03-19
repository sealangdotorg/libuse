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

#ifndef _LIB_STDHL_LIST_H_
#define _LIB_STDHL_LIST_H_

#include "Default.h"
#include "Type.h"

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

        using const_iterator =
            typename std::vector< typename T::Ptr >::const_iterator;

        using reference = typename std::vector< typename T::Ptr >::reference;

        using const_reference =
            typename std::vector< typename T::Ptr >::const_reference;

        List( void )
        {
        }

        List( const std::vector< typename T::Ptr >& elements )
        : m_elements( elements )
        {
        }

        std::size_t size() const
        {
            return m_elements.size();
        }

        typename T::Ptr operator[]( std::size_t index ) const
        {
            return m_elements[ index ];
        }

        void add( const typename T::Ptr& node )
        {
            m_elements.push_back( node );
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
            return m_elements.cbegin();
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
            return m_elements.cend();
        }

        const_iterator cend( void ) const
        {
            return m_elements.cend();
        }

        // http://en.cppreference.com/w/cpp/container/vector/front

        reference front( void )
        {
            return m_elements.front();
        }

        const_reference front() const
        {
            return m_elements.front();
        }

        // http://en.cppreference.com/w/cpp/container/vector/back

        reference back()
        {
            return m_elements.back();
        }

        const_reference back() const
        {
            return m_elements.back();
        }

        // http://en.cppreference.com/w/cpp/container/vector/clear

        void clear( void )
        {
            m_elements.clear();
        }

      private:
        std::vector< typename T::Ptr > m_elements;
    };
}

#endif // _LIB_STDHL_LIST_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
