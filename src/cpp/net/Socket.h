//
//  Copyright (C) 2014-2024 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber et al.
//  <https://github.com/casm-lang/libstdhl/graphs/contributors>
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
#ifndef _LIBSTDHL_CPP_NETWORK_SOCKET_H_
#define _LIBSTDHL_CPP_NETWORK_SOCKET_H_

#include <libstdhl/Type>

#include <memory>
#include <string>

/**
   @brief    TBD

   TBD
*/

namespace libstdhl
{
    /**
       @extends Stdhl
    */
    namespace Network
    {
        /**
           @class Socket
        */
        template < typename T >
        class Socket
        {
          public:
            using Ptr = std::shared_ptr< Socket >;

            Socket( const std::string& name )
            : m_name( name )
            , m_id( 0 )
            {
            }

            virtual ~Socket( void ) = default;

            std::string name( void ) const
            {
                return m_name;
            }

            i32 id( void ) const
            {
                return m_id;
            }

            u1 connected( void ) const
            {
                return id() != 0;
            }

            virtual void connect( void ) = 0;

            virtual void disconnect( void ) = 0;

            virtual std::size_t send( const T& data ) const = 0;

            virtual std::size_t receive( T& data ) const = 0;

          protected:
            void setId( const i32 id )
            {
                m_id = id;
            }

          private:
            const std::string m_name;
            i32 m_id;
        };
    }
}

#endif  // _LIBSTDHL_CPP_NETWORK_SOCKET_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
