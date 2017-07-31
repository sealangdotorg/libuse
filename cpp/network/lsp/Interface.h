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

                /**
                   user interface for message interactions
                */

                void respond( const ResponseMessage& message );

                void notify( const NotificationMessage& message );

                void flush(
                    const std::function< void( const Message& ) >& callback );

                /**
                   general
                */

                virtual InitializeResult initialize(
                    const InitializeParams& params )
                    = 0;

                virtual void initialized( void ) noexcept = 0;

                virtual void shutdown( void ) = 0;

                virtual void exit( void ) noexcept = 0;

                // virtual void dollar_cancelRequest( void ) noexcept = 0;

                /**
                   window
                */

                // :arrow_left: window/showMessage
                // :arrow_right_hook: window/showMessageRequest
                // :arrow_left: window/logMessage
                // :arrow_left: telemetry/event

                /**
                   new client
                */

                // :arrow_right_hook: client/registerCapability
                // :arrow_right_hook: client/unregisterCapability

                /**
                   workspace
                */

                // :arrow_right: workspace/didChangeConfiguration
                // :arrow_right: workspace/didChangeWatchedFiles
                // :leftwards_arrow_with_hook: workspace/symbol

                // New :leftwards_arrow_with_hook: workspace/executeCommand
                virtual ExecuteCommandResult workspace_executeCommand(
                    const ExecuteCommandParams& params )
                    = 0;

                // New :arrow_right_hook: workspace/applyEdit

                /**
                    document
                 */

                // :arrow_left: textDocument/publishDiagnostics
                virtual void textDocument_publishDiagnostics(
                    const PublishDiagnosticsParams& params ) noexcept final;

                // :arrow_right: textDocument/didOpen
                virtual void textDocument_didOpen(
                    const DidOpenTextDocumentParams& params ) noexcept
                    = 0;

                // :arrow_right: textDocument/didChange
                virtual void textDocument_didChange(
                    const DidChangeTextDocumentParams& params ) noexcept
                    = 0;

                // :arrow_right: textDocument/willSave
                // New :leftwards_arrow_with_hook:
                // textDocument/willSaveWaitUntil
                // New :arrow_right: textDocument/didSave
                // :arrow_right: textDocument/didClose
                // :leftwards_arrow_with_hook: textDocument/completion
                // :leftwards_arrow_with_hook: completionItem/resolve

                // :leftwards_arrow_with_hook: textDocument/hover
                virtual HoverResult textDocument_hover(
                    const HoverParams& params )
                    = 0;

                // :leftwards_arrow_with_hook: textDocument/signatureHelp
                // :leftwards_arrow_with_hook: textDocument/references
                // :leftwards_arrow_with_hook:
                // textDocument/documentHighlight
                // :leftwards_arrow_with_hook: textDocument/documentSymbol
                // :leftwards_arrow_with_hook: textDocument/formatting
                // :leftwards_arrow_with_hook: textDocument/rangeFormatting
                // :leftwards_arrow_with_hook: textDocument/onTypeFormatting
                // :leftwards_arrow_with_hook: textDocument/definition

                // :leftwards_arrow_with_hook: textDocument/codeAction
                virtual CodeActionResult textDocument_codeAction(
                    const CodeActionParams& params )
                    = 0;

                // :leftwards_arrow_with_hook: textDocument/codeLens
                virtual CodeLensResult textDocument_codeLens(
                    const CodeLensParams& params )
                    = 0;

                // :leftwards_arrow_with_hook: codeLens/resolve
                // :leftwards_arrow_with_hook: textDocument/documentLink
                // :leftwards_arrow_with_hook: documentLink/resolve
                // :leftwards_arrow_with_hook: textDocument/rename

              private:
                std::vector< Message > m_responseBuffer[ 2 ];
                std::size_t m_responseBufferSlot;
                std::mutex m_responseBufferLock;

                std::vector< Message > m_notificationBuffer[ 2 ];
                std::size_t m_notificationBufferSlot;
                std::mutex m_notificationBufferLock;

                std::mutex m_serverFlushLock;
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
