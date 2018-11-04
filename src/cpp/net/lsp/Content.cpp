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

CompletionItem::CompletionItem( const Data& data )
: Data( data )
{
    validate( data );
}

CompletionItem::CompletionItem( void )
: Data( Data::object() )
{
}

u1 CompletionItem::hasSnippetSupport( void ) const
{
    return find( Identifier::snippetSupport ) != end();
}

u1 CompletionItem::snippetSupport( void ) const
{
    return at( Identifier::snippetSupport ).get< u1 >();
}

void CompletionItem::setSnippetSupport( const u1 snippetSupport )
{
    operator[]( Identifier::snippetSupport ) = snippetSupport;
}

void CompletionItem::validate( const Data& data )
{
    static const auto context = CONTENT + " CompletionItem:";
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsBoolean( context, data, Identifier::snippetSupport, false );
}

//
//
// Completion
//

Completion::Completion( const Data& data )
: Data( data )
{
    validate( data );
}

Completion::Completion( void )
: Data( Data::object() )
{
}

u1 Completion::hasDynamicRegistration( void ) const
{
    return find( Identifier::dynamicRegistration ) != end();
}

u1 Completion::dynamicRegistration( void ) const
{
    return at( Identifier::dynamicRegistration ).get< u1 >();
}

void Completion::setDynamicRegistration( const u1 dynamicRegistration )
{
    operator[]( Identifier::dynamicRegistration ) = dynamicRegistration;
}

u1 Completion::hasCompletionItem( void ) const
{
    return find( Identifier::completionItem ) != end();
}

CompletionItem Completion::completionItem( void ) const
{
    return at( Identifier::completionItem );
}

void Completion::completionItem( const CompletionItem& completionItem )
{
    operator[]( Identifier::completionItem ) = Data::from_cbor( Data::to_cbor( completionItem ) );
}

void Completion::validate( const Data& data )
{
    static const auto context = CONTENT + " Completion:";
    Content::validateTypeIsObject( context, data );
    Content::validatePropertyIsBoolean( context, data, Identifier::dynamicRegistration, false );
    Content::validatePropertyIs< CompletionItem >(
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

Completion TextDocumentClientCapabilities::completion( void ) const
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
