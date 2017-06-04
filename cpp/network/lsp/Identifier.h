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

#ifndef _LIB_STDHL_CPP_NETWORK_LSP_IDENTIFIER_H_
#define _LIB_STDHL_CPP_NETWORK_LSP_IDENTIFIER_H_

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
            namespace Identifier
            {
                constexpr const char* jsonrpc = "jsonrpc";

                constexpr const char* jsonrpc_version = "2.0";

                constexpr const char* id = "id";

                constexpr const char* method = "method";

                constexpr const char* params = "params";

                constexpr const char* result = "result";

                constexpr const char* error = "error";

                constexpr const char* data = "data";

                constexpr const char* newText = "newText";

                constexpr const char* version = "version";

                constexpr const char* textDocument = "textDocument";

                constexpr const char* textDocumentSync = "textDocumentSync";

                constexpr const char* edits = "edits";

                constexpr const char* documentChanges = "documentChanges";

                constexpr const char* language = "language";

                constexpr const char* languageId = "languageId";

                constexpr const char* text = "text";

                constexpr const char* position = "position";

                constexpr const char* scheme = "scheme";

                constexpr const char* pattern = "pattern";

                constexpr const char* dynamicRegistration
                    = "dynamicRegistration";

                constexpr const char* applyEdit = "applyEdit";

                constexpr const char* workspaceEdit = "workspaceEdit";

                constexpr const char* didChangeConfiguration
                    = "didChangeConfiguration";

                constexpr const char* didChangeWatchedFiles
                    = "didChangeWatchedFiles";

                constexpr const char* symbol = "symbol";

                constexpr const char* executeCommand = "executeCommand";

                constexpr const char* willSave = "willSave";

                constexpr const char* willSaveWaitUntil = "willSaveWaitUntil";

                constexpr const char* didSave = "didSave";

                constexpr const char* save = "save";

                constexpr const char* snippetSupport = "snippetSupport";

                constexpr const char* completionItem = "completionItem";

                constexpr const char* synchronization = "synchronization";

                constexpr const char* completion = "completion";

                constexpr const char* hover = "hover";

                constexpr const char* signatureHelp = "signatureHelp";

                constexpr const char* references = "references";

                constexpr const char* documentHighlight = "documentHighlight";

                constexpr const char* documentSymbol = "documentSymbol";

                constexpr const char* formatting = "formatting";

                constexpr const char* rangeFormatting = "rangeFormatting";

                constexpr const char* onTypeFormatting = "onTypeFormatting";

                constexpr const char* definition = "definition";

                constexpr const char* codeAction = "codeAction";

                constexpr const char* codeLens = "codeLens";

                constexpr const char* documentLink = "documentLink";

                constexpr const char* rename = "rename";

                constexpr const char* retry = "retry";

                constexpr const char* workspace = "workspace";

                constexpr const char* experimental = "experimental";

                constexpr const char* includeText = "includeText";

                constexpr const char* openClose = "openClose";

                constexpr const char* change = "change";

                constexpr const char* resolveProvider = "resolveProvider";

                constexpr const char* triggerCharacters = "triggerCharacters";

                constexpr const char* moreTriggerCharacter
                    = "moreTriggerCharacter";

                constexpr const char* firstTriggerCharacter
                    = "firstTriggerCharacter";

                constexpr const char* hoverProvider = "hoverProvider";

                constexpr const char* completionProvider = "completionProvider";

                constexpr const char* signatureHelpProvider
                    = "signatureHelpProvider";

                constexpr const char* definitionProvider = "definitionProvider";

                constexpr const char* referencesProvider = "referencesProvider";

                constexpr const char* documentHighlightProvider
                    = "documentHighlightProvider";

                constexpr const char* documentSymbolProvider
                    = "documentSymbolProvider";

                constexpr const char* workspaceSymbolProvider
                    = "workspaceSymbolProvider";

                constexpr const char* codeActionProvider = "codeActionProvider";

                constexpr const char* codeLensProvider = "codeLensProvider";

                constexpr const char* documentFormattingProvider
                    = "documentFormattingProvider";

                constexpr const char* documentRangeFormattingProvider
                    = "documentRangeFormattingProvider";

                constexpr const char* documentOnTypeFormattingProvider
                    = "documentOnTypeFormattingProvider";

                constexpr const char* renameProvider = "renameProvider";

                constexpr const char* documentLinkProvider
                    = "documentLinkProvider";

                constexpr const char* executeCommandProvider
                    = "executeCommandProvider";

                constexpr const char* uri = "uri";

                constexpr const char* range = "range";

                constexpr const char* line = "line";

                constexpr const char* character = "character";

                constexpr const char* start = "start";

                constexpr const char* end = "end";

                constexpr const char* message = "message";

                constexpr const char* severity = "severity";

                constexpr const char* code = "code";

                constexpr const char* source = "source";

                constexpr const char* command = "command";

                constexpr const char* commands = "commands";

                constexpr const char* title = "title";

                constexpr const char* arguments = "arguments";

                constexpr const char* processId = "processId";

                constexpr const char* rootUri = "rootUri";

                constexpr const char* capabilities = "capabilities";

                constexpr const char* trace = "trace";

                constexpr const char* initializationOptions
                    = "initializationOptions";

                constexpr const char* initialize = "initialize";

                constexpr const char* initialized = "initialized";

                constexpr const char* shutdown = "shutdown";

                constexpr const char* exit = "exit";

                constexpr const char* rangeLength = "rangeLength";

                constexpr const char* contentChanges = "contentChanges";

                constexpr const char* diagnostics = "diagnostics";

                constexpr const char* context = "context";

                constexpr const char* value = "value";

                constexpr const char* contents = "contents";

                constexpr const char* textDocument_publishDiagnostics
                    = "textDocument/publishDiagnostics";

                constexpr const char* textDocument_didOpen
                    = "textDocument/didOpen";

                constexpr const char* textDocument_didChange
                    = "textDocument/didChange";

                constexpr const char* textDocument_hover = "textDocument/hover";

                constexpr const char* textDocument_codeAction
                    = "textDocument/codeAction";
            }
        }
    }
}

#endif // _LIB_STDHL_CPP_NETWORK_LSP_IDENTIFIER_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
