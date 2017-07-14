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

#ifndef _LIB_STDHL_CPP_TYPE_FLOATING_H_
#define _LIB_STDHL_CPP_TYPE_FLOATING_H_

#include "Layout.h"

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    namespace Type
    {
        class Floating : public Layout
        {
          public:
            using Ptr = std::shared_ptr< Floating >;

            Floating( double value );

            Floating( const std::string& value );

            ~Floating( void ) = default;

            inline friend Floating operator-( Floating arg )
            {
                auto tmp = -static_cast< Layout& >( arg );
                return static_cast< Floating& >( tmp );
            }

            u1 operator==( const Floating& rhs ) const;

            inline u1 operator!=( const Floating& rhs ) const
            {
                return not( operator==( rhs ) );
            }
        };
    }
}

#endif // _LIB_STDHL_CPP_TYPE_FLOATING_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
