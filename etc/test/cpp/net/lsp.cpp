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

#include <libstdhl/Test>

#include <libstdhl/net/lsp/LSP>

using namespace libstdhl;
using namespace Network;
using namespace LSP;

class TestInterface final : public ServerInterface
{
  public:
    TestInterface( void )
    {
    }

    InitializeResult initialize( const InitializeParams& params ) override
    {
        // throw Exception( "init failed", InitializeError( false ) );
        return InitializeResult( ServerCapabilities() );
    }

    void initialized( void ) noexcept override
    {
    }

    void shutdown( void ) override
    {
    }

    void exit( void ) noexcept override
    {
    }

    ExecuteCommandResult workspace_executeCommand( const ExecuteCommandParams& params ) override
    {
        return ExecuteCommandResult();
    }

    void workspace_didChangeWorkspaceFolders(
        const DidChangeWorkspaceFoldersParams& params ) noexcept override
    {
    }

    void workspace_didChangeConfiguration(
        const DidChangeConfigurationParams& params ) noexcept override
    {
    }

    void workspace_didChangeWatchedFiles(
        const DidChangeWatchedFilesParams& params ) noexcept override
    {
    }

    void workspace_symbol( const WorkspaceSymbolParams& params ) override
    {
    }

    void textDocument_didOpen( const DidOpenTextDocumentParams& params ) noexcept override
    {
    }

    void textDocument_didChange( const DidChangeTextDocumentParams& params ) noexcept override
    {
    }

    void textDocument_willSave( const WillSaveTextDocumentParams& params ) noexcept override
    {
    }

    WillSaveWaitUntilResponse textDocument_willSaveWaitUntil(
        const WillSaveTextDocumentParams& params ) override
    {
        return WillSaveWaitUntilResponse();
    }

    void textDocument_didSave( const DidSaveTextDocumentParams& params ) noexcept override
    {
    }

    void textDocument_didClose( const DidCloseTextDocumentParams& params ) noexcept override
    {
    }

    HoverResult textDocument_hover( const HoverParams& params ) override
    {
        return HoverResult();
    }

    CodeActionResult textDocument_codeAction( const CodeActionParams& params ) override
    {
        return CodeActionResult();
    }

    CodeLensResult textDocument_codeLens( const CodeLensParams& params ) override
    {
        return CodeLensResult();
    }
};

TEST( libstdhl_cpp_network_lsp, parse_packet_request_initialize_monaco )
{
    std::string req = "Content-Length: 1035\r\n";
    req += "\r\n";
    req +=
        "{\"jsonrpc\":\"2.0\",\"id\":0,\"method\":\"initialize\",\"params\":{"
        "\"rootPath\":null,\"rootUri\":null,\"capabilities\":{\"workspace\":{"
        "\"didChangeConfiguration\":{\"dynamicRegistration\":false},"
        "\"didChangeWatchedFiles\":{\"dynamicRegistration\":false},\"symbol\":"
        "{\"dynamicRegistration\":true},\"executeCommand\":{"
        "\"dynamicRegistration\":true}},\"textDocument\":{\"synchronization\":"
        "{\"dynamicRegistration\":true},\"completion\":{\"completionItem\":{"
        "\"snippetSupport\":true},\"dynamicRegistration\":true},\"hover\":{"
        "\"dynamicRegistration\":true},\"signatureHelp\":{"
        "\"dynamicRegistration\":true},\"references\":{"
        "\"dynamicRegistration\":true},\"documentHighlight\":{"
        "\"dynamicRegistration\":true},\"documentSymbol\":{"
        "\"dynamicRegistration\":true},\"formatting\":{"
        "\"dynamicRegistration\":true},\"rangeFormatting\":{"
        "\"dynamicRegistration\":true},\"onTypeFormatting\":{"
        "\"dynamicRegistration\":true},\"definition\":{"
        "\"dynamicRegistration\":true},\"codeAction\":{"
        "\"dynamicRegistration\":true},\"codeLens\":{\"dynamicRegistration\":"
        "true},\"documentLink\":{\"dynamicRegistration\":true},\"rename\":{"
        "\"dynamicRegistration\":true}}},\"trace\":\"off\"}}";

    const auto request = libstdhl::Network::LSP::Packet::parse( req );

    TestInterface server;

    request.process( server );

    server.flush( [&]( const Message& response ) {
        const auto packet = libstdhl::Network::LSP::Packet( response );

        std::string ack = "Content-Length: 55\r\n";
        ack += "Content-Type: application/vscode-jsonrpc; charset=utf-8\r\n";
        ack += "\r\n";
        ack += "{\"id\":\"0\"";
        ack += ",\"jsonrpc\":\"2.0\"";
        ack += ",\"result\":{\"capabilities\":{}}";
        ack += "}";

        EXPECT_STREQ( packet.dump().c_str(), ack.c_str() );
    } );
}

