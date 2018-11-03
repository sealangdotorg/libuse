//
//  Copyright (C) 2014-2018 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
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
#ifndef _LIBSTDHL_CPP_NETWORK_LSP_MESSAGE_H_
#define _LIBSTDHL_CPP_NETWORK_LSP_MESSAGE_H_

#include <libstdhl/net/lsp/Content>

/**
   @brief    TBD

   TBD

   https://github.com/Microsoft/language-server-protocol/blob/master/protocol.md
*/

namespace libstdhl
{
    namespace Network
    {
        namespace LSP
        {
            class ServerInterface;

            /**
               Message

             */

            class Message : public Data
            {
              public:
                enum class ID : u8
                {
                    UNKNOWN = 0,
                    REQUEST_MESSAGE,
                    NOTIFICATION_MESSAGE,
                    RESPONSE_MESSAGE,
                    _SIZE_,
                };

                Message( const ID id, const Data& data = Data::object() );

                Message( void ) = default;

                virtual ~Message( void ) = default;

                ID id( void ) const;

                const Data& obj( void ) const;

                virtual void process( ServerInterface& interface ) const;

                static u1 isValid( const Data& data );

                static Message parse( const std::string& data );

              protected:
                ID m_id;
            };

            /**
               Request Message

               > A request message to describe a request between the client and
               > the server. Every processed request must send a response back
               > to the sender of the request.
             */

            class RequestMessage final : public Message
            {
              public:
                RequestMessage( const Data& data );

                RequestMessage( const std::size_t id, const std::string& method );

                RequestMessage( const std::string& id, const std::string& method );

                std::string id( void ) const;

                std::string method( void ) const;

                u1 hasParams( void ) const;

                Data params( void ) const;

                void setParams( const Data& data );

                virtual void process( ServerInterface& interface ) const override;

                static u1 isValid( const Data& data );
            };

            /**
               Notification Message

               > A notification message. A processed notification message must
               > not send a response back. They work like events.

               > $ Notifications and Requests
               > Notification and requests ids starting with '$/' are messages
               > which are protocol implementation dependent and might not be
               > implementable in all clients or servers. For example if the
               > server implementation uses a single threaded synchronous
               > programming language then there is little a server can do to
               > react to a '$/cancelRequest'. If a server or client receives
               > notifications or requests starting with '$/' it is free to
               > ignore them if they are unknown.

               > Cancellation Support
               > The base protocol offers support for request cancellation. To
               > cancel a request, a notification message with the following
               > properties is sent:
               > Notification:
               > - method: '$/cancelRequest'
               > - params: CancelParams defined as follows:
               >   interface CancelParams { id: number | string; }
            */

            class NotificationMessage final : public Message
            {
              public:
                NotificationMessage( const Data& data );

                NotificationMessage( const std::string& method );

                std::string method( void ) const;

                u1 hasParams( void ) const;

                Data params( void ) const;

                void setParams( const Data& data );

                virtual void process( ServerInterface& interface ) const override;

                static u1 isValid( const Data& data );
            };

            /**
               Response Message

               > Response Message sent as a result of a request. If a request
               > doesn't provide a result value the receiver of a request still
               > needs to return a response message to conform to the JSON RPC
               > specification. The result property of the ResponseMessage
               > should be set to null in this case to signal a successful
               > request.
             */

            class ResponseMessage final : public Message
            {
              public:
                ResponseMessage( const Data& data );

                ResponseMessage( const std::size_t id );

                ResponseMessage( const std::string id );

                ResponseMessage( void );

                u1 hasResult( void ) const;

                Data result( void ) const;

                void setResult( const Data& result );

                u1 hasError( void ) const;

                ResponseError error( void ) const;

                void setError( const ResponseError& error );

                void setError( const ErrorCode code, const std::string& name );

                void setError( const ErrorCode code, const std::string& name, const Data& data );

                virtual void process( ServerInterface& interface ) const override;

                static u1 isValid( const Data& data );
            };
        }
    }
}

#endif  // _LIBSTDHL_CPP_NETWORK_LSP_MESSAGE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
