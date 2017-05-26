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

#ifndef _LIB_STDHL_CPP_NETWORK_TCP_IPV4_H_
#define _LIB_STDHL_CPP_NETWORK_TCP_IPV4_H_

#include "../Interface.h"

#include "Session.h"
// #include "Socket.h"

namespace libstdhl
{
    namespace Network
    {
        namespace TCP
        {
            class IPv4 final : public Interface< Network::Packet >,
                               public Session< IPv4 >
            {
              public:
                using Ptr = std::shared_ptr< IPv4 >;

                IPv4( const std::string& name, const u1 server = false );

              private:
                IPv4( void );

              public:
                void send( const Network::Packet& data ) override;

                void receive( Network::Packet& data ) override;

                IPv4 session( void ) override;

                void send( const std::string& data );

                void receive( std::string& data );
            };
        }
    };
}

#endif // _LIB_STDHL_CPP_NETWORK_TCP_IPV4_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
