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

#ifndef _LIB_STDHL_CPP_NETWORK_INTERFACE_H_
#define _LIB_STDHL_CPP_NETWORK_INTERFACE_H_

#include "Protocol.h"
#include "Socket.h"

/**
   @brief    TBD

   TBD
*/

namespace libstdhl
{
    namespace Network
    {
        /**
           @extends Network
           @class Interface
        */
        template < typename T >
        class Interface
        {
          public:
            virtual void send( const std::string& data ) = 0;

            virtual void send( const std::vector< u8 >& data ) = 0;

            virtual void receive( std::string& data ) = 0;

            virtual void receive( std::vector< u8 >& data ) = 0;

            inline u1 connected( void ) const
            {
                if( m_socket )
                {
                    return m_socket->connected();
                }

                return false;
            }

            inline void connect( void )
            {
                assert( m_socket );
                m_socket->connect();
            }

            inline void disconnect( void )
            {
                assert( m_socket );
                m_socket->disconnect();
            }

            void setSocket( Socket< T >& s )
            {
                m_socket = &s;
            }

            Socket< T >& socket( void )
            {
                assert( m_socket );
                return *m_socket;
            }

          private:
            Socket< T >* m_socket = 0;
        };
    }
}

#endif // _LIB_STDHL_CPP_NETWORK_INTERFACE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
