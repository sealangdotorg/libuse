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

#include <libstdhl/net/lsp/Identifier>

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

TEST( libstdhl_cpp_network_lsp_content, CancelParams )
{
    auto params = CancelParams( 1 );
    auto p = CancelParams( params );
    auto s = CancelParams( std::string( "2" ) );
    EXPECT_STREQ( params.id().c_str(), "1" );
    EXPECT_STREQ( s.id().c_str(), "2" );
}

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

TEST( libstdhl_cpp_network_lsp_content, ShowMessageRequestResult )
{
    auto item = MessageActionItem( title );
    auto result = ShowMessageRequestResult( title );
    auto empty = ShowMessageRequestResult();
    Data data = Data::object();
    data[ Identifier::title ] = title;
    auto r = ShowMessageRequestResult( data );
    ShowMessageRequestResult test( item );
    EXPECT_STREQ( item.title().c_str(), result[ Identifier::title ].get< std::string >().c_str() );
    EXPECT_STREQ( empty.dump().c_str(), Data().dump().c_str() );
    EXPECT_STREQ( r.dump().c_str(), item.dump().c_str() );
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

TEST( libstdhl_cpp_network_lsp_content, ConfigurationResult )
{
    auto empty = ConfigurationResult();
    auto result = ConfigurationResult( Data::array() );
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
    auto infos = SymbolInformations();
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

TEST( libstdhl_cpp_network_lsp_content, CompletionRegistrationOptions )
{
    auto filters = std::vector< DocumentFilter >();
    filters.push_back( DocumentFilter() );
    auto options = CompletionRegistrationOptions( DocumentSelector( filters ) );
    CompletionRegistrationOptions o( options );
    EXPECT_FALSE( options.hasTriggerCharacters() );
    options.addTriggerCharacter( text );
    EXPECT_TRUE( options.hasTriggerCharacters() );
    EXPECT_STREQ( options.triggerCharacters()[ 0 ].c_str(), text.c_str() );

    EXPECT_FALSE( options.hasResolveProvider() );
    options.setResolveProvider( true );
    EXPECT_TRUE( options.hasResolveProvider() );
    EXPECT_TRUE( options.resolveProvider() );
}

TEST( libstdhl_cpp_network_lsp_content, CompletionContext )
{
    auto context = CompletionContext( CompletionTriggerKind::Invoked );
    CompletionContext c( context );
    EXPECT_EQ( context.triggerKind(), CompletionTriggerKind::Invoked );
    EXPECT_FALSE( context.hasTriggerCharacter() );
    context.setTriggerCharacter( text );
    EXPECT_TRUE( context.hasTriggerCharacter() );
    EXPECT_STREQ( context.triggerCharacter().c_str(), text.c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, CompletionParams )
{
    auto params = CompletionParams( TextDocumentIdentifier( uri ), Position( 1, 1 ) );
    CompletionParams p( params );
    EXPECT_FALSE( params.hasContext() );
    params.setContext( CompletionContext( CompletionTriggerKind::Invoked ) );
    EXPECT_TRUE( params.hasContext() );
    EXPECT_EQ( params.context().triggerKind(), CompletionTriggerKind::Invoked );
    EXPECT_STREQ(
        params.textDocument().dump().c_str(), TextDocumentIdentifier( uri ).dump().c_str() );
    EXPECT_STREQ( params.position().dump().c_str(), Position( 1, 1 ).dump().c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, MarkupContent )
{
    auto mc = MarkupContent( text );
    auto content = MarkupContent( MarkupKind::MARKDOWN, text );
    MarkupContent m( mc );
    MarkupContent c( content );
    EXPECT_STREQ( mc.value().c_str(), text.c_str() );
    EXPECT_STREQ( mc.kind().c_str(), Identifier::plaintext );
    EXPECT_STREQ( content.kind().c_str(), Identifier::markdown );
}

TEST( libstdhl_cpp_network_lsp_content, CompletionItem )
{
    CompletionItem obj( label );
    EXPECT_STREQ( obj.label().c_str(), label.c_str() );
    Data data = Data::object();
    data[ Identifier::label ] = label;
    data[ Identifier::documentation ] = text;
    CompletionItem item( data );
    CompletionItem test( label );
    test.setDocumentation( name );
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

TEST( libstdhl_cpp_network_lsp_content, CompletionList )
{
    auto items = CompletionItems();
    items.emplace_back( CompletionItem( label ) );
    auto list = CompletionList( true, items );
    CompletionList l( list );
    EXPECT_TRUE( list.isIncomplete() );
    EXPECT_STREQ( list.items()[ 0 ].label().c_str(), label.c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, ParameterInformation )
{
    auto information = ParameterInformation( label );
    ParameterInformation i( information );
    EXPECT_FALSE( information.hasDocumentation() );
    information.setDocumentation( text );
    EXPECT_TRUE( information.hasDocumentation() );
    EXPECT_STREQ( information.label().c_str(), label.c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, SignatureInformation )
{
    auto information = SignatureInformation( label );
    SignatureInformation i( information );
    EXPECT_STREQ( information.label().c_str(), label.c_str() );
    EXPECT_FALSE( information.hasDocumentation() );
    information.setDocumentation( MarkupContent( text ) );
    EXPECT_TRUE( information.hasDocumentation() );
    EXPECT_FALSE( information.hasParameters() );
    information.setParameters( ParameterInformations() );
    EXPECT_TRUE( information.hasParameters() );
}

TEST( libstdhl_cpp_network_lsp_content, SignatureHelp )
{
    auto signatures = SignatureInformations();
    signatures.emplace_back( SignatureInformation( label ) );
    auto help = SignatureHelp( signatures );
    SignatureHelp h( help );
    EXPECT_STREQ( help.signatures()[ 0 ].label().c_str(), label.c_str() );
    EXPECT_FALSE( help.hasActiveSignature() );
    help.setActiveSignature( 1 );
    EXPECT_TRUE( help.hasActiveSignature() );
    EXPECT_EQ( help.activeSignature(), 1 );
    EXPECT_FALSE( help.hasActiveParameter() );
    help.setActiveParameter( 1 );
    EXPECT_TRUE( help.hasActiveParameter() );
    EXPECT_EQ( help.activeParameter(), 1 );
}

TEST( libstdhl_cpp_network_lsp_content, TypeDefinitionResult )
{
    auto result = TypeDefinitionResult( Data() );
    auto r = TypeDefinitionResult( Location( uri, range ) );
    EXPECT_STREQ( r[ Identifier::uri ].get< std::string >().c_str(), uri.toString().c_str() );
    auto locations = Locations{ location, location, location };
    auto typeDefinitionResult = TypeDefinitionResult( locations );
    auto res = TypeDefinitionResult( typeDefinitionResult );
    EXPECT_STREQ(
        typeDefinitionResult[ 0 ][ Identifier::uri ].get< std::string >().c_str(),
        uri.toString().c_str() );
    TypeDefinitionResult tdr( typeDefinitionResult );
}

TEST( libstdhl_cpp_network_lsp_content, ReferenceContext )
{
    auto context = ReferenceContext( true );
    EXPECT_TRUE( context.includeDeclaration() );
    ReferenceContext c( context );
}

TEST( libstdhl_cpp_network_lsp_content, ReferenceParams )
{
    auto uri = TextDocumentIdentifier( DocumentUri::fromString( "test://uri" ) );
    auto pos = Position( 1, 1 );
    auto params = ReferenceParams( uri, pos, ReferenceContext( true ) );
    ReferenceParams p( params );
    EXPECT_TRUE( params.context().includeDeclaration() );
}
TEST( libstdhl_cpp_network_lsp_content, ReferenceResult )
{
    auto result = ReferenceResult( Data() );
    auto locations = Locations{ location, location, location };
    auto refResult = ReferenceResult( locations );
    ReferenceResult r( refResult );
}
TEST( libstdhl_cpp_network_lsp_content, DocumentHighlight )
{
    auto highlight = DocumentHighlight( range );
    DocumentHighlight h( highlight );
    EXPECT_FALSE( highlight.hasKind() );
    highlight.setKind( DocumentHighlightKind::Read );
    EXPECT_TRUE( highlight.hasKind() );
    EXPECT_EQ( highlight.kind(), DocumentHighlightKind::Read );
}

TEST( libstdhl_cpp_network_lsp_content, DocumentHighlightResult )
{
    auto null = DocumentHighlightResult();
    auto result = DocumentHighlightResult( null );
    auto res = DocumentHighlightResult( DocumentHighlights() );
}

TEST( libstdhl_cpp_network_lsp_content, DocumentSymbolParams )
{
    auto params = DocumentSymbolParams( TextDocumentIdentifier( uri ) );
    DocumentSymbolParams p( params );
    EXPECT_STREQ( params.textDocument().uri().toString().c_str(), uri.toString().c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, DocumentSymbol )
{
    auto symbol = DocumentSymbol( name, SymbolKind::Array, range, range );
    DocumentSymbol s( symbol );
    EXPECT_STREQ( symbol.name().c_str(), name.c_str() );
    EXPECT_EQ( symbol.kind(), SymbolKind::Array );
    EXPECT_STREQ( symbol.range().dump().c_str(), range.dump().c_str() );
    EXPECT_STREQ( symbol.selectionRange().dump().c_str(), range.dump().c_str() );

    EXPECT_FALSE( symbol.hasDetail() );
    symbol.setDetail( text );
    EXPECT_TRUE( symbol.hasDetail() );
    EXPECT_STREQ( symbol.detail().c_str(), text.c_str() );

    EXPECT_FALSE( symbol.hasChildren() );
    symbol.addChild( symbol );
    EXPECT_TRUE( symbol.hasChildren() );
    EXPECT_STREQ( symbol.children()[ 0 ].name().c_str(), name.c_str() );

    EXPECT_FALSE( symbol.hasDeprecated() );
    symbol.setDeprecated( true );
    EXPECT_TRUE( symbol.hasDeprecated() );
    EXPECT_TRUE( symbol.deprecated() );
}

TEST( libstdhl_cpp_network_lsp_content, DocumentSymbolResult )
{
    auto result = DocumentSymbolResult( DocumentSymbols() );
    auto res = DocumentSymbolResult( SymbolInformations() );
    auto data = Data::array();
    data.push_back( SymbolInformation( name, SymbolKind::Array, location ) );
    auto array = DocumentSymbolResult( data );
    DocumentSymbolResult r( result );
    DocumentSymbolResult obj( res );
    DocumentSymbolResult();
}

TEST( libstdhl_cpp_network_lsp_content, CodeActionContext )
{
    auto diagnostics = Diagnostics();
    diagnostics.push_back( Diagnostic( range, text ) );
    auto context = CodeActionContext( diagnostics );
    EXPECT_STREQ( context.diagnostics()[ 0 ].message().c_str(), text.c_str() );
    EXPECT_FALSE( context.hasKind() );
    context.setKind( CodeActionKind::QuickFix );
    EXPECT_TRUE( context.hasKind() );
    EXPECT_STREQ( context.kind().c_str(), Identifier::quickfix );
}

TEST( libstdhl_cpp_network_lsp_content, CodeActionParams )
{
    auto doc = TextDocumentIdentifier( DocumentUri::fromString( "test://uri" ) );
    auto params = CodeActionParams( doc, range, CodeActionContext( Diagnostics() ) );
    EXPECT_STREQ( params.range().dump().c_str(), range.dump().c_str() );
    EXPECT_STREQ( params.textDocument().uri().toString().c_str(), uri.toString().c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, CodeActionResult )
{
    auto r = CodeActionResult();
    auto commands = std::vector< Command >();
    auto result = CodeActionResult( commands );
    result.addCommand( Command( title, text ) );
    auto code( result );
}

TEST( libstdhl_cpp_network_lsp_content, CodeAction )
{
    auto action = CodeAction( title );
    EXPECT_FALSE( action.hasKind() );
    action.setKind( CodeActionKind::QuickFix );
    EXPECT_TRUE( action.hasKind() );
    EXPECT_STREQ( action.kind().c_str(), Identifier::quickfix );

    EXPECT_FALSE( action.hasDiagnostics() );
    action.addDiagnostic( Diagnostic( range, text ) );
    EXPECT_TRUE( action.hasDiagnostics() );
    EXPECT_STREQ( action.diagnostics()[ 0 ].message().c_str(), text.c_str() );

    EXPECT_FALSE( action.hasEdit() );
    action.setEdit( WorkspaceEdit() );
    EXPECT_TRUE( action.hasEdit() );
    EXPECT_STREQ( action.edit().dump().c_str(), WorkspaceEdit().dump().c_str() );

    EXPECT_FALSE( action.hasCommand() );
    action.setCommand( Command( title, text ) );
    EXPECT_TRUE( action.hasCommand() );
    EXPECT_STREQ( action.command().title().c_str(), title.c_str() );
    CodeAction test( action );
}

TEST( libstdhl_cpp_network_lsp_content, CodeActionOptions )
{
    auto options = CodeActionOptions( Data::object() );
    EXPECT_FALSE( options.hasCodeActionKinds() );
    options.addCodeActionKind( CodeActionKind::Refactor );
    EXPECT_TRUE( options.hasCodeActionKinds() );
    EXPECT_STREQ( options.codeActionKinds()[ 0 ].c_str(), Identifier::refactor );
    CodeActionOptions o( options );
}

TEST( libstdhl_cpp_network_lsp_content, CodeLensResult )
{
    auto result = CodeLensResult( std::vector< CodeLens >() );
    result.addCodeLens( CodeLens( range ) );
    CodeLensResult r( result );
    auto empty = CodeLensResult( Data() );
}

TEST( libstdhl_cpp_network_lsp_content, DocumentLinkParams )
{
    auto params = DocumentLinkParams( TextDocumentIdentifier( uri ) );
    auto p = DocumentLinkParams( params );
    EXPECT_STREQ( params.textDocument().uri().toString().c_str(), uri.toString().c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, DocumentLink )
{
    auto link = DocumentLink( range );
    auto l = DocumentLink( link );
    EXPECT_FALSE( link.hasTarget() );
    EXPECT_FALSE( link.hasData() );
    link.setTarget( uri );
    link.setData( Data::object() );
    EXPECT_TRUE( link.hasTarget() );
    EXPECT_TRUE( link.hasData() );
}

TEST( libstdhl_cpp_network_lsp_content, DocumentLinkResult )
{
    auto empty = DocumentLinkResult();
    auto result = DocumentLinkResult( DocumentLink( range ) );
    auto r = DocumentLinkResult( result );
}

TEST( libstdhl_cpp_network_lsp_content, DocumentColorParams )
{
    auto params = DocumentColorParams( TextDocumentIdentifier( uri ) );
    auto p = DocumentColorParams( params );
    EXPECT_STREQ( params.textDocument().uri().toString().c_str(), uri.toString().c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, Color )
{
    auto color = Color( 0.2, 0.3, 0.4, 0.4 );
    auto c = Color( color );
    EXPECT_EQ( 0.2f, color.red() );
    EXPECT_EQ( 0.3f, color.green() );
    EXPECT_EQ( 0.4f, color.blue() );
    EXPECT_EQ( 0.4f, color.alpha() );
}

auto color = Color( 0.2, 0.3, 0.4, 0.4 );

TEST( libstdhl_cpp_network_lsp_content, ColorInformation )
{
    auto color = Color( 0.2, 0.3, 0.4, 0.4 );
    auto information = ColorInformation( range, color );
    EXPECT_STREQ( information.range().dump().c_str(), range.dump().c_str() );
    EXPECT_STREQ( information.color().dump().c_str(), color.dump().c_str() );
    auto i = ColorInformation( information );
}

TEST( libstdhl_cpp_network_lsp_content, DocumentColorResult )
{
    auto result = DocumentColorResult( ColorInformations() );
    auto r = DocumentColorResult( result );
}

TEST( libstdhl_cpp_network_lsp_content, ColorPresentationParams )
{
    auto params = ColorPresentationParams( TextDocumentIdentifier( uri ), color, range );
    EXPECT_STREQ( params.textDocument().uri().toString().c_str(), uri.toString().c_str() );
    EXPECT_STREQ( params.color().dump().c_str(), color.dump().c_str() );
    EXPECT_STREQ( params.range().dump().c_str(), range.dump().c_str() );
    auto p = ColorPresentationParams( params );
    EXPECT_STREQ( params.dump().c_str(), p.dump().c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, ColorPresentation )
{
    auto presentation = ColorPresentation( label );
    EXPECT_FALSE( presentation.hasTextEdit() );
    EXPECT_FALSE( presentation.hasAdditionalTextEdits() );
    auto edit = TextEdit( range, text );
    presentation.setTextEdit( edit );
    presentation.addAdditionalTextEdit( edit );
    EXPECT_TRUE( presentation.hasTextEdit() );
    EXPECT_TRUE( presentation.hasAdditionalTextEdits() );
    EXPECT_STREQ( presentation.label().c_str(), label.c_str() );
    EXPECT_STREQ( presentation.textEdit().dump().c_str(), edit.dump().c_str() );
    EXPECT_STREQ( presentation.additionalTextEdits()[ 0 ].dump().c_str(), edit.dump().c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, ColorPresentationResult )
{
    auto presentations = ColorPresentations();
    presentations.emplace_back( ColorPresentation( label ) );
    auto result = ColorPresentationResult( ColorPresentations() );
    auto r = ColorPresentationResult( result );
}

TEST( libstdhl_cpp_network_lsp_content, FormattingOptions )
{
    auto options = FormattingOptions( 2, true );
    options.addBool( "key", true );
    options.addString( "string", text );
    options.addNumber( "number", 201 );
    auto test = FormattingOptions( options );
}

TEST( libstdhl_cpp_network_lsp_content, DocumentFormattingParams )
{
    auto options = FormattingOptions( 2, true );
    options.addBool( "key", true );
    options.addString( "string", text );
    options.addNumber( "number", 201 );
    auto params = DocumentFormattingParams( TextDocumentIdentifier( uri ), options );
    auto p = DocumentFormattingParams( params );
    params.textDocument();
    params.options();
}

TEST( libstdhl_cpp_network_lsp_content, DocumentFormattingResult )
{
    auto empty = DocumentFormattingResult();
    auto edits = TextEdits();
    edits.push_back( TextEdit( range, text ) );
    auto result = DocumentFormattingResult( edits );
    auto r = DocumentFormattingResult( result );
}

TEST( libstdhl_cpp_network_lsp_content, DocumentRangeFormattingParams )
{
    auto options = FormattingOptions( 2, true );
    options.addBool( "key", true );
    options.addString( "string", text );
    options.addNumber( "number", 201 );
    auto params = DocumentRangeFormattingParams( TextDocumentIdentifier( uri ), range, options );
    auto p = DocumentRangeFormattingParams( params );
    params.textDocument();
    params.options();
    params.range();
}

TEST( libstdhl_cpp_network_lsp_content, DocumentOnTypeFormattingParams )
{
    auto options = FormattingOptions( 2, true );
    options.addBool( "key", true );
    options.addString( "string", text );
    options.addNumber( "number", 201 );
    auto params = DocumentOnTypeFormattingParams(
        TextDocumentIdentifier( uri ), options, Position( 1, 1 ), "c" );
    auto p = DocumentOnTypeFormattingParams( params );
    params.textDocument();
    params.options();
    params.position();
    EXPECT_STREQ( params.ch().c_str(), "c" );
}

TEST( libstdhl_cpp_network_lsp_content, DocumentOnTypeFormattingRegistrationOptions )
{
    auto filter = DocumentFilter();
    auto filters = std::vector< DocumentFilter >();
    filters.emplace_back( filter );
    auto selector = DocumentSelector( filters );
    auto options = DocumentOnTypeFormattingRegistrationOptions( selector, "a" );
    EXPECT_FALSE( options.hasMoreTriggerCharacter() );
    options.addMoreTriggerCharacter( "b" );
    EXPECT_TRUE( options.hasMoreTriggerCharacter() );
    EXPECT_STREQ( options.moreTriggerCharacter()[ 0 ].c_str(), "b" );
    EXPECT_STREQ( options.firstTriggerCharacter().c_str(), "a" );
    auto o = DocumentOnTypeFormattingRegistrationOptions( options );
}

TEST( libstdhl_cpp_network_lsp_content, RenameParams )
{
    auto params = RenameParams( TextDocumentIdentifier( uri ), Position( 1, 1 ), name );
    EXPECT_STREQ( params.textDocument().uri().toString().c_str(), uri.toString().c_str() );
    params.position();
    EXPECT_STREQ( params.newName().c_str(), name.c_str() );
    auto p = RenameParams( params );
}

TEST( libstdhl_cpp_network_lsp_content, RenameRegistrationOptions )
{
    auto filter = DocumentFilter();
    auto filters = std::vector< DocumentFilter >();
    filters.emplace_back( filter );
    auto selector = DocumentSelector( filters );
    auto options = RenameRegistrationOptions( selector );
    EXPECT_FALSE( options.hasPrepareProvider() );
    options.setPrepareProvider( true );
    EXPECT_TRUE( options.hasPrepareProvider() );
    EXPECT_TRUE( options.prepareProvider() );
    auto o = RenameRegistrationOptions( options );
}

TEST( libstdhl_cpp_network_lsp_content, RenameResult )
{
    auto empty = RenameResult();
    auto result = RenameResult( WorkspaceEdit() );
    auto r = RenameResult( result );
    EXPECT_STREQ( result.dump().c_str(), r.dump().c_str() );
}

TEST( libstdhl_cpp_network_lsp_content, PrepareRenameResult )
{
    auto empty = PrepareRenameResult();
    auto result = PrepareRenameResult( range );
    auto wplaceholder = PrepareRenameResult( range, "placeholder" );
    auto test = PrepareRenameResult( result );
    test = PrepareRenameResult( wplaceholder );
}

TEST( libstdhl_cpp_network_lsp_content, FoldingRangeParams )
{
    auto params = FoldingRangeParams( TextDocumentIdentifier( uri ) );
    EXPECT_STREQ(
        params.textDocument().dump().c_str(), TextDocumentIdentifier( uri ).dump().c_str() );
    auto p = FoldingRangeParams( params );
}

TEST( libstdhl_cpp_network_lsp_content, FoldingRange )
{
    auto foldingRange = FoldingRange( 1, 2 );
    EXPECT_FALSE( foldingRange.hasStartCharacter() );
    EXPECT_FALSE( foldingRange.hasEndCharacter() );
    EXPECT_FALSE( foldingRange.hasKind() );
    foldingRange.setStartCharacter( 3 );
    foldingRange.setEndCharacter( 4 );
    foldingRange.setKind( FoldingRangeKind::Comment );
    EXPECT_STREQ( foldingRange.kind().c_str(), Identifier::comment );
    foldingRange.setKind( FoldingRangeKind::Imports );
    EXPECT_STREQ( foldingRange.kind().c_str(), Identifier::imports );
    foldingRange.setKind( FoldingRangeKind::Region );
    EXPECT_STREQ( foldingRange.kind().c_str(), Identifier::region );
    EXPECT_TRUE( foldingRange.hasStartCharacter() );
    EXPECT_TRUE( foldingRange.hasEndCharacter() );
    EXPECT_TRUE( foldingRange.hasKind() );
    EXPECT_EQ( foldingRange.startLine(), 1 );
    EXPECT_EQ( foldingRange.endLine(), 2 );
    EXPECT_EQ( foldingRange.startCharacter(), 3 );
    EXPECT_EQ( foldingRange.endCharacter(), 4 );
    auto f = FoldingRange( foldingRange );
}
TEST( libstdhl_cpp_network_lsp_content, FoldingRangeResult )
{
    auto empty = FoldingRangeResult();
    auto ranges = FoldingRanges();
    ranges.push_back( FoldingRange( 1, 2 ) );
    auto result = FoldingRangeResult( ranges );
    auto test = FoldingRangeResult( empty );
    test = FoldingRangeResult( result );
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
