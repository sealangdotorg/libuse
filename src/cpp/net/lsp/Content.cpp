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

u1 Content::hasProperty( const Data& data, const std::string& field )
{
    return data.find( field ) != data.end();
}

void Content::validateTypeIsObject( const std::string& context, const Data& data )
{
    if( not data.is_object() )
    {
        throw std::invalid_argument( context + " invalid data type, shall be 'object'" );
    }
}

void Content::validateTypeIsString( const std::string& context, const Data& data )
{
    if( not data.is_string() )
    {
        throw std::invalid_argument( context + " invalid data type, shall be 'string'" );
    }
}

void Content::validateTypeIsArray( const std::string& context, const Data& data )
{
    if( not data.is_array() )
    {
        throw std::invalid_argument( context + " invalid data type, shall be 'array'" );
    }
}

void Content::validatePropertyIs(
    const std::string& context,
    const Data& data,
    const std::string& field,
    const u1 required,
    const std::function< u1( const Data& property ) >& condition )
{
    if( hasProperty( data, field ) )
    {
        if( not condition( data[ field ] ) )
        {
            throw std::invalid_argument( context + " invalid property '" + field + "'" );
        }
    }
    else
    {
        if( required )
        {
            throw std::invalid_argument( context + " missing property '" + field + "'" );
        }
    }
}

void Content::validatePropertyIsObject(
    const std::string& context, const Data& data, const std::string& field, const u1 required )
{
    validatePropertyIs( context, data, field, required, []( const Data& property ) -> u1 {
        return property.is_object();
    } );
}

void Content::validatePropertyIsArray(
    const std::string& context, const Data& data, const std::string& field, const u1 required )
{
    validatePropertyIs( context, data, field, required, []( const Data& property ) -> u1 {
        return property.is_array();
    } );
}

void Content::validatePropertyIsString(
    const std::string& context, const Data& data, const std::string& field, const u1 required )
{
    validatePropertyIs( context, data, field, required, []( const Data& property ) -> u1 {
        return property.is_string();
    } );
}

void Content::validatePropertyIsNumber(
    const std::string& context, const Data& data, const std::string& field, const u1 required )
{
    validatePropertyIs( context, data, field, required, []( const Data& property ) -> u1 {
        return property.is_number();
    } );
}

void Content::validatePropertyIsNumberOrNull(
    const std::string& context, const Data& data, const std::string& field, const u1 required )
{
    validatePropertyIs( context, data, field, required, []( const Data& property ) -> u1 {
        return property.is_number() or property.is_null();
    } );
}

void Content::validatePropertyIsBoolean(
    const std::string& context, const Data& data, const std::string& field, const u1 required )
{
    validatePropertyIs( context, data, field, required, []( const Data& property ) -> u1 {
        return property.is_boolean();
    } );
}

void Content::validatePropertyIsUuid(
    const std::string& context, const Data& data, const std::string& field, const u1 required )
{
    validatePropertyIs( context, data, field, required, []( const Data& property ) -> u1 {
        return property.is_string() or property.is_number();
    } );
}

void Content::validatePropertyIsUri(
    const std::string& context, const Data& data, const std::string& field, const u1 required )
{
    validatePropertyIsString( context, data, field, required );
    try
    {
        DocumentUri::fromString( data[ field ].get< std::string >() );
    }
    catch( const std::exception& e )
    {
        throw std::invalid_argument( context + " DocumentUri: " + e.what() );
    }
}

void Content::validatePropertyIsUriOrNull(
    const std::string& context, const Data& data, const std::string& field, const u1 required )
{
    validatePropertyIs( context, data, field, required, []( const Data& property ) -> u1 {
        return property.is_string() or property.is_null();
    } );
    if( hasProperty( data, field ) and data[ field ].is_string() )
    {
        validatePropertyIsUri( context, data, field, required );
    }
}

