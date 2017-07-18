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

#ifndef _LIB_STDHL_CPP_TYPE_NATURAL_H_
#define _LIB_STDHL_CPP_TYPE_NATURAL_H_

#include "Integer.h"

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    namespace Type
    {
        class Natural : public Integer
        {
          public:
            using Ptr = std::shared_ptr< Natural >;

            using Integer::Integer;

            static Natural fromString(
                const std::string& value, const Radix radix );

            inline friend Natural operator~( Natural arg )
            {
                auto tmp = ~static_cast< Integer& >( arg );
                return static_cast< Natural& >( tmp );
            }
        };
    }
}

#endif // _LIB_STDHL_CPP_TYPE_NATURAL_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
