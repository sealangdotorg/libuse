//
//  Copyright (C) 2014-2020 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
//                Christian Lascsak
//                <https://github.com/casm-lang/libstdhl>
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
//  Additional permission under GNU GPL version 3 section 7
//
//  libstdhl is distributed under the terms of the GNU General Public License
//  with the following clarification and special exception: Linking libstdhl
//  statically or dynamically with other modules is making a combined work
//  based on libstdhl. Thus, the terms and conditions of the GNU General
//  Public License cover the whole combination. As a special exception,
//  the copyright holders of libstdhl give you permission to link libstdhl
//  with independent modules to produce an executable, regardless of the
//  license terms of these independent modules, and to copy and distribute
//  the resulting executable under terms of your choice, provided that you
//  also meet, for each linked independent module, the terms and conditions
//  of the license of that module. An independent module is a module which
//  is not derived from or based on libstdhl. If you modify libstdhl, you
//  may extend this exception to your version of the library, but you are
//  not obliged to do so. If you do not wish to do so, delete this exception
//  statement from your version.
//

#pragma once
#ifndef _LIBSTDHL_CPP_NETWORK_DAP_MESSAGE_H_
#define _LIBSTDHL_CPP_NETWORK_DAP_MESSAGE_H_

#include <libstdhl/net/dap/Content>

/**
   @brief    TBD

   TBD

   https://microsoft.github.io/debug-adapter-protocol/specification#base-protocol
*/

namespace libstdhl
{
    namespace Network
    {
        namespace DAP
        {
            class ServerInterface;

            //
            //
            // Message
            //

            class Message : public Content
            {
              public:
                enum class ID : u8
                {
                    UNKNOWN = 0,
                    REQUEST_MESSAGE,
                    EVENT_MESSAGE,
                    RESPONSE_MESSAGE,
                    _SIZE_,
                };

                Message(
                    const ID id, const std::size_t seq, const Content& data = Content::object() );

                Message( void ) = default;

                virtual ~Message( void ) = default;

                ID id( void ) const;

                const Content& obj( void ) const;

                std::size_t seq( void ) const;

                std::string type( void ) const;

                virtual void process( ServerInterface& interface ) const;

                static void validate( const Content& data );

                static Message parse( const std::string& data );

              private:
                ID m_id;
            };

            //
            //
            // Request Message
            //

            class RequestMessage final : public Message
            {
              public:
                RequestMessage( const Content& data );

                RequestMessage( const std::size_t seq, const std::string& command );

                std::string command( void ) const;

                u1 hasArguments( void ) const;

                Content arguments( void ) const;

                void setArguments( const Content& data );

                virtual void process( ServerInterface& interface ) const override;

                static void validate( const Content& data );
            };

            //
            //
            // Event Message
            //

            class EventMessage final : public Message
            {
              public:
                EventMessage( const Content& data );

                EventMessage( const std::size_t seq, const std::string& event );

                std::string event( void ) const;

                u1 hasBody( void ) const;

                Content body( void ) const;

                void setBody( const Content& data );

                virtual void process( ServerInterface& interface ) const override;

                static void validate( const Content& data );
            };

            //
            //
            // Response Message
            //

            class ResponseMessage final : public Message
            {
              public:
                ResponseMessage( const Content& data );

                ResponseMessage(
                    const std::size_t seq,
                    const std::size_t request_seq,
                    const u1 success,
                    const std::string& command );

                std::size_t request_seq( void ) const;

                u1 success( void ) const;

                std::string command( void ) const;

                u1 hasMessage( void ) const;

                std::string message( void ) const;

                void setMessage( const std::string& message );

                u1 hasBody( void ) const;

                Content body( void ) const;

                void setBody( const Content& body );

                // error response, if success 'false' body contains optional 'error' property

                virtual void process( ServerInterface& interface ) const override;

                static void validate( const Content& data );
            };
        }
    }
}

#endif  // _LIBSTDHL_CPP_NETWORK_DAP_MESSAGE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
