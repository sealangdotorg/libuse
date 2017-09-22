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
// Response Error
//

ResponseError::ResponseError( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'ResponseError'" );
    }
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

u1 ResponseError::isValid( const Data& data )
{
    if( data.is_object() and data.find( Identifier::code ) != data.end()
        and data[ Identifier::code ].is_number()
        and data.find( Identifier::message ) != data.end()
        and data[ Identifier::message ].is_string() )
    {
        if( data.find( Identifier::data ) != data.end()
            and not data[ Identifier::data ].is_object() )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// Position
//

Position::Position( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument( "invalid data for interface 'Position'" );
    }
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

u1 Position::isValid( const Data& data )
{
    return data.is_object() and data.find( Identifier::line ) != data.end()
           and data[ Identifier::line ].is_number()
           and data.find( Identifier::character ) != data.end()
           and data[ Identifier::character ].is_number();
}

//
//
// Range
//

Range::Range( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument( "invalid data for interface 'Range'" );
    }
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

u1 Range::isValid( const Data& data )
{
    return data.is_object() and data.find( Identifier::start ) != data.end()
           and Position::isValid( data[ Identifier::start ] )
           and data.find( Identifier::end ) != data.end()
           and Position::isValid( data[ Identifier::end ] );
}

//
//
// Location
//

Location::Location( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument( "invalid data for interface 'Location'" );
    }
}

Location::Location( const DocumentUri& uri, const Range& range )
: Data( Data::object() )
{
    operator[]( Identifier::uri ) = uri.toString();
    operator[]( Identifier::range ) = Data::from_cbor( Data::to_cbor( range ) );
}

DocumentUri Location::uri( void ) const
{
    return DocumentUri::fromString(
        at( Identifier::uri ).get< std::string >() );
}

Range Location::range( void ) const
{
    return at( Identifier::range );
}

u1 Location::isValid( const Data& data )
{
    if( data.is_object() and data.find( Identifier::uri ) != data.end()
        and data[ Identifier::uri ].is_string()
        and data.find( Identifier::range ) != data.end()
        and data[ Identifier::range ].is_object() )
    {
        if( not Position::isValid( data[ Identifier::range ] ) )
        {
            return false;
        }

        try
        {
            DocumentUri::fromString(
                data[ Identifier::uri ].get< std::string >() );
        }
        catch( const std::exception& e )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// Diagnostic
//

Diagnostic::Diagnostic( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'Diagnostic'" );
    }
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
    return static_cast< DiagnosticSeverity >(
        at( Identifier::severity ).get< std::size_t >() );
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

u1 Diagnostic::isValid( const Data& data )
{
    if( data.is_object() and data.find( Identifier::range ) != data.end()
        and data[ Identifier::range ].is_object()
        and data.find( Identifier::message ) != data.end()
        and data[ Identifier::message ].is_string() )
    {
        if( not Range::isValid( data[ Identifier::range ] ) )
        {
            return false;
        }

        if( data.find( Identifier::severity ) != data.end()
            and not data[ Identifier::severity ].is_number() )
        {
            return false;
        }

        if( data.find( Identifier::code ) != data.end()
            and not( data[ Identifier::code ].is_string()
                     or data[ Identifier::code ].is_number() ) )
        {
            return false;
        }

        if( data.find( Identifier::source ) != data.end()
            and not data[ Identifier::source ].is_string() )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// Command
//

Command::Command( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument( "invalid data for interface 'Command'" );
    }
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

u1 Command::isValid( const Data& data )
{
    if( data.is_object() and data.find( Identifier::title ) != data.end()
        and data[ Identifier::title ].is_string()
        and data.find( Identifier::command ) != data.end()
        and data[ Identifier::command ].is_string() )
    {
        if( data.find( Identifier::arguments ) != data.end()
            and not data[ Identifier::arguments ].is_array() )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// TextEdit
//

TextEdit::TextEdit( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument( "invalid data for interface 'TextEdit'" );
    }
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

u1 TextEdit::isValid( const Data& data )
{
    if( data.is_object() and data.find( Identifier::range ) != data.end()
        and data[ Identifier::range ].is_object()
        and data.find( Identifier::newText ) != data.end()
        and data[ Identifier::newText ].is_string() )
    {
        if( not Range::isValid( data[ Identifier::range ] ) )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// TextDocumentIdentifier
//

TextDocumentIdentifier::TextDocumentIdentifier( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'TextDocumentIdentifier'" );
    }
}

TextDocumentIdentifier::TextDocumentIdentifier( const DocumentUri& uri )
: Data( Data::object() )
{
    operator[]( Identifier::uri ) = uri.toString();
}

DocumentUri TextDocumentIdentifier::uri( void ) const
{
    return DocumentUri::fromString(
        at( Identifier::uri ).get< std::string >() );
}

u1 TextDocumentIdentifier::isValid( const Data& data )
{
    if( data.is_object() and data.find( Identifier::uri ) != data.end()
        and data[ Identifier::uri ].is_string() )
    {
        try
        {
            DocumentUri::fromString(
                data[ Identifier::uri ].get< std::string >() );
        }
        catch( const std::exception& e )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// VersionedTextDocumentIdentifier
//

VersionedTextDocumentIdentifier::VersionedTextDocumentIdentifier(
    const Data& data )
: TextDocumentIdentifier( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'VersionedTextDocumentIdentifier'" );
    }
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

u1 VersionedTextDocumentIdentifier::isValid( const Data& data )
{
    // if( TextDocumentIdentifier::isValid( data ) )
    // {
    if( data.find( Identifier::version ) != data.end()
        and not data[ Identifier::version ].is_number() )
    {
        return false;
    }

    return true;
    // }
    // else
    // {
    //     return false;
    // }
}

//
//
// TextDocumentEdit
//

TextDocumentEdit::TextDocumentEdit( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'TextDocumentEdit'" );
    }
}

TextDocumentEdit::TextDocumentEdit(
    const VersionedTextDocumentIdentifier& textDocument,
    const std::vector< TextEdit >& edits )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument )
        = Data::from_cbor( Data::to_cbor( textDocument ) );
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

u1 TextDocumentEdit::isValid( const Data& data )
{
    if( data.is_object() and data.find( Identifier::textDocument ) != data.end()
        and data[ Identifier::textDocument ].is_object()
        and data.find( Identifier::edits ) != data.end()
        and data[ Identifier::edits ].is_array() )
    {
        if( not VersionedTextDocumentIdentifier::isValid(
                data[ Identifier::textDocument ] ) )
        {
            return false;
        }

        for( auto edit : data[ Identifier::edits ] )
        {
            if( not TextEdit::isValid( edit ) )
            {
                return false;
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// WorkspaceEdit
//

WorkspaceEdit::WorkspaceEdit( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'WorkspaceEdit'" );
    }
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

u1 WorkspaceEdit::isValid( const Data& data )
{
    if( data.is_object()
        and data.find( Identifier::documentChanges ) != data.end()
        and data[ Identifier::documentChanges ].is_array() )
    {
        for( auto documentChange : data[ Identifier::documentChanges ] )
        {
            if( not TextDocumentEdit::isValid( documentChange ) )
            {
                return false;
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// TextDocumentItem
//

TextDocumentItem::TextDocumentItem( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'TextDocumentItem'" );
    }
}

TextDocumentItem::TextDocumentItem( const DocumentUri& uri,
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
    return DocumentUri::fromString(
        at( Identifier::uri ).get< std::string >() );
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

u1 TextDocumentItem::isValid( const Data& data )
{
    if( data.is_object() and data.find( Identifier::uri ) != data.end()
        and data[ Identifier::uri ].is_string()
        and data.find( Identifier::languageId ) != data.end()
        and data[ Identifier::languageId ].is_string()
        and data.find( Identifier::version ) != data.end()
        and data[ Identifier::version ].is_number()
        and data.find( Identifier::text ) != data.end()
        and data[ Identifier::text ].is_string() )
    {
        try
        {
            DocumentUri::fromString(
                data[ Identifier::uri ].get< std::string >() );
        }
        catch( const std::exception& e )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// TextDocumentPositionParams
//

TextDocumentPositionParams::TextDocumentPositionParams( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'TextDocumentPositionParams'" );
    }
}

TextDocumentPositionParams::TextDocumentPositionParams(
    const TextDocumentIdentifier& textDocument, const Position& position )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument )
        = Data::from_cbor( Data::to_cbor( textDocument ) );
    operator[]( Identifier::position )
        = Data::from_cbor( Data::to_cbor( position ) );
}

TextDocumentIdentifier TextDocumentPositionParams::textDocument( void ) const
{
    return at( Identifier::textDocument );
}

Position TextDocumentPositionParams::position( void ) const
{
    return at( Identifier::position );
}

u1 TextDocumentPositionParams::isValid( const Data& data )
{
    if( data.is_object() and data.find( Identifier::textDocument ) != data.end()
        and data[ Identifier::textDocument ].is_object()
        and data.find( Identifier::position ) != data.end()
        and data[ Identifier::position ].is_object() )
    {
        if( not TextDocumentIdentifier::isValid(
                data[ Identifier::textDocument ] ) )
        {
            return false;
        }

        if( not Position::isValid( data[ Identifier::position ] ) )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// DocumentFilter
//

DocumentFilter::DocumentFilter( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'DocumentFilter'" );
    }
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

u1 DocumentFilter::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( Identifier::language ) != data.end()
            and not data[ Identifier::language ].is_string() )
        {
            return false;
        }

        if( data.find( Identifier::scheme ) != data.end()
            and not data[ Identifier::scheme ].is_string() )
        {
            return false;
        }

        if( data.find( Identifier::pattern ) != data.end()
            and not data[ Identifier::pattern ].is_string() )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// DocumentSelector
//

DocumentSelector::DocumentSelector( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'DocumentSelector'" );
    }
}

DocumentSelector::DocumentSelector(
    const std::vector< DocumentFilter >& documentFilters )
: Data( Data::array() )
{
    for( auto documentFilter : documentFilters )
    {
        this->push_back( documentFilter );
    }
}

u1 DocumentSelector::isValid( const Data& data )
{
    if( data.is_array() )
    {
        for( auto documentFilter : data )
        {
            if( not DocumentFilter::isValid( documentFilter ) )
            {
                return false;
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// DynamicRegistration
//

DynamicRegistration::DynamicRegistration( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'DynamicRegistration'" );
    }
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

u1 DynamicRegistration::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( Identifier::dynamicRegistration ) != data.end()
            and not data[ Identifier::dynamicRegistration ].is_boolean() )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// WorkspaceClientCapabilities
//

WorkspaceClientCapabilities::WorkspaceClientCapabilities( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'WorkspaceClientCapabilities'" );
    }
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

void WorkspaceClientCapabilities::setWorkspaceEdit(
    const WorkspaceEdit& workspaceEdit )
{
    operator[]( Identifier::workspaceEdit )
        = Data::from_cbor( Data::to_cbor( workspaceEdit ) );
}

u1 WorkspaceClientCapabilities::hasDidChangeConfiguration( void ) const
{
    return find( Identifier::didChangeConfiguration ) != end();
}

DynamicRegistration WorkspaceClientCapabilities::didChangeConfiguration(
    void ) const
{
    return at( Identifier::didChangeConfiguration );
}

void WorkspaceClientCapabilities::setDidChangeConfiguration(
    const DynamicRegistration& didChangeConfiguration )
{
    operator[]( Identifier::didChangeConfiguration )
        = Data::from_cbor( Data::to_cbor( didChangeConfiguration ) );
}

u1 WorkspaceClientCapabilities::hasDidChangeWatchedFiles( void ) const
{
    return find( Identifier::didChangeWatchedFiles ) != end();
}

DynamicRegistration WorkspaceClientCapabilities::didChangeWatchedFiles(
    void ) const
{
    return at( Identifier::didChangeWatchedFiles );
}

void WorkspaceClientCapabilities::didChangeWatchedFiles(
    const DynamicRegistration& didChangeWatchedFiles )
{
    operator[]( Identifier::didChangeWatchedFiles )
        = Data::from_cbor( Data::to_cbor( didChangeWatchedFiles ) );
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
    operator[]( Identifier::symbol )
        = Data::from_cbor( Data::to_cbor( symbol ) );
}

u1 WorkspaceClientCapabilities::hasExecuteCommand( void ) const
{
    return find( Identifier::executeCommand ) != end();
}

DynamicRegistration WorkspaceClientCapabilities::executeCommand( void ) const
{
    return at( Identifier::executeCommand );
}

void WorkspaceClientCapabilities::executeCommand(
    const DynamicRegistration& executeCommand )
{
    operator[]( Identifier::executeCommand )
        = Data::from_cbor( Data::to_cbor( executeCommand ) );
}

u1 WorkspaceClientCapabilities::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( Identifier::applyEdit ) != data.end()
            and not data[ Identifier::applyEdit ].is_boolean() )
        {
            return false;
        }

        if( data.find( Identifier::workspaceEdit ) != data.end()
            and not WorkspaceEdit::isValid(
                    data[ Identifier::workspaceEdit ] ) )
        {
            return false;
        }

        if( data.find( Identifier::didChangeConfiguration ) != data.end()
            and not DynamicRegistration::isValid(
                    data[ Identifier::didChangeConfiguration ] ) )
        {
            return false;
        }

        if( data.find( Identifier::didChangeWatchedFiles ) != data.end()
            and not DynamicRegistration::isValid(
                    data[ Identifier::didChangeWatchedFiles ] ) )
        {
            return false;
        }

        if( data.find( Identifier::symbol ) != data.end()
            and not DynamicRegistration::isValid( data[ Identifier::symbol ] ) )
        {
            return false;
        }

        if( data.find( Identifier::executeCommand ) != data.end()
            and not DynamicRegistration::isValid(
                    data[ Identifier::executeCommand ] ) )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// Synchronization
//

Synchronization::Synchronization( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'Synchronization'" );
    }
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

u1 Synchronization::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( Identifier::dynamicRegistration ) != data.end()
            and not data[ Identifier::dynamicRegistration ].is_boolean() )
        {
            return false;
        }

        if( data.find( Identifier::willSave ) != data.end()
            and not data[ Identifier::willSave ].is_boolean() )
        {
            return false;
        }

        if( data.find( Identifier::willSaveWaitUntil ) != data.end()
            and not data[ Identifier::willSaveWaitUntil ].is_boolean() )
        {
            return false;
        }

        if( data.find( Identifier::didSave ) != data.end()
            and not data[ Identifier::didSave ].is_boolean() )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// CompletionItem
//

CompletionItem::CompletionItem( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'CompletionItem'" );
    }
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

u1 CompletionItem::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( Identifier::snippetSupport ) != data.end()
            and not data[ Identifier::snippetSupport ].is_boolean() )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// Completion
//

Completion::Completion( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'Completion'" );
    }
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
    operator[]( Identifier::completionItem )
        = Data::from_cbor( Data::to_cbor( completionItem ) );
}

u1 Completion::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( Identifier::dynamicRegistration ) != data.end()
            and not data[ Identifier::dynamicRegistration ].is_boolean() )
        {
            return false;
        }

        if( data.find( Identifier::completionItem ) != data.end()
            and not CompletionItem::isValid(
                    data[ Identifier::completionItem ] ) )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// TextDocumentClientCapabilities
//

TextDocumentClientCapabilities::TextDocumentClientCapabilities(
    const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'TextDocumentClientCapabilities'" );
    }
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

void TextDocumentClientCapabilities::setSynchronization(
    const Synchronization& synchronization )
{
    operator[]( Identifier::synchronization )
        = Data::from_cbor( Data::to_cbor( synchronization ) );
}

u1 TextDocumentClientCapabilities::hasCompletion( void ) const
{
    return find( Identifier::completion ) != end();
}

Completion TextDocumentClientCapabilities::completion( void ) const
{
    return at( Identifier::completion );
}

void TextDocumentClientCapabilities::setCompletion(
    const Completion& completion )
{
    operator[]( Identifier::completion )
        = Data::from_cbor( Data::to_cbor( completion ) );
}

u1 TextDocumentClientCapabilities::hasHover( void ) const
{
    return find( Identifier::hover ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::hover( void ) const
{
    return at( Identifier::hover );
}

void TextDocumentClientCapabilities::setHover(
    const DynamicRegistration& hover )
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

void TextDocumentClientCapabilities::setSignatureHelp(
    const DynamicRegistration& signatureHelp )
{
    operator[]( Identifier::signatureHelp )
        = Data::from_cbor( Data::to_cbor( signatureHelp ) );
}

u1 TextDocumentClientCapabilities::hasReferences( void ) const
{
    return find( Identifier::references ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::references( void ) const
{
    return at( Identifier::references );
}

void TextDocumentClientCapabilities::setReferences(
    const DynamicRegistration& references )
{
    operator[]( Identifier::references )
        = Data::from_cbor( Data::to_cbor( references ) );
}

u1 TextDocumentClientCapabilities::hasDocumentHighlight( void ) const
{
    return find( Identifier::documentHighlight ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::documentHighlight(
    void ) const
{
    return at( Identifier::documentHighlight );
}

void TextDocumentClientCapabilities::setDocumentHighlight(
    const DynamicRegistration& documentHighlight )
{
    operator[]( Identifier::documentHighlight )
        = Data::from_cbor( Data::to_cbor( documentHighlight ) );
}

u1 TextDocumentClientCapabilities::hasDocumentSymbol( void ) const
{
    return find( Identifier::documentSymbol ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::documentSymbol( void ) const
{
    return at( Identifier::documentSymbol );
}

void TextDocumentClientCapabilities::setDocumentSymbol(
    const DynamicRegistration& documentSymbol )
{
    operator[]( Identifier::documentSymbol )
        = Data::from_cbor( Data::to_cbor( documentSymbol ) );
}

u1 TextDocumentClientCapabilities::hasFormatting( void ) const
{
    return find( Identifier::formatting ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::formatting( void ) const
{
    return at( Identifier::formatting );
}

void TextDocumentClientCapabilities::setFormatting(
    const DynamicRegistration& formatting )
{
    operator[]( Identifier::formatting )
        = Data::from_cbor( Data::to_cbor( formatting ) );
}

u1 TextDocumentClientCapabilities::hasRangeFormatting( void ) const
{
    return find( Identifier::rangeFormatting ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::rangeFormatting(
    void ) const
{
    return at( Identifier::rangeFormatting );
}

void TextDocumentClientCapabilities::setRangeFormatting(
    const DynamicRegistration& rangeFormatting )
{
    operator[]( Identifier::rangeFormatting )
        = Data::from_cbor( Data::to_cbor( rangeFormatting ) );
}

u1 TextDocumentClientCapabilities::hasOnTypeFormatting( void ) const
{
    return find( Identifier::onTypeFormatting ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::onTypeFormatting(
    void ) const
{
    return at( Identifier::onTypeFormatting );
}

void TextDocumentClientCapabilities::setOnTypeFormatting(
    const DynamicRegistration& onTypeFormatting )
{
    operator[]( Identifier::onTypeFormatting )
        = Data::from_cbor( Data::to_cbor( onTypeFormatting ) );
}

u1 TextDocumentClientCapabilities::hasDefinition( void ) const
{
    return find( Identifier::definition ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::definition( void ) const
{
    return at( Identifier::definition );
}

void TextDocumentClientCapabilities::setDefinition(
    const DynamicRegistration& definition )
{
    operator[]( Identifier::definition )
        = Data::from_cbor( Data::to_cbor( definition ) );
}

u1 TextDocumentClientCapabilities::hasCodeAction( void ) const
{
    return find( Identifier::codeAction ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::codeAction( void ) const
{
    return at( Identifier::codeAction );
}

void TextDocumentClientCapabilities::setCodeAction(
    const DynamicRegistration& codeAction )
{
    operator[]( Identifier::codeAction )
        = Data::from_cbor( Data::to_cbor( codeAction ) );
}

u1 TextDocumentClientCapabilities::hasCodeLens( void ) const
{
    return find( Identifier::codeLens ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::codeLens( void ) const
{
    return at( Identifier::codeLens );
}

void TextDocumentClientCapabilities::setCodeLens(
    const DynamicRegistration& codeLens )
{
    operator[]( Identifier::codeLens )
        = Data::from_cbor( Data::to_cbor( codeLens ) );
}

u1 TextDocumentClientCapabilities::hasDocumentLink( void ) const
{
    return find( Identifier::documentLink ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::documentLink( void ) const
{
    return at( Identifier::documentLink );
}

void TextDocumentClientCapabilities::setDocumentLink(
    const DynamicRegistration& documentLink )
{
    operator[]( Identifier::documentLink )
        = Data::from_cbor( Data::to_cbor( documentLink ) );
}

u1 TextDocumentClientCapabilities::hasRename( void ) const
{
    return find( Identifier::rename ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::rename( void ) const
{
    return at( Identifier::rename );
}

void TextDocumentClientCapabilities::setRename(
    const DynamicRegistration& rename )
{
    operator[]( Identifier::rename )
        = Data::from_cbor( Data::to_cbor( rename ) );
}

u1 TextDocumentClientCapabilities::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( Identifier::synchronization ) != data.end()
            and not Synchronization::isValid(
                    data[ Identifier::synchronization ] ) )
        {
            return false;
        }

        if( data.find( Identifier::completion ) != data.end()
            and not Completion::isValid( data[ Identifier::completion ] ) )
        {
            return false;
        }

        if( data.find( Identifier::hover ) != data.end()
            and not DynamicRegistration::isValid( data[ Identifier::hover ] ) )
        {
            return false;
        }

        if( data.find( Identifier::signatureHelp ) != data.end()
            and not DynamicRegistration::isValid(
                    data[ Identifier::signatureHelp ] ) )
        {
            return false;
        }

        if( data.find( Identifier::references ) != data.end()
            and not DynamicRegistration::isValid(
                    data[ Identifier::references ] ) )
        {
            return false;
        }

        if( data.find( Identifier::documentHighlight ) != data.end()
            and not DynamicRegistration::isValid(
                    data[ Identifier::documentHighlight ] ) )
        {
            return false;
        }

        if( data.find( Identifier::documentSymbol ) != data.end()
            and not DynamicRegistration::isValid(
                    data[ Identifier::documentSymbol ] ) )
        {
            return false;
        }

        if( data.find( Identifier::formatting ) != data.end()
            and not DynamicRegistration::isValid(
                    data[ Identifier::formatting ] ) )
        {
            return false;
        }

        if( data.find( Identifier::rangeFormatting ) != data.end()
            and not DynamicRegistration::isValid(
                    data[ Identifier::rangeFormatting ] ) )
        {
            return false;
        }

        if( data.find( Identifier::onTypeFormatting ) != data.end()
            and not DynamicRegistration::isValid(
                    data[ Identifier::onTypeFormatting ] ) )
        {
            return false;
        }

        if( data.find( Identifier::definition ) != data.end()
            and not DynamicRegistration::isValid(
                    data[ Identifier::definition ] ) )
        {
            return false;
        }

        if( data.find( Identifier::codeAction ) != data.end()
            and not DynamicRegistration::isValid(
                    data[ Identifier::codeAction ] ) )
        {
            return false;
        }

        if( data.find( Identifier::codeLens ) != data.end()
            and not DynamicRegistration::isValid(
                    data[ Identifier::codeLens ] ) )
        {
            return false;
        }

        if( data.find( Identifier::documentLink ) != data.end()
            and not DynamicRegistration::isValid(
                    data[ Identifier::documentLink ] ) )
        {
            return false;
        }

        if( data.find( Identifier::rename ) != data.end()
            and not DynamicRegistration::isValid( data[ Identifier::rename ] ) )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// ClientCapabilities
//

ClientCapabilities::ClientCapabilities( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'ClientCapabilities'" );
    }
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

void ClientCapabilities::setWorkspace(
    const WorkspaceClientCapabilities& workspace )
{
    operator[]( Identifier::workspace )
        = Data::from_cbor( Data::to_cbor( workspace ) );
}

u1 ClientCapabilities::hasTextDocument( void ) const
{
    return find( Identifier::textDocument ) != end();
}

TextDocumentClientCapabilities ClientCapabilities::textDocument( void ) const
{
    return at( Identifier::textDocument );
}

void ClientCapabilities::setTextDocument(
    const TextDocumentClientCapabilities& textDocument )
{
    operator[]( Identifier::textDocument )
        = Data::from_cbor( Data::to_cbor( textDocument ) );
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
    operator[]( Identifier::experimental )
        = Data::from_cbor( Data::to_cbor( experimental ) );
}

u1 ClientCapabilities::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( Identifier::workspace ) != data.end()
            and not WorkspaceClientCapabilities::isValid(
                    data[ Identifier::workspace ] ) )
        {
            return false;
        }

        if( data.find( Identifier::textDocument ) != data.end()
            and not TextDocumentClientCapabilities::isValid(
                    data[ Identifier::textDocument ] ) )
        {
            return false;
        }

        if( data.find( Identifier::experimental ) != data.end()
            and not data[ Identifier::experimental ].is_object() )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// SaveOptions
//

SaveOptions::SaveOptions( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'SaveOptions'" );
    }
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

u1 SaveOptions::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( Identifier::includeText ) != data.end()
            and not data[ Identifier::includeText ].is_boolean() )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// TextDocumentSyncOptions
//

TextDocumentSyncOptions::TextDocumentSyncOptions( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'TextDocumentSyncOptions'" );
    }
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
    return static_cast< TextDocumentSyncKind >(
        at( Identifier::change ).get< std::size_t >() );
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

u1 TextDocumentSyncOptions::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( Identifier::openClose ) != data.end()
            and not data[ Identifier::openClose ].is_boolean() )
        {
            return false;
        }

        if( data.find( Identifier::change ) != data.end()
            and not data[ Identifier::change ].is_number() )
        {
            return false;
        }

        if( data.find( Identifier::willSave ) != data.end()
            and not data[ Identifier::willSave ].is_boolean() )
        {
            return false;
        }

        if( data.find( Identifier::willSaveWaitUntil ) != data.end()
            and not data[ Identifier::willSaveWaitUntil ].is_boolean() )
        {
            return false;
        }

        if( data.find( Identifier::save ) != data.end()
            and not SaveOptions::isValid( data[ Identifier::save ] ) )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// CompletionOptions
//

CompletionOptions::CompletionOptions( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'CompletionOptions'" );
    }
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

void CompletionOptions::addTriggerCharacters(
    const std::string& triggerCharacter )
{
    operator[]( Identifier::triggerCharacters ).push_back( triggerCharacter );
}

u1 CompletionOptions::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( Identifier::resolveProvider ) != data.end()
            and not data[ Identifier::resolveProvider ].is_boolean() )
        {
            return false;
        }

        if( data.find( Identifier::triggerCharacters ) != data.end()
            and data[ Identifier::triggerCharacters ].is_array() )
        {
            for( auto triggerCharacter : data[ Identifier::triggerCharacters ] )
            {
                if( not triggerCharacter.is_string() )
                {
                    return false;
                }
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// SignatureHelpOptions
//

SignatureHelpOptions::SignatureHelpOptions( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'SignatureHelpOptions'" );
    }
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

void SignatureHelpOptions::addTriggerCharacters(
    const std::string& triggerCharacter )
{
    operator[]( Identifier::triggerCharacters ).push_back( triggerCharacter );
}

u1 SignatureHelpOptions::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( Identifier::triggerCharacters ) != data.end()
            and data[ Identifier::triggerCharacters ].is_array() )
        {
            for( auto triggerCharacter : data[ Identifier::triggerCharacters ] )
            {
                if( not triggerCharacter.is_string() )
                {
                    return false;
                }
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// CodeLensOptions
//

CodeLensOptions::CodeLensOptions( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'CodeLensOptions'" );
    }
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

u1 CodeLensOptions::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( Identifier::resolveProvider ) != data.end()
            and not data[ Identifier::resolveProvider ].is_boolean() )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
//
//

DocumentOnTypeFormattingOptions::DocumentOnTypeFormattingOptions(
    const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'DocumentOnTypeFormattingOptions'" );
    }
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

void DocumentOnTypeFormattingOptions::addMoreTriggerCharacter(
    const Data& moreTriggerCharacter )
{
    operator[]( Identifier::moreTriggerCharacter )
        .push_back( moreTriggerCharacter );
}

u1 DocumentOnTypeFormattingOptions::isValid( const Data& data )
{
    if( data.is_object()
        and data.find( Identifier::firstTriggerCharacter ) != data.end()
        and data[ Identifier::firstTriggerCharacter ].is_string() )
    {
        if( data.find( Identifier::moreTriggerCharacter ) != data.end()
            and not data[ Identifier::moreTriggerCharacter ].is_array() )
        {
            for( auto moreTriggerCharacter :
                data[ Identifier::moreTriggerCharacter ] )
            {
                if( not moreTriggerCharacter.is_string() )
                {
                    return false;
                }
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// ExecuteCommandOptions
//

ExecuteCommandOptions::ExecuteCommandOptions( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'ExecuteCommandOptions'" );
    }
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

u1 ExecuteCommandOptions::isValid( const Data& data )
{
    if( data.is_object() and data.find( Identifier::commands ) != data.end()
        and data[ Identifier::commands ].is_array() )
    {
        for( auto command : data[ Identifier::commands ] )
        {
            if( not command.is_string() )
            {
                return false;
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// ServerCapabilities
//

ServerCapabilities::ServerCapabilities( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'ServerCapabilities'" );
    }
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

void ServerCapabilities::setTextDocumentSync(
    const TextDocumentSyncOptions& textDocumentSync )
{
    operator[]( Identifier::textDocumentSync )
        = Data::from_cbor( Data::to_cbor( textDocumentSync ) );
}

void ServerCapabilities::setTextDocumentSync(
    const TextDocumentSyncKind& textDocumentSync )
{
    operator[]( Identifier::textDocumentSync )
        = static_cast< std::size_t >( textDocumentSync );
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

void ServerCapabilities::setCompletionProvider(
    const CompletionOptions& completionProvider )
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

void ServerCapabilities::setDocumentHighlightProvider(
    const u1 documentHighlightProvider )
{
    operator[]( Identifier::documentHighlightProvider )
        = documentHighlightProvider;
}

u1 ServerCapabilities::hasDocumentSymbolProvider( void ) const
{
    return find( Identifier::documentSymbolProvider ) != end();
}

u1 ServerCapabilities::documentSymbolProvider( void ) const
{
    return at( Identifier::documentSymbolProvider ).get< u1 >();
}

void ServerCapabilities::setDocumentSymbolProvider(
    const u1 documentSymbolProvider )
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

void ServerCapabilities::setWorkspaceSymbolProvider(
    const u1 workspaceSymbolProvider )
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

void ServerCapabilities::setCodeLensProvider(
    const CodeLensOptions& codeLensProvider )
{
    operator[]( Identifier::codeLensProvider )
        = Data::from_cbor( Data::to_cbor( codeLensProvider ) );
}

u1 ServerCapabilities::hasDocumentFormattingProvider( void ) const
{
    return find( Identifier::documentFormattingProvider ) != end();
}

u1 ServerCapabilities::documentFormattingProvider( void ) const
{
    return at( Identifier::documentFormattingProvider ).get< u1 >();
}

void ServerCapabilities::setDocumentFormattingProvider(
    const u1 documentFormattingProvider )
{
    operator[]( Identifier::documentFormattingProvider )
        = documentFormattingProvider;
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
    operator[]( Identifier::documentRangeFormattingProvider )
        = documentRangeFormattingProvider;
}

u1 ServerCapabilities::hasDocumentOnTypeFormattingProvider( void ) const
{
    return find( Identifier::documentOnTypeFormattingProvider ) != end();
}

DocumentOnTypeFormattingOptions
ServerCapabilities::documentOnTypeFormattingProvider( void ) const
{
    return at( Identifier::documentOnTypeFormattingProvider );
}

void ServerCapabilities::setDocumentOnTypeFormattingProvider(
    const DocumentOnTypeFormattingOptions& documentOnTypeFormattingProvider )
{
    operator[]( Identifier::documentOnTypeFormattingProvider )
        = Data::from_cbor( Data::to_cbor( documentOnTypeFormattingProvider ) );
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

void ServerCapabilities::setDocumentLinkProvider(
    const DocumentLinkOptions& documentLinkProvider )
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
    operator[]( Identifier::executeCommandProvider )
        = Data::from_cbor( Data::to_cbor( executeCommandProvider ) );
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
    operator[]( Identifier::experimental )
        = Data::from_cbor( Data::to_cbor( experimental ) );
}

u1 ServerCapabilities::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( Identifier::textDocumentSync ) != data.end()
            and not TextDocumentSyncOptions::isValid(
                    data[ Identifier::textDocumentSync ] ) )
        {
            return false;
        }

        if( data.find( Identifier::hoverProvider ) != data.end()
            and not data[ Identifier::hoverProvider ].is_boolean() )
        {
            return false;
        }

        if( data.find( Identifier::completionProvider ) != data.end()
            and not CompletionOptions::isValid(
                    data[ Identifier::completionProvider ] ) )
        {
            return false;
        }

        if( data.find( Identifier::signatureHelpProvider ) != data.end()
            and not SignatureHelpOptions::isValid(
                    data[ Identifier::signatureHelpProvider ] ) )
        {
            return false;
        }

        if( data.find( Identifier::definitionProvider ) != data.end()
            and not data[ Identifier::definitionProvider ].is_boolean() )
        {
            return false;
        }

        if( data.find( Identifier::referencesProvider ) != data.end()
            and not data[ Identifier::referencesProvider ].is_boolean() )
        {
            return false;
        }

        if( data.find( Identifier::documentHighlightProvider ) != data.end()
            and not data[ Identifier::documentHighlightProvider ].is_boolean() )
        {
            return false;
        }

        if( data.find( Identifier::documentSymbolProvider ) != data.end()
            and not data[ Identifier::documentSymbolProvider ].is_boolean() )
        {
            return false;
        }

        if( data.find( Identifier::workspaceSymbolProvider ) != data.end()
            and not data[ Identifier::workspaceSymbolProvider ].is_boolean() )
        {
            return false;
        }

        if( data.find( Identifier::codeActionProvider ) != data.end()
            and not data[ Identifier::codeActionProvider ].is_boolean() )
        {
            return false;
        }

        if( data.find( Identifier::codeLensProvider ) != data.end()
            and not CodeLensOptions::isValid(
                    data[ Identifier::codeLensProvider ] ) )
        {
            return false;
        }

        if( data.find( Identifier::documentFormattingProvider ) != data.end()
            and not data[ Identifier::documentFormattingProvider ]
                        .is_boolean() )
        {
            return false;
        }

        if( data.find( Identifier::documentRangeFormattingProvider )
                != data.end()
            and not data[ Identifier::documentRangeFormattingProvider ]
                        .is_boolean() )
        {
            return false;
        }

        if( data.find( Identifier::documentOnTypeFormattingProvider )
                != data.end()
            and not DocumentOnTypeFormattingOptions::isValid(
                    data[ Identifier::documentOnTypeFormattingProvider ] ) )
        {
            return false;
        }

        if( data.find( Identifier::renameProvider ) != data.end()
            and not data[ Identifier::renameProvider ].is_boolean() )
        {
            return false;
        }

        if( data.find( Identifier::documentLinkProvider ) != data.end()
            and not DocumentLinkOptions::isValid(
                    data[ Identifier::documentLinkProvider ] ) )
        {
            return false;
        }

        if( data.find( Identifier::executeCommandProvider ) != data.end()
            and not ExecuteCommandOptions::isValid(
                    data[ Identifier::executeCommandProvider ] ) )
        {
            return false;
        }

        if( data.find( Identifier::experimental ) != data.end()
            and not data[ Identifier::experimental ].is_object() )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
//
//

//
//
//
//

//
//
//
//

//
//
//
//

// -----------------------------------------------------------------------------

//
//
// InitializeParams
//

InitializeParams::InitializeParams( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'InitializeParams'" );
    }
}

InitializeParams::InitializeParams( const std::size_t processId,
    const DocumentUri& rootUri,
    const ClientCapabilities& capabilities )
: Data( Data::object() )
{
    operator[]( Identifier::processId ) = processId;
    operator[]( Identifier::rootUri ) = rootUri.toString();
    operator[]( Identifier::capabilities )
        = Data::from_cbor( Data::to_cbor( capabilities ) );
}

std::size_t InitializeParams::processId( void ) const
{
    return at( Identifier::processId ).get< std::size_t >();
}

DocumentUri InitializeParams::rootUri( void ) const
{
    return DocumentUri::fromString(
        at( Identifier::processId ).get< std::string >() );
}

u1 InitializeParams::hasInitializationOptions( void ) const
{
    return find( Identifier::initializationOptions ) != end();
}

Data InitializeParams::initializationOptions( void ) const
{
    return at( Identifier::initializationOptions );
}

void InitializeParams::setInitializationOptions(
    const Data& initializationOptions )
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

u1 InitializeParams::isValid( const Data& data )
{
    if( data.is_object() and data.find( Identifier::rootUri ) != data.end()
        and ( data[ Identifier::rootUri ].is_string()
                or data[ Identifier::rootUri ].is_null() )
        and data.find( Identifier::capabilities ) != data.end()
        and ClientCapabilities::isValid( data[ Identifier::capabilities ] ) )
    {
        if( data.find( Identifier::processId ) != data.end()
            and not( data[ Identifier::processId ].is_number()
                     or data[ Identifier::processId ].is_null() ) )
        {
            return false;
        }

        if( not data[ Identifier::rootUri ].is_null() )
        {
            try
            {
                DocumentUri::fromString(
                    data[ Identifier::uri ].get< std::string >() );
            }
            catch( const std::exception& e )
            {
                return false;
            }
        }

        if( data.find( Identifier::initializationOptions ) != data.end()
            and not data[ Identifier::initializationOptions ].is_object() )
        {
            return false;
        }

        if( data.find( Identifier::trace ) != data.end()
            and not data[ Identifier::trace ].is_string() )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// InitializeResult
//

InitializeResult::InitializeResult( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'InitializeResult'" );
    }
}

InitializeResult::InitializeResult( const ServerCapabilities& capabilities )
: Data( Data::object() )
{
    operator[]( Identifier::capabilities )
        = Data::from_cbor( Data::to_cbor( capabilities ) );
}

ServerCapabilities InitializeResult::capabilities( void ) const
{
    return at( Identifier::capabilities );
}

u1 InitializeResult::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( Identifier::capabilities ) != data.end()
            and not ServerCapabilities::isValid(
                    data[ Identifier::capabilities ] ) )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// InitializeError
//

InitializeError::InitializeError( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'InitializeError'" );
    }
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

u1 InitializeError::isValid( const Data& data )
{
    if( data.is_object() and data.find( Identifier::retry ) != data.end()
        and data[ Identifier::retry ].is_boolean() )
    {
        return true;
    }
    else
    {
        return false;
    }
}

//
//
// DidOpenTextDocumentParams
//

DidOpenTextDocumentParams::DidOpenTextDocumentParams( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'DidOpenTextDocumentParams'" );
    }
}

DidOpenTextDocumentParams::DidOpenTextDocumentParams(
    const TextDocumentItem& textDocument )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument )
        = Data::from_cbor( Data::to_cbor( textDocument ) );
}

TextDocumentItem DidOpenTextDocumentParams::textDocument( void ) const
{
    return at( Identifier::textDocument );
}

u1 DidOpenTextDocumentParams::isValid( const Data& data )
{
    if( data.is_object() and data.find( Identifier::textDocument ) != data.end()
        and TextDocumentItem::isValid( data[ Identifier::textDocument ] ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

//
//
// TextDocumentContentChangeEvent
//

TextDocumentContentChangeEvent::TextDocumentContentChangeEvent(
    const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'TextDocumentContentChangeEvent'" );
    }
}

TextDocumentContentChangeEvent::TextDocumentContentChangeEvent(
    const std::string& text )
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

void TextDocumentContentChangeEvent::setRangeLength(
    const std::size_t rangeLength )
{
    operator[]( Identifier::rangeLength ) = rangeLength;
}

std::string TextDocumentContentChangeEvent::text( void ) const
{
    return at( Identifier::text ).get< std::string >();
}

u1 TextDocumentContentChangeEvent::isValid( const Data& data )
{
    if( data.is_object() and data.find( Identifier::text ) != data.end()
        and data[ Identifier::text ].is_string() )
    {
        if( data.find( Identifier::range ) != data.end()
            and not Range::isValid( data[ Identifier::range ] ) )
        {
            return false;
        }

        if( data.find( Identifier::rangeLength ) != data.end()
            and not data[ Identifier::rangeLength ].is_number() )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// DidChangeTextDocumentParams
//

DidChangeTextDocumentParams::DidChangeTextDocumentParams( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'DidChangeTextDocumentParams'" );
    }
}

DidChangeTextDocumentParams::DidChangeTextDocumentParams(
    const VersionedTextDocumentIdentifier& textDocument,
    const std::vector< TextDocumentContentChangeEvent >& contentChanges )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument )
        = Data::from_cbor( Data::to_cbor( Identifier::textDocument ) );
    operator[]( Identifier::contentChanges ) = Data::array();

    for( auto contentChange : contentChanges )
    {
        operator[]( Identifier::contentChanges ).push_back( contentChange );
    }
}

VersionedTextDocumentIdentifier DidChangeTextDocumentParams::textDocument(
    void ) const
{
    return at( Identifier::textDocument );
}

Data DidChangeTextDocumentParams::contentChanges( void ) const
{
    return at( Identifier::contentChanges );
}

u1 DidChangeTextDocumentParams::isValid( const Data& data )
{
    if( data.is_object() and data.find( Identifier::textDocument ) != data.end()
        and data[ Identifier::textDocument ].is_object()
        and data.find( Identifier::contentChanges ) != data.end()
        and data[ Identifier::contentChanges ].is_array() )
    {
        if( not VersionedTextDocumentIdentifier::isValid(
                data[ Identifier::textDocument ] ) )
        {
            return false;
        }

        for( auto contentChange : data[ Identifier::contentChanges ] )
        {
            if( not TextDocumentContentChangeEvent::isValid( contentChange ) )
            {
                return false;
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// CodeActionContext
//

CodeActionContext::CodeActionContext( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'CodeActionContext'" );
    }
}

CodeActionContext::CodeActionContext(
    const std::vector< Diagnostic >& diagnostics )
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

u1 CodeActionContext::isValid( const Data& data )
{
    if( data.is_object() and data.find( Identifier::diagnostics ) != data.end()
        and data[ Identifier::diagnostics ].is_array() )
    {
        for( auto diagnostic : data[ Identifier::diagnostics ] )
        {
            if( not TextDocumentContentChangeEvent::isValid( diagnostic ) )
            {
                return false;
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// CodeActionParams
//

CodeActionParams::CodeActionParams( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'CodeActionParams'" );
    }
}

CodeActionParams::CodeActionParams( const TextDocumentIdentifier& textDocument,
    const Range& range, const CodeActionContext& context )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument )
        = Data::from_cbor( Data::to_cbor( Identifier::textDocument ) );
    operator[]( Identifier::range )
        = Data::from_cbor( Data::to_cbor( Identifier::range ) );
    operator[]( Identifier::context )
        = Data::from_cbor( Data::to_cbor( Identifier::context ) );
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

u1 CodeActionParams::isValid( const Data& data )
{
    if( data.is_object() and data.find( Identifier::textDocument ) != data.end()
        and data[ Identifier::textDocument ].is_object()
        and data.find( Identifier::range ) != data.end()
        and data[ Identifier::range ].is_object()
        and data.find( Identifier::context ) != data.end()
        and data[ Identifier::context ].is_object() )
    {
        return true;
    }
    else
    {
        return false;
    }
}

//
//
// CodeActionResult
//

CodeActionResult::CodeActionResult( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'CodeActionResult'" );
    }
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

u1 CodeActionResult::isValid( const Data& data )
{
    if( data.is_array() )
    {
        for( auto command : data )
        {
            if( not Command::isValid( command ) )
            {
                return false;
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// PublishDiagnosticsParams
//

PublishDiagnosticsParams::PublishDiagnosticsParams( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'PublishDiagnosticsParams'" );
    }
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
    return DocumentUri::fromString(
        at( Identifier::uri ).get< std::string >() );
}

Data PublishDiagnosticsParams::diagnostics( void ) const
{
    return at( Identifier::diagnostics );
}

u1 PublishDiagnosticsParams::isValid( const Data& data )
{
    if( data.is_object() and data.find( Identifier::uri ) != data.end()
        and data[ Identifier::uri ].is_string()
        and data.find( Identifier::diagnostics ) != data.end()
        and data[ Identifier::diagnostics ].is_array() )
    {
        try
        {
            DocumentUri::fromString(
                data[ Identifier::uri ].get< std::string >() );
        }
        catch( const std::exception& e )
        {
            return false;
        }

        for( auto diagnostic : data[ Identifier::diagnostics ] )
        {
            if( not Diagnostic::isValid( diagnostic ) )
            {
                return false;
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// MarkedString
//

MarkedString::MarkedString( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'MarkedString'" );
    }
}

MarkedString::MarkedString(
    const std::string& language, const std::string& value )
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

u1 MarkedString::isValid( const Data& data )
{
    if( data.is_string() )
    {
        return true;
    }
    else if( data.is_object()
             and data.find( Identifier::language ) != data.end()
             and data[ Identifier::language ].is_string()
             and data.find( Identifier::value ) != data.end()
             and data[ Identifier::value ].is_string() )
    {
        return true;
    }
    else
    {
        return false;
    }
}

//
//
// HoverResult
//

HoverResult::HoverResult( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'HoverResult'" );
    }
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

u1 HoverResult::isValid( const Data& data )
{
    if( data.is_object() and data.find( Identifier::contents ) != data.end()
        and ( data[ Identifier::contents ].is_object()
                or data[ Identifier::contents ].is_array() ) )
    {
        if( data[ Identifier::contents ].is_object()
            and not MarkedString::isValid( data[ Identifier::contents ] ) )
        {
            return false;
        }

        if( data[ Identifier::contents ].is_array() )
        {
            for( auto content : data[ Identifier::contents ] )
            {
                if( not MarkedString::isValid( content ) )
                {
                    return false;
                }
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// CodeLensParams
//

CodeLensParams::CodeLensParams( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'CodeLensParams'" );
    }
}

CodeLensParams::CodeLensParams( const TextDocumentIdentifier& textDocument )
: Data( Data::object() )
{
    operator[]( Identifier::textDocument )
        = Data::from_cbor( Data::to_cbor( textDocument ) );
}

TextDocumentIdentifier CodeLensParams::textDocument( void ) const
{
    return at( Identifier::textDocument );
}

u1 CodeLensParams::isValid( const Data& data )
{
    if( data.is_object() and data.find( Identifier::textDocument ) != data.end()
        and TextDocumentIdentifier::isValid(
                data[ Identifier::textDocument ] ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

//
//
// CodeLens
//

CodeLens::CodeLens( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument( "invalid data for interface 'CodeLens'" );
    }
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
    operator[]( Identifier::command )
        = Data::from_cbor( Data::to_cbor( command ) );
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

u1 CodeLens::isValid( const Data& data )
{
    if( data.is_object() and data.find( Identifier::range ) != data.end()
        and Range::isValid( data[ Identifier::range ] ) )
    {
        if( data.find( Identifier::command ) != data.end()
            and not Command::isValid( data[ Identifier::command ] ) )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// CodeLensResult
//

CodeLensResult::CodeLensResult( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'CodeLensResult'" );
    }
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

u1 CodeLensResult::isValid( const Data& data )
{
    if( data.is_array() )
    {
        for( auto codeLens : data )
        {
            if( not CodeLens::isValid( codeLens ) )
            {
                return false;
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

//
//
// ExecuteCommandParams
//

ExecuteCommandParams::ExecuteCommandParams( const Data& data )
: Data( data )
{
    if( not isValid( data ) )
    {
        throw std::invalid_argument(
            "invalid data for interface 'ExecuteCommandParams'" );
    }
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

u1 ExecuteCommandParams::isValid( const Data& data )
{
    if( data.is_object() and data.find( Identifier::command ) != data.end()
        and data[ Identifier::command ].is_string() )
    {
        if( data.find( Identifier::arguments ) != data.end()
            and not data[ Identifier::arguments ].is_array() )
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
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
