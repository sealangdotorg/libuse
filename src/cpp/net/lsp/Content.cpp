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

#include "Content.h"

#include "Identifier.h"

/**
   @brief    TBD

   TBD
*/

using namespace libstdhl;
using namespace Network;
using namespace LSP;

//
//
// Content
//

static const std::string CONTENT = "LSP: Content:";

//
//
// Response Error
//

ResponseError::ResponseError( const Data& data )
: Data( data )
{
    validate( data );
}

ResponseError::ResponseError( const ErrorCode code, const std::string& message )
: Data( Data::object() )
{
    operator[]( Identifier::code ) = static_cast< i32 >( code );
    operator[]( Identifier::message ) = message;
}

ErrorCode ResponseError::code( void ) const
{
    const auto c = at( Identifier::code ).get< i32 >();
    return static_cast< ErrorCode >( c );
}

std::string ResponseError::message( void ) const
{
    return at( Identifier::message );
}

u1 ResponseError::hasData( void ) const
{
    return find( Identifier::data ) != end();
}

Data ResponseError::data( void ) const
{
    return at( Identifier::data );
}

void ResponseError::setData( const Data& data )
{
    operator[]( Identifier::data ) = Data::from_cbor( Data::to_cbor( data ) );
}

void ResponseError::validate( const Data& data )
{
    static const auto context = CONTENT + " ResponseError:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsNumber( context, data, Identifier::code, true );
    Json::validatePropertyIsString( context, data, Identifier::message, true );
    Json::validatePropertyIsObject( context, data, Identifier::data, false );
}

//
//
// Position
//

Position::Position( const Data& data )
: Data( data )
{
    validate( data );
}

Position::Position( const std::size_t line, const std::size_t character )
: Data( Data::object() )
{
    operator[]( Identifier::line ) = line;
    operator[]( Identifier::character ) = character;
}

std::size_t Position::line( void ) const
{
    return at( Identifier::line ).get< std::size_t >();
}

std::size_t Position::character( void ) const
{
    return at( Identifier::character ).get< std::size_t >();
}

void Position::validate( const Data& data )
{
    static const auto context = CONTENT + " Position:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsNumber( context, data, Identifier::line, true );
    Json::validatePropertyIsNumber( context, data, Identifier::character, true );
}

//
//
// Range
//

Range::Range( const Data& data )
: Data( data )
{
    validate( data );
}

Range::Range( const Position& start, const Position& end )
: Data( Data::object() )
{
    operator[]( Identifier::start ) = Data::from_cbor( Data::to_cbor( start ) );
    operator[]( Identifier::end ) = Data::from_cbor( Data::to_cbor( end ) );
}

Position Range::start( void ) const
{
    return at( Identifier::start );
}

Position Range::end( void ) const
{
    return at( Identifier::end );
}

void Range::validate( const Data& data )
{
    static const auto context = CONTENT + " Range:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< Position >( context, data, Identifier::start, true );
    Json::validatePropertyIs< Position >( context, data, Identifier::end, true );
}

//
//
// Location
//

Location::Location( const Data& data )
: Data( data )
{
    validate( data );
}

Location::Location( const DocumentUri& uri, const Range& range )
: Data( Data::object() )
{
    operator[]( Identifier::uri ) = uri.toString();
    operator[]( Identifier::range ) = Data::from_cbor( Data::to_cbor( range ) );
}

DocumentUri Location::uri( void ) const
{
    return DocumentUri::fromString( at( Identifier::uri ).get< std::string >() );
}

Range Location::range( void ) const
{
    return at( Identifier::range );
}

void Location::validate( const Data& data )
{
    static const auto context = CONTENT + " Location:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsUri( context, data, Identifier::uri, true );
    Json::validatePropertyIs< Range >( context, data, Identifier::range, false );
}

//
//
// DiagnosticRelatedInformation
//

DiagnosticRelatedInformation::DiagnosticRelatedInformation( const Data& data )
: Data( data )
{
    validate( data );
}

DiagnosticRelatedInformation::DiagnosticRelatedInformation(
    const Location& location, const std::string& message )
: Data( Data::object() )
{
    operator[]( Identifier::location ) = Data::from_cbor( Data::to_cbor( location ) );
    operator[]( Identifier::message ) = message;
}

Location DiagnosticRelatedInformation::location( void ) const
{
    return operator[]( Identifier::location );
}

std::string DiagnosticRelatedInformation::message( void ) const
{
    return operator[]( Identifier::message ).get< std::string >();
}

void DiagnosticRelatedInformation::validate( const Data& data )
{
    static const auto context = CONTENT + " DiagnosticRelatedInformation:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< Location >( context, data, Identifier::location, true );
    Json::validatePropertyIsString( context, data, Identifier::message, true );
}

//
//
// Diagnostic
//

Diagnostic::Diagnostic( const Data& data )
: Data( data )
{
    validate( data );
}

Diagnostic::Diagnostic( const Range& range, const std::string& message )
: Data( Data::object() )
{
    operator[]( Identifier::range ) = Data::from_cbor( Data::to_cbor( range ) );
    operator[]( Identifier::message ) = message;
}

Range Diagnostic::range( void ) const
{
    return at( Identifier::range );
}

std::string Diagnostic::message( void ) const
{
    return at( Identifier::message ).get< std::string >();
}

u1 Diagnostic::hasSeverity( void ) const
{
    return find( Identifier::severity ) != end();
}

DiagnosticSeverity Diagnostic::severity( void ) const
{
    return static_cast< DiagnosticSeverity >( at( Identifier::severity ).get< std::size_t >() );
}

void Diagnostic::setSeverity( const DiagnosticSeverity& severity )
{
    operator[]( Identifier::severity ) = static_cast< std::size_t >( severity );
}

u1 Diagnostic::hasCode( void ) const
{
    return find( Identifier::code ) != end();
}

std::string Diagnostic::code( void ) const
{
    return at( Identifier::code ).get< std::string >();
}

void Diagnostic::setCode( const std::string& code )
{
    operator[]( Identifier::code ) = code;
}

void Diagnostic::setCode( const std::size_t code )
{
    operator[]( Identifier::code ) = code;
}

u1 Diagnostic::hasSource( void ) const
{
    return find( Identifier::source ) != end();
}

std::string Diagnostic::source( void ) const
{
    return at( Identifier::source ).get< std::string >();
}

void Diagnostic::setSource( const std::string& source )
{
    operator[]( Identifier::source ) = source;
}

void Diagnostic::validate( const Data& data )
{
    static const auto context = CONTENT + " Diagnostic:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::message, true );
    Json::validatePropertyIs< Range >( context, data, Identifier::range, true );
    Json::validatePropertyIsUuid( context, data, Identifier::code, false );
    Json::validatePropertyIsNumber( context, data, Identifier::severity, false );
    Json::validatePropertyIsString( context, data, Identifier::source, false );
}

//
//
// Command
//

Command::Command( const Data& data )
: Data( data )
{
    validate( data );
}

Command::Command( const std::string& title, const std::string& command )
: Data( Data::object() )
{
    operator[]( Identifier::title ) = title;
    operator[]( Identifier::command ) = command;
    operator[]( Identifier::arguments ) = Data::array();
}

std::string Command::title( void ) const
{
    return at( Identifier::title ).get< std::string >();
}

std::string Command::command( void ) const
{
    return at( Identifier::command ).get< std::string >();
}

u1 Command::hasArguments( void ) const
{
    return find( Identifier::arguments ) != end();
}

Data Command::arguments( void ) const
{
    return at( Identifier::arguments );
}

void Command::addArgument( const Data& argument )
{
    operator[]( Identifier::arguments ).push_back( argument );
}

void Command::validate( const Data& data )
{
    static const auto context = CONTENT + " Command:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::title, true );
    Json::validatePropertyIsString( context, data, Identifier::command, true );
    Json::validatePropertyIsArray( context, data, Identifier::arguments, false );
}

//
//
// TextEdit
//

TextEdit::TextEdit( const Data& data )
: Data( data )
{
    validate( data );
}

TextEdit::TextEdit( const Range& range, const std::string& newText )
: Data( Data::object() )
{
    operator[]( Identifier::range ) = Data::from_cbor( Data::to_cbor( range ) );
    operator[]( Identifier::newText ) = newText;
}

Range TextEdit::range( void ) const
{
    return at( Identifier::range );
}

std::string TextEdit::newText( void ) const
{
    return at( Identifier::newText ).get< std::string >();
}

void TextEdit::validate( const Data& data )
{
    static const auto context = CONTENT + " TextEdit:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::newText, true );
    Json::validatePropertyIs< Range >( context, data, Identifier::range, true );
}

//
//
// TextDocumentIdentifier
//

TextDocumentIdentifier::TextDocumentIdentifier( const Data& data )
: Data( data )
{
    validate( data );
}

TextDocumentIdentifier::TextDocumentIdentifier( const DocumentUri& uri )
: Data( Data::object() )
{
    operator[]( Identifier::uri ) = uri.toString();
}

DocumentUri TextDocumentIdentifier::uri( void ) const
{
    return DocumentUri::fromString( at( Identifier::uri ).get< std::string >() );
}

void TextDocumentIdentifier::validate( const Data& data )
{
    static const auto context = CONTENT + " TextDocumentIdentifier:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsUri( context, data, Identifier::uri, true );
}

//
//
// VersionedTextDocumentIdentifier
//

VersionedTextDocumentIdentifier::VersionedTextDocumentIdentifier( const Data& data )
: TextDocumentIdentifier( data )
{
    validate( data );
}

VersionedTextDocumentIdentifier::VersionedTextDocumentIdentifier(
    const DocumentUri& uri, const std::size_t version )
: TextDocumentIdentifier( uri )
{
    operator[]( Identifier::version ) = version;
}

std::size_t VersionedTextDocumentIdentifier::version( void ) const
{
    return at( Identifier::version ).get< std::size_t >();
}

void VersionedTextDocumentIdentifier::validate( const Data& data )
{
    static const auto context = CONTENT + " VersionedTextDocumentIdentifier:";
    TextDocumentIdentifier::validate( data );
    Json::validatePropertyIsNumber( context, data, Identifier::version, true );
}

//
//
// TextDocumentEdit
//

TextDocumentEdit::TextDocumentEdit( const Data& data )
: Data( data )
{
    validate( data );
}

TextDocumentEdit::TextDocumentEdit(
    const VersionedTextDocumentIdentifier& textDocument, const std::vector< TextEdit >& edits )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument ) = Data::from_cbor( Data::to_cbor( textDocument ) );
    operator[]( Identifier::edits ) = Data::array();

    for( auto edit : edits )
    {
        operator[]( Identifier::edits ).push_back( edit );
    }
}

VersionedTextDocumentIdentifier TextDocumentEdit::textDocument( void ) const
{
    return at( Identifier::textDocument );
}

Data TextDocumentEdit::edits( void ) const
{
    return at( Identifier::edits );
}

void TextDocumentEdit::validate( const Data& data )
{
    static const auto context = CONTENT + " TextDocumentEdit:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< VersionedTextDocumentIdentifier >(
        context, data, Identifier::textDocument, true );
    Json::validatePropertyIsArrayOf< TextEdit >( context, data, Identifier::edits, true );
}

//
//
// WorkspaceEdit
//

WorkspaceEdit::WorkspaceEdit( const Data& data )
: Data( data )
{
    validate( data );
}

WorkspaceEdit::WorkspaceEdit( void )
: Data( Data::object() )
{
    operator[]( Identifier::documentChanges ) = Data::array();
}

u1 WorkspaceEdit::hasDocumentChanges( void ) const
{
    return find( Identifier::documentChanges ) != end();
}

Data WorkspaceEdit::documentChanges( void ) const
{
    return at( Identifier::documentChanges );
}

void WorkspaceEdit::addDocumentChange( const TextDocumentEdit& documentChange )
{
    operator[]( Identifier::documentChanges ).push_back( documentChange );
}

void WorkspaceEdit::validate( const Data& data )
{
    static const auto context = CONTENT + " WorkspaceEdit:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsArrayOf< TextDocumentEdit >(
        context, data, Identifier::documentChanges, false );
}

//
//
// CreateFileOptions
//

CreateFileOptions::CreateFileOptions( const Data& data )
: Data( data )
{
    validate( data );
}

CreateFileOptions::CreateFileOptions( const u1 overwrite, const u1 ignoreIfExists )
: Data( Data::object() )
{
    operator[]( Identifier::overwrite ) = overwrite;
    operator[]( Identifier::ignoreIfExists ) = ignoreIfExists;
}

u1 CreateFileOptions::hasOverwrite( void ) const
{
    return find( Identifier::overwrite ) != end();
}

u1 CreateFileOptions::overwrite( void ) const
{
    return at( Identifier::overwrite ).get< u1 >();
}

void CreateFileOptions::setOverwrite( const u1 overwrite )
{
    operator[]( Identifier::overwrite ) = overwrite;
}
u1 CreateFileOptions::hasIgnoreIfExists( void ) const
{
    return find( Identifier::ignoreIfExists ) != end();
}

u1 CreateFileOptions::ignoreIfExists( void ) const
{
    return at( Identifier::ignoreIfExists ).get< u1 >();
}

void CreateFileOptions::setIgnoreIfExists( const u1 ignoreIfExists )
{
    operator[]( Identifier::ignoreIfExists ) = ignoreIfExists;
}
void CreateFileOptions::validate( const Data& data )
{
    static const auto context = CONTENT + " CreateFileOptions:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsBoolean( context, data, Identifier::overwrite, false );
    Json::validatePropertyIsBoolean( context, data, Identifier::ignoreIfExists, false );
}

//
//
// CreateFile
//

CreateFile::CreateFile( const Data& data )
: Data( data )
{
    validate( data );
}

CreateFile::CreateFile( const DocumentUri& uri )
: Data( Data::object() )
{
    operator[]( Identifier::kind ) = std::string( Identifier::create );
    operator[]( Identifier::uri ) = uri.toString();
}

DocumentUri CreateFile::uri( void ) const
{
    return DocumentUri::fromString( operator[]( Identifier::uri ).get< std::string >() );
}

std::string CreateFile::kind( void ) const
{
    return operator[]( Identifier::kind ).get< std::string >();
}

u1 CreateFile::hasOptions( void ) const
{
    return find( Identifier::options ) != end();
}

void CreateFile::setOptions( const CreateFileOptions& options )
{
    operator[]( Identifier::options ) = Data::from_cbor( Data::to_cbor( options ) );
}

CreateFileOptions CreateFile::options( void ) const
{
    return at( Identifier::options );
}

void CreateFile::validate( const Data& data )
{
    static const auto context = CONTENT + " CreateFile:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::kind, true );
    Json::validatePropertyIsString( context, data, Identifier::uri, true );
    Json::validatePropertyIs< CreateFileOptions >( context, data, Identifier::options, false );
}

//
//
// RenameFile
//

RenameFile::RenameFile( const Data& data )
: Data( data )
{
    validate( data );
}

RenameFile::RenameFile( const DocumentUri& oldUri, const DocumentUri& newUri )
: Data( Data::object() )
{
    operator[]( Identifier::kind ) = std::string( Identifier::rename );
    operator[]( Identifier::oldUri ) = oldUri.toString();
    operator[]( Identifier::newUri ) = newUri.toString();
}

DocumentUri RenameFile::oldUri( void ) const
{
    return DocumentUri::fromString( operator[]( Identifier::oldUri ).get< std::string >() );
}

DocumentUri RenameFile::newUri( void ) const
{
    return DocumentUri::fromString( operator[]( Identifier::newUri ).get< std::string >() );
}

std::string RenameFile::kind( void ) const
{
    return operator[]( Identifier::kind ).get< std::string >();
}

u1 RenameFile::hasOptions( void ) const
{
    return find( Identifier::options ) != end();
}

void RenameFile::setOptions( const RenameFileOptions& options )
{
    operator[]( Identifier::options ) = Data::from_cbor( Data::to_cbor( options ) );
}

RenameFileOptions RenameFile::options( void ) const
{
    return at( Identifier::options );
}

void RenameFile::validate( const Data& data )
{
    static const auto context = CONTENT + " RenameFile:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::kind, true );
    Json::validatePropertyIsString( context, data, Identifier::newUri, true );
    Json::validatePropertyIsString( context, data, Identifier::oldUri, true );
    Json::validatePropertyIs< CreateFileOptions >( context, data, Identifier::options, false );
}

//
//
// DeleteFileOptions
//

DeleteFileOptions::DeleteFileOptions( const Data& data )
: Data( data )
{
    validate( data );
}

u1 DeleteFileOptions::hasRecursive( void ) const
{
    return find( Identifier::recursive ) != end();
}

u1 DeleteFileOptions::recursive( void ) const
{
    return at( Identifier::recursive ).get< u1 >();
}

void DeleteFileOptions::setRecursive( const u1 recursive )
{
    operator[]( Identifier::recursive ) = recursive;
}

u1 DeleteFileOptions::hasIgnoreIfExists( void ) const
{
    return find( Identifier::ignoreIfExists ) != end();
}

u1 DeleteFileOptions::ignoreIfExists( void ) const
{
    return at( Identifier::ignoreIfExists );
}

void DeleteFileOptions::setIgnoreIfExists( const u1 ignoreIfExists )
{
    operator[]( Identifier::ignoreIfExists ) = ignoreIfExists;
}

void DeleteFileOptions::validate( const Data& data )
{
    static const auto context = CONTENT + " DeleteFileOptions:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsBoolean( context, data, Identifier::recursive, false );
    Json::validatePropertyIsBoolean( context, data, Identifier::ignoreIfExists, false );
}

//
//
//  DeleteFile
//

DeleteFile::DeleteFile( const Data& data )
: Data( data )
{
    validate( data );
}

DeleteFile::DeleteFile( const DocumentUri& uri )
: Data( Data::object() )
{
    operator[]( Identifier::kind ) = Identifier::DELETE;
    operator[]( Identifier::uri ) = uri.toString();
}

DocumentUri DeleteFile::uri( void ) const
{
    return DocumentUri::fromString( operator[]( Identifier::uri ).get< std::string >() );
}

std::string DeleteFile::kind( void ) const
{
    return operator[]( Identifier::kind ).get< std::string >();
}

u1 DeleteFile::hasOptions( void ) const
{
    return find( Identifier::options ) != end();
}

void DeleteFile::setOptions( const DeleteFileOptions& options )
{
    operator[]( Identifier::options ) = Data::from_cbor( Data::to_cbor( options ) );
}

DeleteFileOptions DeleteFile::options( void ) const
{
    return at( Identifier::options );
}
void DeleteFile::validate( const Data& data )
{
    static const auto context = CONTENT + " DeleteFile:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::kind, true );
    Json::validatePropertyIsString( context, data, Identifier::uri, true );
    Json::validatePropertyIs< DeleteFileOptions >( context, data, Identifier::options, false );
}

//
//
// TextDocumentItem
//

TextDocumentItem::TextDocumentItem( const Data& data )
: Data( data )
{
    validate( data );
}

TextDocumentItem::TextDocumentItem(
    const DocumentUri& uri,
    const std::string& languageId,
    const std::size_t version,
    const std::string& text )
: Data( Data::object() )
{
    operator[]( Identifier::uri ) = uri.toString();
    operator[]( Identifier::languageId ) = languageId;
    operator[]( Identifier::version ) = version;
    operator[]( Identifier::text ) = text;
}

DocumentUri TextDocumentItem::uri( void ) const
{
    return DocumentUri::fromString( at( Identifier::uri ).get< std::string >() );
}

std::string TextDocumentItem::languageId( void ) const
{
    return at( Identifier::languageId ).get< std::string >();
}

std::size_t TextDocumentItem::version( void ) const
{
    return at( Identifier::version ).get< std::size_t >();
}

std::string TextDocumentItem::text( void ) const
{
    return at( Identifier::text ).get< std::string >();
}

void TextDocumentItem::validate( const Data& data )
{
    static const auto context = CONTENT + " TextDocumentItem:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsUri( context, data, Identifier::uri, true );
    Json::validatePropertyIsString( context, data, Identifier::languageId, true );
    Json::validatePropertyIsNumber( context, data, Identifier::version, true );
    Json::validatePropertyIsString( context, data, Identifier::text, true );
}

//
//
// TextDocumentPositionParams
//

TextDocumentPositionParams::TextDocumentPositionParams( const Data& data )
: Data( data )
{
    validate( data );
}

TextDocumentPositionParams::TextDocumentPositionParams(
    const TextDocumentIdentifier& textDocument, const Position& position )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument ) = Data::from_cbor( Data::to_cbor( textDocument ) );
    operator[]( Identifier::position ) = Data::from_cbor( Data::to_cbor( position ) );
}

TextDocumentIdentifier TextDocumentPositionParams::textDocument( void ) const
{
    return at( Identifier::textDocument );
}

Position TextDocumentPositionParams::position( void ) const
{
    return at( Identifier::position );
}

void TextDocumentPositionParams::validate( const Data& data )
{
    static const auto context = CONTENT + " TextDocumentPositionParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< TextDocumentIdentifier >(
        context, data, Identifier::textDocument, true );
    Json::validatePropertyIs< Position >( context, data, Identifier::position, true );
}

//
//
// DocumentFilter
//

DocumentFilter::DocumentFilter( const Data& data )
: Data( data )
{
    validate( data );
}

DocumentFilter::DocumentFilter( void )
: Data( Data::object() )
{
}

u1 DocumentFilter::hasLanguage( void ) const
{
    return find( Identifier::language ) != end();
}

std::string DocumentFilter::language( void ) const
{
    return at( Identifier::language ).get< std::string >();
}

void DocumentFilter::setLanguage( const std::string& language )
{
    operator[]( Identifier::language ) = language;
}

u1 DocumentFilter::hasScheme( void ) const
{
    return find( Identifier::scheme ) != end();
}

std::string DocumentFilter::scheme( void ) const
{
    return at( Identifier::scheme ).get< std::string >();
}

void DocumentFilter::setScheme( const std::string& scheme )
{
    operator[]( Identifier::scheme ) = scheme;
}

u1 DocumentFilter::hasPattern( void ) const
{
    return find( Identifier::pattern ) != end();
}

std::string DocumentFilter::pattern( void ) const
{
    return at( Identifier::pattern ).get< std::string >();
}

void DocumentFilter::setPattern( const std::string& pattern )
{
    operator[]( Identifier::pattern ) = pattern;
}

void DocumentFilter::validate( const Data& data )
{
    static const auto context = CONTENT + " DocumentFilter:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::language, false );
    Json::validatePropertyIsString( context, data, Identifier::scheme, false );
    Json::validatePropertyIsString( context, data, Identifier::pattern, false );
}

//
//
// DocumentSelector
//

DocumentSelector::DocumentSelector( const Data& data )
: Data( data )
{
    validate( data );
}

DocumentSelector::DocumentSelector( const std::vector< DocumentFilter >& documentFilters )
: Data( Data::array() )
{
    for( auto documentFilter : documentFilters )
    {
        this->push_back( documentFilter );
    }
}

void DocumentSelector::validate( const Data& data )
{
    static const auto context = CONTENT + " DocumentSelector:";
    Json::validateTypeIsArrayOf< DocumentFilter >( context, data );
}

//
//
// DynamicRegistration
//

DynamicRegistration::DynamicRegistration( const Data& data )
: Data( data )
{
    validate( data );
}

DynamicRegistration::DynamicRegistration( void )
: Data( Data::object() )
{
}

u1 DynamicRegistration::hasDynamicRegistration( void ) const
{
    return find( Identifier::dynamicRegistration ) != end();
}

