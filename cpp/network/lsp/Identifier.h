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

#include "Message.h"

#include "../../standard/rfc3986.h"

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
            constexpr const char* JSONRPC = "jsonrpc";
            constexpr const char* JSONRPC_VERSION = "2.0";

            constexpr const char* ID = "id";
            constexpr const char* METHOD = "method";
            constexpr const char* PARAMS = "params";
            constexpr const char* RESULT = "result";
            constexpr const char* ERROR = "error";
            constexpr const char* DATA = "data";

            constexpr const char* NEW_TEXT = "newText";
            constexpr const char* VERSION = "version";
            constexpr const char* TEXT_DOCUMENT = "textDocument";
            constexpr const char* EDITS = "edits";
            constexpr const char* DOCUMENT_CHANGES = "documentChanges";
            constexpr const char* LANGUAGE = "language";
            constexpr const char* LANGUAGE_ID = "languageId";
            constexpr const char* TEXT = "text";
            constexpr const char* POSITION = "position";
            constexpr const char* SCHEME = "scheme";
            constexpr const char* PATTERN = "pattern";

            constexpr const char* DYNAMIC_REGISTRATION = "dynamicRegistration";
            constexpr const char* APPLY_EDIT = "applyEdit";
            constexpr const char* WORKSPACE_EDIT = "workspaceEdit";
            constexpr const char* DID_CHANGE_CONFIGURATION
                = "didChangeConfiguration";
            constexpr const char* DID_CHANGE_WATCHED_FILES
                = "didChangeWatchedFiles";
            constexpr const char* SYMBOL = "symbol";
            constexpr const char* EXECUTE_COMMAND = "executeCommand";

            constexpr const char* WILL_SAVE = "willSave";
            constexpr const char* WILL_SAVE_WAIT_UNITL = "willSaveWaitUntil";
            constexpr const char* DID_SAVE = "didSave";
            constexpr const char* SAVE = "save";

            constexpr const char* SNIPPET_SUPPORT = "snippetSupport";
            constexpr const char* COMPLETION_ITEM = "completionItem";

            constexpr const char* SYNCHRONIZATION = "synchronization";
            constexpr const char* COMPLETION = "completion";
            constexpr const char* HOVER = "hover";
            constexpr const char* SIGNATURE_HELP = "signatureHelp";
            constexpr const char* REFERENCES = "references";
            constexpr const char* DOCUMENT_HIGHLIGHT = "documentHighlight";
            constexpr const char* DOCUMENT_SYMBOL = "documentSymbol";
            constexpr const char* FORMATTING = "formatting";
            constexpr const char* RANGE_FORMATTING = "rangeFormatting";
            constexpr const char* ON_TYPE_FORMATTING = "onTypeFormatting";
            constexpr const char* DEFINITION = "definition";
            constexpr const char* CODE_ACTION = "codeAction";
            constexpr const char* CODE_LENS = "codeLens";
            constexpr const char* DOCUMENT_LINK = "documentLink";
            constexpr const char* RENAME = "rename";
            constexpr const char* RETRY = "retry";

            constexpr const char* WORKSPACE = "workspace";
            constexpr const char* EXPERIMENTAL = "experimental";
            constexpr const char* INCLUDE_TEXT = "includeText";
            constexpr const char* OPEN_CLOSE = "openClose";
            constexpr const char* CHANGE = "change";

            constexpr const char* RESOLVE_PROVIDER = "resolveProvider";
            constexpr const char* TRIGGER_CHARACTERS = "triggerCharacters";
            constexpr const char* MORE_TRIGGER_CHARACTER
                = "moreTriggerCharacter";
            constexpr const char* FIRST_TRIGGER_CHARACTER
                = "firstTriggerCharacter";

            constexpr const char* TEXT_DOCUMENT_SYNC = "textDocumentSync";
            constexpr const char* HOVER_PROVIDER = "hoverProvider";
            constexpr const char* COMPLETION_PROVIDER = "completionProvider";
            constexpr const char* SIGNATURE_HELP_PROVIDER
                = "signatureHelpProvider";
            constexpr const char* DEFINITION_PROVIDER = "definitionProvider";
            constexpr const char* REFERENCES_PROVIDER = "referencesProvider";
            constexpr const char* DOCUMENT_HIGHLIGHT_PROVIDER
                = "documentHighlightProvider";
            constexpr const char* DOCUMENT_SYMBOL_PROVIDER
                = "documentSymbolProvider";
            constexpr const char* WORKSPACE_SYMBOL_PROVIDER
                = "workspaceSymbolProvider";
            constexpr const char* CODE_ACTION_PROVIDER = "codeActionProvider";
            constexpr const char* CODE_LENS_PROVIDER = "codeLensProvider";
            constexpr const char* DOCUMENT_FORMATTING_PROVIDER
                = "documentFormattingProvider";
            constexpr const char* DOCUMENT_RANGE_FORMATTING_PROVIDER
                = "documentRangeFormattingProvider";
            constexpr const char* DOCUMENT_ON_TYPE_FORMATTING_PROVIDER
                = "documentOnTypeFormattingProvider";
            constexpr const char* RENAME_PROVIDER = "renameProvider";
            constexpr const char* DOCUMENT_LINK_PROVIDER
                = "documentLinkProvider";
            constexpr const char* EXECUTE_COMMAND_PROVIDER
                = "executeCommandProvider";

            constexpr const char* URI = "uri";
            constexpr const char* RANGE = "range";
            constexpr const char* LINE = "line";
            constexpr const char* CHARACTER = "character";
            constexpr const char* START = "start";
            constexpr const char* END = "end";
            constexpr const char* MESSAGE = "message";
            constexpr const char* SEVERITY = "severity";
            constexpr const char* CODE = "code";
            constexpr const char* SOURCE = "source";
            constexpr const char* COMMAND = "command";
            constexpr const char* COMMANDS = "commands";
            constexpr const char* TITLE = "title";
            constexpr const char* ARGUMENTS = "arguments";
            constexpr const char* PROCESS_ID = "processId";
            constexpr const char* ROOT_URI = "rootUri";
            constexpr const char* CAPABILITIES = "capabilities";
            constexpr const char* TRACE = "trace";

            constexpr const char* INITIALIZATION_OPTIONS
                = "initializationOptions";

            constexpr const char* INITIALIZE = "initialize";
            constexpr const char* INITIALIZED = "initialized";

            constexpr const char* SHUTDOWN = "shutdown";
            constexpr const char* EXIT = "exit";

            constexpr const char* RANGE_LENGTH = "rangeLength";
            constexpr const char* CONTENT_CHANGES = "contentChanges";
            constexpr const char* DIAGNOSTICS = "diagnostics";
            constexpr const char* CONTEXT = "context";

            namespace Identifier
            {
                constexpr const char* textDocument_publishDiagnostics
                    = "textDocument/publishDiagnostics";

                constexpr const char* textDocument_didOpen
                    = "textDocument/didOpen";

                constexpr const char* textDocument_didChange
                    = "textDocument/didChange";

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
