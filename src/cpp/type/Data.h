//
//  Copyright (C) 2014-2018 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
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
#ifndef _LIBSTDHL_CPP_TYPE_DATA_H_
#define _LIBSTDHL_CPP_TYPE_DATA_H_

#include <libstdhl/Hash>
#include <libstdhl/type/Layout>

#include <memory>

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    /**
       @extends Stdhl
    */
    namespace Type
    {
        class Data
        {
          public:
            using Ptr = std::shared_ptr< Data >;

            /**
               trivial defined data
             */
            explicit Data( const u64 value, const u1 sign );

            /**
               non-trivial defined data
             */
            explicit Data( Layout* ptr );

            /**
               undefined data
             */
            Data( void );

            ~Data( void );

            Data( const Data& other );

            Data( Data&& other ) noexcept;

            Data& operator=( const Data& other );

            Data& operator=( Data&& other ) noexcept;

            u64 value( void ) const;

            Layout* ptr( void ) const;

            u1 sign( void ) const;

            u1 trivial( void ) const;

            u1 defined( void ) const;

            std::size_t hash( void ) const;

          protected:
            union content
            {
                u64 value;
                Layout* ptr;
            } m_data;

            u1 m_sign : 1;
            u1 m_trivial : 1;

          public:
            friend Data operator-( Data arg )
            {
                arg.m_sign = not arg.m_sign;
                return arg;
            }

            u1 operator==( const Data& rhs ) const;

            inline u1 operator!=( const Data& rhs ) const
            {
                return not( operator==( rhs ) );
            }

            template < const Radix RADIX, const Literal LITERAL = STDHL >
            inline std::string to( void ) const
            {
                return to_string( RADIX, LITERAL );
            }

            std::string to_string(
                const Radix radix = DECIMAL, const Literal literal = NONE ) const;

            static u64 to_digit(
                const char character, const Radix radix = DECIMAL, const Literal literal = NONE );
        };
    }

    namespace Hash
    {
        inline std::size_t value( const Type::Data& data )
        {
            return data.hash();
        }
    }
}

#endif  // _LIBSTDHL_CPP_TYPE_LAYOUT_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
