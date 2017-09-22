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

#ifndef _LIBSTDHL_CPP_NETWORK_IPV4_PROTOCOL_H_
#define _LIBSTDHL_CPP_NETWORK_IPV4_PROTOCOL_H_

#include <libstdhl/Array>
#include <libstdhl/network/Protocol>

namespace libstdhl
{
    namespace Network
    {
        namespace IPv4
        {
            static constexpr std::size_t ADDR = 4;

            static constexpr std::size_t HEADER
                = 1 + 1 + 2 + 2 + 2 + 1 + 1 + 2 + ADDR + ADDR;

            static constexpr std::size_t LENGTH = 65535;

            using Address = std::array< u8, ADDR >;

            // template < std::size_t N >
            class Protocol final : public Network::Protocol
            {
              public:
                Protocol( const Address& source, const Address& destination )
                : m_source( source )
                , m_destination( destination )
                {
                }

                inline Protocol( void )
                : Protocol( { { 0 } }, { { 0 } } )
                {
                }

                // constexpr std::array< u8, HEADER + N > data( void ) const
                // {
                //     return m_version4_ihl4 + m_dscp6_ecn2 + m_length
                //            + m_identification + m_flags3_fragmentoffset13
                //            + m_timetolive + m_protocol + m_headerchecksum
                //            + m_source + m_destination + m_options;
                // }

                const u8* buffer( void ) const override
                {
                    return 0; // m_version4_ihl4.data();
                }

                std::size_t size( void ) const override
                {
                    return 0; // m_header.size() + m_size;
                }

                const Address& source( void ) const
                {
                    return m_source;
                }

                const Address& destination( void ) const
                {
                    return m_destination;
                }

              private:
                // const std::array< u8, 1 > m_version4_ihl4;
                // const std::array< u8, 1 > m_dscp6_ecn2;
                // const std::array< u8, 2 > m_length;
                // const std::array< u8, 2 > m_identification;
                // const std::array< u8, 2 > m_flags3_fragmentoffset13;
                // const std::array< u8, 1 > m_timetolive;
                // const std::array< u8, 1 > m_protocol;
                // const std::array< u8, 2 > m_headerchecksum;

                Address m_source;
                Address m_destination;

                // std::array< u8, N > m_options;
            };
        }
    }
}

#endif // _LIBSTDHL_CPP_NETWORK_PROTOCOL_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