void Content::validatePropertyIsArrayOfString(
    const std::string& context, const Data& data, const std::string& field, const u1 required )
{
    validatePropertyIsArray( context, data, field, required );
    if( hasProperty( data, field ) )
    {
        for( auto element : data[ field ] )
        {
            validateTypeIsString( context, element );
        }
    }
}

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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsNumber( context, data, Identifier::code, true );
    Content::validatePropertyIsString( context, data, Identifier::message, true );
    Content::validatePropertyIsObject( context, data, Identifier::data, false );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsNumber( context, data, Identifier::line, true );
    Content::validatePropertyIsNumber( context, data, Identifier::character, true );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIs< Range >( context, data, Identifier::start, true );
    Content::validatePropertyIs< Range >( context, data, Identifier::end, true );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsUri( context, data, Identifier::uri, true );
    Content::validatePropertyIs< Range >( context, data, Identifier::range, false );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsString( context, data, Identifier::message, true );
    Content::validatePropertyIs< Range >( context, data, Identifier::range, true );
    Content::validatePropertyIsUuid( context, data, Identifier::code, false );
    Content::validatePropertyIsNumber( context, data, Identifier::severity, false );
    Content::validatePropertyIsString( context, data, Identifier::source, false );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsString( context, data, Identifier::title, true );
    Content::validatePropertyIsString( context, data, Identifier::command, true );
    Content::validatePropertyIsArray( context, data, Identifier::arguments, false );
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

TextEdit::TextEdit( const Range& range, const std::string newText )
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsString( context, data, Identifier::newText, true );
    Content::validatePropertyIs< Range >( context, data, Identifier::range, true );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsUri( context, data, Identifier::uri, true );
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
    Content::validatePropertyIsNumber( context, data, Identifier::version, true );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIs< VersionedTextDocumentIdentifier >(
        context, data, Identifier::textDocument, true );
    Content::validatePropertyIsArrayOf< TextEdit >( context, data, Identifier::edits, true );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsArrayOf< TextDocumentEdit >(
        context, data, Identifier::documentChanges, false );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsUri( context, data, Identifier::uri, true );
    Content::validatePropertyIsString( context, data, Identifier::languageId, true );
    Content::validatePropertyIsNumber( context, data, Identifier::version, true );
    Content::validatePropertyIsString( context, data, Identifier::text, true );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIs< TextDocumentIdentifier >(
        context, data, Identifier::textDocument, true );
    Content::validatePropertyIs< Position >( context, data, Identifier::position, true );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsString( context, data, Identifier::language, false );
    Content::validatePropertyIsString( context, data, Identifier::scheme, false );
    Content::validatePropertyIsString( context, data, Identifier::pattern, false );
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
    Content::validateTypeIsArrayOf< DocumentFilter >( context, data );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsBoolean( context, data, Identifier::dynamicRegistration, false );
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