TEST( libstdhl_cpp_network_lsp, parse_packet_request_initialize_vscode )
{
    std::string req = "Content-Length: 2280\r\n";
    req += "Content-Type: application/vscode-jsonrpc; charset=utf-8\r\n";
    req += "\r\n";
    req +=
        "{\"id\":0,\"jsonrpc\":\"2.0\",\"method\":\"initialize\",\"params\":{\"capabilities\":{"
        "\"textDocument\":{\"codeAction\":{\"codeActionLiteralSupport\":{\"codeActionKind\":{"
        "\"valueSet\":[\"\",\"quickfix\",\"refactor\",\"refactor.extract\",\"refactor.inline\","
        "\"refactor.rewrite\",\"source\",\"source.organizeImports\"]}},\"dynamicRegistration\":"
        "true},\"codeLens\":{\"dynamicRegistration\":true},\"colorProvider\":{"
        "\"dynamicRegistration\":true},\"completion\":{\"completionItem\":{"
        "\"commitCharactersSupport\":true,\"deprecatedSupport\":true,\"documentationFormat\":["
        "\"markdown\",\"plaintext\"],\"preselectSupport\":true,\"snippetSupport\":true},"
        "\"completionItemKind\":{\"valueSet\":[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,"
        "21,22,23,24,25]},\"contextSupport\":true,\"dynamicRegistration\":true},\"definition\":{"
        "\"dynamicRegistration\":true},\"documentHighlight\":{\"dynamicRegistration\":true},"
        "\"documentLink\":{\"dynamicRegistration\":true},\"documentSymbol\":{"
        "\"dynamicRegistration\":true,\"hierarchicalDocumentSymbolSupport\":true,\"symbolKind\":{"
        "\"valueSet\":[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26]}},"
        "\"foldingRange\":{\"dynamicRegistration\":true,\"lineFoldingOnly\":true,\"rangeLimit\":"
        "5000},\"formatting\":{\"dynamicRegistration\":true},\"hover\":{\"contentFormat\":["
        "\"markdown\",\"plaintext\"],\"dynamicRegistration\":true},\"implementation\":{"
        "\"dynamicRegistration\":true},\"onTypeFormatting\":{\"dynamicRegistration\":true},"
        "\"publishDiagnostics\":{\"relatedInformation\":true},\"rangeFormatting\":{"
        "\"dynamicRegistration\":true},\"references\":{\"dynamicRegistration\":true},\"rename\":{"
        "\"dynamicRegistration\":true},\"signatureHelp\":{\"dynamicRegistration\":true,"
        "\"signatureInformation\":{\"documentationFormat\":[\"markdown\",\"plaintext\"]}},"
        "\"synchronization\":{\"didSave\":true,\"dynamicRegistration\":true,\"willSave\":true,"
        "\"willSaveWaitUntil\":true},\"typeDefinition\":{\"dynamicRegistration\":true}},"
        "\"workspace\":{\"applyEdit\":true,\"configuration\":true,\"didChangeConfiguration\":{"
        "\"dynamicRegistration\":true},\"didChangeWatchedFiles\":{\"dynamicRegistration\":true},"
        "\"executeCommand\":{\"dynamicRegistration\":true},\"symbol\":{\"dynamicRegistration\":"
        "true,\"symbolKind\":{\"valueSet\":[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,"
        "22,23,24,25,26]}},\"workspaceEdit\":{\"documentChanges\":true},\"workspaceFolders\":true}}"
        ",\"processId\":14607,\"rootPath\":null,\"rootUri\":null,\"trace\":\"off\","
        "\"workspaceFolders\":null}}";

    const auto request = libstdhl::Network::LSP::Packet::parse( req );

    TestInterface server;

    request.process( server );

    server.flush( [&]( const Message& response ) {
        const auto packet = libstdhl::Network::LSP::Packet( response );

        std::string ack = "Content-Length: 55\r\n";
        ack += "Content-Type: application/vscode-jsonrpc; charset=utf-8\r\n";
        ack += "\r\n";
        ack += "{\"id\":\"0\"";
        ack += ",\"jsonrpc\":\"2.0\"";
        ack += ",\"result\":{\"capabilities\":{}}";
        ack += "}";

        EXPECT_STREQ( packet.dump().c_str(), ack.c_str() );
    } );
}

