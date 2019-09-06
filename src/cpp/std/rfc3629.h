//
//  Copyright (C) 2014-2019 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
//                Christian Lascsak
//                <https://github.com/casm-lang/libstdhl>
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
#ifndef _LIBSTDHL_CPP_STANDARD_RFC3629_H_
#define _LIBSTDHL_CPP_STANDARD_RFC3629_H_

#include <libstdhl/Type>

#include <memory>
#include <vector>

/**
   @brief    UTF-8, a transformation format of ISO 10646

   TBD

   https://tools.ietf.org/html/rfc3629
*/

namespace libstdhl
{
    namespace Standard
    {
        /**
           @extends Standard
        */
        namespace RFC3629
        {
            /**
               @extends RFC3629
            */
            class UTF8
            {
              public:
                using Ptr = std::shared_ptr< UTF8 >;

                UTF8( const u32 code, const u32 point );

                const u32 code( void ) const;

                const u32 point( void ) const;

                std::string unicode( void ) const;

                std::string description( void ) const;

                std::string toString( void ) const;

              private:
                u32 m_code;
                u32 m_point;

              public:
                static std::size_t byteSequenceLengthIndication( const u8 byte );

                static UTF8 fromString( const std::string& byteSequence );
            };
        }
    }
}

#endif  // _LIBSTDHL_CPP_STANDARD_RFC3629_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