u1 DynamicRegistration::dynamicRegistration( void ) const
{
    return at( Identifier::dynamicRegistration ).get< u1 >();
}

void DynamicRegistration::setDynamicRegistration( const u1 dynamicRegistration )
{
    operator[]( Identifier::dynamicRegistration ) = dynamicRegistration;
}

void DynamicRegistration::validate( const Data& data )
{
    static const auto context = CONTENT + " DynamicRegistration:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsBoolean( context, data, Identifier::dynamicRegistration, false );
}

//
//
// WorkspaceClientCapabilities
//

WorkspaceClientCapabilities::WorkspaceClientCapabilities( const Data& data )
: Data( data )
{
    validate( data );
}

WorkspaceClientCapabilities::WorkspaceClientCapabilities( void )
: Data( Data::object() )
{
}

u1 WorkspaceClientCapabilities::hasApplyEdit( void ) const
{
    return find( Identifier::applyEdit ) != end();
}

u1 WorkspaceClientCapabilities::applyEdit( void ) const
{
    return at( Identifier::applyEdit ).get< u1 >();
}

void WorkspaceClientCapabilities::setApplyEdit( const u1 applyEdit )
{
    operator[]( Identifier::applyEdit ) = applyEdit;
}

u1 WorkspaceClientCapabilities::hasWorkspaceEdit( void ) const
{
    return find( Identifier::workspaceEdit ) != end();
}

WorkspaceEdit WorkspaceClientCapabilities::workspaceEdit( void ) const
{
    return at( Identifier::workspaceEdit );
}

void WorkspaceClientCapabilities::setWorkspaceEdit( const WorkspaceEdit& workspaceEdit )
{
    operator[]( Identifier::workspaceEdit ) = Data::from_cbor( Data::to_cbor( workspaceEdit ) );
}

u1 WorkspaceClientCapabilities::hasDidChangeConfiguration( void ) const
{
    return find( Identifier::didChangeConfiguration ) != end();
}

DynamicRegistration WorkspaceClientCapabilities::didChangeConfiguration( void ) const
{
    return at( Identifier::didChangeConfiguration );
}

void WorkspaceClientCapabilities::setDidChangeConfiguration(
    const DynamicRegistration& didChangeConfiguration )
{
    operator[]( Identifier::didChangeConfiguration ) =
        Data::from_cbor( Data::to_cbor( didChangeConfiguration ) );
}

u1 WorkspaceClientCapabilities::hasDidChangeWatchedFiles( void ) const
{
    return find( Identifier::didChangeWatchedFiles ) != end();
}

DynamicRegistration WorkspaceClientCapabilities::didChangeWatchedFiles( void ) const
{
    return at( Identifier::didChangeWatchedFiles );
}

void WorkspaceClientCapabilities::didChangeWatchedFiles(
    const DynamicRegistration& didChangeWatchedFiles )
{
    operator[]( Identifier::didChangeWatchedFiles ) =
        Data::from_cbor( Data::to_cbor( didChangeWatchedFiles ) );
}

u1 WorkspaceClientCapabilities::hasSymbol( void ) const
{
    return find( Identifier::symbol ) != end();
}

DynamicRegistration WorkspaceClientCapabilities::symbol( void ) const
{
    return at( Identifier::symbol );
}

void WorkspaceClientCapabilities::setSymbol( const DynamicRegistration& symbol )
{
    operator[]( Identifier::symbol ) = Data::from_cbor( Data::to_cbor( symbol ) );
}

u1 WorkspaceClientCapabilities::hasExecuteCommand( void ) const
{
    return find( Identifier::executeCommand ) != end();
}

DynamicRegistration WorkspaceClientCapabilities::executeCommand( void ) const
{
    return at( Identifier::executeCommand );
}

void WorkspaceClientCapabilities::executeCommand( const DynamicRegistration& executeCommand )
{
    operator[]( Identifier::executeCommand ) = Data::from_cbor( Data::to_cbor( executeCommand ) );
}
u1 WorkspaceClientCapabilities::hasWorkspaceFolders( void ) const
{
    return find( Identifier::workspaceFolders ) != end();
}

void WorkspaceClientCapabilities::setWorkspaceFolders( const u1 workspaceFolders )
{
    operator[]( Identifier::workspaceFolders ) = workspaceFolders;
}

u1 WorkspaceClientCapabilities::workspaceFolders( void ) const
{
    return at( Identifier::workspaceFolders ).get< u1 >();
}

u1 WorkspaceClientCapabilities::hasConfiguration( void ) const
{
    return find( Identifier::configuration ) != end();
}

void WorkspaceClientCapabilities::setConfiguration( const u1 configuration )
{
    operator[]( Identifier::configuration ) = configuration;
}

u1 WorkspaceClientCapabilities::configuration( void ) const
{
    return at( Identifier::configuration ).get< u1 >();
}

void WorkspaceClientCapabilities::validate( const Data& data )
{
    static const auto context = CONTENT + " WorkspaceClientCapabilities:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsBoolean( context, data, Identifier::applyEdit, false );
    Json::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::workspaceEdit, false );
    Json::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::didChangeConfiguration, false );
    Json::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::didChangeWatchedFiles, false );
    Json::validatePropertyIs< DynamicRegistration >( context, data, Identifier::symbol, false );
    Json::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::executeCommand, false );
    Json::validatePropertyIsBoolean( context, data, Identifier::configuration, false );
    Json::validatePropertyIsBoolean( context, data, Identifier::workspaceFolders, false );
}

//
//
// Synchronization
//

Synchronization::Synchronization( const Data& data )
: Data( data )
{
    validate( data );
}

Synchronization::Synchronization( void )
: Data( Data::object() )
{
}

u1 Synchronization::hasDynamicRegistration( void ) const
{
    return find( Identifier::dynamicRegistration ) != end();
}

u1 Synchronization::dynamicRegistration( void ) const
{
    return at( Identifier::dynamicRegistration ).get< u1 >();
}

void Synchronization::setDynamicRegistration( const u1 dynamicRegistration )
{
    operator[]( Identifier::dynamicRegistration ) = dynamicRegistration;
}

u1 Synchronization::hasWillSave( void ) const
{
    return find( Identifier::willSave ) != end();
}

u1 Synchronization::willSave( void ) const
{
    return at( Identifier::willSave ).get< u1 >();
}

void Synchronization::setWillSave( const u1 willSave )
{
    operator[]( Identifier::willSave ) = willSave;
}

u1 Synchronization::hasWillSaveWaitUntil( void ) const
{
    return find( Identifier::willSaveWaitUntil ) != end();
}

u1 Synchronization::willSaveWaitUntil( void ) const
{
    return at( Identifier::willSaveWaitUntil ).get< u1 >();
}

void Synchronization::setWillSaveWaitUntil( const u1 willSaveWaitUntil )
{
    operator[]( Identifier::willSaveWaitUntil ) = willSaveWaitUntil;
}

u1 Synchronization::hasDidSave( void ) const
{
    return find( Identifier::didSave ) != end();
}

u1 Synchronization::didSave( void ) const
{
    return at( Identifier::didSave ).get< u1 >();
}

void Synchronization::setDidSave( const u1 didSave )
{
    operator[]( Identifier::didSave ) = didSave;
}

void Synchronization::validate( const Data& data )
{
    static const auto context = CONTENT + " Synchronization:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsBoolean( context, data, Identifier::dynamicRegistration, false );
    Json::validatePropertyIsBoolean( context, data, Identifier::willSave, false );
    Json::validatePropertyIsBoolean( context, data, Identifier::willSaveWaitUntil, false );
    Json::validatePropertyIsBoolean( context, data, Identifier::didSave, false );
}

//
//
// CompletionItem
//

TextDocumentClientCapabilities::CompletionItem::CompletionItem( const Data& data )
: Data( data )
{
    validate( data );
}

TextDocumentClientCapabilities::CompletionItem::CompletionItem( void )
: Data( Data::object() )
{
}

u1 TextDocumentClientCapabilities::CompletionItem::hasSnippetSupport( void ) const
{
    return find( Identifier::snippetSupport ) != end();
}

u1 TextDocumentClientCapabilities::CompletionItem::snippetSupport( void ) const
{
    return at( Identifier::snippetSupport ).get< u1 >();
}

void TextDocumentClientCapabilities::CompletionItem::setSnippetSupport( const u1 snippetSupport )
{
    operator[]( Identifier::snippetSupport ) = snippetSupport;
}

void TextDocumentClientCapabilities::CompletionItem::validate( const Data& data )
{
    static const auto context = CONTENT + " CompletionItem:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsBoolean( context, data, Identifier::snippetSupport, false );
}

//
//
// Completion
//

TextDocumentClientCapabilities::Completion::Completion( const Data& data )
: Data( data )
{
    validate( data );
}

TextDocumentClientCapabilities::Completion::Completion( void )
: Data( Data::object() )
{
}

u1 TextDocumentClientCapabilities::Completion::hasDynamicRegistration( void ) const
{
    return find( Identifier::dynamicRegistration ) != end();
}

u1 TextDocumentClientCapabilities::Completion::dynamicRegistration( void ) const
{
    return at( Identifier::dynamicRegistration ).get< u1 >();
}

void TextDocumentClientCapabilities::Completion::setDynamicRegistration(
    const u1 dynamicRegistration )
{
    operator[]( Identifier::dynamicRegistration ) = dynamicRegistration;
}

u1 TextDocumentClientCapabilities::Completion::hasCompletionItem( void ) const
{
    return find( Identifier::completionItem ) != end();
}

TextDocumentClientCapabilities::CompletionItem
TextDocumentClientCapabilities::Completion::completionItem( void ) const
{
    return at( Identifier::completionItem );
}

void TextDocumentClientCapabilities::Completion::completionItem(
    const TextDocumentClientCapabilities::CompletionItem& completionItem )
{
    operator[]( Identifier::completionItem ) = Data::from_cbor( Data::to_cbor( completionItem ) );
}

void TextDocumentClientCapabilities::Completion::validate( const Data& data )
{
    static const auto context = CONTENT + " Completion:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsBoolean( context, data, Identifier::dynamicRegistration, false );
    Json::validatePropertyIs< TextDocumentClientCapabilities::CompletionItem >(
        context, data, Identifier::completionItem, false );
}

//
//
// TextDocumentClientCapabilities
//

TextDocumentClientCapabilities::TextDocumentClientCapabilities( const Data& data )
: Data( data )
{
    validate( data );
}

TextDocumentClientCapabilities::TextDocumentClientCapabilities( void )
: Data( Data::object() )
{
}

u1 TextDocumentClientCapabilities::hasSynchronization( void ) const
{
    return find( Identifier::synchronization ) != end();
}

Synchronization TextDocumentClientCapabilities::synchronization( void ) const
{
    return at( Identifier::synchronization );
}

void TextDocumentClientCapabilities::setSynchronization( const Synchronization& synchronization )
{
    operator[]( Identifier::synchronization ) = Data::from_cbor( Data::to_cbor( synchronization ) );
}

u1 TextDocumentClientCapabilities::hasCompletion( void ) const
{
    return find( Identifier::completion ) != end();
}

TextDocumentClientCapabilities::Completion TextDocumentClientCapabilities::completion( void ) const
{
    return at( Identifier::completion );
}

void TextDocumentClientCapabilities::setCompletion( const Completion& completion )
{
    operator[]( Identifier::completion ) = Data::from_cbor( Data::to_cbor( completion ) );
}