TEST( libstdhl_cpp_network_lsp, window_showMessage )
{
    TestInterface server;
    server.window_showMessage( ShowMessageParams( MessageType::Error, "Error Message" ) );
    server.window_showMessage( ShowMessageParams( MessageType::Info, "Info Message" ) );
    server.window_showMessage( ShowMessageParams( MessageType::Log, "Log Message" ) );
    server.window_showMessage( ShowMessageParams( MessageType::Warning, "Warning Message" ) );

    server.flush( [&]( const Message& response ) {
        const auto packet = libstdhl::Network::LSP::Packet( response );
    } );
}

TEST( libstdhl_cpp_network_lsp, window_showMessageRequest )
{
    TestInterface server;
    ShowMessageRequestParams errorParams =
        ShowMessageRequestParams( MessageType::Error, "Error Message" );
    errorParams.addAction( MessageActionItem( std::string( "Title" ) ) );
    EXPECT_TRUE( errorParams.hasActions() );
    // TODO
    // EXPECT_STREQ( errorParams.actions()[ Identifier::title ].c_str(), std::string{ "Title"
    // }.c_str() );
    server.window_showMessageRequest( errorParams );

    ShowMessageRequestParams infoParams =
        ShowMessageRequestParams( MessageType::Info, "Info Message" );
    server.window_showMessageRequest( infoParams );
    EXPECT_FALSE( infoParams.hasActions() );

    server.window_showMessageRequest( ShowMessageRequestParams( MessageType::Log, "Log Message" ) );
    server.window_showMessageRequest(
        ShowMessageRequestParams( MessageType::Warning, "Warning Message" ) );

    server.flush( [&]( const Message& response ) {
        const auto packet = libstdhl::Network::LSP::Packet( response );
    } );
}

TEST( libstdhl_cpp_network_lsp, window_logMessage )
{
    TestInterface server;
    server.window_logMessage( LogMessageParams( MessageType::Error, "Error Message" ) );
    server.window_logMessage( LogMessageParams( MessageType::Info, "Info Message" ) );
    server.window_logMessage( LogMessageParams( MessageType::Log, "Log Message" ) );
    server.window_logMessage( LogMessageParams( MessageType::Warning, "Warning Message" ) );

    server.flush( [&]( const Message& response ) {
        const auto packet = libstdhl::Network::LSP::Packet( response );
    } );
}

TEST( libstdhl_cpp_network_lsp, telemetry_event )
{
    TestInterface server;
    server.telemetry_event( TelemetryEventParams( Data::object() ) );

    server.flush( [&]( const Message& response ) {
        const auto packet = libstdhl::Network::LSP::Packet( response );
    } );
}

TEST( libstdhl_cpp_network_lsp, client_registerCapability )
{
    TestInterface server;
    Registration reg( "1", "test/method" );
    Registration reg2( "2", "test/method" );
    std::vector< Registration > registrations { reg, reg2 };

    server.client_registerCapability( RegistrationParams( registrations ) );

    Data registrationsData( Data::object() );
    registrationsData[ "registrations" ].push_back( reg );

    server.client_registerCapability( RegistrationParams( registrationsData ) );

    server.flush( [&]( const Message& response ) {
        const auto packet = libstdhl::Network::LSP::Packet( response );
    } );
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
