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

#ifndef _LIBSTDHL_CPP_NETWORK_UDP_IPV4_H_
#define _LIBSTDHL_CPP_NETWORK_UDP_IPV4_H_

#include <libstdhl/network/Interface>
#include <libstdhl/network/udp/Packet>

namespace libstdhl
{
    namespace Network
    {
        namespace UDP
        {
            class IPv4 final : public Interface< IPv4Packet >
            {
              public:
                using Ptr = std::shared_ptr< IPv4 >;

                IPv4( const std::string& name );

                void send( const IPv4Packet& data ) override;

                void send( const std::string& data ) override;

                void send( const std::string& data, const Address& address,
                    const Port& port );

                void send( const Network::Packet& data, const Address& address,
                    const Port& port );

                void send( const Network::Packet& data,
                    const IPv4Packet& destination );

                void receive( IPv4Packet& data ) override;

                IPv4Packet receive( std::string& data ) override;
            };
        }
    };
}

#endif // _LIBSTDHL_CPP_NETWORK_UDP_IPV4_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
