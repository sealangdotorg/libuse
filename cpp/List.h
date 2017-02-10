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
    template < typename T >
    class List : public Stdhl
    {
      public:
        using Ptr = std::shared_ptr< List >;

        using iterator = typename std::vector< typename T::Ptr >::iterator;

        using const_iterator =
            typename std::vector< typename T::Ptr >::const_iterator;

        List()
        {
        }

        std::size_t size() const
        {
            return m_elements.size();
        }

        void add( const typename T::Ptr& node )
        {
            m_elements.push_back( node );
        }

        void remove( const iterator& it )
        {
            m_elements.erase( it );
        }

        iterator begin()
        {
            return m_elements.begin();
        }

        iterator end()
        {
            return m_elements.end();
        }

        const_iterator cbegin() const
        {
            return m_elements.cbegin();
        }

        const_iterator cend() const
        {
            return m_elements.cend();
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
