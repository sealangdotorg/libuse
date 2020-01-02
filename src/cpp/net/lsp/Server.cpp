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

#include "Server.h"

#include "Identifier.h"

using namespace libstdhl;
using namespace Network;
using namespace LSP;

Server::Server( void )
{
}

//
//
//  Workspace
//

void Server::workspace_didChangeWorkspaceFolders(
    const DidChangeWorkspaceFoldersParams& params ) noexcept
{
}

void Server::workspace_didChangeConfiguration( const DidChangeConfigurationParams& params ) noexcept
{
}

void Server::workspace_didChangeWatchedFiles( const DidChangeWatchedFilesParams& params ) noexcept
{
}

WorkspaceSymbolResult Server::workspace_symbol( const WorkspaceSymbolParams& params )
{
    return WorkspaceSymbolResult( SymbolInformations() );
}

ExecuteCommandResult Server::workspace_executeCommand( const ExecuteCommandParams& params )
{
    return ExecuteCommandResult();
}

//
//
//  Text Synchronization
//

void Server::textDocument_didOpen( const DidOpenTextDocumentParams& params ) noexcept
{
}

void Server::textDocument_didChange( const DidChangeTextDocumentParams& params ) noexcept
{
}

void Server::textDocument_willSave( const WillSaveTextDocumentParams& params ) noexcept
{
}

WillSaveWaitUntilResponse Server::textDocument_willSaveWaitUntil(
    const WillSaveTextDocumentParams& params )
{
    return WillSaveWaitUntilResponse( TextEdits() );
}

void Server::textDocument_didSave( const DidSaveTextDocumentParams& params ) noexcept
{
}

void Server::textDocument_didClose( const DidCloseTextDocumentParams& params ) noexcept
{
}

//
//
//  Language Features
//

CompletionResult Server::textDocument_completion( const CompletionParams& params )
{
    return CompletionResult();
}

CompletionResolveResult Server::completionItem_resolve( const CompletionParams& params )
{
    return CompletionResolveResult( Data() );
}

HoverResult Server::textDocument_hover( const HoverParams& params )
{
    return HoverResult();
}

SignatureHelpResult Server::textDocument_signatureHelp( const SignatureHelpParams& params )
{
    return SignatureHelpResult();
}

// https://microsoft.github.io/language-server-protocol/specification#textDocument_declaration
// client to server request
// TODO: FIXME: @ppaulweber: API introduced in version 3.14.0, as goes for
// LocationLink, next PR

DefinitionResult Server::textDocument_definition( const DefinitionParams& params )
{
    return DefinitionResult( Data() );
}

TypeDefinitionResult Server::textDocument_typeDefinition( const TypeDefinitionParams& params )
{
    return TypeDefinitionResult( Data() );
}

TextDocumentImplementationResult Server::textDocument_implementation(
    const TextDocumentImplementationParams& params )
{
    return TextDocumentImplementationResult( Data() );
}

ReferenceResult Server::textDocument_references( const ReferenceParams& params )
{
    return ReferenceResult( Data() );
}

DocumentHighlightResult Server::textDocument_documentHighlight(
    const DocumentHighlightParams& params )
{
    return DocumentHighlightResult();
}

DocumentSymbolResult Server::textDocument_documentSymbol( const DocumentSymbolParams& params )
{
    return DocumentSymbolResult();
}

CodeActionResult Server::textDocument_codeAction( const CodeActionParams& params )
{
    return CodeActionResult();
}

CodeLensResult Server::textDocument_codeLens( const CodeLensParams& params )
{
    return CodeLensResult();
}

CodeLensResolveResult Server::codeLens_resolve( const CodeLensResolveParams& params )
{
    return CodeLensResolveResult( Data() );
}

DocumentLinkResult Server::textDocument_documentLink( const DocumentLinkParams& params )
{
    return DocumentLinkResult();
}

DocumentLinkResolveResult Server::documentLink_resolve( const DocumentLinkResolveParams& params )
{
    return DocumentLinkResolveResult( Data() );
}

DocumentColorResult Server::textDocument_documentColor( const DocumentColorParams& params )
{
    return DocumentColorResult( ColorInformations() );
}

ColorPresentationResult Server::textDocument_colorPresentation(
    const ColorPresentationParams& params )
{
    return ColorPresentationResult( ColorPresentations() );
}

DocumentFormattingResult Server::textDocument_formatting( const DocumentFormattingParams& params )
{
    return DocumentFormattingResult();
}

DocumentRangeFormattingResult Server::textDocument_rangeFormatting(
    const DocumentRangeFormattingParams& params )
{
    return DocumentRangeFormattingResult();
}

DocumentOnTypeFormattingResult Server::textDocument_onTypeFormatting(
    const DocumentOnTypeFormattingParams& params )
{
    return DocumentOnTypeFormattingResult();
}

RenameResult Server::textDocument_rename( const RenameParams& params )
{
    return RenameResult();
}

PrepareRenameResult Server::textDocument_prepareRename( const PrepareRenameParams& params )
{
    return PrepareRenameResult();
}

FoldingRangeResult Server::textDocument_foldingRange( const FoldingRangeParams& params )
{
    return FoldingRangeResult();
}

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