void WorkspaceClientCapabilities::validate( const Data& data )
{
    static const auto context = CONTENT + " WorkspaceClientCapabilities:";
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsBoolean( context, data, Identifier::applyEdit, false );
    Content::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::workspaceEdit, false );
    Content::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::didChangeConfiguration, false );
    Content::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::didChangeWatchedFiles, false );
    Content::validatePropertyIs< DynamicRegistration >( context, data, Identifier::symbol, false );
    Content::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::executeCommand, false );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsBoolean( context, data, Identifier::dynamicRegistration, false );
    Content::validatePropertyIsBoolean( context, data, Identifier::willSave, false );
    Content::validatePropertyIsBoolean( context, data, Identifier::willSaveWaitUntil, false );
    Content::validatePropertyIsBoolean( context, data, Identifier::didSave, false );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsBoolean( context, data, Identifier::snippetSupport, false );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsBoolean( context, data, Identifier::dynamicRegistration, false );
    Content::validatePropertyIs< TextDocumentClientCapabilities::CompletionItem >(
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIs< Synchronization >(
        context, data, Identifier::synchronization, false );
    Content::validatePropertyIs< Completion >( context, data, Identifier::completion, false );
    Content::validatePropertyIs< DynamicRegistration >( context, data, Identifier::hover, false );
    Content::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::signatureHelp, false );
    Content::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::references, false );
    Content::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::documentHighlight, false );
    Content::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::documentSymbol, false );
    Content::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::formatting, false );
    Content::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::rangeFormatting, false );
    Content::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::onTypeFormatting, false );
    Content::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::definition, false );
    Content::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::codeAction, false );
    Content::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::codeLens, false );
    Content::validatePropertyIs< DynamicRegistration >(
        context, data, Identifier::documentLink, false );
    Content::validatePropertyIs< DynamicRegistration >( context, data, Identifier::rename, false );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIs< WorkspaceClientCapabilities >(
        context, data, Identifier::workspace, false );
    Content::validatePropertyIs< TextDocumentClientCapabilities >(
        context, data, Identifier::textDocument, false );
    Content::validatePropertyIsObject( context, data, Identifier::experimental, false );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsBoolean( context, data, Identifier::includeText, false );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsBoolean( context, data, Identifier::openClose, false );
    Content::validatePropertyIsNumber( context, data, Identifier::change, false );
    Content::validatePropertyIsBoolean( context, data, Identifier::willSave, false );
    Content::validatePropertyIsBoolean( context, data, Identifier::willSaveWaitUntil, false );
    Content::validatePropertyIs< SaveOptions >( context, data, Identifier::save, false );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsBoolean( context, data, Identifier::resolveProvider, false );
    Content::validatePropertyIsArrayOfString( context, data, Identifier::triggerCharacters, false );
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
    Content::validateTypeIsObject( context, data );
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
    Content::validateTypeIsObject( context, data );
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
    Content::validateTypeIsObject( context, data );
    // TODO: FIXME: @ppaulweber
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
    Content::validateTypeIsObject( context, data );
    // TODO: FIXME: @ppaulweber
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

