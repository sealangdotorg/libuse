//
//  Copyright (c) 2014-2017 CASM Organization https://casm-lang.org
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
//                https://github.com/casm-lang/libstdhl
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

#ifndef _LIBSTDHL_CPP_NETWORK_SOCKET_H_
#define _LIBSTDHL_CPP_NETWORK_SOCKET_H_

#include <libstdhl/String>

#include <arpa/inet.h>
#include <linux/if.h>
#include <linux/if_packet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

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
            {
            }

            virtual ~Socket( void ) = default;

            std::string name( void ) const
            {
                return m_name;
            }

            virtual i32 id( void ) const = 0;

            virtual u1 connected( void ) const = 0;

            virtual void connect( void ) = 0;

            virtual void disconnect( void ) = 0;

            virtual void send( const T& data ) const = 0;

            virtual void receive( T& data ) const = 0;

          private:
            const std::string m_name;
        };

        /**
           @class PosixSocket
        */
        template < typename T >
        class PosixSocket : public Socket< T >
        {
          public:
            using Ptr = std::shared_ptr< PosixSocket >;

            PosixSocket(
                const std::string& name, const i32 domain, const i32 type, const i32 protocol )
            : Network::Socket< T >( name )
            , m_domain( 0 )
            , m_type( 0 )
            , m_protocol( 0 )
            , m_socket( 0 )
            , m_ifridx( 0 )
            , m_connected( false )
            , m_server( false )
            {
                m_socket = socket( domain, type, protocol );

                if( m_socket <= 0 )
                {
                    throw std::domain_error( "unable to open socket '" + this->name() + "'" );
                }
            }

            PosixSocket( const PosixSocket& server, const i32 id )
            : Network::Socket< T >( server.name() + "@" + std::to_string( id ) )
            , m_domain( 0 )
            , m_type( 0 )
            , m_protocol( 0 )
            , m_socket( id )
            , m_ifridx( 0 )
            , m_connected( false )
            , m_server( false )
            {
                if( m_socket <= 0 )
                {
                    throw std::domain_error( "unable to attach sub-socket '" + this->name() + "'" );
                }

                m_connected = true;
            }

            i32 id( void ) const override final
            {
                return m_socket;
            }

            u1 connected( void ) const override final
            {
                return m_socket > 0 and m_connected;
            }

            void setConnected( const u1 connected )
            {
                m_connected = true;
            }

            void connect( void ) override
            {
                if( not connected() )
                {
                    throw std::domain_error(
                        "undefined socket kind to connect to '" + this->name() + "'" );
                }
            }

            void disconnect( void ) override final
            {
                if( not connected() )
                {
                    return;
                }

                if( close( m_socket ) )
                {
                    throw std::domain_error( "unable to close socket '" + this->name() + "'" );
                }

                setConnected( false );
            }

            void send( const T& data ) const override
            {
                if( connected() )
                {
                    const auto result = ::send( id(), (void*)( data.buffer() ), data.size(), 0 );

                    if( result < 0 )
                    {
                        throw std::domain_error(
                            "SOCKET: unable to send, failed with '" + std::to_string( result ) );
                    }
                }
                else
                {
                    throw std::domain_error( "SOCKET: unable to send, not connected" );
                }
            }

            void receive( T& data ) const override
            {
                if( connected() )
                {
                    const auto result = ::recv( id(), (void*)( data.buffer() ), data.size(), 0 );

                    if( result < 0 )
                    {
                        throw std::domain_error(
                            "SOCKET: unable to receive, failed with '" + std::to_string( result ) );
                    }
                }
                else
                {
                    throw std::domain_error( "SOCKET: unable to receive, not connected" );
                }
            }

            void setServer( const u1 enable )
            {
                m_server = true;
            }

            u1 server( void ) const
            {
                return m_server;
            }

          private:
            i32 m_domain;
            i32 m_type;
            i32 m_protocol;
            i32 m_socket;
            i32 m_ifridx;
            u1 m_connected;
            u1 m_server;
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