u1 TextDocumentClientCapabilities::hasHover( void ) const
{
    return find( Identifier::hover ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::hover( void ) const
{
    return at( Identifier::hover );
}

void TextDocumentClientCapabilities::setHover( const DynamicRegistration& hover )
{
    operator[]( Identifier::hover ) = Data::from_cbor( Data::to_cbor( hover ) );
}

u1 TextDocumentClientCapabilities::hasSignatureHelp( void ) const
{
    return find( Identifier::signatureHelp ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::signatureHelp( void ) const
{
    return at( Identifier::signatureHelp );
}

void TextDocumentClientCapabilities::setSignatureHelp( const DynamicRegistration& signatureHelp )
{
    operator[]( Identifier::signatureHelp ) = Data::from_cbor( Data::to_cbor( signatureHelp ) );
}

u1 TextDocumentClientCapabilities::hasReferences( void ) const
{
    return find( Identifier::references ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::references( void ) const
{
    return at( Identifier::references );
}

void TextDocumentClientCapabilities::setReferences( const DynamicRegistration& references )
{
    operator[]( Identifier::references ) = Data::from_cbor( Data::to_cbor( references ) );
}

u1 TextDocumentClientCapabilities::hasDocumentHighlight( void ) const
{
    return find( Identifier::documentHighlight ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::documentHighlight( void ) const
{
    return at( Identifier::documentHighlight );
}

void TextDocumentClientCapabilities::setDocumentHighlight(
    const DynamicRegistration& documentHighlight )
{
    operator[]( Identifier::documentHighlight ) =
        Data::from_cbor( Data::to_cbor( documentHighlight ) );
}

u1 TextDocumentClientCapabilities::hasDocumentSymbol( void ) const
{
    return find( Identifier::documentSymbol ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::documentSymbol( void ) const
{
    return at( Identifier::documentSymbol );
}

void TextDocumentClientCapabilities::setDocumentSymbol( const DynamicRegistration& documentSymbol )
{
    operator[]( Identifier::documentSymbol ) = Data::from_cbor( Data::to_cbor( documentSymbol ) );
}

u1 TextDocumentClientCapabilities::hasFormatting( void ) const
{
    return find( Identifier::formatting ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::formatting( void ) const
{
    return at( Identifier::formatting );
}

void TextDocumentClientCapabilities::setFormatting( const DynamicRegistration& formatting )
{
    operator[]( Identifier::formatting ) = Data::from_cbor( Data::to_cbor( formatting ) );
}

u1 TextDocumentClientCapabilities::hasRangeFormatting( void ) const
{
    return find( Identifier::rangeFormatting ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::rangeFormatting( void ) const
{
    return at( Identifier::rangeFormatting );
}

void TextDocumentClientCapabilities::setRangeFormatting(
    const DynamicRegistration& rangeFormatting )
{
    operator[]( Identifier::rangeFormatting ) = Data::from_cbor( Data::to_cbor( rangeFormatting ) );
}

u1 TextDocumentClientCapabilities::hasOnTypeFormatting( void ) const
{
    return find( Identifier::onTypeFormatting ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::onTypeFormatting( void ) const
{
    return at( Identifier::onTypeFormatting );
}

void TextDocumentClientCapabilities::setOnTypeFormatting(
    const DynamicRegistration& onTypeFormatting )
{
    operator[]( Identifier::onTypeFormatting ) =
        Data::from_cbor( Data::to_cbor( onTypeFormatting ) );
}

u1 TextDocumentClientCapabilities::hasDefinition( void ) const
{
    return find( Identifier::definition ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::definition( void ) const
{
    return at( Identifier::definition );
}

void TextDocumentClientCapabilities::setDefinition( const DynamicRegistration& definition )
{
    operator[]( Identifier::definition ) = Data::from_cbor( Data::to_cbor( definition ) );
}

u1 TextDocumentClientCapabilities::hasCodeAction( void ) const
{
    return find( Identifier::codeAction ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::codeAction( void ) const
{
    return at( Identifier::codeAction );
}

void TextDocumentClientCapabilities::setCodeAction( const DynamicRegistration& codeAction )
{
    operator[]( Identifier::codeAction ) = Data::from_cbor( Data::to_cbor( codeAction ) );
}

u1 TextDocumentClientCapabilities::hasCodeLens( void ) const
{
    return find( Identifier::codeLens ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::codeLens( void ) const
{
    return at( Identifier::codeLens );
}

void TextDocumentClientCapabilities::setCodeLens( const DynamicRegistration& codeLens )
{
    operator[]( Identifier::codeLens ) = Data::from_cbor( Data::to_cbor( codeLens ) );
}

u1 TextDocumentClientCapabilities::hasDocumentLink( void ) const
{
    return find( Identifier::documentLink ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::documentLink( void ) const
{
    return at( Identifier::documentLink );
}

void TextDocumentClientCapabilities::setDocumentLink( const DynamicRegistration& documentLink )
{
    operator[]( Identifier::documentLink ) = Data::from_cbor( Data::to_cbor( documentLink ) );
}

u1 TextDocumentClientCapabilities::hasRename( void ) const
{
    return find( Identifier::rename ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::rename( void ) const
{
    return at( Identifier::rename );
}

void TextDocumentClientCapabilities::setRename( const DynamicRegistration& rename )
{
    operator[]( Identifier::rename ) = Data::from_cbor( Data::to_cbor( rename ) );
}

void TextDocumentClientCapabilities::validate( const Data& data )
{
    static const auto context = CONTENT + " TextDocumentClientCapabilities:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< Synchronization >(
        context, data, Identifier::synchronization, false );
    Json::validatePropertyIs< Completion >( context, data, Identifier::completion, false );
    Json::validatePropertyIs< DynamicRegistration >( context, data, Identifier::hover, false );
    Json::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::signatureHelp, false );
    Json::validatePropertyIs< DynamicRegistration >( context, data, Identifier::references, false );
    Json::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::documentHighlight, false );
    Json::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::documentSymbol, false );
    Json::validatePropertyIs< DynamicRegistration >( context, data, Identifier::formatting, false );
    Json::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::rangeFormatting, false );
    Json::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::onTypeFormatting, false );
    Json::validatePropertyIs< DynamicRegistration >( context, data, Identifier::definition, false );
    Json::validatePropertyIs< DynamicRegistration >( context, data, Identifier::codeAction, false );
    Json::validatePropertyIs< DynamicRegistration >( context, data, Identifier::codeLens, false );
    Json::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::documentLink, false );
    Json::validatePropertyIs< DynamicRegistration >( context, data, Identifier::rename, false );
}

//
//
// ClientCapabilities
//

ClientCapabilities::ClientCapabilities( const Data& data )
: Data( data )
{
    validate( data );
}

ClientCapabilities::ClientCapabilities( void )
: Data( Data::object() )
{
}

u1 ClientCapabilities::hasWorkspace( void ) const
{
    return find( Identifier::workspace ) != end();
}

WorkspaceClientCapabilities ClientCapabilities::workspace( void ) const
{
    return at( Identifier::workspace );
}

void ClientCapabilities::setWorkspace( const WorkspaceClientCapabilities& workspace )
{
    operator[]( Identifier::workspace ) = Data::from_cbor( Data::to_cbor( workspace ) );
}

u1 ClientCapabilities::hasTextDocument( void ) const
{
    return find( Identifier::textDocument ) != end();
}

TextDocumentClientCapabilities ClientCapabilities::textDocument( void ) const
{
    return at( Identifier::textDocument );
}

void ClientCapabilities::setTextDocument( const TextDocumentClientCapabilities& textDocument )
{
    operator[]( Identifier::textDocument ) = Data::from_cbor( Data::to_cbor( textDocument ) );
}

u1 ClientCapabilities::hasExperimental( void ) const
{
    return find( Identifier::experimental ) != end();
}

Data ClientCapabilities::experimental( void ) const
{
    return at( Identifier::experimental );
}

void ClientCapabilities::setExperimental( const Data& experimental )
{
    operator[]( Identifier::experimental ) = Data::from_cbor( Data::to_cbor( experimental ) );
}

void ClientCapabilities::validate( const Data& data )
{
    static const auto context = CONTENT + " ClientCapabilities:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< WorkspaceClientCapabilities >(
        context, data, Identifier::workspace, false );
    Json::validatePropertyIs< TextDocumentClientCapabilities >(
        context, data, Identifier::textDocument, false );
    Json::validatePropertyIsObject( context, data, Identifier::experimental, false );
}

//
//
// SaveOptions
//

SaveOptions::SaveOptions( const Data& data )
: Data( data )
{
    validate( data );
}

SaveOptions::SaveOptions( void )
: Data( Data::object() )
{
}

u1 SaveOptions::hasIncludeText( void ) const
{
    return find( Identifier::includeText ) != end();
}

u1 SaveOptions::includeText( void ) const
{
    return at( Identifier::includeText ).get< u1 >();
}

void SaveOptions::setIncludeText( const u1 includeText )
{
    operator[]( Identifier::includeText ) = includeText;
}

void SaveOptions::validate( const Data& data )
{
    static const auto context = CONTENT + " SaveOptions:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsBoolean( context, data, Identifier::includeText, false );
}

//
//
// TextDocumentSyncOptions
//

TextDocumentSyncOptions::TextDocumentSyncOptions( const Data& data )
: Data( data )
{
    validate( data );
}

TextDocumentSyncOptions::TextDocumentSyncOptions( void )
: Data( Data::object() )
{
}

u1 TextDocumentSyncOptions::hasOpenClose( void ) const
{
    return find( Identifier::openClose ) != end();
}

u1 TextDocumentSyncOptions::openClose( void ) const
{
    return at( Identifier::openClose ).get< u1 >();
}

void TextDocumentSyncOptions::setOpenClose( const u1 openClose )
{
    operator[]( Identifier::openClose ) = openClose;
}

u1 TextDocumentSyncOptions::hasChange( void ) const
{
    return find( Identifier::change ) != end();
}

TextDocumentSyncKind TextDocumentSyncOptions::change( void ) const
{
    assert( hasChange() );
    return static_cast< TextDocumentSyncKind >( at( Identifier::change ).get< std::size_t >() );
}

void TextDocumentSyncOptions::setChange( const TextDocumentSyncKind& change )
{
    operator[]( Identifier::change ) = static_cast< std::size_t >( change );
}

u1 TextDocumentSyncOptions::hasWillSave( void ) const
{
    return find( Identifier::willSave ) != end();
}

u1 TextDocumentSyncOptions::willSave( void ) const
{
    return at( Identifier::willSave ).get< u1 >();
}

void TextDocumentSyncOptions::setWillSave( const u1 willSave )
{
    operator[]( Identifier::willSave ) = willSave;
}

u1 TextDocumentSyncOptions::hasWillSaveWaitUntil( void ) const
{
    return find( Identifier::willSaveWaitUntil ) != end();
}

u1 TextDocumentSyncOptions::willSaveWaitUntil( void ) const
{
    return at( Identifier::willSaveWaitUntil ).get< u1 >();
}

void TextDocumentSyncOptions::setWillSaveWaitUntil( const u1 willSaveWaitUntil )
{
    operator[]( Identifier::willSaveWaitUntil ) = willSaveWaitUntil;
}

u1 TextDocumentSyncOptions::hasSave( void ) const
{
    return find( Identifier::save ) != end();
}

SaveOptions TextDocumentSyncOptions::save( void ) const
{
    return at( Identifier::save );
}

void TextDocumentSyncOptions::setSave( const SaveOptions& save )
{
    operator[]( Identifier::save ) = Data::from_cbor( Data::to_cbor( save ) );
}

void TextDocumentSyncOptions::validate( const Data& data )
{
    static const auto context = CONTENT + " TextDocumentSyncOptions:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsBoolean( context, data, Identifier::openClose, false );
    Json::validatePropertyIsNumber( context, data, Identifier::change, false );
    Json::validatePropertyIsBoolean( context, data, Identifier::willSave, false );
    Json::validatePropertyIsBoolean( context, data, Identifier::willSaveWaitUntil, false );
    Json::validatePropertyIs< SaveOptions >( context, data, Identifier::save, false );
}

//
//
// CompletionOptions
//

CompletionOptions::CompletionOptions( const Data& data )
: Data( data )
{
    validate( data );
}

CompletionOptions::CompletionOptions( void )
: Data( Data::object() )
{
    operator[]( Identifier::triggerCharacters ) = Data::array();
}

u1 CompletionOptions::hasResolveProvider( void ) const
{
    return find( Identifier::resolveProvider ) != end();
}

u1 CompletionOptions::resolveProvider( void ) const
{
    return at( Identifier::resolveProvider ).get< u1 >();
}

void CompletionOptions::setResolveProvider( const u1 resolveProvider )
{
    operator[]( Identifier::resolveProvider ) = resolveProvider;
}

u1 CompletionOptions::hasTriggerCharacters( void ) const
{
    return find( Identifier::triggerCharacters ) != end();
}

Data CompletionOptions::triggerCharacters( void ) const
{
    return at( Identifier::triggerCharacters );
}

void CompletionOptions::addTriggerCharacters( const std::string& triggerCharacter )
{
    operator[]( Identifier::triggerCharacters ).push_back( triggerCharacter );
}

void CompletionOptions::validate( const Data& data )
{
    static const auto context = CONTENT + " CompletionOptions:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsBoolean( context, data, Identifier::resolveProvider, false );
    Json::validatePropertyIsArrayOfString( context, data, Identifier::triggerCharacters, false );
}

//
//
// SignatureHelpOptions
//

SignatureHelpOptions::SignatureHelpOptions( const Data& data )
: Data( data )
{
    validate( data );
}

SignatureHelpOptions::SignatureHelpOptions( void )
: Data( Data::object() )
{
    operator[]( Identifier::moreTriggerCharacter ) = Data::array();
}

u1 SignatureHelpOptions::hasTriggerCharacters( void ) const
{
    return find( Identifier::triggerCharacters ) != end();
}

Data SignatureHelpOptions::triggerCharacters( void ) const
{
    return at( Identifier::triggerCharacters );
}

void SignatureHelpOptions::addTriggerCharacters( const std::string& triggerCharacter )
{
    operator[]( Identifier::triggerCharacters ).push_back( triggerCharacter );
}

void SignatureHelpOptions::validate( const Data& data )
{
    static const auto context = CONTENT + " SignatureHelpOptions:";
    Json::validateTypeIsObject( context, data );
    // TODO: FIXME: @ppaulweber
}

//
//
// CodeLensOptions
//

CodeLensOptions::CodeLensOptions( const Data& data )
: Data( data )
{
    validate( data );
}

CodeLensOptions::CodeLensOptions( void )
: Data( Data::object() )
{
}

u1 CodeLensOptions::hasResolveProvider( void ) const
{
    return find( Identifier::resolveProvider ) != end();
}

u1 CodeLensOptions::resolveProvider( void ) const
{
    return at( Identifier::resolveProvider ).get< u1 >();
}

void CodeLensOptions::setResolveProvider( const u1 resolveProvider )
{
    operator[]( Identifier::resolveProvider ) = resolveProvider;
}

void CodeLensOptions::validate( const Data& data )
{
    static const auto context = CONTENT + " SignatureHelpOptions:";
    Json::validateTypeIsObject( context, data );
    // TODO: FIXME: @ppaulweber
}

//
//
//
//

DocumentOnTypeFormattingOptions::DocumentOnTypeFormattingOptions( const Data& data )
: Data( data )
{
    validate( data );
}

DocumentOnTypeFormattingOptions::DocumentOnTypeFormattingOptions(
    const std::string& firstTriggerCharacter )
: Data( Data::object() )
{
    operator[]( Identifier::firstTriggerCharacter ) = firstTriggerCharacter;
    operator[]( Identifier::moreTriggerCharacter ) = Data::array();
}

std::string DocumentOnTypeFormattingOptions::firstTriggerCharacter( void ) const
{
    return at( Identifier::firstTriggerCharacter ).get< std::string >();
}

u1 DocumentOnTypeFormattingOptions::hasMoreTriggerCharacter( void ) const
{
    return find( Identifier::moreTriggerCharacter ) != end();
}

Data DocumentOnTypeFormattingOptions::moreTriggerCharacter( void ) const
{
    return at( Identifier::moreTriggerCharacter );
}

void DocumentOnTypeFormattingOptions::addMoreTriggerCharacter( const Data& moreTriggerCharacter )
{
    operator[]( Identifier::moreTriggerCharacter ).push_back( moreTriggerCharacter );
}

void DocumentOnTypeFormattingOptions::validate( const Data& data )
{
    static const auto context = CONTENT + " SignatureHelpOptions:";
    Json::validateTypeIsObject( context, data );
    // TODO: FIXME: @ppaulweber
}

//
//
// RenameOptions
//

RenameOptions::RenameOptions( const Data& data )
: Data( data )
{
    validate( data );
}

u1 RenameOptions::hasPrepareProvider( void ) const
{
    return find( Identifier::prepareProvider ) != end();
}

u1 RenameOptions::prepareProvider( void ) const
{
    return at( Identifier::prepareProvider );
}

void RenameOptions::setPrepareProvider( const u1 prepareProvider )
{
    operator[]( Identifier::prepareProvider ) = prepareProvider;
}

void RenameOptions::validate( const Data& data )
{
    static const auto context = CONTENT + " RenameOptions:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsBoolean( context, data, Identifier::prepareProvider, false );
}

//
//
// ExecuteCommandOptions
//

ExecuteCommandOptions::ExecuteCommandOptions( const Data& data )
: Data( data )
{
    validate( data );
}

ExecuteCommandOptions::ExecuteCommandOptions( void )
: Data( Data::object() )
{
    operator[]( Identifier::commands ) = Data::array();
}

u1 ExecuteCommandOptions::hasCommands( void ) const
{
    return find( Identifier::commands ) != end();
}

Data ExecuteCommandOptions::commands( void ) const
{
    return at( Identifier::commands );
}

void ExecuteCommandOptions::addCommand( const std::string& command )
{
    operator[]( Identifier::commands ).push_back( command );
}

void ExecuteCommandOptions::validate( const Data& data )
{
    static const auto context = CONTENT + " SignatureHelpOptions:";
    Json::validateTypeIsObject( context, data );
    // TODO: FIXME: @ppaulweber
}

//
//
// StaticRegistrationOptions
//

StaticRegistrationOptions::StaticRegistrationOptions( const Data& data )
: Data( data )
{
    validate( data );
}

u1 StaticRegistrationOptions::hasId( void ) const
{
    return find( Identifier::id ) != end();
}

std::string StaticRegistrationOptions::id( void ) const
{
    return at( Identifier::id ).get< std::string >();
}

void StaticRegistrationOptions::setId( std::string id )
{
    operator[]( Identifier::id ) = id;
}

void StaticRegistrationOptions::validate( const Data& data )
{
    static const auto context = CONTENT + " StaticRegistrationOptions:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::id, false );
}

//
//
// TypeDefinitionProvider
//

TypeDefinitionProvider::TypeDefinitionProvider( const Data& data )
: Data( data )
{
    validate( data );
}

u1 TypeDefinitionProvider::hasId( void ) const
{
    return find( Identifier::id ) != end();
}

std::string TypeDefinitionProvider::id( void ) const
{
    return at( Identifier::id ).get< std::string >();
}

void TypeDefinitionProvider::setId( std::string id )
{
    operator[]( Identifier::id ) = id;
}

DocumentSelector TypeDefinitionProvider::documentSelector( void ) const
{
    return operator[]( Identifier::documentSelector );
}

void TypeDefinitionProvider::validate( const Data& data )
{
    static const auto context = CONTENT + " TypeDefinitionProvider:";
    if( data.is_boolean() )
    {
        // ok, do nothing
    }
    else
    {
        TextDocumentRegistrationOptions::validate( data );
        StaticRegistrationOptions::validate( data );
    }
}

//
//
// ColorProviderOptions
//

ColorProviderOptions::ColorProviderOptions( const Data& data )
: Data( data )
{
    validate( data );
}

void ColorProviderOptions::validate( const Data& data )
{
    static const auto context = CONTENT + " ColorProviderOptions:";
    Json::validateTypeIsObject( context, data );
}

//
//
// ColorProvider
//

ColorProvider::ColorProvider( const Data& data )
: Data( data )
{
    validate( data );
}

u1 ColorProvider::hasId( void ) const
{
    return find( Identifier::id ) != end();
}

std::string ColorProvider::id( void ) const
{
    return at( Identifier::id ).get< std::string >();
}

void ColorProvider::setId( std::string id )
{
    operator[]( Identifier::id ) = id;
}

DocumentSelector ColorProvider::documentSelector( void ) const
{
    return operator[]( Identifier::documentSelector );
}

void ColorProvider::validate( const Data& data )
{
    static const auto context = CONTENT + " ColorProvider:";
    if( data.is_boolean() )
    {
        // ok, do nothing
    }
    else
    {
        try
        {
            ColorProviderOptions::validate( data );
        }
        catch( std::invalid_argument a )
        {
            TextDocumentRegistrationOptions::validate( data );
            StaticRegistrationOptions::validate( data );
            ColorProviderOptions::validate( data );
        }
    }
}

//
//
// Workspace
//

Workspace::Workspace( const Data& data )
: Data( data )
{
    validate( data );
}

void Workspace::validate( const Data& data )
{
    static const auto context = CONTENT + " Workspace:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< Workspace::WorkspaceFolders >(
        context, data, Identifier::workspaceFolders, false );
}

Workspace::WorkspaceFolders Workspace::workspaceFolders( void ) const
{
    return at( Identifier::workspaceFolders );
}

u1 Workspace::hasWorkspaceFolders( void ) const
{
    return find( Identifier::workspaceFolders ) != end();
}

void Workspace::setWorkspaceFolders( const Workspace::WorkspaceFolders& workspaceFolders )
{
    operator[]( Identifier::workspaceFolders ) =
        Data::from_cbor( Data::to_cbor( workspaceFolders ) );
}

//
//
// WorkspaceFolders
//

Workspace::WorkspaceFolders::WorkspaceFolders( const Data& data )
: Data( data )
{
    validate( data );
}

u1 Workspace::WorkspaceFolders::supported( void ) const
{
    return at( Identifier::supported );
}

u1 Workspace::WorkspaceFolders::hasSupported( void ) const
{
    return find( Identifier::supported ) != end();
}

void Workspace::WorkspaceFolders::setSupported( const u1 supported )
{
    operator[]( Identifier::supported ) = supported;
}

void Workspace::WorkspaceFolders::setChangeNotifications( const std::string& changeNotifications )
{
    operator[]( Identifier::changeNotifications ) = changeNotifications;
}

void Workspace::WorkspaceFolders::setChangeNotifications( const u1 changeNotifications )
{
    operator[]( Identifier::changeNotifications ) = changeNotifications;
}

u1 Workspace::WorkspaceFolders::hasChangeNotifications( void ) const
{
    return find( Identifier::changeNotifications ) != end();
}

std::string Workspace::WorkspaceFolders::changeNotifications( void ) const
{
    if( at( Identifier::changeNotifications ).is_boolean() )
    {
        return std::to_string( operator[]( Identifier::changeNotifications ).get< u1 >() );
    }
    else
    {
        return operator[]( Identifier::changeNotifications ).get< std::string >();
    }
}

void Workspace::WorkspaceFolders::validate( const Data& data )
{
    static const auto context = CONTENT + " Workspace::WorkspaceFolders:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsBoolean( context, data, Identifier::supported, false );
    try
    {
        Json::validatePropertyIsString( context, data, Identifier::changeNotifications, false );
    }
    catch( std::invalid_argument a )
    {
        Json::validatePropertyIsBoolean( context, data, Identifier::changeNotifications, false );
    }
}

//
//
// ServerCapabilities
//

ServerCapabilities::ServerCapabilities( const Data& data )
: Data( data )
{
    validate( data );
}

ServerCapabilities::ServerCapabilities( void )
: Data( Data::object() )
{
}

u1 ServerCapabilities::hasTextDocumentSync( void ) const
{
    return find( Identifier::textDocumentSync ) != end();
}

TextDocumentSyncOptions ServerCapabilities::textDocumentSync( void ) const
{
    return at( Identifier::textDocumentSync );
}

void ServerCapabilities::setTextDocumentSync( const TextDocumentSyncOptions& textDocumentSync )
{
    operator[]( Identifier::textDocumentSync ) =
        Data::from_cbor( Data::to_cbor( textDocumentSync ) );
}

void ServerCapabilities::setTextDocumentSync( const TextDocumentSyncKind& textDocumentSync )
{
    operator[]( Identifier::textDocumentSync ) = static_cast< std::size_t >( textDocumentSync );
}

u1 ServerCapabilities::hasHoverProvider( void ) const
{
    return find( Identifier::hoverProvider ) != end();
}

u1 ServerCapabilities::hoverProvider( void ) const
{
    return at( Identifier::hoverProvider ).get< u1 >();
}

void ServerCapabilities::setHoverProvider( const u1 hoverProvider )
{
    operator[]( Identifier::hoverProvider ) = hoverProvider;
}

u1 ServerCapabilities::hasCompletionProvider( void ) const
{
    return find( Identifier::completionProvider ) != end();
}

CompletionOptions ServerCapabilities::completionProvider( void ) const
{
    return at( Identifier::completionProvider );
}

void ServerCapabilities::setCompletionProvider( const CompletionOptions& completionProvider )
{
    operator[]( Identifier::completionProvider ) = completionProvider;
}

u1 ServerCapabilities::hasSignatureHelpProvider( void ) const
{
    return find( Identifier::signatureHelpProvider ) != end();
}

SignatureHelpOptions ServerCapabilities::signatureHelpProvider( void ) const
{
    return at( Identifier::signatureHelpProvider );
}

void ServerCapabilities::setSignatureHelpProvider(
    const SignatureHelpOptions& signatureHelpProvider )
{
    operator[]( Identifier::signatureHelpProvider ) = signatureHelpProvider;
}

u1 ServerCapabilities::hasDefinitionProvider( void ) const
{
    return find( Identifier::definitionProvider ) != end();
}

u1 ServerCapabilities::definitionProvider( void ) const
{
    return at( Identifier::definitionProvider ).get< u1 >();
}

void ServerCapabilities::setDefinitionProvider( const u1 definitionProvider )
{
    operator[]( Identifier::definitionProvider ) = definitionProvider;
}

u1 ServerCapabilities::hasTypeDefinitionProvider( void ) const
{
    return find( Identifier::typeDefinitionProvider ) != end();
}

TypeDefinitionProvider ServerCapabilities::typeDefinitionProvider( void ) const
{
    return at( Identifier::typeDefinitionProvider );
}

void ServerCapabilities::setTypeDefinitionProvider(
    const TypeDefinitionProvider& typeDefinitionProvider )
{
    operator[]( Identifier::typeDefinitionProvider ) =
        Data::from_cbor( Data::to_cbor( typeDefinitionProvider ) );
}

u1 ServerCapabilities::hasImplementationProvider( void ) const
{
    return find( Identifier::implementationProvider ) != end();
}

ImplementationProvider ServerCapabilities::implementationProvider( void ) const
{
    return at( Identifier::implementationProvider );
}

void ServerCapabilities::setImplementationProvider(
    const ImplementationProvider& implementationProvider )
{
    operator[]( Identifier::implementationProvider ) =
        Data::from_cbor( Data::to_cbor( implementationProvider ) );
}

u1 ServerCapabilities::hasReferencesProvider( void ) const
{
    return find( Identifier::referencesProvider ) != end();
}

u1 ServerCapabilities::referencesProvider( void ) const
{
    return at( Identifier::referencesProvider ).get< u1 >();
}

void ServerCapabilities::setReferencesProvider( const u1 referencesProvider )
{
    operator[]( Identifier::referencesProvider ) = referencesProvider;
}

u1 ServerCapabilities::hasDocumentHighlightProvider( void ) const
{
    return find( Identifier::documentHighlightProvider ) != end();
}

u1 ServerCapabilities::documentHighlightProvider( void ) const
{
    return at( Identifier::documentHighlightProvider ).get< u1 >();
}

void ServerCapabilities::setDocumentHighlightProvider( const u1 documentHighlightProvider )
{
    operator[]( Identifier::documentHighlightProvider ) = documentHighlightProvider;
}

u1 ServerCapabilities::hasDocumentSymbolProvider( void ) const
{
    return find( Identifier::documentSymbolProvider ) != end();
}

u1 ServerCapabilities::documentSymbolProvider( void ) const
{
    return at( Identifier::documentSymbolProvider ).get< u1 >();
}

void ServerCapabilities::setDocumentSymbolProvider( const u1 documentSymbolProvider )
{
    operator[]( Identifier::documentSymbolProvider ) = documentSymbolProvider;
}

u1 ServerCapabilities::hasWorkspaceSymbolProvider( void ) const
{
    return find( Identifier::workspaceSymbolProvider ) != end();
}

u1 ServerCapabilities::workspaceSymbolProvider( void ) const
{
    return at( Identifier::workspaceSymbolProvider ).get< u1 >();
}

void ServerCapabilities::setWorkspaceSymbolProvider( const u1 workspaceSymbolProvider )
{
    operator[]( Identifier::workspaceSymbolProvider ) = workspaceSymbolProvider;
}

u1 ServerCapabilities::hasCodeActionProvider( void ) const
{
    return find( Identifier::codeActionProvider ) != end();
}

u1 ServerCapabilities::codeActionProvider( void ) const
{
    return at( Identifier::codeActionProvider ).get< u1 >();
}

void ServerCapabilities::setCodeActionProvider( const u1 codeActionProvider )
{
    operator[]( Identifier::codeActionProvider ) = codeActionProvider;
}

u1 ServerCapabilities::hasCodeLensProvider( void ) const
{
    return find( Identifier::codeLensProvider ) != end();
}

CodeLensOptions ServerCapabilities::codeLensProvider( void ) const
{
    return at( Identifier::codeLensProvider );
}

void ServerCapabilities::setCodeLensProvider( const CodeLensOptions& codeLensProvider )
{
    operator[]( Identifier::codeLensProvider ) =
        Data::from_cbor( Data::to_cbor( codeLensProvider ) );
}

u1 ServerCapabilities::hasDocumentFormattingProvider( void ) const
{
    return find( Identifier::documentFormattingProvider ) != end();
}

u1 ServerCapabilities::documentFormattingProvider( void ) const
{
    return at( Identifier::documentFormattingProvider ).get< u1 >();
}

void ServerCapabilities::setDocumentFormattingProvider( const u1 documentFormattingProvider )
{
    operator[]( Identifier::documentFormattingProvider ) = documentFormattingProvider;
}

u1 ServerCapabilities::hasDocumentRangeFormattingProvider( void ) const
{
    return find( Identifier::documentRangeFormattingProvider ) != end();
}

u1 ServerCapabilities::documentRangeFormattingProvider( void ) const
{
    return at( Identifier::documentRangeFormattingProvider ).get< u1 >();
}

void ServerCapabilities::setDocumentRangeFormattingProvider(
    const u1 documentRangeFormattingProvider )
{
    operator[]( Identifier::documentRangeFormattingProvider ) = documentRangeFormattingProvider;
}

u1 ServerCapabilities::hasDocumentOnTypeFormattingProvider( void ) const
{
    return find( Identifier::documentOnTypeFormattingProvider ) != end();
}

DocumentOnTypeFormattingOptions ServerCapabilities::documentOnTypeFormattingProvider( void ) const
{
    return at( Identifier::documentOnTypeFormattingProvider );
}

void ServerCapabilities::setDocumentOnTypeFormattingProvider(
    const DocumentOnTypeFormattingOptions& documentOnTypeFormattingProvider )
{
    operator[]( Identifier::documentOnTypeFormattingProvider ) =
        Data::from_cbor( Data::to_cbor( documentOnTypeFormattingProvider ) );
}

u1 ServerCapabilities::hasRenameProvider( void ) const
{
    return find( Identifier::renameProvider ) != end();
}

u1 ServerCapabilities::renameProvider( void ) const
{
    return at( Identifier::renameProvider ).get< u1 >();
}

void ServerCapabilities::setRenameProvider( const u1 renameProvider )
{
    operator[]( Identifier::renameProvider ) = renameProvider;
}

u1 ServerCapabilities::hasDocumentLinkProvider( void ) const
{
    return find( Identifier::documentLinkProvider ) != end();
}

DocumentLinkOptions ServerCapabilities::documentLinkProvider( void ) const
{
    return at( Identifier::documentLinkProvider );
}

void ServerCapabilities::setDocumentLinkProvider( const DocumentLinkOptions& documentLinkProvider )
{
    operator[]( Identifier::documentLinkProvider ) = documentLinkProvider;
}

u1 ServerCapabilities::hasColorProvider( void ) const
{
    return find( Identifier::colorProvider ) != end();
}

ColorProvider ServerCapabilities::colorProvider( void ) const
{
    return at( Identifier::colorProvider );
}

void ServerCapabilities::setColorProvider( const ColorProvider& colorProvider )
{
    operator[]( Identifier::colorProvider ) = Data::from_cbor( Data::to_cbor( colorProvider ) );
}

u1 ServerCapabilities::hasFoldingRangeProvider( void ) const
{
    return find( Identifier::foldingRangeProvider ) != end();
}

FoldingRangeProvider ServerCapabilities::foldingRangeProvider( void ) const
{
    return at( Identifier::foldingRangeProvider );
}

void ServerCapabilities::setFoldingRangeProvider( const FoldingRangeProvider& foldingRangeProvider )
{
    operator[]( Identifier::foldingRangeProvider ) =
        Data::from_cbor( Data::to_cbor( foldingRangeProvider ) );
}

u1 ServerCapabilities::hasExecuteCommandProvider( void ) const
{
    return find( Identifier::executeCommandProvider ) != end();
}

ExecuteCommandOptions ServerCapabilities::executeCommandProvider( void ) const
{
    return at( Identifier::executeCommandProvider );
}

void ServerCapabilities::setExecuteCommandProvider(
    const ExecuteCommandOptions& executeCommandProvider )
{
    operator[]( Identifier::executeCommandProvider ) =
        Data::from_cbor( Data::to_cbor( executeCommandProvider ) );
}

u1 ServerCapabilities::hasExperimental( void ) const
{
    return find( Identifier::experimental ) != end();
}

Data ServerCapabilities::experimental( void ) const
{
    return at( Identifier::experimental );
}

void ServerCapabilities::setExperimental( const Data& experimental )
{
    operator[]( Identifier::experimental ) = Data::from_cbor( Data::to_cbor( experimental ) );
}

u1 ServerCapabilities::hasWorkspace( void ) const
{
    return find( Identifier::workspace ) != end();
}

void ServerCapabilities::setWorkspace( const Workspace& workspace )
{
    operator[]( Identifier::workspace ) = Data::from_cbor( Data::to_cbor( workspace ) );
}

Workspace ServerCapabilities::workspace( void ) const
{
    return at( Identifier::workspace );
}

void ServerCapabilities::validate( const Data& data )
{
    static const auto context = CONTENT + " SignatureHelpOptions:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< TextDocumentSyncOptions >(
        context, data, Identifier::textDocumentSync, false );
    Json::validatePropertyIsBoolean( context, data, Identifier::hoverProvider, false );
    Json::validatePropertyIs< CompletionOptions >(
        context, data, Identifier::completionProvider, false );
    Json::validatePropertyIs< SignatureHelpOptions >(
        context, data, Identifier::signatureHelpProvider, false );
    Json::validatePropertyIsBoolean( context, data, Identifier::definitionProvider, false );
    Json::validatePropertyIsBoolean( context, data, Identifier::referencesProvider, false );
    Json::validatePropertyIsBoolean( context, data, Identifier::documentHighlightProvider, false );
    Json::validatePropertyIsBoolean( context, data, Identifier::documentSymbolProvider, false );
    Json::validatePropertyIsBoolean( context, data, Identifier::workspaceSymbolProvider, false );
    Json::validatePropertyIsBoolean( context, data, Identifier::codeActionProvider, false );
    Json::validatePropertyIs< CodeLensOptions >(
        context, data, Identifier::codeLensProvider, false );
    Json::validatePropertyIsBoolean( context, data, Identifier::documentFormattingProvider, false );
    Json::validatePropertyIsBoolean(
        context, data, Identifier::documentRangeFormattingProvider, false );
    Json::validatePropertyIs< DocumentOnTypeFormattingOptions >(
        context, data, Identifier::documentOnTypeFormattingProvider, false );
    Json::validatePropertyIsBoolean( context, data, Identifier::renameProvider, false );
    Json::validatePropertyIs< DocumentLinkOptions >(
        context, data, Identifier::documentLinkProvider, false );
    Json::validatePropertyIs< ExecuteCommandOptions >(
        context, data, Identifier::executeCommandProvider, false );
    Json::validatePropertyIsObject( context, data, Identifier::experimental, false );
    Json::validatePropertyIs< Workspace >( context, data, Identifier::workspace, false );
    Json::validatePropertyIs< ColorProvider >( context, data, Identifier::colorProvider, false );
    Json::validatePropertyIs< FoldingRangeProvider >(
        context, data, Identifier::foldingRangeProvider, false );
    Json::validatePropertyIs< ImplementationProvider >(
        context, data, Identifier::implementationProvider, false );
    Json::validatePropertyIs< TypeDefinitionProvider >(
        context, data, Identifier::typeDefinitionProvider, false );
}

// -----------------------------------------------------------------------------

//
//
// InitializeParams
//

InitializeParams::InitializeParams( const Data& data )
: Data( data )
{
    validate( data );
}

InitializeParams::InitializeParams(
    const std::size_t processId,
    const DocumentUri& rootUri,
    const ClientCapabilities& capabilities )
: Data( Data::object() )
{
    operator[]( Identifier::processId ) = processId;
    operator[]( Identifier::rootUri ) = rootUri.toString();
    operator[]( Identifier::capabilities ) = Data::from_cbor( Data::to_cbor( capabilities ) );
}

std::size_t InitializeParams::processId( void ) const
{
    return at( Identifier::processId ).get< std::size_t >();
}

DocumentUri InitializeParams::rootUri( void ) const
{
    return DocumentUri::fromString( at( Identifier::processId ).get< std::string >() );
}

u1 InitializeParams::hasInitializationOptions( void ) const
{
    return find( Identifier::initializationOptions ) != end();
}

Data InitializeParams::initializationOptions( void ) const
{
    return at( Identifier::initializationOptions );
}

void InitializeParams::setInitializationOptions( const Data& initializationOptions )
{
    operator[]( Identifier::initializationOptions ) = initializationOptions;
}

ClientCapabilities InitializeParams::capabilities( void ) const
{
    return at( Identifier::capabilities );
}

u1 InitializeParams::hasTrace( void ) const
{
    return find( Identifier::trace ) != end();
}

std::string InitializeParams::trace( void ) const
{
    return at( Identifier::trace );
}

void InitializeParams::setTrace( const std::string& trace )
{
    operator[]( Identifier::trace ) = trace;
}

WorkspaceFolders InitializeParams::workspaceFolders( void ) const
{
    auto folders = WorkspaceFolders();
    for( auto folder : at( Identifier::workspaceFolders ) )
    {
        folders.push_back( folder );
    }
    return folders;
}

u1 InitializeParams::hasWorkspaceFolders( void ) const
{
    return find( Identifier::workspaceFolders ) != end();
}

void InitializeParams::addWorkspaceFolder( WorkspaceFolder folder )
{
    if( not hasWorkspaceFolders() )
    {
        operator[]( Identifier::workspaceFolders ) = Data::array();
    }
    operator[]( Identifier::workspace_workspaceFolders ).push_back( folder );
}

void InitializeParams::validate( const Data& data )
{
    static const auto context = CONTENT + " InitializeParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsUriOrNull( context, data, Identifier::rootUri, true );
    Json::validatePropertyIs< ClientCapabilities >( context, data, Identifier::capabilities, true );

    Json::validatePropertyIsNumberOrNull( context, data, Identifier::processId, false );
    Json::validatePropertyIsObject( context, data, Identifier::initializationOptions, false );
    Json::validatePropertyIsString( context, data, Identifier::trace, false );
    if( Json::hasProperty( data, Identifier::workspaceFolders ) &&
        data[ Identifier::workspaceFolders ].is_null() )
    {
        // ok, do nothing
    }
    else
    {
        Json::validatePropertyIsArrayOf< WorkspaceFolder >(
            context, data, Identifier::workspaceFolders, false );
    }
}

//
//
// InitializeResult
//

InitializeResult::InitializeResult( const Data& data )
: Data( data )
{
    validate( data );
}

InitializeResult::InitializeResult( const ServerCapabilities& capabilities )
: Data( Data::object() )
{
    operator[]( Identifier::capabilities ) = Data::from_cbor( Data::to_cbor( capabilities ) );
}

ServerCapabilities InitializeResult::capabilities( void ) const
{
    return at( Identifier::capabilities );
}

void InitializeResult::validate( const Data& data )
{
    static const auto context = CONTENT + " InitializeResult:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< ServerCapabilities >(
        context, data, Identifier::capabilities, false );
}

//
//
// InitializeError
//

InitializeError::InitializeError( const Data& data )
: Data( data )
{
    validate( data );
}

InitializeError::InitializeError( const u1 retry )
: Data( Data::object() )
{
    operator[]( Identifier::retry ) = retry;
}

u1 InitializeError::retry( void ) const
{
    return at( Identifier::retry ).get< u1 >();
}

void InitializeError::validate( const Data& data )
{
    static const auto context = CONTENT + " InitializeError:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsBoolean( context, data, Identifier::retry, false );
}

//
//
// CancelParams
//

CancelParams::CancelParams( const Data& data )
: Data( data )
{
    validate( data );
}

CancelParams::CancelParams( const std::size_t& id )
: Data( Data::object() )
{
    operator[]( Identifier::id ) = id;
}

CancelParams::CancelParams( const std::string& id )
: Data( Data::object() )
{
    operator[]( Identifier::id ) = id;
}
std::string CancelParams::id( void ) const
{
    std::string result;
    if( operator[]( Identifier::id ).is_number() )
    {
        return std::to_string( operator[]( Identifier::id ).get< std::size_t >() );
    }
    return operator[]( Identifier::id ).get< std::string >();
}

void CancelParams::validate( const Data& data )
{
    static const auto context = CONTENT + " CancelParams:";
    Json::validateTypeIsObject( context, data );
    if( data[ Identifier::id ].is_number() )
    {
        Json::validatePropertyIsNumber( context, data, Identifier::id, true );
    }
    else
    {
        Json::validatePropertyIsString( context, data, Identifier::id, true );
    }
}

//
//
// ShowMessageParams
//

ShowMessageParams::ShowMessageParams( const Data& data )
: Data( data )
{
    validate( data );
}

ShowMessageParams::ShowMessageParams( const MessageType type, const std::string& message )
: Data( Data::object() )
{
    operator[]( Identifier::type ) = static_cast< std::size_t >( type );
    operator[]( Identifier::message ) = message;
}

MessageType ShowMessageParams::messageType( void ) const
{
    return static_cast< MessageType >( at( Identifier::type ).get< std::size_t >() );
}

std::string ShowMessageParams::message( void ) const
{
    return at( Identifier::message ).get< std::string >();
}

void ShowMessageParams::validate( const Data& data )
{
    static const auto context = CONTENT + " ShowMessageParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::message, true );
    Json::validatePropertyIsNumber( context, data, Identifier::type, true );
}

//
//
// MessageActionItem
//

MessageActionItem::MessageActionItem( const Data& data )
: Data( data )
{
    validate( data );
}

MessageActionItem::MessageActionItem( const std::string& title )
: Data( Data::object() )
{
    operator[]( Identifier::title ) = title;
}

std::string MessageActionItem::title( void ) const
{
    return operator[]( Identifier::title ).get< std::string >();
}

void MessageActionItem::validate( const Data& data )
{
    static const auto context = CONTENT + " MessageActionItem:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::title, true );
}

//
//
// ShowMessageRequestResult
//

ShowMessageRequestResult::ShowMessageRequestResult( void )
: Data()
{
}
ShowMessageRequestResult::ShowMessageRequestResult( const Data& data )
: Data( data )
{
    validate( data );
}

ShowMessageRequestResult::ShowMessageRequestResult( const MessageActionItem& item )
: Data( Data::from_cbor( Data::to_cbor( item ) ) )
{
}

ShowMessageRequestResult::ShowMessageRequestResult( const std::string& title )
: Data( Data::object() )
{
    operator[]( Identifier::title ) = title;
}

void ShowMessageRequestResult::validate( const Data& data )
{
    static const auto context = CONTENT + " ShowMessageRequestResult:";
    if( data.is_null() )
    {
        // ok, do nothing.
    }
    else
    {
        MessageActionItem::validate( data );
    }
}

//
//
// ShowMessageRequestParams
//

ShowMessageRequestParams::ShowMessageRequestParams( const Data& data )
: Data( data )
{
    validate( data );
}

ShowMessageRequestParams::ShowMessageRequestParams(
    const MessageType type, const std::string& message )
: Data( Data::object() )
{
    operator[]( Identifier::type ) = static_cast< std::size_t >( type );
    operator[]( Identifier::message ) = message;
}

MessageType ShowMessageRequestParams::messageType( void ) const
{
    return static_cast< MessageType >( at( Identifier::type ).get< std::size_t >() );
}

u1 ShowMessageRequestParams::hasActions( void )
{
    return find( Identifier::actions ) != end();
}

MessageActionItems ShowMessageRequestParams::actions( void ) const
{
    auto actions = operator[]( Identifier::actions );
    auto vector = std::vector< MessageActionItem >();
    for( auto action : actions )
    {
        vector.emplace_back( MessageActionItem( action ) );
    }
    return vector;
}

void ShowMessageRequestParams::addAction( const MessageActionItem& action )
{
    if( not ShowMessageRequestParams::hasActions() )
    {
        operator[]( Identifier::actions ) = Data::array();
    }
    operator[]( Identifier::actions ).push_back( action );
}

std::string ShowMessageRequestParams::message( void ) const
{
    return at( Identifier::message ).get< std::string >();
}

void ShowMessageRequestParams::validate( const Data& data )
{
    static const auto context = CONTENT + " ShowMessageRequestParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::title, true );
    Json::validatePropertyIsArray( context, data, Identifier::actions, false );
}

TelemetryEventParams::TelemetryEventParams( const Data& data )
: Data( data )
{
    validate( data );
}

void TelemetryEventParams::validate( const Data& data )
{
    static const auto context = CONTENT + " TelemetryEventParams:";
    Json::validateTypeIsObject( context, data );
}

//
//
// Registration
//

Registration::Registration( const Data& data )
: Data( data )
{
    validate( data );
}

Registration::Registration( const std::string& id, const std::string& method )
: Data( Data::object() )
{
    operator[]( Identifier::id ) = id;
    operator[]( Identifier::method ) = method;
}

std::string Registration::id( void ) const
{
    return operator[]( Identifier::id ).get< std::string >();
}

std::string Registration::method( void ) const
{
    return operator[]( Identifier::method ).get< std::string >();
}

Data Registration::registerOptions( void ) const
{
    return at( Identifier::registerOptions );
}

u1 Registration::hasRegisterOptions( void ) const
{
    return find( Identifier::registerOptions ) != end();
}

void Registration::addRegisterOption( const Data& option )
{
    if( not Registration::hasRegisterOptions() )
    {
        operator[]( Identifier::registerOptions ) = Data::array();
    }
    operator[]( Identifier::registerOptions ).push_back( option );
}

void Registration::validate( const Data& data )
{
    static const auto context = CONTENT + " Registration:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::id, true );
    Json::validatePropertyIsString( context, data, Identifier::method, true );
    Json::validatePropertyIsArray( context, data, Identifier::registerOptions, false );
}

//
//
// RegistrationParams
//

RegistrationParams::RegistrationParams( const Data& data )
: Data( data )
{
    validate( data );
}

RegistrationParams::RegistrationParams( const Registrations& registrations )
: Data( Data::object() )
{
    operator[]( Identifier::registrations ) = Data::array();
    for( auto registration : registrations )
    {
        operator[]( Identifier::registrations ).push_back( registration );
    }
}

Registrations RegistrationParams::registrations( void ) const
{
    auto array = operator[]( Identifier::registrations );
    auto registrations = std::vector< Registration >();
    for( auto registration : array )
    {
        registrations.push_back( Registration( registration ) );
    }
    return registrations;
}

void RegistrationParams::validate( const Data& data )
{
    static const auto context = CONTENT + " RegistrationParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsArray( context, data, Identifier::registrations, true );
}

//
//
// TextDocumentRegistrationOptions
//

TextDocumentRegistrationOptions::TextDocumentRegistrationOptions( const Data& data )
: Data( data )
{
    validate( data );
}

TextDocumentRegistrationOptions::TextDocumentRegistrationOptions(
    const DocumentSelector& documentSelector )
: Data( Data::object() )
{
    operator[]( Identifier::documentSelector ) = documentSelector;
}

DocumentSelector TextDocumentRegistrationOptions::documentSelector( void ) const
{
    return operator[]( Identifier::documentSelector );
}

void TextDocumentRegistrationOptions::validate( const Data& data )
{
    static const auto context = CONTENT + " TextDocumentRegistrationOptions:";
    Json::validateTypeIsObject( context, data );
    if( data.find( Identifier::documentSelector ) != data.end() and
        data[ Identifier::documentSelector ].is_null() )
    {
        // ok, due to the possibility that the property can be null
    }
    else
    {
        Json::validatePropertyIs< DocumentSelector >(
            context, data, Identifier::documentSelector, true );
    }
}

//
//
// TextDocumentChangeRegistrationOptions
//

TextDocumentChangeRegistrationOptions::TextDocumentChangeRegistrationOptions( const Data& data )
: TextDocumentRegistrationOptions( data )
{
    validate( data );
}

TextDocumentChangeRegistrationOptions::TextDocumentChangeRegistrationOptions(
    const DocumentSelector& documentSelector, const TextDocumentSyncKind kind )
: TextDocumentRegistrationOptions( documentSelector )
{
    operator[]( Identifier::kind ) = static_cast< std::size_t >( kind );
}

TextDocumentSyncKind TextDocumentChangeRegistrationOptions::kind( void ) const
{
    return static_cast< TextDocumentSyncKind >( operator[]( Identifier::kind )
                                                    .get< std::size_t >() );
}

void TextDocumentChangeRegistrationOptions::validate( const Data& data )
{
    static const auto context = CONTENT + " TextDocumentChangeRegistrationOptions:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsNumber( context, data, Identifier::kind, true );
}

//
//
// Unregistration
//

Unregistration::Unregistration( const Data& data )
: Data( data )
{
    validate( data );
}

Unregistration::Unregistration( const std::string& id, const std::string& method )
: Data( Data::object() )
{
    operator[]( Identifier::id ) = id;
    operator[]( Identifier::method ) = method;
}

std::string Unregistration::id( void ) const
{
    return operator[]( Identifier::id ).get< std::string >();
}

std::string Unregistration::method( void ) const
{
    return operator[]( Identifier::method ).get< std::string >();
}

void Unregistration::validate( const Data& data )
{
    static const auto context = CONTENT + " Unregistration:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::id, true );
    Json::validatePropertyIsString( context, data, Identifier::method, true );
}

//
//
// UnregistrationParams
//

UnregistrationParams::UnregistrationParams( const std::vector< Unregistration >& unregistrations )
: Data( Data::object() )
{
    operator[]( Identifier::unregistrations ) = Data::array();
    for( auto unregistration : unregistrations )
    {
        operator[]( Identifier::unregistrations ).push_back( unregistration );
    }
}

UnregistrationParams::UnregistrationParams( const Data& data )
: Data( data )
{
    validate( data );
}

Unregistrations UnregistrationParams::unregistrations( void ) const
{
    auto unregistrations = operator[]( Identifier::unregistrations );
    auto vector = std::vector< Unregistration >();
    for( auto unregistration : unregistrations )
    {
        vector.emplace_back( unregistration );
    }
    return vector;
}

void UnregistrationParams::validate( const Data& data )
{
    static const auto context = CONTENT + " UnregistrationParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsArray( context, data, Identifier::unregistrations, true );
}

//
//
// WorkspaceFolder
//

WorkspaceFolder::WorkspaceFolder( const std::string& uri, const std::string& name )
: Data( Data::object() )
{
    operator[]( Identifier::uri ) = uri;
    operator[]( Identifier::name ) = name;
}

WorkspaceFolder::WorkspaceFolder( const Data& data )
: Data( data )
{
    validate( data );
}

std::string WorkspaceFolder::uri( void ) const
{
    return operator[]( Identifier::uri ).get< std::string >();
}

std::string WorkspaceFolder::name( void ) const
{
    return operator[]( Identifier::name ).get< std::string >();
}

void WorkspaceFolder::validate( const Data& data )
{
    static const auto context = CONTENT + " WorkspaceFolder:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::uri, true );
    Json::validatePropertyIsString( context, data, Identifier::name, true );
}

//
//
// WorkspaceFoldersResult
//

WorkspaceFoldersResult::WorkspaceFoldersResult()
: Data( Data() )
{
}

WorkspaceFoldersResult::WorkspaceFoldersResult( const Data& data )
: Data( data )
{
    validate( data );
}

WorkspaceFoldersResult::WorkspaceFoldersResult( const WorkspaceFolders& workspaceFolders )
: Data( Data::array() )
{
    for( auto folder : workspaceFolders )
    {
        push_back( folder );
    }
}

WorkspaceFolders WorkspaceFoldersResult::toVec( void ) const
{
    auto vector = WorkspaceFolders();

    for( auto folder : *this )
    {
        vector.emplace_back( folder );
    }
    return vector;
}

void WorkspaceFoldersResult::validate( const Data& data )
{
    static const auto context = CONTENT + " WorkSpaceFolderResponse:";

    if( data.is_null() )
    {
        // ok, due to the possibility that array can be null
    }
    else
    {
        Json::validateTypeIsArrayOf< WorkspaceFolder >( context, data );
    }
}

//
//
// WorkspaceFoldersChangeEvent
//

WorkspaceFoldersChangeEvent::WorkspaceFoldersChangeEvent(
    const WorkspaceFolders& added, const WorkspaceFolders& removed )
: Data( Data::object() )
{
    operator[]( Identifier::added ) = Data::array();
    operator[]( Identifier::removed ) = Data::array();

    for( auto element : added )
    {
        operator[]( Identifier::added ).push_back( element );
    }
    for( auto element : removed )
    {
        operator[]( Identifier::removed ).push_back( element );
    }
}

WorkspaceFoldersChangeEvent::WorkspaceFoldersChangeEvent( const Data& data )
: Data( data )
{
    validate( data );
}

WorkspaceFolders WorkspaceFoldersChangeEvent::added( void ) const
{
    auto vector = WorkspaceFolders();
    auto added = operator[]( Identifier::added );

    for( auto element : added )
    {
        vector.emplace_back( element );
    }
    return vector;
}

WorkspaceFolders WorkspaceFoldersChangeEvent::removed( void ) const
{
    auto vector = WorkspaceFolders();
    auto removed = operator[]( Identifier::removed );

    for( auto element : removed )
    {
        vector.emplace_back( element );
    }
    return vector;
}

void WorkspaceFoldersChangeEvent::validate( const Data& data )
{
    static const auto context = CONTENT + " WorkspaceFoldersChangeEvent:";
    Json::validateTypeIsObject( context, data );

    Json::validatePropertyIsArrayOf< WorkspaceFolder >( context, data, Identifier::added, true );
    Json::validatePropertyIsArrayOf< WorkspaceFolder >( context, data, Identifier::removed, true );
}

//
//
// DidChangeWorkspaceFoldersParams
//

DidChangeWorkspaceFoldersParams::DidChangeWorkspaceFoldersParams( const Data& data )
: Data( Data::object() )
{
    validate( data );
}

DidChangeWorkspaceFoldersParams::DidChangeWorkspaceFoldersParams(
    const WorkspaceFoldersChangeEvent& event )
{
    operator[]( Identifier::event ) = Data::from_cbor( Data::to_cbor( event ) );
}

WorkspaceFoldersChangeEvent DidChangeWorkspaceFoldersParams::event( void ) const
{
    return operator[]( Identifier::event );
}

void DidChangeWorkspaceFoldersParams::validate( const Data& data )
{
    static const auto context = CONTENT + " DidChangeWorkspaceFoldersParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< WorkspaceFoldersChangeEvent >(
        context, data, Identifier::event, true );
}

//
//
// DidChangeConfigurationParams
//

DidChangeConfigurationParams::DidChangeConfigurationParams(
    const DidChangeConfigurationSettings& settings )
: Data( Data::object() )
{
    operator[]( Identifier::settings ) = Data::from_cbor( Data::to_cbor( settings ) );
    validate( *this );
}

DidChangeConfigurationSettings DidChangeConfigurationParams::settings( void ) const
{
    return operator[]( Identifier::settings );
}

void DidChangeConfigurationParams::validate( const Data& data )
{
    static const auto context = CONTENT + " DidChangeConfigurationParams:";
    Json::validatePropertyIsObject( context, data, Identifier::settings, true );
}

//
//
// ConfigurationItem
//

ConfigurationItem::ConfigurationItem( const Data& data )
: Data( data )
{
    validate( data );
}

ConfigurationItem::ConfigurationItem( const std::string& scopeUri, const std::string& section )
: Data( Data::object() )
{
    operator[]( Identifier::scopeUri ) = scopeUri;
    operator[]( Identifier::section ) = section;
}

u1 ConfigurationItem::hasScopeUri( void ) const
{
    return find( Identifier::scopeUri ) != end();
}

u1 ConfigurationItem::hasSection( void ) const
{
    return find( Identifier::section ) != end();
}

std::string ConfigurationItem::scopeUri( void ) const
{
    return at( Identifier::scopeUri ).get< std::string >();
}

std::string ConfigurationItem::section( void ) const
{
    return at( Identifier::section ).get< std::string >();
}

void ConfigurationItem::setSection( const std::string& section )
{
    operator[]( Identifier::section ) = section;
}

void ConfigurationItem::setScopeUri( const std::string& uri )
{
    operator[]( Identifier::scopeUri ) = uri;
}

void ConfigurationItem::validate( const Data& data )
{
    static const auto context = CONTENT + " ConfigurationItem:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::section, false );
    Json::validatePropertyIsString( context, data, Identifier::scopeUri, false );
}

//
//
// ConfigurationResult
//

ConfigurationResult::ConfigurationResult( void )
: Data( Data::array() )
{
}

ConfigurationResult::ConfigurationResult( const Data& data )
: Data( data )
{
    validate( data );
}

void ConfigurationResult::validate( const Data& data )
{
    static const auto context = CONTENT + " ConfigurationResult:";
    Json::validateTypeIsArray( context, data );
}

//
//
// ConfigurationParams
//

ConfigurationParams::ConfigurationParams( const std::vector< ConfigurationItem >& items )
: Data( Data::object() )
{
    operator[]( Identifier::items ) = Data::array();
    for( auto item : items )
    {
        operator[]( Identifier::items ).push_back( item );
    }
}

ConfigurationParams::ConfigurationParams( const Data& data )
: Data( data )
{
    validate( data );
}

ConfigurationItems ConfigurationParams::items( void ) const
{
    auto items = operator[]( Identifier::items );
    auto vector = ConfigurationItems();

    for( auto item : items )
    {
        vector.emplace_back( item );
    }
    return vector;
}

void ConfigurationParams::validate( const Data& data )
{
    static const auto context = CONTENT + " ConfigurationParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsArrayOf< ConfigurationItem >( context, data, Identifier::items, true );
}

//
//
// FileEvent
//

FileEvent::FileEvent( const Data& data )
: Data( data )
{
    validate( data );
}

FileEvent::FileEvent( const DocumentUri& uri, const FileChangeType type )
: Data( Data::object() )
{
    operator[]( Identifier::uri ) = uri.toString();
    operator[]( Identifier::type ) = static_cast< std::size_t >( type );
}

DocumentUri FileEvent::documentUri( void ) const
{
    return DocumentUri::fromString( operator[]( Identifier::uri ).get< std::string >() );
}

FileChangeType FileEvent::type( void ) const
{
    return static_cast< FileChangeType >( operator[]( Identifier::type ).get< std::size_t >() );
}

void FileEvent::validate( const Data& data )
{
    static const auto context = CONTENT + " FileEvent:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsUri( context, data, Identifier::uri, true );
    Json::validatePropertyIsNumber( context, data, Identifier::type, true );
}

//
//
// DidChangeWatchedFilesParams
//

DidChangeWatchedFilesParams::DidChangeWatchedFilesParams( const std::vector< FileEvent >& changes )
: Data( Data::object() )
{
    operator[]( Identifier::changes ) = Data::array();
    for( auto change : changes )
    {
        operator[]( Identifier::changes ).push_back( change );
    }
}

DidChangeWatchedFilesParams::DidChangeWatchedFilesParams( const Data& data )
: Data( data )
{
    validate( data );
}

FileEvents DidChangeWatchedFilesParams::changes( void ) const
{
    auto changes = operator[]( Identifier::changes );
    auto vector = FileEvents();
    for( auto change : changes )
    {
        vector.emplace_back( change );
    }
    return vector;
}

void DidChangeWatchedFilesParams::validate( const Data& data )
{
    static const auto context = CONTENT + " DidChangeWatchedFilesParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsArray( context, data, Identifier::changes, true );
}

//
//
// FileSystemWatcher
//

FileSystemWatcher::FileSystemWatcher( const Data& data )
: Data( data )
{
    validate( data );
}

FileSystemWatcher::FileSystemWatcher( const std::string& globPattern )
: Data( Data::object() )
{
    operator[]( Identifier::globPattern ) = globPattern;
}

FileSystemWatcher::FileSystemWatcher( const std::string& globPattern, const WatchKind kind )
: FileSystemWatcher( globPattern )
{
    operator[]( Identifier::kind ) = static_cast< std::size_t >( kind );
}

std::string FileSystemWatcher::globPattern( void )
{
    return operator[]( Identifier::globPattern ).get< std::string >();
}

u1 FileSystemWatcher::hasKind( void ) const
{
    return find( Identifier::kind ) != end();
}

WatchKind FileSystemWatcher::kind( void ) const
{
    return static_cast< WatchKind >( at( Identifier::kind ).get< std::size_t >() );
}

void FileSystemWatcher::setKind( const WatchKind kind )
{
    operator[]( Identifier::kind ) = static_cast< std::size_t >( kind );
}

void FileSystemWatcher::validate( const Data& data )
{
    static const auto context = CONTENT + " FileSystemWatchers:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::globPattern, true );
    Json::validatePropertyIsNumber( context, data, Identifier::kind, false );
}

//
//
// DidChangeWatchedFilesRegistrationOptions
//

DidChangeWatchedFilesRegistrationOptions::DidChangeWatchedFilesRegistrationOptions(
    const FileSystemWatchers& watchers )
: Data( Data::object() )
{
    operator[]( Identifier::watchers ) = Data::array();
    for( auto watcher : watchers )
    {
        operator[]( Identifier::watchers ).push_back( watcher );
    }
}

DidChangeWatchedFilesRegistrationOptions::DidChangeWatchedFilesRegistrationOptions(
    const Data& data )
: Data( data )
{
    validate( data );
}

FileSystemWatchers DidChangeWatchedFilesRegistrationOptions::watchers( void ) const
{
    auto watchers = operator[]( Identifier::watchers );
    auto result = FileSystemWatchers();

    for( auto watcher : watchers )
    {
        result.emplace_back( watcher );
    }
    return result;
}

void DidChangeWatchedFilesRegistrationOptions::validate( const Data& data )
{
    static const auto context = CONTENT + " DidChangeWatchedFilesRegistrationOptions:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsArrayOf< FileSystemWatcher >(
        context, data, Identifier::watchers, true );
}

//
//
// WorkspaceSymbolParams
//

WorkspaceSymbolParams::WorkspaceSymbolParams( const Data& data )
: Data( data )
{
    validate( data );
}

WorkspaceSymbolParams::WorkspaceSymbolParams( const std::string& query )
: Data( Data::object() )
{
    operator[]( Identifier::query ) = query;
}

std::string WorkspaceSymbolParams::query( void ) const
{
    return operator[]( Identifier::query ).get< std::string >();
}

void WorkspaceSymbolParams::validate( const Data& data )
{
    static const auto context = CONTENT + " WorkspaceSymbolParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::query, true );
}

//
//
// SymbolInformation
//

SymbolInformation::SymbolInformation(
    const std::string& name, const SymbolKind kind, const Location& location )
: Data( Data::object() )
{
    operator[]( Identifier::name ) = name;
    operator[]( Identifier::kind ) = static_cast< std::size_t >( kind );
    operator[]( Identifier::location ) = Data::from_cbor( Data::to_cbor( location ) );
}

SymbolInformation::SymbolInformation( const Data& data )
: Data( data )
{
    validate( data );
}

u1 SymbolInformation::isDeprecated( void ) const
{
    return at( Identifier::deprecated ).get< bool >();
}

u1 SymbolInformation::hasDeprecated( void ) const
{
    return find( Identifier::deprecated ) != end();
}

void SymbolInformation::setDeprecated( const u1 deprecated )
{
    operator[]( Identifier::deprecated ) = deprecated;
}

std::string SymbolInformation::containerName( void ) const
{
    return at( Identifier::containerName ).get< std::string >();
}

u1 SymbolInformation::hasContainerName( void ) const
{
    return find( Identifier::containerName ) != end();
}

void SymbolInformation::setContainerName( const std::string& containerName )
{
    operator[]( Identifier::containerName ) = containerName;
}

Location SymbolInformation::location( void ) const
{
    return operator[]( Identifier::location );
}

std::string SymbolInformation::name( void ) const
{
    return operator[]( Identifier::name ).get< std::string >();
}

SymbolKind SymbolInformation::kind( void ) const
{
    return static_cast< SymbolKind >( operator[]( Identifier::kind ).get< std::size_t >() );
}

void SymbolInformation::validate( const Data& data )
{
    static const auto context = CONTENT + " SymbolInformation:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::name, true );
    Json::validatePropertyIsNumber( context, data, Identifier::kind, true );
    Json::validatePropertyIs< Location >( context, data, Identifier::location, true );
    Json::validatePropertyIsString( context, data, Identifier::containerName, false );
    Json::validatePropertyIsBoolean( context, data, Identifier::deprecated, false );
}

//
//
// workspaceSymbolResult
//

WorkspaceSymbolResult::WorkspaceSymbolResult( const Data& data )
: Data( data )
{
    validate( data );
}

WorkspaceSymbolResult::WorkspaceSymbolResult( const SymbolInformations& symbolInformation )
: Data( Data::array() )
{
    for( auto information : symbolInformation )
    {
        push_back( information );
    }
}

void WorkspaceSymbolResult::addSymbolInformation( const SymbolInformation& information )
{
    push_back( information );
}

SymbolInformations WorkspaceSymbolResult::symbolInformation( void ) const
{
    auto result = SymbolInformations();

    for( auto info : *this )
    {
        result.emplace_back( info );
    }
    return result;
}

void WorkspaceSymbolResult::validate( const Data& data )
{
    static const auto context = CONTENT + " workspaceSymbolResult:";
    Json::validateTypeIsObject( context, data );
    if( data.is_null() )
    {
        // ok, do nothing
    }
    else
    {
        Json::validateTypeIsArrayOf< SymbolInformation >( context, data );
    }
}

//
//
// ApplyWorkspaceEditParams
//

ApplyWorkspaceEditParams::ApplyWorkspaceEditParams( const Data& data )
: Data( data )
{
    validate( data );
}

ApplyWorkspaceEditParams::ApplyWorkspaceEditParams( const WorkspaceEdit& edit )
: Data( Data::object() )
{
    operator[]( Identifier::edit ) = Data::from_cbor( Data::to_cbor( edit ) );
}

WorkspaceEdit ApplyWorkspaceEditParams::edit( void ) const
{
    return operator[]( Identifier::edit );
}

u1 ApplyWorkspaceEditParams::hasLabel( void ) const
{
    return find( Identifier::label ) != end();
}

std::string ApplyWorkspaceEditParams::label( void ) const
{
    return at( Identifier::label ).get< std::string >();
}

void ApplyWorkspaceEditParams::setLabel( const std::string& label )
{
    operator[]( Identifier::label ) = label;
}

void ApplyWorkspaceEditParams::validate( const Data& data )
{
    static const auto context = CONTENT + " ApplyWorkspaceEditParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< WorkspaceEdit >( context, data, Identifier::edit, true );
    Json::validatePropertyIsString( context, data, Identifier::label, false );
}

//
//
// ApplyWorkspaceEditResult
//

ApplyWorkspaceEditResult::ApplyWorkspaceEditResult( const Data& data )
: Data( data )
{
    validate( data );
}

ApplyWorkspaceEditResult::ApplyWorkspaceEditResult( const u1 applied )
: Data( Data::object() )
{
    operator[]( Identifier::applied ) = applied;
}

u1 ApplyWorkspaceEditResult::applied( void ) const
{
    return operator[]( Identifier::applied ).get< u1 >();
}

void ApplyWorkspaceEditResult::validate( const Data& data )
{
    static const auto context = CONTENT + " ApplyWorkspaceEditResult:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsBoolean( context, data, Identifier::applied, true );
}

//
//
// DidOpenTextDocumentParams
//

DidOpenTextDocumentParams::DidOpenTextDocumentParams( const Data& data )
: Data( data )
{
    validate( data );
}

DidOpenTextDocumentParams::DidOpenTextDocumentParams( const TextDocumentItem& textDocument )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument ) = Data::from_cbor( Data::to_cbor( textDocument ) );
}

TextDocumentItem DidOpenTextDocumentParams::textDocument( void ) const
{
    return at( Identifier::textDocument );
}

void DidOpenTextDocumentParams::validate( const Data& data )
{
    static const auto context = CONTENT + " DidOpenTextDocumentParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< TextDocumentItem >( context, data, Identifier::textDocument, false );
}

//
//
// TextDocumentContentChangeEvent
//

TextDocumentContentChangeEvent::TextDocumentContentChangeEvent( const Data& data )
: Data( data )
{
    validate( data );
}

TextDocumentContentChangeEvent::TextDocumentContentChangeEvent( const std::string& text )
: Data( Data::object() )
{
    operator[]( Identifier::text ) = text;
}

u1 TextDocumentContentChangeEvent::hasRange( void ) const
{
    return find( Identifier::range ) != end();
}

Range TextDocumentContentChangeEvent::range( void ) const
{
    return at( Identifier::range );
}

void TextDocumentContentChangeEvent::setRange( const Range& range )
{
    operator[]( Identifier::range ) = Data::from_cbor( Data::to_cbor( range ) );
}

u1 TextDocumentContentChangeEvent::hasRangeLength( void ) const
{
    return find( Identifier::rangeLength ) != end();
}

std::size_t TextDocumentContentChangeEvent::rangeLength( void ) const
{
    return at( Identifier::rangeLength ).get< std::size_t >();
}

void TextDocumentContentChangeEvent::setRangeLength( const std::size_t rangeLength )
{
    operator[]( Identifier::rangeLength ) = rangeLength;
}

std::string TextDocumentContentChangeEvent::text( void ) const
{
    return at( Identifier::text ).get< std::string >();
}

void TextDocumentContentChangeEvent::validate( const Data& data )
{
    static const auto context = CONTENT + " TextDocumentContentChangeEvent:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::text, true );
    Json::validatePropertyIs< Range >( context, data, Identifier::range, false );
    Json::validatePropertyIsNumber( context, data, Identifier::rangeLength, false );
}

//
//
// DidChangeTextDocumentParams
//

DidChangeTextDocumentParams::DidChangeTextDocumentParams( const Data& data )
: Data( data )
{
    validate( data );
}

DidChangeTextDocumentParams::DidChangeTextDocumentParams(
    const VersionedTextDocumentIdentifier& textDocument,
    const TextDocumentContentChangeEvents& contentChanges )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument ) = Data::from_cbor( Data::to_cbor( textDocument ) );
    operator[]( Identifier::contentChanges ) = Data::array();

    for( auto contentChange : contentChanges )
    {
        operator[]( Identifier::contentChanges ).push_back( contentChange );
    }
}

VersionedTextDocumentIdentifier DidChangeTextDocumentParams::textDocument( void ) const
{
    return at( Identifier::textDocument );
}

TextDocumentContentChangeEvents DidChangeTextDocumentParams::contentChanges( void ) const
{
    auto contentChanges = at( Identifier::contentChanges );
    auto result = TextDocumentContentChangeEvents();
    for( auto change : contentChanges )
    {
        result.emplace_back( change );
    }
    return result;
}

void DidChangeTextDocumentParams::validate( const Data& data )
{
    static const auto context = CONTENT + " DidChangeTextDocumentParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< VersionedTextDocumentIdentifier >(
        context, data, Identifier::textDocument, true );
    Json::validatePropertyIsArrayOf< TextDocumentContentChangeEvent >(
        context, data, Identifier::contentChanges, true );
}

//
//
// WillSaveTextDocumentParams
//

WillSaveTextDocumentParams::WillSaveTextDocumentParams( const Data& data )
: Data( data )
{
    validate( data );
}

WillSaveTextDocumentParams::WillSaveTextDocumentParams(
    const TextDocumentIdentifier& textDocument, TextDocumentSaveReason reason )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument ) = Data::from_cbor( Data::to_cbor( textDocument ) );
    operator[]( Identifier::reason ) = static_cast< std::size_t >( reason );
}

TextDocumentSaveReason WillSaveTextDocumentParams::reason( void ) const
{
    return static_cast< TextDocumentSaveReason >( operator[]( Identifier::reason )
                                                      .get< std::size_t >() );
}

TextDocumentIdentifier WillSaveTextDocumentParams::textDocument( void ) const
{
    return operator[]( Identifier::textDocument );
}

void WillSaveTextDocumentParams::validate( const Data& data )
{
    static const auto context = CONTENT + " WillSaveTextDocumentParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< TextDocumentIdentifier >(
        context, data, Identifier::textDocument, true );
    Json::validatePropertyIsNumber( context, data, Identifier::reason, true );
}

//
//
// WillSaveWaitUntilResponse
//

WillSaveWaitUntilResponse::WillSaveWaitUntilResponse( const Data& data )
: Data( data )
{
    validate( data );
}

WillSaveWaitUntilResponse::WillSaveWaitUntilResponse( const std::vector< TextEdit >& textEdit )
: Data( Data::object() )
{
    operator[]( Identifier::textEdit ) = Data::array();
    for( auto edit : textEdit )
    {
        operator[]( Identifier::textEdit ).push_back( edit );
    }
}

TextEdits WillSaveWaitUntilResponse::textEdit( void ) const
{
    auto textEdits = operator[]( Identifier::textEdit );
    auto result = TextEdits();
    for( auto edit : textEdits )
    {
        result.push_back( edit );
    }
    return result;
}

void WillSaveWaitUntilResponse::validate( const Data& data )
{
    static const auto context = CONTENT + " WillSaveWaitUntilResponse:";
    Json::validateTypeIsObject( context, data );
    if( data.find( Identifier::edit ) != data.end() and data[ Identifier::edit ].is_null() )
    {
        // ok, due to the possibility that the property can be null
    }
    else
    {
        Json::validatePropertyIsArrayOf< TextEdit >( context, data, Identifier::edit, true );
    }
}

//
//
// DidSaveTextDocumentParams
//

DidSaveTextDocumentParams::DidSaveTextDocumentParams( const Data& data )
: Data( data )
{
    validate( data );
}

DidSaveTextDocumentParams::DidSaveTextDocumentParams( const TextDocumentIdentifier& textDocument )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument ) = Data::from_cbor( Data::to_cbor( textDocument ) );
}

u1 DidSaveTextDocumentParams::hasText( void ) const
{
    return find( Identifier::text ) != end();
}

void DidSaveTextDocumentParams::setText( const std::string& text )
{
    operator[]( Identifier::text ) = text;
}

TextDocumentIdentifier DidSaveTextDocumentParams::textDocument( void ) const
{
    return TextDocumentIdentifier( operator[]( Identifier::textDocument ) );
}

std::string DidSaveTextDocumentParams::text( void ) const
{
    return at( Identifier::text ).get< std::string >();
}

void DidSaveTextDocumentParams::validate( const Data& data )
{
    static const auto context = CONTENT + " DidSaveTextDocumentParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< TextDocumentIdentifier >(
        context, data, Identifier::textDocument, true );
    Json::validatePropertyIsString( context, data, Identifier::text, false );
}

//
//
// DidCloseTextDocumentParams
//

DidCloseTextDocumentParams::DidCloseTextDocumentParams( const TextDocumentIdentifier& textDocument )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument ) = Data::from_cbor( Data::to_cbor( textDocument ) );
}

