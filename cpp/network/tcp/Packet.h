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

#ifndef _LIB_STDHL_CPP_NETWORK_TCP_PACKET_H_
#define _LIB_STDHL_CPP_NETWORK_TCP_PACKET_H_

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
        namespace TCP
        {
            /**
               @extends TCP
            */
            class IPv4Packet final : public Network::Packet
            {
              public:
                IPv4Packet( const IPv4::Protocol& ip, const TCP::Protocol& tcp,
                    const std::string& data )
                : m_ip( ip )
                , m_tcp( tcp )
                , m_data( data )
                {
                }

                IPv4Packet( std::string& data )
                : m_ip()
                , m_tcp()
                , m_data( data )
                {
                }

                const u8* buffer( void ) const override
                {
                    return m_data.buffer();
                }

                u64 size( void ) const override
                {
                    return m_data.size();
                }

                const IPv4::Protocol& ip( void ) const
                {
                    return m_ip;
                }

                void setIp( const IPv4::Protocol& ip )
                {
                    m_ip = ip;
                }

                const TCP::Protocol& tcp( void ) const
                {
                    return m_tcp;
                }

                void setTcp( const TCP::Protocol& tcp )
                {
                    m_tcp = tcp;
                }

              private:
                IPv4::Protocol m_ip;
                TCP::Protocol m_tcp;
                StringData m_data;
            };
        }
    }
}

#endif // _LIB_STDHL_CPP_NETWORK_TCP_PACKET_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
