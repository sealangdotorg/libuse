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

#ifndef _LIBSTDHL_CPP_NETWORK_LSP_PROTOCOL_H_
#define _LIBSTDHL_CPP_NETWORK_LSP_PROTOCOL_H_

#include <libstdhl/network/Protocol>

/**
   @brief    TBD

   TBD

   https://github.com/Microsoft/language-server-protocol/blob/master/protocol.md

   Version 3.0
*/

namespace libstdhl
{
    namespace Network
    {
        namespace LSP
        {
            static const std::string NL = "\r\n";
            static const std::string CL = "Content-Length";
            static const std::string CT = "Content-Type";
            static const std::string TYPE
                = "application/vscode-jsonrpc; charset=utf-8";

            class Protocol final : public Network::Protocol
            {
              public:
                Protocol( const u64 length );

                const u8* buffer( void ) const override;

                std::size_t size( void ) const override;

                u64 length( void ) const;

                std::string type( void ) const;

                std::string data( void ) const;

              private:
                u64 m_length;
                std::string m_type;
            };
        }
    }
}

#endif // _LIBSTDHL_CPP_NETWORK_LSP_PROTOCOL_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