DidCloseTextDocumentParams::DidCloseTextDocumentParams( const Data& data )
: Data( data )
{
    validate( data );
}

TextDocumentIdentifier DidCloseTextDocumentParams::textDocument( void ) const
{
    return operator[]( Identifier::textDocument );
}

void DidCloseTextDocumentParams::validate( const Data& data )
{
    static const auto context = CONTENT + " DidCloseTextDocumentParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< TextDocumentIdentifier >(
        context, data, Identifier::textDocument, true );
}

//
//
// CompletionContext
//

CompletionContext::CompletionContext( const Data& data )
: Data( data )
{
    validate( data );
}

CompletionContext::CompletionContext( const CompletionTriggerKind triggerkind )
: Data( Data::object() )
{
    operator[]( Identifier::triggerKind ) = static_cast< std::size_t >( triggerkind );
}

std::string CompletionContext::triggerCharacter( void ) const
{
    return at( Identifier::triggerCharacter );
}

void CompletionContext::setTriggerCharacter( const std::string& triggerCharacter )
{
    operator[]( Identifier::triggerCharacter ) = triggerCharacter;
}

u1 CompletionContext::hasTriggerCharacter( void ) const
{
    return find( Identifier::triggerCharacter ) != end();
}

CompletionTriggerKind CompletionContext::triggerKind( void ) const
{
    return static_cast< CompletionTriggerKind >( operator[]( Identifier::triggerKind )
                                                     .get< std::size_t >() );
}

