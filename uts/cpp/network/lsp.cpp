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

#include "uts/main.h"

#include "cpp/network/Lsp.h"

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

    ExecuteCommandResult workspace_executeCommand(
        const ExecuteCommandParams& params ) override
    {
        return ExecuteCommandResult();
    }

    void textDocument_didOpen(
        const DidOpenTextDocumentParams& params ) noexcept override
    {
    }

    void textDocument_didChange(
        const DidChangeTextDocumentParams& params ) noexcept override
    {
    }

    HoverResult textDocument_hover( const HoverParams& params ) override
    {
        return HoverResult();
    }

    CodeActionResult textDocument_codeAction(
        const CodeActionParams& params ) override
    {
        return CodeActionResult();
    }

    CodeLensResult textDocument_codeLens(
        const CodeLensParams& params ) override
    {
        return CodeLensResult();
    }
};

TEST( libstdhl_cpp_network_lsp, parse_packet_request_initialize )
{
    std::string req = "Content-Length: 1035\r\n";
    req += "\r\n";
    req += "{\"jsonrpc\":\"2.0\",\"id\":0,\"method\":\"initialize\",\"params\":{"
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

    const auto request = libstdhl::Network::LSP::Packet( req );

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

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
