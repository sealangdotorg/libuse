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

#ifndef _LIB_STDHL_CPP_LOG_LEVEL_H_
#define _LIB_STDHL_CPP_LOG_LEVEL_H_

#include "Item.h"

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
        class Formatter;

        class Level : public Item
        {
          public:
            // log severity level
            // https://tools.ietf.org/html/rfc5424#section-6.2.1

            enum ID : u8
            {
                EMERGENCY = 0,
                ALERT,
                CRITICAL,
                ERROR,
                WARNING,
                NOTICE,
                INFORMATIONAL,
                DEBUG,
            };

            // static constexpr std::size_t Size = DEBUG + 1;

            Level( ID id );

            ID id( void ) const;

            inline u1 operator==( const Level& rhs ) const
            {
                if( this != &rhs )
                {
                    if( this->id() != rhs.id() )
                    {
                        return false;
                    }
                }
                return true;
            }

            inline u1 operator!=( const Level& rhs ) const
            {
                return !operator==( rhs );
            }

          private:
            ID m_id;

          public:
            std::string accept( Formatter& formatter ) override;
        };

        using Levels = std::vector< Level >;
    }
}

#endif // _LIB_STDHL_CPP_LOG_LEVEL_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
