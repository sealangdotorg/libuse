//
//  Copyright (C) 2014-2019 CASM Organization <https://casm-lang.org>
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
#ifndef _LIBSTDHL_CPP_NETWORK_LSP_SERVER_H_
#define _LIBSTDHL_CPP_NETWORK_LSP_SERVER_H_

#include <libstdhl/net/lsp/Interface>

/**
   @brief    TBD

   TBD
*/

namespace libstdhl
{
    namespace Network
    {
        namespace LSP
        {
            class Server : public ServerInterface
            {
              public:
                Server( void );

                virtual ~Server( void ) = default;

                //
                //
                //  Workspace
                //

                virtual void workspace_didChangeWorkspaceFolders(
                    const DidChangeWorkspaceFoldersParams& params ) noexcept;

                virtual void workspace_didChangeConfiguration(
                    const DidChangeConfigurationParams& params ) noexcept;

                virtual void workspace_didChangeWatchedFiles(
                    const DidChangeWatchedFilesParams& params ) noexcept;

                virtual WorkspaceSymbolResult workspace_symbol(
                    const WorkspaceSymbolParams& params );

                virtual ExecuteCommandResult workspace_executeCommand(
                    const ExecuteCommandParams& params );

                //
                //
                //  Text Synchronization
                //

                virtual void textDocument_didOpen(
                    const DidOpenTextDocumentParams& params ) noexcept;

                virtual void textDocument_didChange(
                    const DidChangeTextDocumentParams& params ) noexcept;

                virtual void textDocument_willSave(
                    const WillSaveTextDocumentParams& params ) noexcept;

                virtual WillSaveWaitUntilResponse textDocument_willSaveWaitUntil(
                    const WillSaveTextDocumentParams& params );

                virtual void textDocument_didSave(
                    const DidSaveTextDocumentParams& params ) noexcept;

                virtual void textDocument_didClose(
                    const DidCloseTextDocumentParams& params ) noexcept;

                //
                //
                //  Language Features
                //

                virtual CompletionResult textDocument_completion( const CompletionParams& params );

                virtual CompletionResolveResult completionItem_resolve(
                    const CompletionParams& params );

                virtual HoverResult textDocument_hover( const HoverParams& params );

                virtual SignatureHelpResult textDocument_signatureHelp(
                    const SignatureHelpParams& params );

                // https://microsoft.github.io/language-server-protocol/specification#textDocument_declaration
                // client to server request
                // TODO: FIXME: @ppaulweber: API introduced in version 3.14.0, as goes for
                // LocationLink, next PR

                virtual DefinitionResult textDocument_definition( const DefinitionParams& params );

                virtual TypeDefinitionResult textDocument_typeDefinition(
                    const TypeDefinitionParams& params );

                virtual TextDocumentImplementationResult textDocument_implementation(
                    const TextDocumentImplementationParams& params );

                virtual ReferenceResult textDocument_references( const ReferenceParams& params );

                virtual DocumentHighlightResult textDocument_documentHighlight(
                    const DocumentHighlightParams& params );

                virtual DocumentSymbolResult textDocument_documentSymbol(
                    const DocumentSymbolParams& params );

                virtual CodeActionResult textDocument_codeAction( const CodeActionParams& params );

                virtual CodeLensResult textDocument_codeLens( const CodeLensParams& params );

                virtual CodeLensResolveResult codeLens_resolve(
                    const CodeLensResolveParams& params );

                virtual DocumentLinkResult textDocument_documentLink(
                    const DocumentLinkParams& params );

                virtual DocumentLinkResolveResult documentLink_resolve(
                    const DocumentLinkResolveParams& params );

                virtual DocumentColorResult textDocument_documentColor(
                    const DocumentColorParams& params );

                virtual ColorPresentationResult textDocument_colorPresentation(
                    const ColorPresentationParams& params );

                virtual DocumentFormattingResult textDocument_formatting(
                    const DocumentFormattingParams& params );

                virtual DocumentRangeFormattingResult textDocument_rangeFormatting(
                    const DocumentRangeFormattingParams& params );

                virtual DocumentOnTypeFormattingResult textDocument_onTypeFormatting(
                    const DocumentOnTypeFormattingParams& params );

                virtual RenameResult textDocument_rename( const RenameParams& params );

                virtual PrepareRenameResult textDocument_prepareRename(
                    const PrepareRenameParams& params );

                virtual FoldingRangeResult textDocument_foldingRange(
                    const FoldingRangeParams& params );
            };
        }
    }
}

#endif  // _LIBSTDHL_CPP_NETWORK_LSP_SERVER_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
