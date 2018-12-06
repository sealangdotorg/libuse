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

#include <libstdhl/Test>

#include <libstdhl/net/lsp/LSP>

using namespace libstdhl;
using namespace Network;
using namespace LSP;

auto uri = DocumentUri::fromString( "test://uri" );
const auto range = Range( Position( 1, 1 ), Position( 10, 1 ) );
const auto location = Location( uri, range );
const auto text = std::string( "text" );
const auto label = std::string( "label" );
const auto message = std::string( "message" );
const auto title = std::string( "title" );
const auto method = std::string( "method" );
const auto name = std::string( "name" );
const auto id = std::string( "id" );

TEST( libstdhl_cpp_network_lsp_content, ShowMessageParams )
{
    auto params = ShowMessageParams( MessageType::Error, message );
    EXPECT_STREQ( params.message().c_str(), message.c_str() );
    EXPECT_EQ( params.messageType(), MessageType::Error );
}

TEST( libstdhl_cpp_network_lsp_content, MessageActionItem )
{
    auto item = MessageActionItem( title );
    EXPECT_STREQ( item.title().c_str(), title.c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, ShowMessageRequestParams )
{
    auto params = ShowMessageRequestParams( MessageType::Error, message );
    EXPECT_STREQ( params.message().c_str(), message.c_str() );
    EXPECT_EQ( params.messageType(), MessageType::Error );
    EXPECT_FALSE( params.hasActions() );
    params.addAction( MessageActionItem( title ) );
    EXPECT_STREQ( params.actions()[ 0 ].title().c_str(), title.c_str() );
    EXPECT_TRUE( params.hasActions() );
}

TEST( libstdhl_cpp_network_lsp_content, TelemetryEventParams )
{
    auto params = TelemetryEventParams( Data::object() );
    EXPECT_STREQ( params.dump().c_str(), Data::object().dump().c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, Registration )
{
    auto reg = Registration( id, method );
    EXPECT_STREQ( reg.id().c_str(), id.c_str() );
    EXPECT_STREQ( reg.method().c_str(), method.c_str() );

    EXPECT_FALSE( reg.hasRegisterOptions() );
    reg.addRegisterOption( Data::object() );
    EXPECT_TRUE( reg.hasRegisterOptions() );
    EXPECT_STREQ( reg.registerOptions()[ 0 ].dump().c_str(), Data::object().dump().c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, RegistrationParams )
{
    auto reg = Registration( id, method );
    auto registrations = std::vector< Registration >();
    registrations.emplace_back( reg );
    registrations.emplace_back( reg );
    auto params = RegistrationParams( registrations );
    EXPECT_STREQ( params.registrations()[ 0 ].id().c_str(), id.c_str() );
    EXPECT_STREQ( params.registrations()[ 0 ].method().c_str(), method.c_str() );
    EXPECT_STREQ( params.registrations()[ 1 ].id().c_str(), id.c_str() );
    EXPECT_STREQ( params.registrations()[ 1 ].method().c_str(), method.c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, TextDocumentRegistrationOptions )
{
    auto filter = DocumentFilter();
    auto filters = std::vector< DocumentFilter >();
    filters.emplace_back( filter );
    auto selector = DocumentSelector( filters );
    auto options = TextDocumentRegistrationOptions( selector );
    EXPECT_STREQ( selector.dump().c_str(), DocumentSelector( filters ).dump().c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, TextDocumentChangeRegistrationOptions )
{
    auto filter = DocumentFilter();
    auto filters = std::vector< DocumentFilter >();
    filters.emplace_back( filter );
    auto selector = DocumentSelector( filters );
    auto options = TextDocumentChangeRegistrationOptions( selector, TextDocumentSyncKind::None );
    EXPECT_STREQ( selector.dump().c_str(), DocumentSelector( filters ).dump().c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, TextDocumentSaveRegistrationOptions )
{
    auto filter = DocumentFilter();
    auto filters = std::vector< DocumentFilter >();
    filters.emplace_back( filter );
    auto selector = DocumentSelector( filters );
    auto options = TextDocumentSaveRegistrationOptions( selector );
    EXPECT_STREQ( selector.dump().c_str(), DocumentSelector( filters ).dump().c_str() );

    EXPECT_FALSE( options.hasIncludeText() );
    options.setIncludeText( true );
    EXPECT_TRUE( options.includeText() );
    EXPECT_TRUE( options.hasIncludeText() );
}

TEST( libstdhl_cpp_network_lsp_content, SignatureHelpRegistrationOptions )
{
    auto filter = DocumentFilter();
    auto filters = std::vector< DocumentFilter >();
    filters.emplace_back( filter );
    auto selector = DocumentSelector( filters );
    auto options = SignatureHelpRegistrationOptions( selector );
    EXPECT_STREQ( selector.dump().c_str(), DocumentSelector( filters ).dump().c_str() );
    auto characters = std::vector< std::string >();
    characters.emplace_back( "test" );
    options.setTriggerCharacters( characters );
    options.addTriggerCharacter( std::string( "Character" ) );
    options.triggerCharacters();
    EXPECT_STREQ( options.triggerCharacters()[ 0 ].c_str(), "test" );
    EXPECT_STREQ( options.triggerCharacters()[ 1 ].c_str(), "Character" );
}

TEST( libstdhl_cpp_network_lsp_content, Unregistration )
{
    auto unregistration = Unregistration( id, method );
    // test Constructor
    Unregistration u( unregistration );
    EXPECT_STREQ( unregistration.id().c_str(), id.c_str() );
    EXPECT_STREQ( unregistration.method().c_str(), method.c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, UnregistrationParams )
{
    auto unregistration = Unregistration( id, method );
    auto vector = Unregistrations();
    vector.emplace_back( unregistration );
    auto params = UnregistrationParams( vector );
    // test ctor
    UnregistrationParams p( params );
    EXPECT_STREQ( params.unregistrations()[ 0 ].id().c_str(), id.c_str() );
    EXPECT_STREQ( params.unregistrations()[ 0 ].method().c_str(), method.c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, WorkspaceFolder )
{
    auto folder = WorkspaceFolder( "test://uri", name );
    WorkspaceFolder f( folder );
    EXPECT_STREQ( folder.uri().c_str(), "test://uri" );
    EXPECT_STREQ( folder.name().c_str(), name.c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, WorkspaceFoldersResult )
{
    auto folder = WorkspaceFolder( "test://uri", name );
    WorkspaceFoldersResult();
    auto vector = WorkspaceFolders();
    vector.emplace_back( folder );
    auto result = WorkspaceFoldersResult( vector );
    result.push_back( folder );
    EXPECT_STREQ( result.toVec()[ 0 ].uri().c_str(), "test://uri" );
    EXPECT_STREQ( result.toVec()[ 0 ].name().c_str(), name.c_str() );
    EXPECT_STREQ( result[ 1 ][ "uri" ].get< std::string >().c_str(), "test://uri" );
    EXPECT_STREQ( result[ 1 ][ name ].get< std::string >().c_str(), name.c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, WorkspaceFoldersChangeEvent )
{
    auto added = WorkspaceFolders();
    auto removed = WorkspaceFolders();
    auto folder = WorkspaceFolder( "test://uri", name );
    added.push_back( folder );
    removed.push_back( folder );
    auto event = WorkspaceFoldersChangeEvent( added, removed );
    WorkspaceFoldersChangeEvent e( event );
    EXPECT_STREQ( event.added()[ 0 ].uri().c_str(), "test://uri" );
    EXPECT_STREQ( event.added()[ 0 ].name().c_str(), name.c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, DidChangeWorkspaceFoldersParams )
{
    auto added = WorkspaceFolders();
    auto removed = WorkspaceFolders();
    auto folder = WorkspaceFolder( "test://uri", name );
    added.push_back( folder );
    removed.push_back( folder );
    auto event = WorkspaceFoldersChangeEvent( added, removed );
    auto params = DidChangeWorkspaceFoldersParams( event );
    EXPECT_STREQ( params.event().added()[ 0 ].uri().c_str(), "test://uri" );
    EXPECT_STREQ( params.event().added()[ 0 ].name().c_str(), name.c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, DidChangeConfigurationParams )
{
    auto params = DidChangeConfigurationParams( Data::object() );
    EXPECT_STREQ( params.settings().dump().c_str(), Data::object().dump().c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, ConfigurationItem )
{
    auto item = ConfigurationItem( Data::object() );
    EXPECT_FALSE( item.hasScopeUri() );
    EXPECT_FALSE( item.hasSection() );
    item.setScopeUri( "scopeUri" );
    item.setSection( "section" );
    EXPECT_STREQ( item.scopeUri().c_str(), "scopeUri" );
    EXPECT_STREQ( item.section().c_str(), "section" );
    EXPECT_TRUE( item.hasScopeUri() );
    EXPECT_TRUE( item.hasSection() );
}

TEST( libstdhl_cpp_network_lsp_content, ConfigurationParams )
{
    auto item = ConfigurationItem( Data::object() );
    auto items = ConfigurationItems();
    items.emplace_back( item );
    auto params = ConfigurationParams( items );
    ConfigurationParams p( params );
    params.items();
}

TEST( libstdhl_cpp_network_lsp_content, FileEvent )
{
    auto event = FileEvent( uri, FileChangeType::Changed );

    EXPECT_STREQ( event.documentUri().toString().c_str(), "test://uri" );
    EXPECT_EQ( event.type(), FileChangeType::Changed );
}

TEST( libstdhl_cpp_network_lsp_content, DidChangeWatchedFilesParams )
{
    auto event = FileEvent( uri, FileChangeType::Changed );
    auto events = FileEvents();
    events.emplace_back( event );
    auto params = DidChangeWatchedFilesParams( events );
    EXPECT_STREQ( params.changes()[ 0 ].documentUri().toString().c_str(), "test://uri" );
    EXPECT_EQ( params.changes()[ 0 ].type(), FileChangeType::Changed );
}

TEST( libstdhl_cpp_network_lsp_content, FileSystemWatcher )
{
    auto watcher = FileSystemWatcher( std::string( "globPattern" ) );
    FileSystemWatcher w( watcher );
    EXPECT_FALSE( watcher.hasKind() );
    watcher.setKind( WatchKind::Change );
    EXPECT_TRUE( watcher.hasKind() );
    EXPECT_STREQ( watcher.globPattern().c_str(), "globPattern" );
    EXPECT_EQ( watcher.kind(), WatchKind::Change );
}

TEST( libstdhl_cpp_network_lsp_content, DidChangeWatchedFilesRegistrationOptions )
{
    auto watcher = FileSystemWatcher( std::string( "globPattern" ) );
    FileSystemWatcher w( watcher );
    auto watchers = FileSystemWatchers();
    watchers.emplace_back( watcher );
    watchers.emplace_back( w );
    auto options = DidChangeWatchedFilesRegistrationOptions( watchers );
    EXPECT_STREQ( options.watchers()[ 0 ].globPattern().c_str(), "globPattern" );
    EXPECT_STREQ( options.watchers()[ 1 ].globPattern().c_str(), "globPattern" );
}

TEST( libstdhl_cpp_network_lsp_content, WorkspaceSymbolParams )
{
    auto params = WorkspaceSymbolParams( std::string( "query" ) );
    WorkspaceSymbolParams p( params );
    EXPECT_STREQ( params.query().c_str(), "query" );
}

TEST( libstdhl_cpp_network_lsp_content, SymbolInformation )
{
    auto info = SymbolInformation( name, SymbolKind::Array, location );
    EXPECT_FALSE( info.hasDeprecated() );
    EXPECT_FALSE( info.hasContainerName() );
    info.setDeprecated( true );
    info.setContainerName( "Containername" );
    EXPECT_TRUE( info.hasDeprecated() );
    EXPECT_TRUE( info.hasContainerName() );
    EXPECT_TRUE( info.isDeprecated() );
    EXPECT_STREQ( info.containerName().c_str(), "Containername" );

    SymbolInformation s( info );
    EXPECT_STREQ( info.dump().c_str(), s.dump().c_str() );
    EXPECT_STREQ( info.name().c_str(), name.c_str() );
    EXPECT_EQ( info.kind(), SymbolKind::Array );
    EXPECT_STREQ( info.location().dump().c_str(), location.dump().c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, WorkspaceSymbolResult )
{
    auto info = SymbolInformation( name, SymbolKind::Array, location );
    auto infos = SymbolInformationArray();
    infos.emplace_back( info );
    auto result = WorkspaceSymbolResult( infos );
    result.addSymbolInformation( info );
    WorkspaceSymbolResult r( result );
    EXPECT_STREQ( result.dump().c_str(), r.dump().c_str() );
    EXPECT_STREQ( result.symbolInformation()[ 0 ].name().c_str(), name.c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, ApplyWorkspaceEditParams )
{
    auto params = ApplyWorkspaceEditParams( WorkspaceEdit() );
    ApplyWorkspaceEditParams p( params );
    EXPECT_STREQ( params.edit().dump().c_str(), WorkspaceEdit().dump().c_str() );
    EXPECT_FALSE( params.hasLabel() );
    params.setLabel( label );
    EXPECT_TRUE( params.hasLabel() );
    EXPECT_STREQ( params.label().c_str(), label.c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, ApplyWorkspaceEditResult )
{
    auto result = ApplyWorkspaceEditResult( true );
    ApplyWorkspaceEditResult r( result );
    EXPECT_TRUE( result.applied() );
}

TEST( libstdhl_cpp_network_lsp_content, DidOpenTextDocumentParams )
{
    auto params = DidOpenTextDocumentParams( TextDocumentItem( uri, "languageId", 1, text ) );
    DidOpenTextDocumentParams p( params );
    EXPECT_STREQ( params.textDocument().uri().toString().c_str(), uri.toString().c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, TextDocumentContentChangeEvent )
{
    auto event = TextDocumentContentChangeEvent( text );
    TextDocumentContentChangeEvent e( event );
    EXPECT_FALSE( event.hasRange() );
    EXPECT_FALSE( event.hasRangeLength() );
    event.setRange( range );
    event.setRangeLength( 10 );
    EXPECT_TRUE( event.hasRangeLength() );
    EXPECT_TRUE( event.hasRange() );
    EXPECT_STREQ( range.dump().c_str(), event.range().dump().c_str() );
    EXPECT_EQ( event.rangeLength(), 10 );
    EXPECT_STREQ( event.text().c_str(), text.c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, DidChangeTextDocumentParams )
{
    auto events = TextDocumentContentChangeEvents();
    events.push_back( TextDocumentContentChangeEvent( text ) );
    auto params = DidChangeTextDocumentParams( VersionedTextDocumentIdentifier( uri, 1 ), events );
    DidChangeTextDocumentParams p( params );
    EXPECT_STREQ( params.dump().c_str(), p.dump().c_str() );
    EXPECT_STREQ( params.textDocument().uri().toString().c_str(), uri.toString().c_str() );
    EXPECT_STREQ( params.contentChanges()[ 0 ].text().c_str(), text.c_str() );
    EXPECT_STREQ( params.textDocument().uri().toString().c_str(), uri.toString().c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, WillSaveTextDocumentParams )
{
    auto params = WillSaveTextDocumentParams(
        VersionedTextDocumentIdentifier( uri, 1 ), TextDocumentSaveReason::Manual );
    WillSaveTextDocumentParams p( params );
    EXPECT_STREQ( params.dump().c_str(), p.dump().c_str() );
    EXPECT_STREQ( params.textDocument().uri().toString().c_str(), uri.toString().c_str() );
    EXPECT_STREQ( params.textDocument().uri().toString().c_str(), uri.toString().c_str() );
    EXPECT_EQ( params.reason(), TextDocumentSaveReason::Manual );
}

TEST( libstdhl_cpp_network_lsp_content, WillSaveWaitUntilResponse )
{
    auto textEdits = TextEdits();
    textEdits.push_back( TextEdit( range, text ) );
    auto response = WillSaveWaitUntilResponse( textEdits );
    WillSaveWaitUntilResponse r( response );
    EXPECT_STREQ( response.textEdit()[ 0 ].newText().c_str(), text.c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, DidSaveTextDocumentParams )
{
    auto params = DidSaveTextDocumentParams( TextDocumentIdentifier( uri ) );
    DidSaveTextDocumentParams p( params );
    EXPECT_FALSE( params.hasText() );
    params.setText( text );
    EXPECT_TRUE( params.hasText() );
    EXPECT_STREQ( params.text().c_str(), text.c_str() );
    EXPECT_STREQ( params.textDocument().uri().toString().c_str(), uri.toString().c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, DidCloseTextDocumentParams )
{
    auto params = DidCloseTextDocumentParams( TextDocumentIdentifier( uri ) );
    DidCloseTextDocumentParams p( params );
    EXPECT_STREQ( params.textDocument().uri().toString().c_str(), uri.toString().c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, CompletionItem )
{
    CompletionItem obj( label );
    EXPECT_STREQ( obj.label().c_str(), label.c_str() );

    EXPECT_FALSE( obj.hasKind() );
    obj.setKind( CompletionItemKind::Text );
    EXPECT_EQ( obj.kind(), CompletionItemKind::Text );
    EXPECT_TRUE( obj.hasKind() );

    EXPECT_FALSE( obj.hasDetail() );
    obj.setDetail( "detail" );
    EXPECT_STREQ( obj.detail().c_str(), "detail" );
    EXPECT_TRUE( obj.hasDetail() );

    EXPECT_FALSE( obj.hasDocumentation() );
    auto documentation = MarkupContent( std::string( "doc" ) );
    obj.setDocumentation( documentation );
    EXPECT_STREQ( obj.documentation().value().c_str(), "doc" );
    EXPECT_TRUE( obj.hasDocumentation() );

    EXPECT_FALSE( obj.hasDeprecated() );
    obj.setDeprecated( true );
    EXPECT_TRUE( obj.isDeprecated() );
    EXPECT_TRUE( obj.hasDeprecated() );

    EXPECT_FALSE( obj.hasPreselected() );
    obj.setPreselected( true );
    EXPECT_TRUE( obj.isPreselected() );
    EXPECT_TRUE( obj.hasPreselected() );

    EXPECT_FALSE( obj.hasSortText() );
    obj.setSortText( "SortText" );
    EXPECT_STREQ( obj.sortText().c_str(), "SortText" );
    EXPECT_TRUE( obj.hasSortText() );

    EXPECT_FALSE( obj.hasFilterText() );
    obj.setFilterText( "FilterText" );
    EXPECT_STREQ( obj.filterText().c_str(), "FilterText" );
    EXPECT_TRUE( obj.hasFilterText() );

    EXPECT_FALSE( obj.hasInsertText() );
    obj.setInsertText( "InsertText" );
    EXPECT_STREQ( obj.insertText().c_str(), "InsertText" );
    EXPECT_TRUE( obj.hasInsertText() );

    EXPECT_FALSE( obj.hasInsertTextFormat() );
    obj.setInsertTextFormat( InsertTextFormat::PlainText );
    EXPECT_EQ( obj.insertTextFormat(), InsertTextFormat::PlainText );
    EXPECT_TRUE( obj.hasInsertTextFormat() );

    EXPECT_FALSE( obj.hasTextEdit() );
    auto textEdit = TextEdit( range, "newText" );
    obj.setTextEdit( textEdit );
    EXPECT_STREQ( obj.textEdit().newText().c_str(), "newText" );
    EXPECT_TRUE( obj.hasTextEdit() );

    EXPECT_FALSE( obj.hasAdditionalTextEdits() );
    obj.addAdditionalTextEdit( textEdit );
    auto result = obj.additionalTextEdits()[ 0 ];
    EXPECT_STREQ( result.dump().c_str(), textEdit.dump().c_str() );
    EXPECT_TRUE( obj.hasTextEdit() );

    EXPECT_FALSE( obj.hasCommitCharacters() );
    obj.addCommitCharacter( "CommitCharacter" );
    auto commitCharacters = obj.commitCharacters();
    auto getCharacter = commitCharacters[ 0 ];
    EXPECT_STREQ( getCharacter.c_str(), "CommitCharacter" );
    EXPECT_TRUE( obj.hasCommitCharacters() );

    EXPECT_FALSE( obj.hasCommand() );
    auto command = Command( title, "command" );
    obj.setCommand( command );
    EXPECT_STREQ( obj.command().title().c_str(), title.c_str() );
    EXPECT_STREQ( obj.command().command().c_str(), "command" );
    EXPECT_TRUE( obj.hasCommand() );

    EXPECT_FALSE( obj.hasData() );
    obj.setData( Data::object() );
    EXPECT_STREQ( obj.data().dump().c_str(), Data::object().dump().c_str() );
    EXPECT_TRUE( obj.hasData() );
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
