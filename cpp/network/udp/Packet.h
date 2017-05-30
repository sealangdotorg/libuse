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

#ifndef _LIB_STDHL_CPP_NETWORK_UDP_PACKET_H_
#define _LIB_STDHL_CPP_NETWORK_UDP_PACKET_H_

#include "../Packet.h"

#include "Protocol.h"

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
        namespace UDP
        {
            /**
               @extends UDP
            */
            class IPv4Packet final : public Network::Packet
            {
              public:
                IPv4Packet( const IPv4::Protocol& ip, const UDP::Protocol& udp,
                    const std::string& data )
                : m_ip( ip )
                , m_udp( udp )
                , m_data( data )
                , m_ref( m_data )
                {
                }

                IPv4Packet( std::string& data )
                : m_ip()
                , m_udp()
                , m_data()
                , m_ref( data )
                {
                }

                const u8* buffer( void ) const override
                {
                    return reinterpret_cast< const u8* >( m_ref.data() );
                }

                u64 size( void ) const override
                {
                    return m_ref.size();
                }

                const IPv4::Protocol& ip( void ) const
                {
                    return m_ip;
                }

                void setIp( const IPv4::Protocol& ip )
                {
                    m_ip = ip;
                }

                const UDP::Protocol& udp( void ) const
                {
                    return m_udp;
                }

                void setUdp( const UDP::Protocol& udp )
                {
                    m_udp = udp;
                }

              private:
                IPv4::Protocol m_ip;
                UDP::Protocol m_udp;
                std::string m_data;
                std::string& m_ref;

                // StringData data;

                // const u8* buffer( void ) const override
                // {
                //     return data.buffer();
                // }

                // std::size_t size( void ) const override
                // {
                //     return data.size();
                // }
            };

            // class Packet final : public Network::Packet
            // {
            //   public:
            //     using Ptr = std::shared_ptr< Packet >;

            //     Packet( const Port& source, const Port& destination, const
            //     std::vector< u8 >& payload );

            //     const Protocol& header( void ) const;

            //     const Data& payload( void ) const;

            //     const u8* buffer( void ) const override;

            //     std::size_t size( void ) const override;

            //   private:
            //     const Protocol m_header;
            //     std::vector< u8 >
            //     Data m_payload;
            //     const std::size_t m_size;
            // };
        }
    }
}

#endif // _LIB_STDHL_CPP_NETWORK_UDP_PACKET_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
