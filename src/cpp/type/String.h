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

#ifndef _LIBSTDHL_CPP_TYPE_STRING_H_
#define _LIBSTDHL_CPP_TYPE_STRING_H_

#include <libstdhl/type/Data>

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    namespace Type
    {
        class String : public Data
        {
          public:
            using Ptr = std::shared_ptr< String >;

            using Data::Data;

            std::string toString( void ) const;

            u1 operator==( const String& rhs ) const;

            inline u1 operator!=( const String& rhs ) const
            {
                return not( operator==( rhs ) );
            }

            String& operator+=( const String& rhs );

            inline friend String operator+( String lhs, const String& rhs )
            {
                lhs += rhs;
                return lhs;
            }
        };

        class StringLayout final : public Layout
        {
          public:
            using Ptr = std::unique_ptr< StringLayout >;

            StringLayout( const std::string& value );

            Layout* clone( void ) const override;

            const std::string& str( void ) const;

            StringLayout& operator+=( const StringLayout& rhs );

            inline friend StringLayout operator+(
                StringLayout lhs, const StringLayout& rhs )
            {
                lhs += rhs;
                return lhs;
            }

          private:
            std::string m_str;
        };
    }
}

#endif // _LIBSTDHL_CPP_STRING_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
