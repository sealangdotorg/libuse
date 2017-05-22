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

#ifndef _LIB_STDHL_CPP_NETWORK_ETHERNET_PROTOCOL_H_
#define _LIB_STDHL_CPP_NETWORK_ETHERNET_PROTOCOL_H_

#include "../Protocol.h"

#include "../../standard/ieee802.h"

/**
   @brief    TBD

   TBD
*/

namespace libstdhl
{
    namespace Network
    {
        namespace Ethernet
        {
            using libstdhl::Standard::IEEE802::ADDR;
            using libstdhl::Standard::IEEE802::TYPE;

            static constexpr std::size_t LENGTH = 1522;
            static constexpr std::size_t HEADER = ADDR + ADDR + TYPE;

            using Address = libstdhl::Standard::IEEE802::Address;
            using Type = libstdhl::Standard::IEEE802::Type;
            using Data = std::array< u8, LENGTH - HEADER >;

            static constexpr Address BROADCAST = { {
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
            } };

            static constexpr Type RUNT = { {
                0, 64,
            } };

            class Protocol final : public Network::Protocol
            {
              public:
                Protocol( const Address& destination,
                    const Address& source,
                    const Type& type );

                constexpr std::array< u8, HEADER > data( void ) const;

                const Address& destination( void ) const;

                const Address& source( void ) const;

                const Type& type( void ) const;

                const u8* buffer( void ) const override;

                std::size_t size( void ) const override;

              private:
                const Address m_destination;
                const Address m_source;
                const Type m_type;
            };
        }
    }
}

#endif // _LIB_STDHL_CPP_NETWORK_ETHERNET_PROTOCOL_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
