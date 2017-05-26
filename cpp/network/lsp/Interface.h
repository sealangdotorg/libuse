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

#ifndef _LIB_STDHL_CPP_NETWORK_LSP_INTERFACE_H_
#define _LIB_STDHL_CPP_NETWORK_LSP_INTERFACE_H_

/**
   @brief    TBD

   TBD

   https://github.com/Microsoft/language-server-protocol/blob/master/protocol.md
*/

#include "Message.h"

namespace libstdhl
{
    namespace Network
    {
        namespace LSP
        {
            class Packet;

            class ServerInterface
            {
              public:
                ServerInterface( void );

                virtual ~ServerInterface( void ) = default;

                virtual InitializeResult initialize(
                    const InitializeParams& params )
                    = 0;

                virtual void initialized( void ) noexcept = 0;

                virtual void shutdown( void ) = 0;

                virtual void exit( void ) noexcept = 0;

                void response( const Message& message );

                void flush(
                    const std::function< void( const Message& ) >& callback );

              private:
                std::vector< Message > m_responseBuffer[ 2 ];
                std::size_t m_responseBufferSlot;
                std::mutex m_responseBufferLockAccess;
                std::mutex m_responseBufferLockFlush;
            };
        }
    }
}

#endif // _LIB_STDHL_CPP_NETWORK_LSP_INTERFACE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
