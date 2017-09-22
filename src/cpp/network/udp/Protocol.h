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

#ifndef _LIBSTDHL_CPP_NETWORK_UDP_PROTOCOL_H_
#define _LIBSTDHL_CPP_NETWORK_UDP_PROTOCOL_H_

#include <libstdhl/network/Protocol>
#include <libstdhl/network/ipv4/Protocol>

/**
   @brief    TBD

   TBD
*/

namespace libstdhl
{
    namespace Network
    {
        namespace UDP
        {
            using Address = Network::IPv4::Address;

            static constexpr const std::size_t ADDR = 2;

            using Port = std::array< u8, ADDR >;

            class Protocol final : public Network::Protocol
            {
              public:
                Protocol( const Port& source, const Port& destination )
                : m_source( source )
                , m_destination( destination )
                {
                }

                inline Protocol( void )
                : Protocol( { { 0 } }, { { 0 } } )
                {
                }

                const u8* buffer( void ) const override
                {
                    return 0; // m_source.data();
                }

                std::size_t size( void ) const override
                {
                    return 0; // m_header.size() + m_size;
                }

                const Port& source( void ) const
                {
                    return m_source;
                }

                const Port& destination( void ) const
                {
                    return m_destination;
                }

              private:
                Port m_source;
                Port m_destination;
                // TODO length field
                // TODO checksum field
            };
        }
    }
}

#endif // _LIBSTDHL_CPP_NETWORK_UDP_PROTOCOL_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