void CompletionContext::validate( const Data& data )
{
    static const auto context = CONTENT + " CompletionContext:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsNumber( context, data, Identifier::triggerKind, true );
    Json::validatePropertyIsString( context, data, Identifier::triggerCharacter, false );
}

//
//
//  CompletionParams
//

CompletionParams::CompletionParams( const Data& data )
: TextDocumentPositionParams( data )
{
    validate( data );
}

CompletionParams::CompletionParams(
    const TextDocumentIdentifier& textDocument, const Position& position )
: TextDocumentPositionParams( textDocument, position )
{
}

CompletionContext CompletionParams::context( void ) const
{
    return at( Identifier::context );
}

u1 CompletionParams::hasContext( void ) const
{
    return find( Identifier::context ) != end();
}

void CompletionParams::setContext( const CompletionContext& context )
{
    operator[]( Identifier::context ) = Data::from_cbor( Data::to_cbor( context ) );
}

void CompletionParams::validate( const Data& data )
{
    static const auto context = CONTENT + " CompletionParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< CompletionContext >( context, data, Identifier::context, false );
}

//
//
//  CompletionItem
//

CompletionItem::CompletionItem( const Data& data )
: Data( data )
{
    validate( data );
}

CompletionItem::CompletionItem( const std::string& label )
: Data( Data::object() )
{
    operator[]( Identifier::label ) = label;
}

std::string CompletionItem::label( void ) const
{
    return operator[]( Identifier::label ).get< std::string >();
}

u1 CompletionItem::hasKind( void ) const
{
    return find( Identifier::kind ) != end();
}

void CompletionItem::setKind( const CompletionItemKind kind )
{
    operator[]( Identifier::kind ) = static_cast< std::size_t >( kind );
}

CompletionItemKind CompletionItem::kind( void ) const
{
    return static_cast< CompletionItemKind >( at( Identifier::kind ).get< std::size_t >() );
}

u1 CompletionItem::hasDetail( void ) const
{
    return find( Identifier::detail ) != end();
}

void CompletionItem::setDetail( const std::string& detail )
{
    operator[]( Identifier::detail ) = detail;
}

std::string CompletionItem::detail( void ) const
{
    return at( Identifier::detail ).get< std::string >();
}

u1 CompletionItem::hasDocumentation( void ) const
{
    return find( Identifier::documentation ) != end();
}

void CompletionItem::setDocumentation( const MarkupContent& doc )
{
    operator[]( Identifier::documentation ) = Data::from_cbor( Data::to_cbor( doc ) );
    ;
}

MarkupContent CompletionItem::documentation( void ) const
{
    return at( Identifier::documentation );
}

u1 CompletionItem::hasDeprecated( void ) const
{
    return find( Identifier::deprecated ) != end();
}

void CompletionItem::setDeprecated( const u1 deprecated )
{
    operator[]( Identifier::deprecated ) = deprecated;
}

u1 CompletionItem::isDeprecated( void ) const
{
    return at( Identifier::deprecated ).get< u1 >();
}

u1 CompletionItem::hasPreselected( void ) const
{
    return find( Identifier::preselect ) != end();
}

void CompletionItem::setPreselected( const u1 preselected )
{
    operator[]( Identifier::preselect ) = preselected;
}

u1 CompletionItem::isPreselected( void ) const
{
    return at( Identifier::preselect ).get< u1 >();
}

u1 CompletionItem::hasSortText( void ) const
{
    return find( Identifier::sortText ) != end();
}

void CompletionItem::setSortText( const std::string& sortText )
{
    operator[]( Identifier::sortText ) = sortText;
}

std::string CompletionItem::sortText( void ) const
{
    return at( Identifier::sortText ).get< std::string >();
}

u1 CompletionItem::hasFilterText( void ) const
{
    return find( Identifier::filterText ) != end();
}

void CompletionItem::setFilterText( const std::string& filterText )
{
    operator[]( Identifier::filterText ) = filterText;
}

std::string CompletionItem::filterText( void ) const
{
    return at( Identifier::filterText ).get< std::string >();
}

u1 CompletionItem::hasInsertText( void ) const
{
    return find( Identifier::insertText ) != end();
}