void ServerCapabilities::validate( const Data& data )
{
    static const auto context = CONTENT + " SignatureHelpOptions:";
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIs< TextDocumentSyncOptions >(
        context, data, Identifier::textDocumentSync, false );
    Content::validatePropertyIsBoolean( context, data, Identifier::hoverProvider, false );
    Content::validatePropertyIs< CompletionOptions >(
        context, data, Identifier::completionProvider, false );
    Content::validatePropertyIs< SignatureHelpOptions >(
        context, data, Identifier::signatureHelpProvider, false );
    Content::validatePropertyIsBoolean( context, data, Identifier::definitionProvider, false );
    Content::validatePropertyIsBoolean( context, data, Identifier::referencesProvider, false );
    Content::validatePropertyIsBoolean(
        context, data, Identifier::documentHighlightProvider, false );
    Content::validatePropertyIsBoolean( context, data, Identifier::documentSymbolProvider, false );
    Content::validatePropertyIsBoolean( context, data, Identifier::workspaceSymbolProvider, false );
    Content::validatePropertyIsBoolean( context, data, Identifier::codeActionProvider, false );
    Content::validatePropertyIs< CodeLensOptions >(
        context, data, Identifier::codeLensProvider, false );
    Content::validatePropertyIsBoolean(
        context, data, Identifier::documentFormattingProvider, false );
    Content::validatePropertyIsBoolean(
        context, data, Identifier::documentRangeFormattingProvider, false );
    Content::validatePropertyIs< DocumentOnTypeFormattingOptions >(
        context, data, Identifier::documentOnTypeFormattingProvider, false );
    Content::validatePropertyIsBoolean( context, data, Identifier::renameProvider, false );
    Content::validatePropertyIs< DocumentLinkOptions >(
        context, data, Identifier::documentLinkProvider, false );
    Content::validatePropertyIs< ExecuteCommandOptions >(
        context, data, Identifier::executeCommandProvider, false );
    Content::validatePropertyIsObject( context, data, Identifier::experimental, false );
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

void InitializeParams::validate( const Data& data )
{
    static const auto context = CONTENT + " InitializeParams:";
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsUriOrNull( context, data, Identifier::rootUri, true );
    Content::validatePropertyIs< ClientCapabilities >(
        context, data, Identifier::capabilities, true );

    Content::validatePropertyIsNumberOrNull( context, data, Identifier::processId, false );
    Content::validatePropertyIsObject( context, data, Identifier::initializationOptions, false );
    Content::validatePropertyIsString( context, data, Identifier::trace, false );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIs< ServerCapabilities >(
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsBoolean( context, data, Identifier::retry, false );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsString( context, data, Identifier::message, true );
    Content::validatePropertyIsNumber( context, data, Identifier::type, true );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsString( context, data, Identifier::title, true );
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

Data ShowMessageRequestParams::actions( void ) const
{
    return at( Identifier::actions );
}

void ShowMessageRequestParams::addAction( const MessageActionItem& action )
{
    operator[]( Identifier::actions ).push_back( action );
}

std::string ShowMessageRequestParams::message( void ) const
{
    return at( Identifier::message ).get< std::string >();
}

void ShowMessageRequestParams::validate( const Data& data )
{
    static const auto context = CONTENT + " ShowMessageRequestParams:";
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsString( context, data, Identifier::title, true );
    Content::validatePropertyIsArray( context, data, Identifier::actions, false );
}

TelemetryEventParams::TelemetryEventParams( const Data& data )
: Data( data )
{
    validate( data );
}

void TelemetryEventParams::validate( const Data& data )
{
    static const auto context = CONTENT + " TelemetryEventParams:";
    Content::validateTypeIsObject( context, data );
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
    return operator[]( Identifier::registerOptions );
}

u1 Registration::hasRegisterOptions( void ) const
{
    return find( Identifier::actions ) != end();
}

void Registration::addRegisterOption( const Data& option )
{
    operator[]( Identifier::registerOptions ).push_back( option );
}

void Registration::validate( const Data& data )
{
    static const auto context = CONTENT + " Registration:";
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsString( context, data, Identifier::id, true );
    Content::validatePropertyIsString( context, data, Identifier::method, true );
    Content::validatePropertyIsArray( context, data, Identifier::registerOptions, false );
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

RegistrationParams::RegistrationParams( const std::vector< Registration >& registrations )
: Data( Data::object() )
{
    operator[]( Identifier::registrations ) = Data::array();
    for( auto registration : registrations )
    {
        operator[]( Identifier::registrations ).push_back( registration );
    }
}

Data RegistrationParams::registrations( void ) const
{
    return operator[]( Identifier::registrations );
}

void RegistrationParams::validate( const Data& data )
{
    static const auto context = CONTENT + " RegistrationParams:";
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsArray( context, data, Identifier::registrations, true );
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
    Content::validateTypeIsObject( context, data );
    if( data.find( Identifier::documentSelector ) != data.end() and
        data[ Identifier::documentSelector ].is_null() )
    {
        // ok, due to the possibility that the property can be null
    }
    else
    {
        Content::validatePropertyIs< DocumentSelector >(
            context, data, Identifier::documentSelector, true );
    }
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsString( context, data, Identifier::id, true );
    Content::validatePropertyIsString( context, data, Identifier::method, true );
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

Data UnregistrationParams::unregistrations( void ) const
{
    return operator[]( Identifier::unregistrations );
}

void UnregistrationParams::validate( const Data& data )
{
    static const auto context = CONTENT + " UnregistrationParams:";
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsArray( context, data, Identifier::unregistrations, true );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsString( context, data, Identifier::uri, true );
    Content::validatePropertyIsString( context, data, Identifier::name, true );
}

//
//
// WorkspaceFoldersResponse
//

WorkspaceFoldersResponse::WorkspaceFoldersResponse( const Data& data )
{
    validate( data );
}

WorkspaceFoldersResponse::WorkspaceFoldersResponse(
    const std::vector< WorkspaceFolder >& workspaceFolders )
{
    operator[]( Identifier::workspaceFolders ) = Data::array();
    for( auto folder : workspaceFolders )
    {
        operator[]( Identifier::workspaceFolders ).push_back( folder );
    }
}

Data WorkspaceFoldersResponse::workspaceFolders( void ) const
{
    return operator[]( Identifier::workspaceFolders );
}

void WorkspaceFoldersResponse::addWorkspaceFolder( const WorkspaceFolder& workspaceFolder )
{
    operator[]( Identifier::workspaceFolders ).push_back( workspaceFolder );
}

WorkspaceFolder WorkspaceFoldersResponse::at( std::size_t index ) const
{
    auto folder = operator[]( Identifier::workspaceFolders );
    WorkspaceFolder result( folder[ index ] );
    return result;
}

void WorkspaceFoldersResponse::validate( const Data& data )
{
    static const auto context = CONTENT + " WorkSpaceFolderResponse:";
    Content::validateTypeIsObject( context, data );

    if( data.find( Identifier::workspaceFolders ) != data.end() and
        data[ Identifier::workspaceFolders ].is_null() )
    {
        // ok, due to the possibility that the property can be null
    }
    else
    {
        Content::validatePropertyIsArrayOf< WorkspaceFolder >(
            context, data, Identifier::workspaceFolders, true );
    }
}

//
//
// WorkspaceFoldersChangeEvent
//

WorkspaceFoldersChangeEvent::WorkspaceFoldersChangeEvent(
    const std::vector< WorkspaceFolder >& added, const std::vector< WorkspaceFolder >& removed )
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

Data WorkspaceFoldersChangeEvent::added( void ) const
{
    return operator[]( Identifier::added );
}

Data WorkspaceFoldersChangeEvent::removed( void ) const
{
    return operator[]( Identifier::removed );
}

void WorkspaceFoldersChangeEvent::validate( const Data& data )
{
    static const auto context = CONTENT + " WorkspaceFoldersChangeEvent:";
    Content::validateTypeIsObject( context, data );

    Content::validatePropertyIsArrayOf< WorkspaceFolder >( context, data, Identifier::added, true );
    Content::validatePropertyIsArrayOf< WorkspaceFolder >(
        context, data, Identifier::removed, true );
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
    operator[]( Identifier::event ) = event;
}

void DidChangeWorkspaceFoldersParams::validate( const Data& data )
{
    static const auto context = CONTENT + " DidChangeWorkspaceFoldersParams:";
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIs< WorkspaceFoldersChangeEvent >(
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
    operator[]( Identifier::settings ) = settings;
    validate( settings );
}

DidChangeConfigurationSettings DidChangeConfigurationParams::settings( void ) const
{
    return operator[]( Identifier::settings );
}

void DidChangeConfigurationParams::validate( const Data& data )
{
    static const auto context = CONTENT + " DidChangeConfigurationParams:";
    Content::validateTypeIsObject( context, data );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsString( context, data, Identifier::section, false );
    Content::validatePropertyIsString( context, data, Identifier::scopeUri, false );
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

void ConfigurationParams::validate( const Data& data )
{
    static const auto context = CONTENT + " ConfigurationParams:";
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsArrayOf< ConfigurationItem >(
        context, data, Identifier::items, true );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsUri( context, data, Identifier::uri, true );
    Content::validatePropertyIsNumber( context, data, Identifier::type, true );
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

Data DidChangeWatchedFilesParams::changes( void ) const
{
    return operator[]( Identifier::changes );
}

void DidChangeWatchedFilesParams::validate( const Data& data )
{
    static const auto context = CONTENT + " DidChangeWatchedFilesParams:";
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsArray( context, data, Identifier::changes, true );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsString( context, data, Identifier::query, true );
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
    operator[]( Identifier::location ) = location;
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

void SymbolInformation::validate( const Data& data )
{
    static const auto context = CONTENT + " SymbolInformation:";
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsString( context, data, Identifier::name, true );
    Content::validatePropertyIsNumber( context, data, Identifier::kind, true );
    Content::validatePropertyIs< Location >( context, data, Identifier::location, true );
    Content::validatePropertyIsString( context, data, Identifier::containerName, false );
    Content::validatePropertyIsBoolean( context, data, Identifier::deprecated, false );
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

WorkspaceSymbolResult::WorkspaceSymbolResult(
    const std::vector< SymbolInformation >& symbolInformation )
: Data( Data::object() )
{
    operator[]( Identifier::symbolInformation ) = Data::array();

    for( auto information : symbolInformation )
    {
        operator[]( Identifier::symbolInformation ).push_back( information );
    }
}

void WorkspaceSymbolResult::addSymbolInformation( const SymbolInformation& information )
{
    operator[]( Identifier::symbolInformation ).push_back( information );
}

Data WorkspaceSymbolResult::symbolInformation( void ) const
{
    return operator[]( Identifier::symbolInformation );
}

void WorkspaceSymbolResult::validate( const Data& data )
{
    static const auto context = CONTENT + " workspaceSymbolResult:";
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsArrayOf< SymbolInformation >(
        context, data, Identifier::symbolInformation, true );
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
    operator[]( Identifier::edit ) = edit;
}

WorkspaceEdit ApplyWorkspaceEditParams::edit( void ) const
{
    return WorkspaceEdit( operator[]( Identifier::edit ) );
}

u1 ApplyWorkspaceEditParams::hasLabel( void ) const
{
    return find( Identifier::edit ) != end();
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIs< WorkspaceEdit >( context, data, Identifier::edit, true );
    Content::validatePropertyIsString( context, data, Identifier::label, false );
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

u1 ApplyWorkspaceEditResult::isApplied( void ) const
{
    return operator[]( Identifier::applied ).get< u1 >();
}

void ApplyWorkspaceEditResult::applied( u1 applied )
{
    operator[]( Identifier::applied ) = applied;
}

void ApplyWorkspaceEditResult::validate( const Data& data )
{
    static const auto context = CONTENT + " ApplyWorkspaceEditResult:";
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsBoolean( context, data, Identifier::applied, true );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIs< TextDocumentItem >(
        context, data, Identifier::textDocument, false );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsString( context, data, Identifier::text, true );
    Content::validatePropertyIs< Range >( context, data, Identifier::range, false );
    Content::validatePropertyIsNumber( context, data, Identifier::rangeLength, false );
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
    const std::vector< TextDocumentContentChangeEvent >& contentChanges )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument ) =
        Data::from_cbor( Data::to_cbor( Identifier::textDocument ) );
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

Data DidChangeTextDocumentParams::contentChanges( void ) const
{
    return at( Identifier::contentChanges );
}

void DidChangeTextDocumentParams::validate( const Data& data )
{
    static const auto context = CONTENT + " DidChangeTextDocumentParams:";
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIs< VersionedTextDocumentIdentifier >(
        context, data, Identifier::textDocument, true );
    Content::validatePropertyIsArrayOf< TextDocumentContentChangeEvent >(
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
    const TextDocumentIdentifier& textdocument, TextDocumentSaveReason reason )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument ) = textdocument;
    operator[]( Identifier::reason ) = static_cast< std::size_t >( reason );
}

TextDocumentSaveReason WillSaveTextDocumentParams::reason( void ) const
{
    return static_cast< TextDocumentSaveReason >( operator[]( Identifier::reason )
                                                      .get< std::size_t >() );
}

TextDocumentIdentifier WillSaveTextDocumentParams::textdocument( void ) const
{
    return TextDocumentIdentifier( operator[]( Identifier::textDocument ) );
}

void WillSaveTextDocumentParams::validate( const Data& data )
{
    static const auto context = CONTENT + " WillSaveTextDocumentParams:";
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIs< TextDocumentIdentifier >(
        context, data, Identifier::textDocument, true );
    Content::validatePropertyIsNumber( context, data, Identifier::reason, true );
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
        operator[]( Identifier::textEdit ) = edit;
    }
}

Data WillSaveWaitUntilResponse::textEdit( void ) const
{
    return operator[]( Identifier::textEdit );
}

void WillSaveWaitUntilResponse::validate( const Data& data )
{
    static const auto context = CONTENT + " WillSaveWaitUntilResponse:";
    Content::validateTypeIsObject( context, data );
    if( data.find( Identifier::edit ) != data.end() and data[ Identifier::edit ].is_null() )
    {
        // ok, due to the possibility that the property can be null
    }
    else
    {
        Content::validatePropertyIsArrayOf< TextEdit >( context, data, Identifier::edit, true );
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

DidSaveTextDocumentParams::DidSaveTextDocumentParams(
    const TextDocumentIdentifier& textDocumentID, const std::string& text )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument ) = textDocumentID;
    operator[]( Identifier::text ) = text;
}

DidSaveTextDocumentParams::DidSaveTextDocumentParams( const TextDocumentIdentifier& textDocumentID )
{
    operator[]( Identifier::textDocument ) = textDocumentID;
}

u1 DidSaveTextDocumentParams::hasText( void ) const
{
    return find( Identifier::text ) != end();
}

void DidSaveTextDocumentParams::setText( const std::string& text )
{
    operator[]( Identifier::text ) = text;
}

TextDocumentIdentifier DidSaveTextDocumentParams::textDocumentID( void ) const
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIs< TextDocumentIdentifier >(
        context, data, Identifier::textDocument, true );
    Content::validatePropertyIsString( context, data, Identifier::text, false );
}

//
//
// DidCloseTextDocumentParams
//

DidCloseTextDocumentParams::DidCloseTextDocumentParams( const TextDocumentIdentifier& textDocument )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument ) = textDocument;
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIs< TextDocumentIdentifier >(
        context, data, Identifier::textDocument, true );
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
    operator[]( Identifier::documentation ) = doc;
}

MarkupContent CompletionItem::docuemtation( void ) const
{
    return MarkupContent( at( Identifier::documentation ) );
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
    return find( Identifier::preselected ) != end();
}

void CompletionItem::setPreselected( const u1 preselected )
{
    operator[]( Identifier::preselected ) = preselected;
}

u1 CompletionItem::isPreselected( void ) const
{
    return at( Identifier::preselected ).get< u1 >();
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
    operator[]( Identifier::textEdit ) = textEdit;
}

TextEdit CompletionItem::textEdit( void ) const
{
    return TextEdit( at( Identifier::textEdit ) );
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

Data CompletionItem::additionalTextEdits( void ) const
{
    return at( Identifier::additionalTextEdits );
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

Data CompletionItem::commitCharacters( void ) const
{
    return at( Identifier::commitCharacters );
}

u1 CompletionItem::hasCommand( void ) const
{
    return find( Identifier::command ) != end();
}

void CompletionItem::setCommand( const Command& command )
{
    operator[]( Identifier::command ) = command;
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsString( context, data, Identifier::label, true );
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

CompletionList::CompletionList( const u1 isIncomplete, const std::vector< CompletionItem >& items )
: Data( Data::object() )
{
    operator[]( Identifier::isIncomplete ) = isIncomplete;
    operator[]( Identifier::items ) = Data::array();
    for( auto item : items )
    {
        operator[]( Identifier::items ).push_back( item );
    }
}

Data CompletionList::items( void ) const
{
    return operator[]( Identifier::items );
}

u1 CompletionList::isIncomplete( void ) const
{
    return operator[]( Identifier::isIncomplete ).get< u1 >();
}

void CompletionList::validate( const Data& data )
{
    static const auto context = CONTENT + " CompletionList:";
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsArrayOf< CompletionItem >( context, data, Identifier::items, true );
    Content::validatePropertyIsBoolean( context, data, Identifier::isIncomplete, true );
}

//
//
// MarkupContent
//

std::string markupKind_toStr( MarkupKind kind )
{
    switch( kind )
    {
        case MarkupKind::PLAINTEXT:
        {
            return Identifier::plaintext;
            break;
        }
        case MarkupKind::MARKDOWN:
        {
            return Identifier::markdown;
            break;
        }
        default:
            return Identifier::plaintext;
    }
}

MarkupContent::MarkupContent( const Data& data )
: Data( data )
{
    validate( data );
}

MarkupContent::MarkupContent( const MarkupKind kind, const std::string& value )
: Data( Data::object() )
{
    operator[]( Identifier::kind ) = markupKind_toStr( kind );
    operator[]( Identifier::value ) = value;
}

MarkupContent::MarkupContent( const std::string& value )
{
    operator[]( Identifier::kind ) = markupKind_toStr( MarkupKind::PLAINTEXT );
    operator[]( Identifier::value ) = value;
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsString( context, data, Identifier::value, true );
    Content::validatePropertyIsString( context, data, Identifier::kind, true );
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
    operator[]( Identifier::documentation ) = doc;
}

void ParameterInformation::validate( const Data& data )
{
    static const auto context = CONTENT + " ParameterInformation:";
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsString( context, data, Identifier::label, true );
    Content::validatePropertyIsString( context, data, Identifier::documentation, false );
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
    operator[]( Identifier::documentation ) = doc;
}

u1 SignatureInformation::hasParameters( void ) const
{
    return find( Identifier::parameters ) != end();
}

Data SignatureInformation::parameters( void ) const
{
    return at( Identifier::parameters );
}

void SignatureInformation::setParameters( const std::vector< ParameterInformation >& parameters )
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsString( context, data, Identifier::label, true );
    Content::validatePropertyIsString( context, data, Identifier::documentation, false );
    Content::validatePropertyIsArrayOf< ParameterInformation >(
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

SignatureHelp::SignatureHelp( const std::vector< SignatureInformation >& signatures )
: Data( Data::object() )
{
    operator[]( Identifier::signatures ) = Data::array();
    for( auto signature : signatures )
    {
        operator[]( Identifier::signatures ).push_back( signature );
    }
}

Data SignatureHelp::signatures( void ) const
{
    return operator[]( Identifier::signatures );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsArrayOf< SignatureInformation >(
        context, data, Identifier::signatures, true );
    Content::validatePropertyIsNumber( context, data, Identifier::activeSignature, false );
    Content::validatePropertyIsNumber( context, data, Identifier::activeParameter, false );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsBoolean( context, data, Identifier::includeText, false );
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

Data CodeActionContext::diagnostics( void ) const
{
    return at( Identifier::diagnostics );
}

void CodeActionContext::validate( const Data& data )
{
    static const auto context = CONTENT + " CodeActionContext:";
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsArrayOf< TextDocumentContentChangeEvent >(
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
    operator[]( Identifier::textDocument ) =
        Data::from_cbor( Data::to_cbor( Identifier::textDocument ) );
    operator[]( Identifier::range ) = Data::from_cbor( Data::to_cbor( Identifier::range ) );
    operator[]( Identifier::context ) = Data::from_cbor( Data::to_cbor( Identifier::context ) );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIs< TextDocumentIdentifier >(
        context, data, Identifier::textDocument, true );
    Content::validatePropertyIs< Range >( context, data, Identifier::range, true );
    Content::validatePropertyIs< CodeActionContext >( context, data, Identifier::context, true );
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
    Content::validateTypeIsArrayOf< Command >( context, data );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsUri( context, data, Identifier::uri, true );
    Content::validatePropertyIsArrayOf< Diagnostic >(
        context, data, Identifier::diagnostics, true );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsString( context, data, Identifier::language, true );
    Content::validatePropertyIsString( context, data, Identifier::value, true );
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
    static const auto context = CONTENT + " HoverResult:";
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsArrayOf< MarkedString >( context, data, Identifier::contents, true );
    Content::validatePropertyIsString( context, data, Identifier::value, true );
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
    Content::validateTypeIsObject( context, data );
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
                Content::validatePropertyIsArrayOf< Location >(
                    context, data, Identifier::location, true );
            }
            else
            {
                // object is a single Location object
                Content::validatePropertyIs< Location >(
                    context, data, Identifier::location, true );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIs< TextDocumentIdentifier >(
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIs< Range >( context, data, Identifier::range, true );
    Content::validatePropertyIs< Command >( context, data, Identifier::command, false );
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
    Content::validateTypeIsArrayOf< CodeLens >( context, data );
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
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsString( context, data, Identifier::command, true );
    Content::validatePropertyIsArray( context, data, Identifier::arguments, false );
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
