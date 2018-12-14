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
#include <unordered_map>

/**
   @brief    TBD

   TBD

   https://microsoft.github.io/language-server-protocol/specification#version_3_13_0
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

                void request(
                    const RequestMessage& message,
                    const std::function< void( const ResponseMessage& ) >& callback );

                void handle( const ResponseMessage& message );

                void flush( const std::function< void( const Message& ) >& callback );

                //
                //
                //  Base Protocol
                //

                // https://microsoft.github.io/language-server-protocol/specification#initialize
                // client to server request
                virtual InitializeResult initialize( const InitializeParams& params ) = 0;

                // https://microsoft.github.io/language-server-protocol/specification#initialized
                // client to server notification
                virtual void initialized( void ) noexcept = 0;

                // https://microsoft.github.io/language-server-protocol/specification#shutdown
                // client to server request
                virtual void shutdown( void ) = 0;

                // https://microsoft.github.io/language-server-protocol/specification#exit
                // client to server notification
                virtual void exit( void ) noexcept = 0;

                // https://microsoft.github.io/language-server-protocol/specification#cancelRequest
                // server to client notification
                virtual void server_cancel( const CancelParams& params ) noexcept final;
                // client to server notification
                virtual void client_cancel( const CancelParams& params ) noexcept = 0;
                //
                //
                //  Window
                //

                // https://microsoft.github.io/language-server-protocol/specification#window_showMessage
                // server to client notification
                virtual void window_showMessage( const ShowMessageParams& params ) noexcept final;

                // https://microsoft.github.io/language-server-protocol/specification#window_showMessageRequest
                // server to client request
                virtual void window_showMessageRequest(
                    const ShowMessageRequestParams& params,
                    const std::function< void( const ShowMessageRequestResult& ) >& callback )
                    final;

                // https://microsoft.github.io/language-server-protocol/specification#window_logMessage
                // server to client notification
                virtual void window_logMessage( const LogMessageParams& params ) noexcept final;

                //
                //
                //  Telemetry
                //

                // https://microsoft.github.io/language-server-protocol/specification#telemetry_event
                // server to client notification
                virtual void telemetry_event( const TelemetryEventParams& params ) noexcept final;

                //
                //
                //  Client
                //

                // https://microsoft.github.io/language-server-protocol/specification#client_registerCapability
                // server to client request
                virtual void client_registerCapability( const RegistrationParams& params ) final;

                // https://microsoft.github.io/language-server-protocol/specification#client_unregisterCapability
                // server to client request
                virtual void client_unregisterCapability(
                    const UnregistrationParams& params ) final;

                //
                //
                //  Workspace
                //

                // https://microsoft.github.io/language-server-protocol/specification#workspace_workspaceFolders
                // server to client request
                virtual WorkspaceFoldersResult workspace_workspaceFolders( void ) final;

                // https://microsoft.github.io/language-server-protocol/specification#workspace_didChangeWorkspaceFolders
                // client to server notification
                virtual void workspace_didChangeWorkspaceFolders(
                    const DidChangeWorkspaceFoldersParams& params ) noexcept = 0;

                // https://microsoft.github.io/language-server-protocol/specification#workspace_didChangeConfiguration
                // client to server notification
                virtual void workspace_didChangeConfiguration(
                    const DidChangeConfigurationParams& params ) noexcept = 0;

                // https://microsoft.github.io/language-server-protocol/specification#workspace_configuration
                // server to client request
                virtual ConfigurationResult workspace_configuration(
                    const ConfigurationParams& params ) final;

                // https://microsoft.github.io/language-server-protocol/specification#workspace_didChangeWatchedFiles
                // client to server notification
                virtual void workspace_didChangeWatchedFiles(
                    const DidChangeWatchedFilesParams& params ) noexcept = 0;

                // https://microsoft.github.io/language-server-protocol/specification#workspace_symbol
                // client to server request
                // TODO: FIXME: @ppaulweber: replace 'void' with 'SymbolInformation|null' @Clasc
                virtual WorkspaceSymbolResult workspace_symbol(
                    const WorkspaceSymbolParams& params ) = 0;

                // https://microsoft.github.io/language-server-protocol/specification#workspace_executeCommand
                // client to server request
                virtual ExecuteCommandResult workspace_executeCommand(
                    const ExecuteCommandParams& params ) = 0;

                // https://microsoft.github.io/language-server-protocol/specification#workspace_applyEdit
                // server to client request
                virtual ApplyWorkspaceEditResult workspace_applyEdit(
                    const ApplyWorkspaceEditParams& params ) final;

                //
                //
                //  Text Document
                //

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_didOpen
                // client to server notification
                virtual void textDocument_didOpen(
                    const DidOpenTextDocumentParams& params ) noexcept = 0;

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_didChange
                // client to server notification
                virtual void textDocument_didChange(
                    const DidChangeTextDocumentParams& params ) noexcept = 0;

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_willSave
                // client to server notification
                virtual void textDocument_willSave(
                    const WillSaveTextDocumentParams& params ) noexcept = 0;

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_willSaveWaitUntil
                // New :leftwards_arrow_with_hook: textDocument/willSaveWaitUntil
                // client to server request
                virtual WillSaveWaitUntilResponse textDocument_willSaveWaitUntil(
                    const WillSaveTextDocumentParams& params ) = 0;

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_didSave
                // client to server notification
                virtual void textDocument_didSave(
                    const DidSaveTextDocumentParams& params ) noexcept = 0;

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_didClose
                // client to server notification
                virtual void textDocument_didClose(
                    const DidCloseTextDocumentParams& params ) noexcept = 0;

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_publishDiagnostics
                // server to client notification
                virtual void textDocument_publishDiagnostics(
                    const PublishDiagnosticsParams& params ) noexcept final;

                //
                //
                //  Language Features
                //

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_completion
                // client to server request
                // TODO: FIXME: @ppaulweber: provide interface @Clasc
                virtual CompletionResult textDocument_completion(
                    const CompletionParams& params ) = 0;
                // https://microsoft.github.io/language-server-protocol/specification#completionItem_resolve
                // client to server request
                // TODO: FIXME: @ppaulweber: provide interface @Clasc
                virtual CompletionResolveResult completionItem_resolve(
                    const CompletionParams& params ) = 0;
                // https://microsoft.github.io/language-server-protocol/specification#textDocument_hover
                // client to server request
                virtual HoverResult textDocument_hover( const HoverParams& params ) = 0;

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_signatureHelp
                // client to server request
                // TODO: FIXME: @ppaulweber: provide interface @Clasc
                virtual SignatureHelpResult textDocument_signatureHelp(
                    const SignatureHelpParams& params ) = 0;

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_definition
                // client to server request
                // TODO: FIXME: @ppaulweber: provide interface @Clasc
                virtual DefinitionResult textDocument_definition(
                    const DefinitionParams& params ) = 0;

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_typeDefinition
                // client to server request
                // TODO: FIXME: @ppaulweber: provide interface @Clasc
                virtual TypeDefinitionResult textDocument_typeDefinition(
                    const TypeDefinitionParams& params ) = 0;

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_implementation
                // client to server request
                // TODO: FIXME: @ppaulweber: provide interface @Clasc
                virtual TextDocumentImplementationResult textDocument_implementation(
                    const TextDocumentImplementationParams& params ) = 0;

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_references
                // client to server request
                // TODO: FIXME: @ppaulweber: provide interface @Clasc
                virtual ReferenceResult textDocument_references(
                    const ReferenceParams& params ) = 0;
                // https://microsoft.github.io/language-server-protocol/specification#textDocument_documentHighlight
                // client to server request
                // TODO: FIXME: @ppaulweber: provide interface @Clasc
                virtual DocumentHighlightResult textDocument_documentHighlight(
                    const DocumentHighlightParams& params ) = 0;

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_documentSymbol
                // client to server request
                // TODO: FIXME: @ppaulweber: provide interface @Clasc
                virtual DocumentSymbolResult textDocument_documentSymbol(
                    const DocumentSymbolParams& params ) = 0;

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_codeAction
                // client to server request
                virtual CodeActionResult textDocument_codeAction(
                    const CodeActionParams& params ) = 0;

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_codeLens
                // client to server request
                virtual CodeLensResult textDocument_codeLens( const CodeLensParams& params ) = 0;

                // https://microsoft.github.io/language-server-protocol/specification#codeLens_resolve
                // client to server request
                // TODO: FIXME: @ppaulweber: provide interface @Clasc
                virtual CodeLensResolveResult codeLens_resolve(
                    const CodeLensResolveParams& params ) = 0;

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_documentLink
                // client to server request
                // TODO: FIXME: @ppaulweber: provide interface @Clasc
                virtual DocumentLinkResult textDocument_documentLink(
                    const DocumentLinkParams& params ) = 0;

                // https://microsoft.github.io/language-server-protocol/specification#documentLink_resolve
                // client to server request
                // TODO: FIXME: @ppaulweber: provide interface @Clasc
                virtual DocumentLinkResolveResult documentLink_resolve(
                    const DocumentLinkResolveParams& params ) = 0;

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_documentColor
                // client to server request
                // TODO: FIXME: @ppaulweber: provide interface @Clasc
                virtual DocumentColorResult textDocument_documentColor(
                    const DocumentColorParams& params ) = 0;

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_colorPresentation
                // client to server request
                // TODO: FIXME: @ppaulweber: provide interface @Clasc
                virtual ColorPresentationResult textDocument_colorPresentation(
                    const ColorPresentationParams& params ) = 0;

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_formatting
                // client to server request
                // TODO: FIXME: @ppaulweber: provide interface @Clasc
                virtual DocumentFormattingResult textDocument_formatting(
                    const DocumentFormattingParams& params ) = 0;

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_rangeFormatting
                // client to server request
                // TODO: FIXME: @ppaulweber: provide interface @Clasc
                virtual DocumentRangeFormattingResult textDocument_rangeFormatting(
                    const DocumentRangeFormattingParams& params ) = 0;

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_onTypeFormatting
                // client to server request
                // TODO: FIXME: @ppaulweber: provide interface @Clasc
                virtual DocumentOnTypeFormattingResult textDocument_onTypeFormatting(
                    const DocumentOnTypeFormattingParams& params ) = 0;
                // https://microsoft.github.io/language-server-protocol/specification#textDocument_rename
                // client to server request
                // TODO: FIXME: @ppaulweber: provide interface @Clasc
                virtual RenameResult textDocument_rename( const RenameParams& params ) = 0;

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_prepareRename
                // client to server request
                // TODO: FIXME: @ppaulweber: provide interface @Clasc
                virtual PrepareRenameResult textDocument_prepareRename(
                    const PrepareRenameParams& params ) = 0;

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_foldingRange
                // client to server request
                // TODO: FIXME: @ppaulweber: provide interface @Clasc
                virtual FoldingRangeResult textDocument_foldingRange(
                    const FoldingRangeParams& params ) = 0;

              private:
                std::vector< Message > m_responseBuffer[ 2 ];
                std::size_t m_responseBufferSlot;
                std::mutex m_responseBufferLock;

                std::vector< Message > m_notificationBuffer[ 2 ];
                std::size_t m_notificationBufferSlot;
                std::mutex m_notificationBufferLock;

                std::vector< Message > m_requestBuffer[ 2 ];
                std::size_t m_requestBufferSlot;
                std::mutex m_requestBufferLock;
                std::unordered_map<
                    std::string,
                    const std::function< void( const ResponseMessage& ) >* >
                    m_requestCallback;

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
