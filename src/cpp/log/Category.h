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

#ifndef _LIBSTDHL_CPP_LOG_CATEGORY_H_
#define _LIBSTDHL_CPP_LOG_CATEGORY_H_

#include <libstdhl/log/Item>

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
        class Category final : public Item
        {
          public:
            using Ptr = std::shared_ptr< Category >;

            Category( const std::string& name, const std::string& description );

            std::string name( void ) const;

            std::string description( void ) const;

            inline u1 operator==( const Category& rhs ) const
            {
                if( this != &rhs )
                {
                    if( ( this->name().compare( rhs.name() ) != 0 )
                        or ( this->description().compare( rhs.description() )
                               != 0 ) )
                    {
                        return false;
                    }
                }
                return true;
            }

            inline u1 operator!=( const Category& rhs ) const
            {
                return !operator==( rhs );
            }

          private:
            std::string m_name;

            std::string m_description;

          public:
            std::string accept( Formatter& formatter ) override;

            static Category::Ptr defaultCategory( void )
            {
                static auto cache = std::make_shared< Category >(
                    "Default", "Default log category of libstdhl" );

                return cache;
            }
        };

        using Categories = libstdhl::List< Category >;
    }
}

#endif // _LIBSTDHL_CPP_LOG_CATEGORY_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
