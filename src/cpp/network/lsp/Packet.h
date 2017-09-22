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

#ifndef _LIBSTDHL_CPP_NETWORK_LSP_PACKET_H_
#define _LIBSTDHL_CPP_NETWORK_LSP_PACKET_H_

#include <libstdhl/network/Packet>
#include <libstdhl/network/lsp/Message>
#include <libstdhl/network/lsp/Protocol>

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
        namespace LSP
        {
            /**
               @extends LSP
            */
            class Packet final : public Network::Packet
            {
              public:
                using Ptr = std::shared_ptr< Packet >;

                // create LSP packet from string
                Packet( const std::string& data );

                // create LSP packet to string
                Packet( const Message& payload );

                const Protocol& header( void ) const;

                const Message& payload( void ) const;

                const u8* buffer( void ) const override;

                std::size_t size( void ) const override;

                std::string dump( const u1 formatted = false ) const;

                void process( ServerInterface& interface ) const;

              private:
                Protocol m_header;
                Message m_payload;
                StringData m_data;
            };
        }
    }
}

#endif // _LIBSTDHL_CPP_NETWORK_LSP_PACKET_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
