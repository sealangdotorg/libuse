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

#ifndef _LIB_STDHL_CPP_LOG_DATA_H_
#define _LIB_STDHL_CPP_LOG_DATA_H_

#include "Category.h"
#include "Item.h"
#include "Level.h"
#include "Source.h"
#include "Timestamp.h"

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    /**
       @extends Stdhl
    */
    namespace Log
    {
        class Data final : public Item
        {
          public:
            using Ptr = std::shared_ptr< Data >;

            Data( Level::ID level, const Source::Ptr& source,
                const Category::Ptr& category, const Items& items );

            Data( Level::ID level );

            Data( Level::ID level, const std::string& text );

            Timestamp timestamp( void ) const;

            Level level( void ) const;

            void setSource( const Source::Ptr& source );

            Source::Ptr source( void ) const;

            void setCategory( const Category::Ptr& category );

            Category::Ptr category( void ) const;

            void setItems( const Items& items );

            Items items( void ) const;

            template < typename T, typename... Args >
            void add( Args&&... args )
            {
                m_items.add( make< T >( std::forward< Args >( args )... ) );
            }

          private:
            Timestamp m_timestamp;
            Level m_level;
            Source::Ptr m_source;
            Category::Ptr m_category;
            Items m_items;

          public:
            std::string accept( Formatter& formatter ) override;
        };
    }
}

#endif // _LIB_STDHL_CPP_LOG_DATA_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