void CompletionItem::setInsertText( const std::string& insertText )
{
    operator[]( Identifier::insertText ) = insertText;
}

std::string CompletionItem::insertText( void ) const
{
    return at( Identifier::insertText ).get< std::string >();
}

u1 CompletionItem::hasInsertTextFormat( void ) const
{
    return find( Identifier::insertTextFormat ) != end();
}

void CompletionItem::setInsertTextFormat( const InsertTextFormat insertTextFormat )
{
    operator[]( Identifier::insertTextFormat ) = static_cast< std::size_t >( insertTextFormat );
}

InsertTextFormat CompletionItem::insertTextFormat( void ) const
{
    return static_cast< InsertTextFormat >(
        at( Identifier::insertTextFormat ).get< std::size_t >() );
}

u1 CompletionItem::hasTextEdit( void ) const
{
    return find( Identifier::textEdit ) != end();
}

void CompletionItem::setTextEdit( const TextEdit& textEdit )
{
    operator[]( Identifier::textEdit ) = Data::from_cbor( Data::to_cbor( textEdit ) );
}

TextEdit CompletionItem::textEdit( void ) const
{
    return at( Identifier::textEdit );
}

u1 CompletionItem::hasAdditionalTextEdits( void ) const
{
    return find( Identifier::additionalTextEdits ) != end();
}

void CompletionItem::addAdditionalTextEdit( const TextEdit& textEdit )
{
    if( not CompletionItem::hasAdditionalTextEdits() )
    {
        operator[]( Identifier::additionalTextEdits ) = Data::array();
    }

    operator[]( Identifier::additionalTextEdits ).push_back( textEdit );
}

TextEdits CompletionItem::additionalTextEdits( void ) const
{
    auto textEdits = at( Identifier::additionalTextEdits );
    auto vector = std::vector< TextEdit >();

    for( auto textEdit : textEdits )
    {
        vector.emplace_back( textEdit );
    }
    return vector;
}

u1 CompletionItem::hasCommitCharacters( void ) const
{
    return find( Identifier::commitCharacters ) != end();
}

void CompletionItem::addCommitCharacter( const std::string& commitCharacter )
{
    if( not CompletionItem::hasCommitCharacters() )
    {
        operator[]( Identifier::commitCharacters ) = Data::array();
    }

    operator[]( Identifier::commitCharacters ).push_back( commitCharacter );
}

CommitCharacters CompletionItem::commitCharacters( void ) const
{
    auto commitCharacters = at( Identifier::commitCharacters );
    auto vector = std::vector< std::string >();

    for( auto character : commitCharacters )
    {
        vector.emplace_back( character.get< std::string >() );
    }
    return vector;
}

u1 CompletionItem::hasCommand( void ) const
{
    return find( Identifier::command ) != end();
}

void CompletionItem::setCommand( const Command& command )
{
    operator[]( Identifier::command ) = Data::from_cbor( Data::to_cbor( command ) );
}

Command CompletionItem::command( void ) const
{
    return Command( at( Identifier::command ) );
}

u1 CompletionItem::hasData( void ) const
{
    return find( Identifier::data ) != end();
}

void CompletionItem::setData( const Data& data )
{
    operator[]( Identifier::data ) = data;
}

Data CompletionItem::data( void ) const
{
    return at( Identifier::data );
}

void CompletionItem::validate( const Data& data )
{
    static const auto context = CONTENT + " CompletionItem:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::label, true );
    Json::validatePropertyIsNumber( context, data, Identifier::kind, false );
    Json::validatePropertyIsString( context, data, Identifier::detail, false );
    Json::validatePropertyIs< MarkupContent >( context, data, Identifier::documentation, false );
    Json::validatePropertyIsBoolean( context, data, Identifier::deprecated, false );
    Json::validatePropertyIsBoolean( context, data, Identifier::preselect, false );
    Json::validatePropertyIsString( context, data, Identifier::sortText, false );
    Json::validatePropertyIsString( context, data, Identifier::filterText, false );
    Json::validatePropertyIsString( context, data, Identifier::insertText, false );
    Json::validatePropertyIsNumber( context, data, Identifier::insertTextFormat, false );
    Json::validatePropertyIs< TextEdit >( context, data, Identifier::textEdit, false );
    Json::validatePropertyIsArrayOf< TextEdit >(
        context, data, Identifier::additionalTextEdits, false );
    Json::validatePropertyIsArrayOfString( context, data, Identifier::commitCharacters, false );
    Json::validatePropertyIs< Command >( context, data, Identifier::command, false );
    Json::validatePropertyIsObject( context, data, Identifier::data, false );
}

//
//
// CompletionList
//

CompletionList::CompletionList( const Data& data )
: Data( data )
{
    validate( data );
}

CompletionList::CompletionList( const u1 isIncomplete, const CompletionItems& items )
: Data( Data::object() )
{
    operator[]( Identifier::isIncomplete ) = isIncomplete;
    operator[]( Identifier::items ) = Data::array();
    for( auto item : items )
    {
        operator[]( Identifier::items ).push_back( item );
    }
}

CompletionItems CompletionList::items( void ) const
{
    auto result = CompletionItems();
    for( auto item : operator[]( Identifier::items ) )
    {
        result.emplace_back( item );
    }
    return result;
}

u1 CompletionList::isIncomplete( void ) const
{
    return operator[]( Identifier::isIncomplete ).get< u1 >();
}

void CompletionList::validate( const Data& data )
{
    static const auto context = CONTENT + " CompletionList:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsArrayOf< CompletionItem >( context, data, Identifier::items, true );
    Json::validatePropertyIsBoolean( context, data, Identifier::isIncomplete, true );
}

//
//
// CompletionResult
//

CompletionResult::CompletionResult( void )
: Data( Data() )
{
}

CompletionResult::CompletionResult( const Data& data )
: Data( data )
{
    validate( data );
}

CompletionResult::CompletionResult( const CompletionList& list )
: Data( Data::from_cbor( Data::to_cbor( list ) ) )
{
}

CompletionResult::CompletionResult( const CompletionItems& items )
: Data( Data::object() )
{
    operator[]( Identifier::isIncomplete ) = false;
    operator[]( Identifier::items ) = Data::array();
    for( auto item : items )
    {
        operator[]( Identifier::items ).push_back( item );
    }
}

void CompletionResult::validate( const Data& data )
{
    if( data.is_null() )
    {
        // ok, do nothing.
    }
    else
    {
        CompletionList::validate( data );
    }
}

//
//
// CompletionRegistrationOptions
//

CompletionRegistrationOptions::CompletionRegistrationOptions( const Data& data )
: TextDocumentRegistrationOptions( data )
{
    validate( data );
}

CompletionRegistrationOptions::CompletionRegistrationOptions(
    const DocumentSelector& documentSelector )
: TextDocumentRegistrationOptions( documentSelector )
{
    // leave empty, only call parent ctor
}

u1 CompletionRegistrationOptions::hasTriggerCharacters( void ) const
{
    return find( Identifier::triggerCharacters ) != end();
}

TriggerCharacters CompletionRegistrationOptions::triggerCharacters( void ) const
{
    auto triggerCharacters = at( Identifier::triggerCharacters );
    auto result = TriggerCharacters();
    for( auto triggerCharacter : triggerCharacters )
    {
        result.push_back( triggerCharacter );
    }
    return result;
}

void CompletionRegistrationOptions::addTriggerCharacter( const std::string& triggerCharacter )
{
    if( not CompletionRegistrationOptions::hasTriggerCharacters() )
    {
        operator[]( Identifier::triggerCharacters ) = Data::array();
    }
    operator[]( Identifier::triggerCharacters ).push_back( triggerCharacter );
}

u1 CompletionRegistrationOptions::hasResolveProvider( void ) const
{
    return find( Identifier::resolveProvider ) != end();
}

u1 CompletionRegistrationOptions::resolveProvider( void ) const
{
    return at( Identifier::resolveProvider ).get< u1 >();
}

void CompletionRegistrationOptions::setResolveProvider( const u1 resolveProvider )
{
    operator[]( Identifier::resolveProvider ) = resolveProvider;
}

void CompletionRegistrationOptions::validate( const Data& data )
{
    static const auto context = CONTENT + " CompletionRegistrationOptions:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsArrayOfString( context, data, Identifier::triggerCharacters, false );
    Json::validatePropertyIsBoolean( context, data, Identifier::resolveProvider, false );
}

//
//
// MarkupContent
//

MarkupContent::MarkupContent( const Data& data )
: Data( data )
{
    validate( data );
}

MarkupContent::MarkupContent( const MarkupKind kind, const std::string& value )
: Data( Data::object() )
{
    switch( kind )
    {
        case MarkupKind::PLAINTEXT:
        {
            operator[]( Identifier::kind ) = std::string( Identifier::plaintext );
            break;
        }
        case MarkupKind::MARKDOWN:
        {
            operator[]( Identifier::kind ) = std::string( Identifier::markdown );
            break;
        }
    }
    operator[]( Identifier::value ) = value;
}

MarkupContent::MarkupContent( const std::string& value )
: MarkupContent( MarkupKind::PLAINTEXT, value )
{
}

std::string MarkupContent::kind( void ) const
{
    return operator[]( Identifier::kind ).get< std::string >();
}

std::string MarkupContent::value( void ) const
{
    return operator[]( Identifier::value ).get< std::string >();
}

void MarkupContent::validate( const Data& data )
{
    static const auto context = CONTENT + " MarkupContent:";
    if( data.is_object() )
    {
        Json::validateTypeIsObject( context, data );
        Json::validatePropertyIsString( context, data, Identifier::value, true );
        Json::validatePropertyIsString( context, data, Identifier::kind, true );
    }
    else if( data.is_string() )
    {
        Json::validateTypeIsString( context, data );
    }
}

//
//
// ParameterInformation
//

ParameterInformation::ParameterInformation( const Data& data )
: Data( data )
{
    validate( data );
}

ParameterInformation::ParameterInformation( const std::string& label )
: Data( Data::object() )
{
    operator[]( Identifier::label ) = label;
}

std::string ParameterInformation::label( void ) const
{
    return operator[]( Identifier::label ).get< std::string >();
}

MarkupContent ParameterInformation::documentation( void ) const
{
    return MarkupContent( at( Identifier::documentation ) );
}

u1 ParameterInformation::hasDocumentation( void ) const
{
    return find( Identifier::documentation ) != end();
}

void ParameterInformation::setDocumentation( const MarkupContent& doc )
{
    operator[]( Identifier::documentation ) = Data::from_cbor( Data::to_cbor( doc ) );
}

void ParameterInformation::validate( const Data& data )
{
    static const auto context = CONTENT + " ParameterInformation:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::label, true );
    Json::validatePropertyIsString( context, data, Identifier::documentation, false );
}

//
//
// SignatureInformation
//

SignatureInformation::SignatureInformation( const Data& data )
: Data( data )
{
    validate( data );
}

SignatureInformation::SignatureInformation( const std::string& label )
: Data( Data::object() )
{
    operator[]( Identifier::label ) = label;
}

std::string SignatureInformation::label( void ) const
{
    return operator[]( Identifier::label ).get< std::string >();
}

MarkupContent SignatureInformation::documentation( void ) const
{
    return MarkupContent( at( Identifier::documentation ) );
}

u1 SignatureInformation::hasDocumentation( void ) const
{
    return find( Identifier::documentation ) != end();
}

void SignatureInformation::setDocumentation( const MarkupContent& doc )
{
    operator[]( Identifier::documentation ) = Data::from_cbor( Data::to_cbor( doc ) );
}

u1 SignatureInformation::hasParameters( void ) const
{
    return find( Identifier::parameters ) != end();
}

ParameterInformations SignatureInformation::parameters( void ) const
{
    auto result = ParameterInformations();
    for( auto parameter : at( Identifier::parameters ) )
    {
        result.emplace_back( parameter );
    }
    return result;
}

void SignatureInformation::setParameters( const ParameterInformations& parameters )
{
    operator[]( Identifier::parameters ) = Data::array();

    for( auto parameter : parameters )
    {
        operator[]( Identifier::parameters ).push_back( parameter );
    }
}

void SignatureInformation::validate( const Data& data )
{
    static const auto context = CONTENT + " SignatureInformation:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::label, true );
    Json::validatePropertyIsString( context, data, Identifier::documentation, false );
    Json::validatePropertyIsArrayOf< ParameterInformation >(
        context, data, Identifier::parameters, false );
}

//
//
// SignatureHelp
//

SignatureHelp::SignatureHelp( const Data& data )
: Data( data )
{
    validate( data );
}

SignatureHelp::SignatureHelp( const SignatureInformations& signatures )
: Data( Data::object() )
{
    operator[]( Identifier::signatures ) = Data::array();
    for( auto signature : signatures )
    {
        operator[]( Identifier::signatures ).push_back( signature );
    }
}

SignatureInformations SignatureHelp::signatures( void ) const
{
    auto result = SignatureInformations();
    for( auto information : operator[]( Identifier::signatures ) )
    {
        result.emplace_back( information );
    }
    return result;
}

u1 SignatureHelp::hasActiveSignature( void ) const
{
    return find( Identifier::activeSignature ) != end();
}

void SignatureHelp::setActiveSignature( const std::size_t activeSignature )
{
    operator[]( Identifier::activeSignature ) = activeSignature;
}

std::size_t SignatureHelp::activeSignature( void ) const
{
    return at( Identifier::activeSignature ).get< std::size_t >();
}

u1 SignatureHelp::hasActiveParameter( void ) const
{
    return find( Identifier::activeParameter ) != end();
}

void SignatureHelp::setActiveParameter( const std::size_t activeParameter )
{
    operator[]( Identifier::activeParameter ) = activeParameter;
}

std::size_t SignatureHelp::activeParameter( void ) const
{
    return at( Identifier::activeParameter ).get< std::size_t >();
}

void SignatureHelp::validate( const Data& data )
{
    static const auto context = CONTENT + " SignatureHelp:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsArrayOf< SignatureInformation >(
        context, data, Identifier::signatures, true );
    Json::validatePropertyIsNumber( context, data, Identifier::activeSignature, false );
    Json::validatePropertyIsNumber( context, data, Identifier::activeParameter, false );
}

//
//
// SignatureHelpResult
//

SignatureHelpResult::SignatureHelpResult( void )
: Data( Data() )
{
}

SignatureHelpResult::SignatureHelpResult( const Data& data )
: Data( data )
{
    validate( data );
}

SignatureHelpResult::SignatureHelpResult( const SignatureHelp& signature )
: Data( Data::from_cbor( Data::to_cbor( signature ) ) )
{
}

void SignatureHelpResult::validate( const Data& data )
{
    if( data.is_null() )
    {
        // ok, do nothing
    }
    else
    {
        SignatureHelp::validate( data );
    }
}

//
//
// LocationLink
//

LocationLink::LocationLink( const Data& data )
: Data( data )
{
    validate( data );
}

LocationLink::LocationLink( const DocumentUri& targetUri, const Range& targetRange )
: Data( Data::object() )
{
    operator[]( Identifier::targetUri ) = targetUri.toString();
    operator[]( Identifier::targetRange ) = Data::from_cbor( Data::to_cbor( targetRange ) );
}

DocumentUri LocationLink::targetUri( void ) const
{
    return DocumentUri::fromString( operator[]( Identifier::targetUri ).get< std::string >() );
}

Range LocationLink::targetRange( void ) const
{
    return operator[]( Identifier::targetRange );
}

u1 LocationLink::hasOriginSelectionRange( void ) const
{
    return find( Identifier::originSelectionRange ) != end();
}

void LocationLink::setOriginSelectionRange( const Range& range )
{
    operator[]( Identifier::originSelectionRange ) = Data::from_cbor( Data::to_cbor( range ) );
}

Range LocationLink::originSelectionRange( void ) const
{
    return operator[]( Identifier::originSelectionRange );
}

u1 LocationLink::hasTargetSelectionRange( void ) const
{
    return find( Identifier::targetSelectionRange ) != end();
}

void LocationLink::setTargetSelectionRange( const Range& range )
{
    operator[]( Identifier::targetSelectionRange ) = Data::from_cbor( Data::to_cbor( range ) );
}

Range LocationLink::targetSelectionRange( void ) const
{
    return operator[]( Identifier::targetSelectionRange );
}

void LocationLink::validate( const Data& data )
{
    static const auto context = CONTENT + " LocationLink:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::targetUri, true );
    Json::validatePropertyIs< Range >( context, data, Identifier::targetRange, true );
    Json::validatePropertyIs< Range >( context, data, Identifier::originSelectionRange, false );
    Json::validatePropertyIs< Range >( context, data, Identifier::targetSelectionRange, false );
}

//
//
// TextDocumentSaveRegistrationOptions
//

TextDocumentSaveRegistrationOptions::TextDocumentSaveRegistrationOptions( const Data& data )
: TextDocumentRegistrationOptions( data )
{
    validate( data );
}

TextDocumentSaveRegistrationOptions::TextDocumentSaveRegistrationOptions(
    const DocumentSelector& documentSelector )
: TextDocumentRegistrationOptions( documentSelector )
{
    // leave empty, call parent ctor
}

u1 TextDocumentSaveRegistrationOptions::hasIncludeText( void ) const
{
    return find( Identifier::includeText ) != end();
}

void TextDocumentSaveRegistrationOptions::setIncludeText( const u1 includeText )
{
    operator[]( Identifier::includeText ) = includeText;
}

u1 TextDocumentSaveRegistrationOptions::includeText( void ) const
{
    return at( Identifier::includeText ).get< u1 >();
}

void TextDocumentSaveRegistrationOptions::validate( const Data& data )
{
    static const auto context = CONTENT + " TextDocumentSaveRegistrationOptions:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsBoolean( context, data, Identifier::includeText, false );
}

//
//
// SignatureHelpRegistrationOptions
//

SignatureHelpRegistrationOptions::SignatureHelpRegistrationOptions( const Data& data )
: TextDocumentRegistrationOptions( data )
{
    validate( data );
}

SignatureHelpRegistrationOptions::SignatureHelpRegistrationOptions(
    const DocumentSelector& documentSelector )
: TextDocumentRegistrationOptions( documentSelector )
{
}

u1 SignatureHelpRegistrationOptions::hasTriggerCharacters( void ) const
{
    return find( Identifier::triggerCharacters ) != end();
}

void SignatureHelpRegistrationOptions::setTriggerCharacters(
    const TriggerCharacters& triggerCharacters )
{
    operator[]( Identifier::triggerCharacters ) = Data::array();
    for( auto character : triggerCharacters )
    {
        operator[]( Identifier::triggerCharacters ).push_back( character );
    }
}

void SignatureHelpRegistrationOptions::addTriggerCharacter( const std::string& triggerCharacter )
{
    if( not SignatureHelpRegistrationOptions::hasTriggerCharacters() )
    {
        operator[]( Identifier::triggerCharacters ) = Data::array();
    }
    operator[]( Identifier::triggerCharacters ).push_back( triggerCharacter );
}

TriggerCharacters SignatureHelpRegistrationOptions::triggerCharacters( void ) const
{
    auto vector = std::vector< std::string >();
    auto triggerCharacters = at( Identifier::triggerCharacters );
    for( auto triggerCharacter : triggerCharacters )
    {
        vector.emplace_back( triggerCharacter.get< std::string >() );
    }
    return vector;
}

void SignatureHelpRegistrationOptions::validate( const Data& data )
{
    static const auto context = CONTENT + " SignatureHelpRegistrationOptions:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsArrayOfString( context, data, Identifier::triggerCharacters, false );
}

//
//
// TypeDefinitionResult
//

TypeDefinitionResult::TypeDefinitionResult( const Data& data )
: Data( data )
{
    validate( data );
}

TypeDefinitionResult::TypeDefinitionResult( const Locations& locations )
: Data( Data::array() )
{
    for( auto location : locations )
    {
        push_back( location );
    }
}

TypeDefinitionResult::TypeDefinitionResult( const LocationLinks& locationlinks )
: Data( Data::array() )
{
    for( auto link : locationlinks )
    {
        push_back( link );
    }
}

TypeDefinitionResult::TypeDefinitionResult( const Location& location )
{
    *this = Data::from_cbor( Data::to_cbor( location ) );
}

void TypeDefinitionResult::validate( const Data& data )
{
    static const auto context = CONTENT + " TypeDefinitionResult:";

    if( data.is_null() )
    {
        // ok, do nothing
    }
    else
    {
        if( data.is_array() )
        {
            Json::validateTypeIsArrayOf< Location, LocationLink >( context, data );
        }
        else
        {
            Json::validateTypeIsObject( context, data );
            Location::validate( data );
        }
    }
}

//
//
// ReferenceContext
//

ReferenceContext::ReferenceContext( const Data& data )
: Data( data )
{
    validate( data );
}

ReferenceContext::ReferenceContext( const u1 includeDeclaration )
: Data( Data::object() )
{
    operator[]( Identifier::includeDeclaration ) = includeDeclaration;
}

u1 ReferenceContext::includeDeclaration( void ) const
{
    return operator[]( Identifier::includeDeclaration ).get< u1 >();
}

void ReferenceContext::validate( const Data& data )
{
    static const auto context = CONTENT + " ReferenceContext:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsBoolean( context, data, Identifier::includeDeclaration, true );
}

//
//
// ReferenceParams
//

ReferenceParams::ReferenceParams( const Data& data )
: TextDocumentPositionParams( data )
{
    validate( data );
}

ReferenceParams::ReferenceParams(
    const TextDocumentIdentifier& textDocument,
    const Position& position,
    const ReferenceContext& context )
: TextDocumentPositionParams( textDocument, position )
{
    operator[]( Identifier::context ) = Data::from_cbor( Data::to_cbor( context ) );
}

ReferenceContext ReferenceParams::context( void ) const
{
    return operator[]( Identifier::context );
}

void ReferenceParams::validate( const Data& data )
{
    static const auto context = CONTENT + " ReferenceParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< ReferenceContext >( context, data, Identifier::context, true );
}

//
//
// ReferenceResult
//

ReferenceResult::ReferenceResult( const Data& data )
: Data( data )
{
    validate( data );
}

ReferenceResult::ReferenceResult( const Locations& locations )
: Data( Data::array() )
{
    for( auto location : locations )
    {
        push_back( location );
    }
}

void ReferenceResult::validate( const Data& data )
{
    static const auto context = CONTENT + " ReferenceResult:";
    if( data.is_null() )
    {
        // ok, do nothing
    }
    else
    {
        Json::validateTypeIsArrayOf< Location >( context, data );
    }
}

//
//
// CodeActionOptions
//

CodeActionOptions::CodeActionOptions( const Data& data )
: Data( data )
{
    validate( data );
}

u1 CodeActionOptions::hasCodeActionKinds( void ) const
{
    return find( Identifier::codeActionKinds ) != end();
}

void CodeActionOptions::addCodeActionKind( CodeActionKind kind )
{
    if( not hasCodeActionKinds() )
    {
        operator[]( Identifier::codeActionKinds ) = Data::array();
    }
    switch( kind )
    {
        case CodeActionKind::QuickFix:
        {
            operator[]( Identifier::codeActionKinds ).push_back( Identifier::quickfix );
            break;
        }
        case CodeActionKind::Refactor:
        {
            operator[]( Identifier::codeActionKinds ).push_back( Identifier::refactor );
            break;
        }
        case CodeActionKind::RefactorExtract:
        {
            operator[]( Identifier::codeActionKinds ).push_back( Identifier::refactorExtract );
            break;
        }
        case CodeActionKind::RefactorInline:
        {
            operator[]( Identifier::codeActionKinds ).push_back( Identifier::refactorInline );
            break;
        }
        case CodeActionKind::RefactorRewrite:
        {
            operator[]( Identifier::codeActionKinds ).push_back( Identifier::refactorRewrite );
            break;
        }
        case CodeActionKind::Source:
        {
            operator[]( Identifier::codeActionKinds ).push_back( Identifier::source );
            break;
        }
        case CodeActionKind::SourceOrganizeImports:
        {
            operator[]( Identifier::codeActionKinds )
                .push_back( Identifier::sourceOrganizeImports );
            break;
        }
    }
}

