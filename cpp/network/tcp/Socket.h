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

#ifndef _LIB_STDHL_CPP_NETWORK_TCP_SOCKET_H_
#define _LIB_STDHL_CPP_NETWORK_TCP_SOCKET_H_

#include "../Socket.h"

#include "Packet.h"
#include "Protocol.h"

namespace libstdhl
{
    namespace Network
    {
        namespace TCP
        {
            class IPv4PosixSocket final : public PosixSocket< IPv4Packet >
            {
              public:
                using Ptr = std::shared_ptr< IPv4PosixSocket >;

                IPv4PosixSocket( const Address& address, const Port& port );

                IPv4PosixSocket( const std::string& name );

                IPv4PosixSocket( const PosixSocket< IPv4Packet >& socket );

              public:
                void connect( void ) override;

                void send( const IPv4Packet& data ) const override;

                void receive( IPv4Packet& data ) const override;

                IPv4PosixSocket accept( void ) const;

              private:
                Address m_address;
                Port m_port;
                u1 m_server;
            };
        }
    }
}

#endif // _LIB_STDHL_CPP_NETWORK_TCP_SOCKET_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
