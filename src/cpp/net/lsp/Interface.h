//
//  Copyright (C) 2014-2019 CASM Organization <https://casm-lang.org>
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
#ifndef _LIBSTDHL_CPP_NETWORK_LSP_INTERFACE_H_
#define _LIBSTDHL_CPP_NETWORK_LSP_INTERFACE_H_

#include <libstdhl/net/lsp/Message>

#include <mutex>

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

                void flush( const std::function< void( const Message& ) >& callback );

                /**
                   general
                */

                virtual InitializeResult initialize( const InitializeParams& params ) = 0;

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
                    const ExecuteCommandParams& params ) = 0;

                // New :arrow_right_hook: workspace/applyEdit

                /**
                    document
                 */

                // :arrow_left: textDocument/publishDiagnostics
                virtual void textDocument_publishDiagnostics(
                    const PublishDiagnosticsParams& params ) noexcept final;

                // :arrow_right: textDocument/didOpen
                virtual void textDocument_didOpen(
                    const DidOpenTextDocumentParams& params ) noexcept = 0;

                // :arrow_right: textDocument/didChange
                virtual void textDocument_didChange(
                    const DidChangeTextDocumentParams& params ) noexcept = 0;

                // :arrow_right: textDocument/willSave
                // New :leftwards_arrow_with_hook:
                // textDocument/willSaveWaitUntil
                // New :arrow_right: textDocument/didSave
                // :arrow_right: textDocument/didClose
                // :leftwards_arrow_with_hook: textDocument/completion
                // :leftwards_arrow_with_hook: completionItem/resolve

                // :leftwards_arrow_with_hook: textDocument/hover
                virtual HoverResult textDocument_hover( const HoverParams& params ) = 0;

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
                    const CodeActionParams& params ) = 0;

                // :leftwards_arrow_with_hook: textDocument/codeLens
                virtual CodeLensResult textDocument_codeLens( const CodeLensParams& params ) = 0;

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

#endif  // _LIBSTDHL_CPP_NETWORK_LSP_INTERFACE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