std::vector< std::string > CodeActionOptions::codeActionKinds( void ) const
{
    auto result = std::vector< std::string >();
    for( auto kind : at( Identifier::codeActionKinds ) )
    {
        result.push_back( kind );
    }
    return result;
}

void CodeActionOptions::validate( const Data& data )
{
    static const auto context = CONTENT + " CodeActionOptions:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsArrayOfString( context, data, Identifier::codeActionKinds, false );
}

//
//
// CodeActionContext
//

CodeActionContext::CodeActionContext( const Data& data )
: Data( data )
{
    validate( data );
}

CodeActionContext::CodeActionContext( const std::vector< Diagnostic >& diagnostics )
: Data( Data::object() )
{
    operator[]( Identifier::diagnostics ) = Data::array();

    for( auto diagnostic : diagnostics )
    {
        operator[]( Identifier::diagnostics ).push_back( diagnostic );
    }
}

u1 CodeActionContext::hasKind( void ) const
{
    return find( Identifier::kind ) != end();
}

void CodeActionContext::setKind( const CodeActionKind kind )
{
    switch( kind )
    {
        case CodeActionKind::QuickFix:
        {
            operator[]( Identifier::kind ) = Identifier::quickfix;
            break;
        }
        case CodeActionKind::Refactor:
        {
            operator[]( Identifier::kind ) = Identifier::refactor;
            break;
        }
        case CodeActionKind::RefactorExtract:
        {
            operator[]( Identifier::kind ) = Identifier::refactorExtract;
            break;
        }
        case CodeActionKind::RefactorInline:
        {
            operator[]( Identifier::kind ) = Identifier::refactorInline;
            break;
        }
        case CodeActionKind::RefactorRewrite:
        {
            operator[]( Identifier::kind ) = Identifier::refactorRewrite;
            break;
        }
        case CodeActionKind::Source:
        {
            operator[]( Identifier::kind ) = Identifier::source;
            break;
        }
        case CodeActionKind::SourceOrganizeImports:
        {
            operator[]( Identifier::kind ) = Identifier::sourceOrganizeImports;
            break;
        }
    }
}

std::string CodeActionContext::kind( void ) const
{
    return at( Identifier::kind ).get< std::string >();
}

Diagnostics CodeActionContext::diagnostics( void ) const
{
    auto result = Diagnostics();
    for( auto diagnostic : at( Identifier::diagnostics ) )
    {
        result.push_back( diagnostic );
    }
    return result;
}

void CodeActionContext::validate( const Data& data )
{
    static const auto context = CONTENT + " CodeActionContext:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsArrayOf< TextDocumentContentChangeEvent >(
        context, data, Identifier::diagnostics, true );
}

//
//
// CodeActionParams
//

CodeActionParams::CodeActionParams( const Data& data )
: Data( data )
{
    validate( data );
}

CodeActionParams::CodeActionParams(
    const TextDocumentIdentifier& textDocument,
    const Range& range,
    const CodeActionContext& context )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument ) = Data::from_cbor( Data::to_cbor( textDocument ) );
    operator[]( Identifier::range ) = Data::from_cbor( Data::to_cbor( range ) );
    operator[]( Identifier::context ) = Data::from_cbor( Data::to_cbor( context ) );
}

TextDocumentIdentifier CodeActionParams::textDocument( void ) const
{
    return at( Identifier::textDocument );
}

Range CodeActionParams::range( void ) const
{
    return at( Identifier::range );
}

CodeActionContext CodeActionParams::context( void ) const
{
    return at( Identifier::context );
}

void CodeActionParams::validate( const Data& data )
{
    static const auto context = CONTENT + " CodeActionParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< TextDocumentIdentifier >(
        context, data, Identifier::textDocument, true );
    Json::validatePropertyIs< Range >( context, data, Identifier::range, true );
    Json::validatePropertyIs< CodeActionContext >( context, data, Identifier::context, true );
}

//
//
// CodeActionResult
//

CodeActionResult::CodeActionResult( const Data& data )
: Data( data )
{
    validate( data );
}

CodeActionResult::CodeActionResult( const std::vector< Command >& commands )
: CodeActionResult()
{
    for( auto command : commands )
    {
        addCommand( command );
    }
}

CodeActionResult::CodeActionResult( void )
: Data( Data::array() )
{
}

void CodeActionResult::addCommand( const Command& command )
{
    this->push_back( command );
}

void CodeActionResult::validate( const Data& data )
{
    static const auto context = CONTENT + " CodeActionResult:";
    Json::validateTypeIsMixedArrayOf< Command, CodeAction >( context, data );
}

//
//
// CodeAction
//

CodeAction::CodeAction( const Data& data )
: Data( Data::object() )
{
    validate( data );
}

CodeAction::CodeAction( const std::string& title )
: Data( Data::object() )
{
    operator[]( Identifier::title ) = title;
}

std::string CodeAction::title( void ) const
{
    return operator[]( Identifier::title ).get< std::string >();
}

u1 CodeAction::hasKind( void ) const
{
    return find( Identifier::kind ) != end();
}

std::string CodeAction::kind( void ) const
{
    return at( Identifier::kind ).get< std::string >();
}

void CodeAction::setKind( const CodeActionKind kind )
{
    switch( kind )
    {
        case CodeActionKind::QuickFix:
        {
            operator[]( Identifier::kind ) = Identifier::quickfix;
            break;
        }
        case CodeActionKind::Refactor:
        {
            operator[]( Identifier::kind ) = Identifier::refactor;
            break;
        }
        case CodeActionKind::RefactorExtract:
        {
            operator[]( Identifier::kind ) = Identifier::refactorExtract;
            break;
        }
        case CodeActionKind::RefactorInline:
        {
            operator[]( Identifier::kind ) = Identifier::refactorInline;
            break;
        }
        case CodeActionKind::RefactorRewrite:
        {
            operator[]( Identifier::kind ) = Identifier::refactorRewrite;
            break;
        }
        case CodeActionKind::Source:
        {
            operator[]( Identifier::kind ) = Identifier::source;
            break;
        }
        case CodeActionKind::SourceOrganizeImports:
        {
            operator[]( Identifier::kind ) = Identifier::sourceOrganizeImports;
            break;
        }
    }
}

u1 CodeAction::hasDiagnostics( void ) const
{
    return find( Identifier::diagnostics ) != end();
}

Diagnostics CodeAction::diagnostics( void ) const
{
    auto result = Diagnostics();
    for( auto diagnostic : at( Identifier::diagnostics ) )
    {
        result.push_back( diagnostic );
    }
    return result;
}

void CodeAction::addDiagnostic( const Diagnostic& diagnostic )
{
    operator[]( Identifier::diagnostics ).push_back( diagnostic );
}

u1 CodeAction::hasEdit( void ) const
{
    return find( Identifier::edit ) != end();
}

WorkspaceEdit CodeAction::edit( void ) const
{
    return at( Identifier::edit );
}

void CodeAction::setEdit( const WorkspaceEdit& edit )
{
    operator[]( Identifier::edit ) = Data::from_cbor( Data::to_cbor( edit ) );
}

void CodeAction::setCommand( const Command& command )
{
    operator[]( Identifier::command ) = Data::from_cbor( Data::to_cbor( command ) );
}

u1 CodeAction::hasCommand( void ) const
{
    return find( Identifier::command ) != end();
}

Command CodeAction::command( void ) const
{
    return at( Identifier::command );
}

void CodeAction::validate( const Data& data )
{
    static const auto context = CONTENT + " CodeAction:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::title, true );
    Json::validatePropertyIsString( context, data, Identifier::kind, false );
    Json::validatePropertyIsArrayOf< Diagnostic >( context, data, Identifier::diagnostics, false );
    Json::validatePropertyIs< WorkspaceEdit >( context, data, Identifier::edit, false );
    Json::validatePropertyIs< Command >( context, data, Identifier::command, false );
}

//
//
// PublishDiagnosticsParams
//

PublishDiagnosticsParams::PublishDiagnosticsParams( const Data& data )
: Data( data )
{
    validate( data );
}

PublishDiagnosticsParams::PublishDiagnosticsParams(
    const DocumentUri& uri, const std::vector< Diagnostic >& diagnostics )
: Data( Data::object() )
{
    operator[]( Identifier::uri ) = uri.toString();
    operator[]( Identifier::diagnostics ) = Data::array();

    for( auto diagnostic : diagnostics )
    {
        operator[]( Identifier::diagnostics ).push_back( diagnostic );
    }
}

DocumentUri PublishDiagnosticsParams::uri( void ) const
{
    return DocumentUri::fromString( at( Identifier::uri ).get< std::string >() );
}

Data PublishDiagnosticsParams::diagnostics( void ) const
{
    return at( Identifier::diagnostics );
}

void PublishDiagnosticsParams::validate( const Data& data )
{
    static const auto context = CONTENT + " PublishDiagnosticsParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsUri( context, data, Identifier::uri, true );
    Json::validatePropertyIsArrayOf< Diagnostic >( context, data, Identifier::diagnostics, true );
}

//
//
// MarkedString
//

MarkedString::MarkedString( const Data& data )
: Data( data )
{
    validate( data );
}

MarkedString::MarkedString( const std::string& language, const std::string& value )
: Data( Data::object() )
{
    operator[]( Identifier::language ) = language;
    operator[]( Identifier::value ) = value;
}

std::string MarkedString::language( void ) const
{
    return at( Identifier::language ).get< std::string >();
}

std::string MarkedString::value( void ) const
{
    return at( Identifier::value ).get< std::string >();
}

void MarkedString::validate( const Data& data )
{
    if( data.is_string() )
    {
        return;
    }

    static const auto context = CONTENT + " MarkedString:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::language, true );
    Json::validatePropertyIsString( context, data, Identifier::value, true );
}

//
//
// HoverResult
//

HoverResult::HoverResult( const Data& data )
: Data( data )
{
    validate( data );
}

HoverResult::HoverResult( const std::vector< MarkedString >& contents )
: Data( Data::object() )
{
    operator[]( Identifier::contents ) = Data::array();

    for( auto content : contents )
    {
        addContent( content );
    }
}

Data HoverResult::contents( void ) const
{
    return at( Identifier::contents );
}

void HoverResult::addContent( const MarkedString& content )
{
    operator[]( Identifier::contents ).push_back( content );
}

u1 HoverResult::hasRange( void ) const
{
    return find( Identifier::range ) != end();
}

Range HoverResult::range( void ) const
{
    return at( Identifier::range );
}

void HoverResult::setRange( const Range& range )
{
    operator[]( Identifier::range ) = Data::from_cbor( Data::to_cbor( range ) );
}

void HoverResult::validate( const Data& data )
{
    if( data.is_null() )
    {
        // ok, do nothing.
    }
    else
    {
        static const auto context = CONTENT + " HoverResult:";
        Json::validateTypeIsObject( context, data );
        try
        {
            Json::validatePropertyIs< MarkedString >( context, data, Identifier::contents, true );
        }
        catch( std::invalid_argument a )
        {
            try
            {
                Json::validatePropertyIs< MarkupContent >(
                    context, data, Identifier::contents, true );
            }
            catch( std::invalid_argument a )
            {
                Json::validatePropertyIsArrayOf< MarkedString >(
                    context, data, Identifier::contents, true );
            }
        }
        Json::validatePropertyIs< Range >( context, data, Identifier::range, false );
    }
}

//
//
// DefinitionResult
//

DefinitionResult::DefinitionResult( const Data& data )
: Data( data )
{
    validate( data );
}

DefinitionResult::DefinitionResult( const Location& location )
: Data( Data::object() )
{
    operator[]( Identifier::location ) = location;
}

DefinitionResult::DefinitionResult( const std::vector< Location > locations )
: Data( Data::object() )
{
    operator[]( Identifier::location ) = Data::array();

    for( auto location : locations )
    {
        operator[]( Identifier::location ).push_back( location );
    }
}

Data DefinitionResult::locations( void ) const
{
    return operator[]( Identifier::location );
}

void DefinitionResult::validate( const Data& data )
{
    static const auto context = CONTENT + " DefinitionResult:";
    Json::validateTypeIsObject( context, data );
    if( data.find( Identifier::location ) != data.end() )
    {
        if( data[ Identifier::location ].is_null() )
        {
            // if it is null, do nothing
        }
        else
        {
            if( data[ Identifier::location ].is_array() )
            {
                // object is an Array
                Json::validatePropertyIsArrayOf< Location >(
                    context, data, Identifier::location, true );
            }
            else
            {
                // object is a single Location object
                Json::validatePropertyIs< Location >( context, data, Identifier::location, true );
            }
        }
    }
    else
    {
        // object does not contain a Location
        throw std::invalid_argument( context + " missing property 'Location' " );
    }
}

//
//
// DocumentHighlight
//

DocumentHighlight::DocumentHighlight( const Data& data )
: Data( data )
{
    validate( data );
}
DocumentHighlight::DocumentHighlight( const Range& range )
: Data( Data::object() )
{
    operator[]( Identifier::range ) = Data::from_cbor( Data::to_cbor( range ) );
}
Range DocumentHighlight::range( void ) const
{
    return operator[]( Identifier::range );
}
u1 DocumentHighlight::hasKind( void ) const
{
    return find( Identifier::kind ) != end();
}
DocumentHighlightKind DocumentHighlight::kind( void ) const
{
    return static_cast< DocumentHighlightKind >( operator[]( Identifier::kind )
                                                     .get< std::size_t >() );
}
void DocumentHighlight::setKind( const DocumentHighlightKind kind )
{
    operator[]( Identifier::kind ) = static_cast< std::size_t >( kind );
}
void DocumentHighlight::validate( const Data& data )
{
    static const auto context = CONTENT + " DocumentHighlight:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< Range >( context, data, Identifier::range, true );
    Json::validatePropertyIsNumber( context, data, Identifier::kind, false );
}

//
//
// DocumentHighlightResult
//

DocumentHighlightResult::DocumentHighlightResult( void )
: Data()
{
}

DocumentHighlightResult::DocumentHighlightResult( const Data& data )
: Data( data )
{
    validate( data );
}

DocumentHighlightResult::DocumentHighlightResult( const DocumentHighlights& highlights )
: Data( Data::array() )
{
    for( auto highlight : highlights )
    {
        push_back( highlight );
    }
}

void DocumentHighlightResult::validate( const Data& data )
{
    static const auto context = CONTENT + " DefinitionResult:";
    Json::validateTypeIsObject( context, data );
    if( data.is_null() )
    {
        // if it is null, do nothing
    }
    else
    {
        Json::validateTypeIsArrayOf< DocumentHighlight >( context, data );
    }
}

//
//
// DocumentSymbolParams
//

DocumentSymbolParams::DocumentSymbolParams( const TextDocumentIdentifier& textDocument )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument ) = Data::from_cbor( Data::to_cbor( textDocument ) );
}

DocumentSymbolParams::DocumentSymbolParams( const Data& data )
: Data( data )
{
    validate( data );
}

TextDocumentIdentifier DocumentSymbolParams::textDocument( void ) const
{
    return operator[]( Identifier::textDocument );
}

void DocumentSymbolParams::validate( const Data& data )
{
    static const auto context = CONTENT + " DocumentSymbolParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< TextDocumentIdentifier >(
        context, data, Identifier::textDocument, true );
}

//
//
// DocumentSymbolResult
//

DocumentSymbolResult::DocumentSymbolResult( void )
: Data()
{
}

DocumentSymbolResult::DocumentSymbolResult( const Data& data )
: Data( data )
{
    validate( data );
}

DocumentSymbolResult::DocumentSymbolResult( const DocumentSymbols& symbols )
: Data( Data::array() )
{
    push_back( symbols );
}

DocumentSymbolResult::DocumentSymbolResult( const SymbolInformations& information )
: Data( Data::array() )
{
    push_back( information );
}

void DocumentSymbolResult::validate( const Data& data )
{
    static const auto context = CONTENT + " DocumentSymbolResult:";
    if( data.is_null() )
    {
    }
    else
    {
        Json::validateTypeIsArrayOf< DocumentSymbol, SymbolInformation >( context, data );
    }
}

//
//
// DocumentSymbol
//

DocumentSymbol::DocumentSymbol( const Data& data )
: Data( data )
{
    validate( data );
}

DocumentSymbol::DocumentSymbol(
    const std::string& name, const SymbolKind kind, Range range, Range selectionRange )
: Data( Data::object() )
{
    operator[]( Identifier::name ) = name;
    operator[]( Identifier::kind ) = static_cast< std::size_t >( kind );
    operator[]( Identifier::range ) = Data::from_cbor( Data::to_cbor( range ) );
    operator[]( Identifier::selectionRange ) = Data::from_cbor( Data::to_cbor( selectionRange ) );
}

std::string DocumentSymbol::name( void ) const
{
    return operator[]( Identifier::name ).get< std::string >();
}

SymbolKind DocumentSymbol::kind( void ) const
{
    return static_cast< SymbolKind >( operator[]( Identifier::kind ).get< std::size_t >() );
}

Range DocumentSymbol::range( void ) const
{
    return operator[]( Identifier::range );
}

Range DocumentSymbol::selectionRange( void ) const
{
    return operator[]( Identifier::selectionRange );
}

u1 DocumentSymbol::hasDetail( void ) const
{
    return find( Identifier::detail ) != end();
}

void DocumentSymbol::setDetail( const std::string& detail )
{
    operator[]( Identifier::detail ) = detail;
}

std::string DocumentSymbol::detail( void ) const
{
    return at( Identifier::detail ).get< std::string >();
}

u1 DocumentSymbol::hasDeprecated( void ) const
{
    return find( Identifier::deprecated ) != end();
}

u1 DocumentSymbol::deprecated( void ) const
{
    return at( Identifier::deprecated ).get< u1 >();
}

void DocumentSymbol::setDeprecated( const u1 deprecated )
{
    operator[]( Identifier::deprecated ) = deprecated;
}

u1 DocumentSymbol::hasChildren( void ) const
{
    return find( Identifier::children ) != end();
}

DocumentSymbols DocumentSymbol::children( void ) const
{
    auto result = DocumentSymbols();
    for( auto symbol : at( Identifier::children ) )
    {
        result.emplace_back( symbol );
    }
    return result;
}

void DocumentSymbol::addChild( const DocumentSymbol& symbol )
{
    if( not hasChildren() )
    {
        operator[]( Identifier::children ) = Data::array();
    }
    operator[]( Identifier::children ).push_back( symbol );
}

void DocumentSymbol::validate( const Data& data )
{
    static const auto context = CONTENT + "DocumentSymbol:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< Range >( context, data, Identifier::range, true );
    Json::validatePropertyIs< Range >( context, data, Identifier::selectionRange, true );
    Json::validatePropertyIsString( context, data, Identifier::name, true );
    Json::validatePropertyIsNumber( context, data, Identifier::kind, true );
    Json::validatePropertyIsArrayOf< DocumentSymbol >( context, data, Identifier::children, false );
    Json::validatePropertyIsString( context, data, Identifier::detail, false );
    Json::validatePropertyIsBoolean( context, data, Identifier::deprecated, false );
}

//
//
// CodeLensParams
//

CodeLensParams::CodeLensParams( const Data& data )
: Data( data )
{
    validate( data );
}

CodeLensParams::CodeLensParams( const TextDocumentIdentifier& textDocument )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument ) = Data::from_cbor( Data::to_cbor( textDocument ) );
}

TextDocumentIdentifier CodeLensParams::textDocument( void ) const
{
    return at( Identifier::textDocument );
}

void CodeLensParams::validate( const Data& data )
{
    static const auto context = CONTENT + " CodeLensParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< TextDocumentIdentifier >(
        context, data, Identifier::textDocument, true );
}

//
//
// CodeLens
//

CodeLens::CodeLens( const Data& data )
: Data( data )
{
    validate( data );
}

CodeLens::CodeLens( const Range& range )
: Data( Data::object() )
{
    operator[]( Identifier::range ) = Data::from_cbor( Data::to_cbor( range ) );
}

Range CodeLens::range( void ) const
{
    return at( Identifier::range );
}

u1 CodeLens::hasCommand( void ) const
{
    return find( Identifier::command ) != end();
}

Command CodeLens::command( void ) const
{
    return at( Identifier::command );
}

void CodeLens::setCommand( const Command& command )
{
    operator[]( Identifier::command ) = Data::from_cbor( Data::to_cbor( command ) );
}

u1 CodeLens::hasData( void ) const
{
    return find( Identifier::data ) != end();
}

Data CodeLens::data( void ) const
{
    return at( Identifier::data );
}

void CodeLens::setData( const Data& data )
{
    operator[]( Identifier::data ) = Data::from_cbor( Data::to_cbor( data ) );
}

void CodeLens::validate( const Data& data )
{
    static const auto context = CONTENT + " CodeLens:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< Range >( context, data, Identifier::range, true );
    Json::validatePropertyIs< Command >( context, data, Identifier::command, false );
}

//
//
// CodeLensResult
//

CodeLensResult::CodeLensResult( const Data& data )
: Data( data )
{
    validate( data );
}

CodeLensResult::CodeLensResult( const std::vector< CodeLens >& codeLens )
: Data( Data::array() )
{
    for( auto cl : codeLens )
    {
        addCodeLens( cl );
    }
}

void CodeLensResult::addCodeLens( const CodeLens& codeLens )
{
    this->push_back( codeLens );
}

void CodeLensResult::validate( const Data& data )
{
    static const auto context = CONTENT + " CodeLensResult:";
    if( data.is_null() )
    {
        // ok, do nothing.
    }
    else
    {
        Json::validateTypeIsArrayOf< CodeLens >( context, data );
    }
}

//
//
// ExecuteCommandParams
//

ExecuteCommandParams::ExecuteCommandParams( const Data& data )
: Data( data )
{
    validate( data );
}

ExecuteCommandParams::ExecuteCommandParams( const std::string& command )
: Data( Data::object() )
{
    operator[]( Identifier::command ) = command;
    operator[]( Identifier::arguments ) = Data::array();
}

std::string ExecuteCommandParams::command( void ) const
{
    return at( Identifier::command ).get< std::string >();
}

u1 ExecuteCommandParams::hasArguments( void ) const
{
    return find( Identifier::arguments ) != end();
}

Data ExecuteCommandParams::arguments( void ) const
{
    return at( Identifier::arguments );
}

void ExecuteCommandParams::addArgument( const Data& argument )
{
    operator[]( Identifier::arguments ).push_back( argument );
}

void ExecuteCommandParams::validate( const Data& data )
{
    static const auto context = CONTENT + " ExecuteCommandParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::command, true );
    Json::validatePropertyIsArray( context, data, Identifier::arguments, false );
}

//
//
// DocumentLinkParams
//

DocumentLinkParams::DocumentLinkParams( const Data& data )
: Data( data )
{
    validate( data );
}

DocumentLinkParams::DocumentLinkParams( const TextDocumentIdentifier& textDocument )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument ) = Data::from_cbor( Data::to_cbor( textDocument ) );
}

TextDocumentIdentifier DocumentLinkParams::textDocument( void ) const
{
    return operator[]( Identifier::textDocument );
}

void DocumentLinkParams::validate( const Data& data )
{
    static const auto context = CONTENT + " DocumentLinkParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< TextDocumentIdentifier >(
        context, data, Identifier::textDocument, true );
}

//
//
// DocumentLink
//

DocumentLink::DocumentLink( const Data& data )
: Data( data )
{
    validate( data );
}

DocumentLink::DocumentLink( const Range& range )
: Data( Data::object() )
{
    operator[]( Identifier::range ) = Data::from_cbor( Data::to_cbor( range ) );
}

Range DocumentLink::range( void ) const
{
    return operator[]( Identifier::range );
}

u1 DocumentLink::hasTarget( void ) const
{
    return find( Identifier::target ) != end();
}

void DocumentLink::setTarget( const DocumentUri& target )
{
    operator[]( Identifier::target ) = target.toString();
}

DocumentUri DocumentLink::target( void ) const
{
    return DocumentUri::fromString( at( Identifier::target ).get< std::string >() );
}

u1 DocumentLink::hasData( void ) const
{
    return find( Identifier::data ) != end();
}

void DocumentLink::setData( const Data& data )
{
    operator[]( Identifier::data ) = Data::from_cbor( Data::to_cbor( data ) );
}

Data DocumentLink::data( void ) const
{
    return operator[]( Identifier::data );
}

void DocumentLink::validate( const Data& data )
{
    static const auto context = CONTENT + " DocumentLink:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< Range >( context, data, Identifier::range, true );
    Json::validatePropertyIsString( context, data, Identifier::target, false );
}

//
//
// DocumentLinkResult
//

DocumentLinkResult::DocumentLinkResult( void )
: Data()
{
}

DocumentLinkResult::DocumentLinkResult( const Data& data )
: Data( data )
{
    validate( data );
}

DocumentLinkResult::DocumentLinkResult( const DocumentLinks links )
: Data( Data::array() )
{
    for( auto link : links )
    {
        push_back( link );
    }
}

void DocumentLinkResult::validate( const Data& data )
{
    static const auto context = CONTENT + " DocumentLink:";
    if( data.is_null() )
    {
        // ok, do nothing.
    }
    else
    {
        Json::validateTypeIsArrayOf< DocumentLink >( data, context );
    }
}

//
//
// DocumentColorParams
//

DocumentColorParams::DocumentColorParams( const Data& data )
: Data( data )
{
    validate( data );
}

DocumentColorParams::DocumentColorParams( const TextDocumentIdentifier& textDocument )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument ) = Data::from_cbor( Data::to_cbor( textDocument ) );
}

TextDocumentIdentifier DocumentColorParams::textDocument( void ) const
{
    return operator[]( Identifier::textDocument );
}

void DocumentColorParams::validate( const Data& data )
{
    static const auto context = CONTENT + " DocumentColorParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< TextDocumentIdentifier >(
        context, data, Identifier::textDocument, true );
}

//
// Color
//
//

Color::Color( const Data& data )
: Data( data )
{
    validate( data );
}

Color::Color( const float red, const float green, const float blue, const float alpha )
{
    operator[]( Identifier::red ) = red;
    operator[]( Identifier::green ) = green;
    operator[]( Identifier::blue ) = blue;
    operator[]( Identifier::alpha ) = alpha;
}

float Color::red( void ) const
{
    return operator[]( Identifier::red ).get< float >();
}

float Color::green( void ) const
{
    return operator[]( Identifier::green ).get< float >();
}

float Color::blue( void ) const
{
    return operator[]( Identifier::blue ).get< float >();
}

float Color::alpha( void ) const
{
    return operator[]( Identifier::alpha ).get< float >();
}

void Color::validate( const Data& data )
{
    static const auto context = CONTENT + " Color:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsNumber( context, data, Identifier::red, true );
    Json::validatePropertyIsNumber( context, data, Identifier::green, true );
    Json::validatePropertyIsNumber( context, data, Identifier::blue, true );
    Json::validatePropertyIsNumber( context, data, Identifier::alpha, true );
}

//
//
// ColorInformation
//

ColorInformation::ColorInformation( const Data& data )
: Data( data )
{
    validate( data );
}

ColorInformation::ColorInformation( const Range& range, const Color& color )
{
    operator[]( Identifier::range ) = Data::from_cbor( Data::to_cbor( range ) );
    operator[]( Identifier::color ) = Data::from_cbor( Data::to_cbor( color ) );
}

Range ColorInformation::range( void ) const
{
    return operator[]( Identifier::range );
}

Color ColorInformation::color( void ) const
{
    return operator[]( Identifier::color );
}

void ColorInformation::validate( const Data& data )
{
    static const auto context = CONTENT + " ColorInformation:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< Range >( context, data, Identifier::range, true );
    Json::validatePropertyIs< Color >( context, data, Identifier::color, true );
}

//
//
// DocumentColorResult
//

DocumentColorResult::DocumentColorResult( const Data& data )
: Data( data )
{
    validate( data );
}

DocumentColorResult::DocumentColorResult( const ColorInformations& colorInformations )
: Data( Data::array() )
{
    for( auto colorInformation : colorInformations )
    {
        push_back( colorInformation );
    }
}

void DocumentColorResult::validate( const Data& data )
{
    static const auto context = CONTENT + " ColorInformation:";
    Json::validateTypeIsArrayOf< ColorInformation >( context, data );
}

//
//
// ColorPresentationParams
//

ColorPresentationParams::ColorPresentationParams( const Data& data )
: DocumentColorParams( data )
{
    validate( data );
}

ColorPresentationParams::ColorPresentationParams(
    const TextDocumentIdentifier& textDocument, const Color& color, const Range& range )
: DocumentColorParams( textDocument )
{
    operator[]( Identifier::color ) = Data::from_cbor( Data::to_cbor( color ) );
    operator[]( Identifier::range ) = Data::from_cbor( Data::to_cbor( range ) );
}

Color ColorPresentationParams::color( void ) const
{
    return operator[]( Identifier::color );
}

Range ColorPresentationParams::range( void ) const
{
    return operator[]( Identifier::range );
}

void ColorPresentationParams::validate( const Data& data )
{
    static const auto context = CONTENT + " ColorPresentationParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< Color >( context, data, Identifier::color, true );
    Json::validatePropertyIs< Range >( context, data, Identifier::range, true );
}

ColorPresentation::ColorPresentation( const Data& data )
: Data( data )
{
    validate( data );
}

ColorPresentation::ColorPresentation( const std::string& label )
: Data( Data::object() )
{
    operator[]( Identifier::label ) = label;
}

std::string ColorPresentation::label( void ) const
{
    return operator[]( Identifier::label ).get< std::string >();
}

u1 ColorPresentation::hasTextEdit( void ) const
{
    return find( Identifier::textEdit ) != end();
}

void ColorPresentation::setTextEdit( const TextEdit& textEdit )
{
    operator[]( Identifier::textEdit ) = Data::from_cbor( Data::to_cbor( textEdit ) );
}

TextEdit ColorPresentation::textEdit( void ) const
{
    return at( Identifier::textEdit );
}

u1 ColorPresentation::hasAdditionalTextEdits( void ) const
{
    return find( Identifier::additionalTextEdits ) != end();
}

void ColorPresentation::addAdditionalTextEdit( const TextEdit& textEdit )
{
    if( not hasAdditionalTextEdits() )
    {
        operator[]( Identifier::additionalTextEdits ) = Data::array();
    }
    operator[]( Identifier::additionalTextEdits ).push_back( textEdit );
}

TextEdits ColorPresentation::additionalTextEdits( void ) const
{
    auto result = TextEdits();
    for( auto textEdit : at( Identifier::additionalTextEdits ) )
    {
        result.push_back( textEdit );
    }
    return result;
}

void ColorPresentation::validate( const Data& data )
{
    static const auto context = CONTENT + " ColorPresentation:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::label, true );
    Json::validatePropertyIs< TextEdit >( context, data, Identifier::textEdit, false );
    Json::validatePropertyIsArrayOf< TextEdit >(
        context, data, Identifier::additionalTextEdits, false );
}

ColorPresentationResult::ColorPresentationResult( const Data& data )
: Data( data )
{
    validate( data );
}

ColorPresentationResult::ColorPresentationResult( ColorPresentations presentations )
: Data( Data::array() )
{
    for( auto presentation : presentations )
    {
        push_back( presentation );
    }
}

void ColorPresentationResult::validate( const Data& data )
{
    static const auto context = CONTENT + " ColorPresentationResult:";
    Json::validateTypeIsArrayOf< ColorPresentation >( context, data );
}

//
//
// DocumentFormattingParams
//

DocumentFormattingParams::DocumentFormattingParams( const Data& data )
: Data( data )
{
    validate( data );
}

DocumentFormattingParams::DocumentFormattingParams(
    const TextDocumentIdentifier& textDocument, const FormattingOptions& options )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument ) = Data::from_cbor( Data::to_cbor( textDocument ) );
    operator[]( Identifier::options ) = Data::from_cbor( Data::to_cbor( options ) );
}

TextDocumentIdentifier DocumentFormattingParams::textDocument( void ) const
{
    return operator[]( Identifier::textDocument );
}

FormattingOptions DocumentFormattingParams::options( void ) const
{
    return operator[]( Identifier::options );
}

void DocumentFormattingParams::validate( const Data& data )
{
    static const auto context = CONTENT + " DocumentFormattingParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< TextDocumentIdentifier >(
        context, data, Identifier::textDocument, true );
    Json::validatePropertyIs< FormattingOptions >( context, data, Identifier::options, true );
}

//
//
// FormattingOptions
//

FormattingOptions::FormattingOptions( const Data& data )
: Data( Data::object() )
{
    validate( data );
}

FormattingOptions::FormattingOptions( const std::size_t tabSize, const u1 insertSpaces )
: Data( Data::object() )
{
    operator[]( Identifier::tabSize ) = tabSize;
    operator[]( Identifier::insertSpaces ) = insertSpaces;
}

void FormattingOptions::validateAdditionalOptions( const Data& data )
{
    static const auto context = CONTENT + " FormattingOptions:";
    for( auto element : data )
    {
        if( element.is_boolean() || element.is_number() || element.is_string() )
        {
            // do nothing
        }
        else
        {
            throw std::invalid_argument(
                context + " invalid data type, shall be 'boolean', 'number' or 'string'" );
        }
    }
}

void FormattingOptions::addBool( const std::string& key, const u1 boolean )
{
    operator[]( key ) = boolean;
}
void FormattingOptions::addNumber( const std::string& key, const float number )
{
    operator[]( key ) = number;
}
void FormattingOptions::addString( const std::string& key, const std::string& string )
{
    operator[]( key ) = string;
}

std::size_t FormattingOptions::tabSize( void ) const
{
    return operator[]( Identifier::tabSize ).get< std::size_t >();
}

u1 FormattingOptions::insertSpaces( void ) const
{
    return operator[]( Identifier::insertSpaces ).get< u1 >();
}

void FormattingOptions::validate( const Data& data )
{
    static const auto context = CONTENT + " FormattingOptions:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsNumber( context, data, Identifier::tabSize, true );
    Json::validatePropertyIsBoolean( context, data, Identifier::insertSpaces, true );
    validateAdditionalOptions( data );
}

//
//
// DocumentFormattingResult
//

DocumentFormattingResult::DocumentFormattingResult( void )
: Data()
{
}
DocumentFormattingResult::DocumentFormattingResult( const Data& data )
: Data( data )
{
    validate( data );
}
DocumentFormattingResult::DocumentFormattingResult( const TextEdits& edits )
: Data( Data::array() )
{
    for( auto edit : edits )
    {
        push_back( edit );
    }
}
void DocumentFormattingResult::validate( const Data& data )
{
    static const auto context = CONTENT + " DocumentFormattingResult:";
    if( data.is_null() )
    {
        // ok, do nothing
    }
    else
    {
        Json::validateTypeIsArrayOf< TextEdit >( context, data );
    }
}

DocumentRangeFormattingParams::DocumentRangeFormattingParams( const Data& data )
: DocumentFormattingParams( data )
{
    validate( data );
}

DocumentRangeFormattingParams::DocumentRangeFormattingParams(
    const TextDocumentIdentifier& textDocument,
    const Range& range,
    const FormattingOptions& options )
: DocumentFormattingParams( textDocument, options )
{
    operator[]( Identifier::range ) = Data::from_cbor( Data::to_cbor( range ) );
}

Range DocumentRangeFormattingParams::range( void ) const
{
    return operator[]( Identifier::range );
}

void DocumentRangeFormattingParams::validate( const Data& data )
{
    static const auto context = CONTENT + " DocumentRangeFormattingParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< Range >( context, data, Identifier::range, true );
}

//
//
// DocumentOnTypeFormattingParams
//

DocumentOnTypeFormattingParams::DocumentOnTypeFormattingParams( const Data& data )
: DocumentFormattingParams( data )
{
    validate( data );
}

DocumentOnTypeFormattingParams::DocumentOnTypeFormattingParams(
    const TextDocumentIdentifier& textDocument,
    const FormattingOptions& options,
    const Position& position,
    const std::string& ch )
: DocumentFormattingParams( textDocument, options )
{
    operator[]( Identifier::position ) = Data::from_cbor( Data::to_cbor( position ) );
    operator[]( Identifier::ch ) = ch;
}

Position DocumentOnTypeFormattingParams::position( void ) const
{
    return operator[]( Identifier::position );
}

std::string DocumentOnTypeFormattingParams::ch( void ) const
{
    return operator[]( Identifier::ch ).get< std::string >();
}

void DocumentOnTypeFormattingParams::validate( const Data& data )
{
    static const auto context = CONTENT + " DocumentOnTypeFormattingParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< Position >( context, data, Identifier::position, true );
    Json::validatePropertyIsString( context, data, Identifier::ch, true );
}

//
//
// DocumentOnTypeFormattingRegistrationOptions
//

DocumentOnTypeFormattingRegistrationOptions::DocumentOnTypeFormattingRegistrationOptions(
    const Data& data )
: TextDocumentRegistrationOptions( data )
{
    validate( data );
}

DocumentOnTypeFormattingRegistrationOptions::DocumentOnTypeFormattingRegistrationOptions(
    const DocumentSelector& documentSelector, const std::string& firstTriggerCharacter )
: TextDocumentRegistrationOptions( documentSelector )
{
    operator[]( Identifier::firstTriggerCharacter ) = firstTriggerCharacter;
}
std::string DocumentOnTypeFormattingRegistrationOptions::firstTriggerCharacter( void ) const
{
    return operator[]( Identifier::firstTriggerCharacter ).get< std::string >();
}

u1 DocumentOnTypeFormattingRegistrationOptions::hasMoreTriggerCharacter( void ) const
{
    return find( Identifier::moreTriggerCharacter ) != end();
}

void DocumentOnTypeFormattingRegistrationOptions::addMoreTriggerCharacter(
    const std::string& character )
{
    if( not hasMoreTriggerCharacter() )
    {
        operator[]( Identifier::moreTriggerCharacter ) = Data::array();
    }
    operator[]( Identifier::moreTriggerCharacter ).push_back( character );
}

std::vector< std::string > DocumentOnTypeFormattingRegistrationOptions::moreTriggerCharacter(
    void ) const
{
    auto result = std::vector< std::string >();
    for( auto ch : operator[]( Identifier::moreTriggerCharacter ) )
    {
        result.push_back( ch );
    }
    return result;
}

void DocumentOnTypeFormattingRegistrationOptions::validate( const Data& data )
{
    static const auto context = CONTENT + " DocumentOnTypeFormattingRegistrationOptions:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsString( context, data, Identifier::firstTriggerCharacter, true );
    Json::validatePropertyIsArrayOfString( context, data, Identifier::moreTriggerCharacter, true );
}

//
//
// RenameParams
//

RenameParams::RenameParams( const Data& data )
: Data( data )
{
    validate( data );
}

RenameParams::RenameParams(
    const TextDocumentIdentifier& textDocument,
    const Position& position,
    const std::string& newName )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument ) = Data::from_cbor( Data::to_cbor( textDocument ) );
    operator[]( Identifier::position ) = Data::from_cbor( Data::to_cbor( position ) );
    operator[]( Identifier::newName ) = newName;
}

TextDocumentIdentifier RenameParams::textDocument( void ) const
{
    return operator[]( Identifier::textDocument );
}

Position RenameParams::position( void ) const
{
    return operator[]( Identifier::position );
}

std::string RenameParams::newName( void ) const
{
    return operator[]( Identifier::newName ).get< std::string >();
}

void RenameParams::validate( const Data& data )
{
    static const auto context = CONTENT + " RenameParams:";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< TextDocumentIdentifier >(
        context, data, Identifier::textDocument, true );
    Json::validatePropertyIs< Position >( context, data, Identifier::position, true );
    Json::validatePropertyIsString( context, data, Identifier::newName, true );
}

RenameRegistrationOptions::RenameRegistrationOptions( const Data& data )
: TextDocumentRegistrationOptions( data )
{
    validate( data );
}

RenameRegistrationOptions::RenameRegistrationOptions( const DocumentSelector& selector )
: TextDocumentRegistrationOptions( selector )
{
}

u1 RenameRegistrationOptions::hasPrepareProvider( void ) const
{
    return find( Identifier::prepareProvider ) != end();
}

void RenameRegistrationOptions::setPrepareProvider( const u1 prepareProvider )
{
    operator[]( Identifier::prepareProvider ) = prepareProvider;
}

u1 RenameRegistrationOptions::prepareProvider( void ) const
{
    return at( Identifier::prepareProvider ).get< u1 >();
}

void RenameRegistrationOptions::validate( const Data& data )
{
    static const auto context = CONTENT + " RenameRegistrationOptions";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsBoolean( context, data, Identifier::prepareProvider, false );
}

//
//
// RenameResult
//

RenameResult::RenameResult( void )
: Data()
{
}

RenameResult::RenameResult( const Data& data )
: Data( data )
{
    validate( data );
}

RenameResult::RenameResult( const WorkspaceEdit& edit )
: Data( Data::from_cbor( Data::to_cbor( edit ) ) )
{
}

void RenameResult::validate( const Data& data )
{
    static const auto context = CONTENT + " RenameResult";
    if( data.is_null() )
    {
        // ok, do nothing.
    }
    else
    {
        WorkspaceEdit::validate( data );
    }
}

//
//
// PrepareRenameResult
//

PrepareRenameResult::PrepareRenameResult( void )
: Data()
{
}

PrepareRenameResult::PrepareRenameResult( const Data& data )
: Data( data )
{
    validate( data );
}

PrepareRenameResult::PrepareRenameResult( const Range& range, const std::string& placeholder )
: Data( Data::object() )
{
    operator[]( Identifier::range ) = Data::from_cbor( Data::to_cbor( range ) );
    operator[]( Identifier::placeholder ) = placeholder;
}

void PrepareRenameResult::validate( const Data& data )
{
    static const auto context = CONTENT + " RenameRegistrationOptions";
    if( data.is_null() )
    {
        // ok, do nothing
    }
    else
    {
        try
        {
            Range::validate( data );
        }
        catch( std::invalid_argument a )
        {
            Json::validateTypeIsObject( context, data );
            Json::validatePropertyIs< Range >( context, data, Identifier::range, true );
            Json::validatePropertyIsString( context, data, Identifier::placeholder, true );
        }
    }
}

//
//
// FoldingRangeParams
//

FoldingRangeParams::FoldingRangeParams( const Data& data )
: Data( data )
{
    validate( data );
}

FoldingRangeParams::FoldingRangeParams( const TextDocumentIdentifier& textDocument )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument ) = Data::from_cbor( Data::to_cbor( textDocument ) );
}

TextDocumentIdentifier FoldingRangeParams::textDocument( void ) const
{
    return operator[]( Identifier::textDocument );
}

void FoldingRangeParams::validate( const Data& data )
{
    static const auto context = CONTENT + " FoldingRangeParams";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIs< TextDocumentIdentifier >(
        context, data, Identifier::textDocument, true );
}

//
//
// FoldingRange
//

FoldingRange::FoldingRange( const Data& data )
: Data( data )
{
    validate( data );
}

FoldingRange::FoldingRange( const std::size_t startLine, const std::size_t endLine )
: Data( Data::object() )
{
    operator[]( Identifier::startLine ) = startLine;
    operator[]( Identifier::endLine ) = endLine;
}

std::size_t FoldingRange::startLine( void ) const
{
    return operator[]( Identifier::startLine ).get< std::size_t >();
}

std::size_t FoldingRange::endLine( void ) const
{
    return operator[]( Identifier::endLine ).get< std::size_t >();
}

u1 FoldingRange::hasStartCharacter( void ) const
{
    return find( Identifier::startCharacter ) != end();
}

void FoldingRange::setStartCharacter( const std::size_t startCharacter )
{
    operator[]( Identifier::startCharacter ) = startCharacter;
}

std::size_t FoldingRange::startCharacter( void ) const
{
    return at( Identifier::startCharacter ).get< std::size_t >();
}

u1 FoldingRange::hasEndCharacter( void ) const
{
    return find( Identifier::endCharacter ) != end();
}

void FoldingRange::setEndCharacter( const std::size_t endCharacter )
{
    operator[]( Identifier::endCharacter ) = endCharacter;
}

std::size_t FoldingRange::endCharacter( void ) const
{
    return at( Identifier::endCharacter ).get< std::size_t >();
}

u1 FoldingRange::hasKind( void ) const
{
    return find( Identifier::kind ) != end();
}

void FoldingRange::setKind( const FoldingRangeKind kind )
{
    switch( kind )
    {
        case FoldingRangeKind::Comment:
        {
            operator[]( Identifier::kind ) = Identifier::comment;
            break;
        }
        case FoldingRangeKind::Imports:
        {
            operator[]( Identifier::kind ) = Identifier::imports;
            break;
        }
        case FoldingRangeKind::Region:
        {
            operator[]( Identifier::kind ) = Identifier::region;
            break;
        }
    }
}

std::string FoldingRange::kind( void ) const
{
    return at( Identifier::kind ).get< std::string >();
}

void FoldingRange::validate( const Data& data )
{
    static const auto context = CONTENT + " FoldingRange";
    Json::validateTypeIsObject( context, data );
    Json::validatePropertyIsNumber( context, data, Identifier::startLine, true );
    Json::validatePropertyIsNumber( context, data, Identifier::endLine, true );
    Json::validatePropertyIsNumber( context, data, Identifier::startCharacter, false );
    Json::validatePropertyIsNumber( context, data, Identifier::endCharacter, false );
    Json::validatePropertyIsString( context, data, Identifier::kind, false );
}

//
//
// FoldingRangeResult
//

FoldingRangeResult::FoldingRangeResult( void )
: Data()
{
}

FoldingRangeResult::FoldingRangeResult( const Data& data )
: Data( data )
{
    validate( data );
}

FoldingRangeResult::FoldingRangeResult( const FoldingRanges ranges )
: Data( Data::array() )
{
    for( auto range : ranges )
    {
        push_back( range );
    }
}

void FoldingRangeResult::validate( const Data& data )
{
    static const auto context = CONTENT + " FoldingRangeResult";
    if( data.is_null() )
    {
        // okay, do nothing
    }
    else
    {
        Json::validateTypeIsArrayOf< FoldingRange >( context, data );
    }
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
