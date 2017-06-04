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
    operator[]( CODE ) = static_cast< i32 >( code );
    operator[]( MESSAGE ) = message;
}

ErrorCode ResponseError::code( void ) const
{
    const auto c = at( CODE ).get< i32 >();
    return static_cast< ErrorCode >( c );
}

std::string ResponseError::message( void ) const
{
    return at( MESSAGE );
}

u1 ResponseError::hasData( void ) const
{
    return find( DATA ) != end();
}

Data ResponseError::data( void ) const
{
    return at( DATA );
}

void ResponseError::setData( const Data& data )
{
    operator[]( DATA ) = Data::from_cbor( Data::to_cbor( data ) );
}

u1 ResponseError::isValid( const Data& data )
{
    if( data.is_object() and data.find( CODE ) != data.end()
        and data[ CODE ].is_number()
        and data.find( MESSAGE ) != data.end()
        and data[ MESSAGE ].is_string() )
    {
        if( data.find( DATA ) != data.end() and not data[ DATA ].is_object() )
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
    operator[]( LINE ) = line;
    operator[]( CHARACTER ) = character;
}

std::size_t Position::line( void ) const
{
    return at( LINE ).get< std::size_t >();
}

std::size_t Position::character( void ) const
{
    return at( CHARACTER ).get< std::size_t >();
}

u1 Position::isValid( const Data& data )
{
    return data.is_object() and data.find( LINE ) != data.end()
           and data[ LINE ].is_number() and data.find( CHARACTER ) != data.end()
           and data[ CHARACTER ].is_number();
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
    operator[]( START ) = Data::from_cbor( Data::to_cbor( start ) );
    operator[]( END ) = Data::from_cbor( Data::to_cbor( end ) );
}

Position Range::start( void ) const
{
    return at( START );
}

Position Range::end( void ) const
{
    return at( END );
}

u1 Range::isValid( const Data& data )
{
    return data.is_object() and data.find( START ) != data.end()
           and Position::isValid( data[ START ] )
           and data.find( END ) != data.end()
           and Position::isValid( data[ END ] );
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
    operator[]( URI ) = uri.toString();
    operator[]( RANGE ) = Data::from_cbor( Data::to_cbor( range ) );
}

DocumentUri Location::uri( void ) const
{
    return DocumentUri::fromString( at( URI ).get< std::string >() );
}

Range Location::range( void ) const
{
    return at( RANGE );
}

u1 Location::isValid( const Data& data )
{
    if( data.is_object() and data.find( URI ) != data.end()
        and data[ URI ].is_string()
        and data.find( RANGE ) != data.end()
        and data[ RANGE ].is_object() )
    {
        if( not Position::isValid( data[ RANGE ] ) )
        {
            return false;
        }

        try
        {
            DocumentUri::fromString( data[ URI ].get< std::string >() );
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
    operator[]( RANGE ) = Data::from_cbor( Data::to_cbor( range ) );
    operator[]( MESSAGE ) = message;
}

Range Diagnostic::range( void ) const
{
    return at( RANGE );
}

std::string Diagnostic::message( void ) const
{
    return at( MESSAGE ).get< std::string >();
}

u1 Diagnostic::hasSeverity( void ) const
{
    return find( SEVERITY ) != end();
}

DiagnosticSeverity Diagnostic::severity( void ) const
{
    return static_cast< DiagnosticSeverity >(
        at( SEVERITY ).get< std::size_t >() );
}

void Diagnostic::setSeverity( const DiagnosticSeverity& severity )
{
    operator[]( SEVERITY ) = static_cast< std::size_t >( severity );
}

u1 Diagnostic::hasCode( void ) const
{
    return find( CODE ) != end();
}

std::string Diagnostic::code( void ) const
{
    return at( CODE ).get< std::string >();
}

void Diagnostic::setCode( const std::string& code )
{
    operator[]( CODE ) = code;
}

void Diagnostic::setCode( const std::size_t code )
{
    operator[]( CODE ) = code;
}

u1 Diagnostic::hasSource( void ) const
{
    return find( SOURCE ) != end();
}

std::string Diagnostic::source( void ) const
{
    return at( SOURCE ).get< std::string >();
}

void Diagnostic::setSource( const std::string& source )
{
    operator[]( SOURCE ) = source;
}

u1 Diagnostic::isValid( const Data& data )
{
    if( data.is_object() and data.find( RANGE ) != data.end()
        and data[ RANGE ].is_object()
        and data.find( MESSAGE ) != data.end()
        and data[ MESSAGE ].is_string() )
    {
        if( not Range::isValid( data[ RANGE ] ) )
        {
            return false;
        }

        if( data.find( SEVERITY ) != data.end()
            and not data[ SEVERITY ].is_number() )
        {
            return false;
        }

        if( data.find( CODE ) != data.end()
            and not( data[ CODE ].is_string() or data[ CODE ].is_number() ) )
        {
            return false;
        }

        if( data.find( SOURCE ) != data.end()
            and not data[ SOURCE ].is_string() )
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
    operator[]( TITLE ) = title;
    operator[]( COMMAND ) = command;
    operator[]( ARGUMENTS ) = Data::array();
}

std::string Command::title( void ) const
{
    return at( TITLE ).get< std::string >();
}

std::string Command::command( void ) const
{
    return at( COMMAND ).get< std::string >();
}

u1 Command::hasArguments( void ) const
{
    return find( ARGUMENTS ) != end();
}

Data Command::arguments( void ) const
{
    return at( ARGUMENTS );
}

void Command::addArgument( const Data& argument )
{
    operator[]( ARGUMENTS ).push_back( argument );
}

u1 Command::isValid( const Data& data )
{
    if( data.is_object() and data.find( TITLE ) != data.end()
        and data[ TITLE ].is_string()
        and data.find( COMMAND ) != data.end()
        and data[ COMMAND ].is_string() )
    {
        if( data.find( ARGUMENTS ) != data.end()
            and not data[ ARGUMENTS ].is_array() )
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
    operator[]( RANGE ) = Data::from_cbor( Data::to_cbor( range ) );
    operator[]( NEW_TEXT ) = newText;
}

Range TextEdit::range( void ) const
{
    return at( RANGE );
}

std::string TextEdit::newText( void ) const
{
    return at( NEW_TEXT ).get< std::string >();
}

u1 TextEdit::isValid( const Data& data )
{
    if( data.is_object() and data.find( RANGE ) != data.end()
        and data[ RANGE ].is_object()
        and data.find( NEW_TEXT ) != data.end()
        and data[ NEW_TEXT ].is_string() )
    {
        if( not Range::isValid( data[ RANGE ] ) )
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
    operator[]( URI ) = uri.toString();
}

DocumentUri TextDocumentIdentifier::uri( void ) const
{
    return DocumentUri::fromString( at( URI ).get< std::string >() );
}

u1 TextDocumentIdentifier::isValid( const Data& data )
{
    if( data.is_object() and data.find( URI ) != data.end()
        and data[ URI ].is_string() )
    {
        try
        {
            DocumentUri::fromString( data[ URI ].get< std::string >() );
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
    operator[]( VERSION ) = version;
}

std::size_t VersionedTextDocumentIdentifier::version( void ) const
{
    return at( VERSION ).get< std::size_t >();
}

u1 VersionedTextDocumentIdentifier::isValid( const Data& data )
{
    // if( TextDocumentIdentifier::isValid( data ) )
    // {
    if( data.find( VERSION ) != data.end() and not data[ VERSION ].is_number() )
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
    operator[]( TEXT_DOCUMENT )
        = Data::from_cbor( Data::to_cbor( textDocument ) );
    operator[]( EDITS ) = Data::array();

    for( auto edit : edits )
    {
        operator[]( EDITS ).push_back( edit );
    }
}

VersionedTextDocumentIdentifier TextDocumentEdit::textDocument( void ) const
{
    return at( TEXT_DOCUMENT );
}

Data TextDocumentEdit::edits( void ) const
{
    return at( EDITS );
}

u1 TextDocumentEdit::isValid( const Data& data )
{
    if( data.is_object() and data.find( TEXT_DOCUMENT ) != data.end()
        and data[ TEXT_DOCUMENT ].is_object()
        and data.find( EDITS ) != data.end()
        and data[ EDITS ].is_array() )
    {
        if( not VersionedTextDocumentIdentifier::isValid(
                data[ TEXT_DOCUMENT ] ) )
        {
            return false;
        }

        for( auto edit : data[ EDITS ] )
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
    operator[]( DOCUMENT_CHANGES ) = Data::array();
}

u1 WorkspaceEdit::hasDocumentChanges( void ) const
{
    return find( DOCUMENT_CHANGES ) != end();
}

Data WorkspaceEdit::documentChanges( void ) const
{
    return at( DOCUMENT_CHANGES );
}

void WorkspaceEdit::addDocumentChange( const TextDocumentEdit& documentChange )
{
    operator[]( DOCUMENT_CHANGES ).push_back( documentChange );
}

u1 WorkspaceEdit::isValid( const Data& data )
{
    if( data.is_object() and data.find( DOCUMENT_CHANGES ) != data.end()
        and data[ DOCUMENT_CHANGES ].is_array() )
    {
        for( auto documentChange : data[ DOCUMENT_CHANGES ] )
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
    operator[]( URI ) = uri.toString();
    operator[]( LANGUAGE_ID ) = languageId;
    operator[]( VERSION ) = version;
    operator[]( TEXT ) = text;
}

DocumentUri TextDocumentItem::uri( void ) const
{
    return DocumentUri::fromString( at( URI ).get< std::string >() );
}

std::string TextDocumentItem::languageId( void ) const
{
    return at( LANGUAGE_ID ).get< std::string >();
}

std::size_t TextDocumentItem::version( void ) const
{
    return at( VERSION ).get< std::size_t >();
}

std::string TextDocumentItem::text( void ) const
{
    return at( TEXT ).get< std::string >();
}

u1 TextDocumentItem::isValid( const Data& data )
{
    if( data.is_object() and data.find( URI ) != data.end()
        and data[ URI ].is_string()
        and data.find( LANGUAGE_ID ) != data.end()
        and data[ LANGUAGE_ID ].is_string()
        and data.find( VERSION ) != data.end()
        and data[ VERSION ].is_number()
        and data.find( TEXT ) != data.end()
        and data[ TEXT ].is_string() )
    {
        try
        {
            DocumentUri::fromString( data[ URI ].get< std::string >() );
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
    operator[]( TEXT_DOCUMENT )
        = Data::from_cbor( Data::to_cbor( textDocument ) );
    operator[]( POSITION ) = Data::from_cbor( Data::to_cbor( position ) );
}

TextDocumentIdentifier TextDocumentPositionParams::textDocument( void ) const
{
    return at( TEXT_DOCUMENT );
}

Position TextDocumentPositionParams::position( void ) const
{
    return at( POSITION );
}

u1 TextDocumentPositionParams::isValid( const Data& data )
{
    if( data.is_object() and data.find( TEXT_DOCUMENT ) != data.end()
        and data[ TEXT_DOCUMENT ].is_object()
        and data.find( POSITION ) != data.end()
        and data[ POSITION ].is_object() )
    {
        if( not TextDocumentIdentifier::isValid( data[ TEXT_DOCUMENT ] ) )
        {
            return false;
        }

        if( not Position::isValid( data[ POSITION ] ) )
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
    return find( LANGUAGE ) != end();
}

std::string DocumentFilter::language( void ) const
{
    return at( LANGUAGE ).get< std::string >();
}

void DocumentFilter::setLanguage( const std::string& language )
{
    operator[]( LANGUAGE ) = language;
}

u1 DocumentFilter::hasScheme( void ) const
{
    return find( SCHEME ) != end();
}

std::string DocumentFilter::scheme( void ) const
{
    return at( SCHEME ).get< std::string >();
}

void DocumentFilter::setScheme( const std::string& scheme )
{
    operator[]( SCHEME ) = scheme;
}

u1 DocumentFilter::hasPattern( void ) const
{
    return find( PATTERN ) != end();
}

std::string DocumentFilter::pattern( void ) const
{
    return at( PATTERN ).get< std::string >();
}

void DocumentFilter::setPattern( const std::string& pattern )
{
    operator[]( PATTERN ) = pattern;
}

u1 DocumentFilter::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( LANGUAGE ) != data.end()
            and not data[ LANGUAGE ].is_string() )
        {
            return false;
        }

        if( data.find( SCHEME ) != data.end()
            and not data[ SCHEME ].is_string() )
        {
            return false;
        }

        if( data.find( PATTERN ) != data.end()
            and not data[ PATTERN ].is_string() )
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
    return find( DYNAMIC_REGISTRATION ) != end();
}

u1 DynamicRegistration::dynamicRegistration( void ) const
{
    return at( DYNAMIC_REGISTRATION ).get< u1 >();
}

void DynamicRegistration::setDynamicRegistration( const u1 dynamicRegistration )
{
    operator[]( DYNAMIC_REGISTRATION ) = dynamicRegistration;
}

u1 DynamicRegistration::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( DYNAMIC_REGISTRATION ) != data.end()
            and not data[ DYNAMIC_REGISTRATION ].is_boolean() )
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
    return find( APPLY_EDIT ) != end();
}

u1 WorkspaceClientCapabilities::applyEdit( void ) const
{
    return at( APPLY_EDIT ).get< u1 >();
}

void WorkspaceClientCapabilities::setApplyEdit( const u1 applyEdit )
{
    operator[]( APPLY_EDIT ) = applyEdit;
}

u1 WorkspaceClientCapabilities::hasWorkspaceEdit( void ) const
{
    return find( WORKSPACE_EDIT ) != end();
}

WorkspaceEdit WorkspaceClientCapabilities::workspaceEdit( void ) const
{
    return at( WORKSPACE_EDIT );
}

void WorkspaceClientCapabilities::setWorkspaceEdit(
    const WorkspaceEdit& workspaceEdit )
{
    operator[]( WORKSPACE_EDIT )
        = Data::from_cbor( Data::to_cbor( workspaceEdit ) );
}

u1 WorkspaceClientCapabilities::hasDidChangeConfiguration( void ) const
{
    return find( DID_CHANGE_CONFIGURATION ) != end();
}

DynamicRegistration WorkspaceClientCapabilities::didChangeConfiguration(
    void ) const
{
    return at( DID_CHANGE_CONFIGURATION );
}

void WorkspaceClientCapabilities::setDidChangeConfiguration(
    const DynamicRegistration& didChangeConfiguration )
{
    operator[]( DID_CHANGE_CONFIGURATION )
        = Data::from_cbor( Data::to_cbor( didChangeConfiguration ) );
}

u1 WorkspaceClientCapabilities::hasDidChangeWatchedFiles( void ) const
{
    return find( DID_CHANGE_WATCHED_FILES ) != end();
}

DynamicRegistration WorkspaceClientCapabilities::didChangeWatchedFiles(
    void ) const
{
    return at( DID_CHANGE_WATCHED_FILES );
}

void WorkspaceClientCapabilities::didChangeWatchedFiles(
    const DynamicRegistration& didChangeWatchedFiles )
{
    operator[]( DID_CHANGE_WATCHED_FILES )
        = Data::from_cbor( Data::to_cbor( didChangeWatchedFiles ) );
}

u1 WorkspaceClientCapabilities::hasSymbol( void ) const
{
    return find( SYMBOL ) != end();
}

DynamicRegistration WorkspaceClientCapabilities::symbol( void ) const
{
    return at( SYMBOL );
}

void WorkspaceClientCapabilities::setSymbol( const DynamicRegistration& symbol )
{
    operator[]( SYMBOL ) = Data::from_cbor( Data::to_cbor( symbol ) );
}

u1 WorkspaceClientCapabilities::hasExecuteCommand( void ) const
{
    return find( EXECUTE_COMMAND ) != end();
}

DynamicRegistration WorkspaceClientCapabilities::executeCommand( void ) const
{
    return at( EXECUTE_COMMAND );
}

void WorkspaceClientCapabilities::executeCommand(
    const DynamicRegistration& executeCommand )
{
    operator[]( EXECUTE_COMMAND )
        = Data::from_cbor( Data::to_cbor( executeCommand ) );
}

u1 WorkspaceClientCapabilities::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( APPLY_EDIT ) != data.end()
            and not data[ APPLY_EDIT ].is_boolean() )
        {
            return false;
        }

        if( data.find( WORKSPACE_EDIT ) != data.end()
            and not WorkspaceEdit::isValid( data[ WORKSPACE_EDIT ] ) )
        {
            return false;
        }

        if( data.find( DID_CHANGE_CONFIGURATION ) != data.end()
            and not DynamicRegistration::isValid(
                    data[ DID_CHANGE_CONFIGURATION ] ) )
        {
            return false;
        }

        if( data.find( DID_CHANGE_WATCHED_FILES ) != data.end()
            and not DynamicRegistration::isValid(
                    data[ DID_CHANGE_WATCHED_FILES ] ) )
        {
            return false;
        }

        if( data.find( SYMBOL ) != data.end()
            and not DynamicRegistration::isValid( data[ SYMBOL ] ) )
        {
            return false;
        }

        if( data.find( EXECUTE_COMMAND ) != data.end()
            and not DynamicRegistration::isValid( data[ EXECUTE_COMMAND ] ) )
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
    return find( DYNAMIC_REGISTRATION ) != end();
}

u1 Synchronization::dynamicRegistration( void ) const
{
    return at( DYNAMIC_REGISTRATION ).get< u1 >();
}

void Synchronization::setDynamicRegistration( const u1 dynamicRegistration )
{
    operator[]( DYNAMIC_REGISTRATION ) = dynamicRegistration;
}

u1 Synchronization::hasWillSave( void ) const
{
    return find( WILL_SAVE ) != end();
}

u1 Synchronization::willSave( void ) const
{
    return at( WILL_SAVE ).get< u1 >();
}

void Synchronization::setWillSave( const u1 willSave )
{
    operator[]( WILL_SAVE ) = willSave;
}

u1 Synchronization::hasWillSaveWaitUntil( void ) const
{
    return find( WILL_SAVE_WAIT_UNITL ) != end();
}

u1 Synchronization::willSaveWaitUntil( void ) const
{
    return at( WILL_SAVE_WAIT_UNITL ).get< u1 >();
}

void Synchronization::setWillSaveWaitUntil( const u1 willSaveWaitUntil )
{
    operator[]( WILL_SAVE_WAIT_UNITL ) = willSaveWaitUntil;
}

u1 Synchronization::hasDidSave( void ) const
{
    return find( DID_SAVE ) != end();
}

u1 Synchronization::didSave( void ) const
{
    return at( DID_SAVE ).get< u1 >();
}

void Synchronization::setDidSave( const u1 didSave )
{
    operator[]( DID_SAVE ) = didSave;
}

u1 Synchronization::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( DYNAMIC_REGISTRATION ) != data.end()
            and not data[ DYNAMIC_REGISTRATION ].is_boolean() )
        {
            return false;
        }

        if( data.find( WILL_SAVE ) != data.end()
            and not data[ WILL_SAVE ].is_boolean() )
        {
            return false;
        }

        if( data.find( WILL_SAVE_WAIT_UNITL ) != data.end()
            and not data[ WILL_SAVE_WAIT_UNITL ].is_boolean() )
        {
            return false;
        }

        if( data.find( DID_SAVE ) != data.end()
            and not data[ DID_SAVE ].is_boolean() )
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
    return find( SNIPPET_SUPPORT ) != end();
}

u1 CompletionItem::snippetSupport( void ) const
{
    return at( SNIPPET_SUPPORT ).get< u1 >();
}

void CompletionItem::setSnippetSupport( const u1 snippetSupport )
{
    operator[]( SNIPPET_SUPPORT ) = snippetSupport;
}

u1 CompletionItem::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( SNIPPET_SUPPORT ) != data.end()
            and not data[ SNIPPET_SUPPORT ].is_boolean() )
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
    return find( DYNAMIC_REGISTRATION ) != end();
}

u1 Completion::dynamicRegistration( void ) const
{
    return at( DYNAMIC_REGISTRATION ).get< u1 >();
}

void Completion::setDynamicRegistration( const u1 dynamicRegistration )
{
    operator[]( DYNAMIC_REGISTRATION ) = dynamicRegistration;
}

u1 Completion::hasCompletionItem( void ) const
{
    return find( COMPLETION_ITEM ) != end();
}

CompletionItem Completion::completionItem( void ) const
{
    return at( COMPLETION_ITEM );
}

void Completion::completionItem( const CompletionItem& completionItem )
{
    operator[]( COMPLETION_ITEM )
        = Data::from_cbor( Data::to_cbor( completionItem ) );
}

u1 Completion::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( DYNAMIC_REGISTRATION ) != data.end()
            and not data[ DYNAMIC_REGISTRATION ].is_boolean() )
        {
            return false;
        }

        if( data.find( COMPLETION_ITEM ) != data.end()
            and not CompletionItem::isValid( data[ COMPLETION_ITEM ] ) )
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
    return find( SYNCHRONIZATION ) != end();
}

Synchronization TextDocumentClientCapabilities::synchronization( void ) const
{
    return at( SYNCHRONIZATION );
}

void TextDocumentClientCapabilities::setSynchronization(
    const Synchronization& synchronization )
{
    operator[]( SYNCHRONIZATION )
        = Data::from_cbor( Data::to_cbor( synchronization ) );
}

u1 TextDocumentClientCapabilities::hasCompletion( void ) const
{
    return find( COMPLETION ) != end();
}

Completion TextDocumentClientCapabilities::completion( void ) const
{
    return at( COMPLETION );
}

void TextDocumentClientCapabilities::setCompletion(
    const Completion& completion )
{
    operator[]( COMPLETION ) = Data::from_cbor( Data::to_cbor( completion ) );
}

u1 TextDocumentClientCapabilities::hasHover( void ) const
{
    return find( HOVER ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::hover( void ) const
{
    return at( HOVER );
}

void TextDocumentClientCapabilities::setHover(
    const DynamicRegistration& hover )
{
    operator[]( HOVER ) = Data::from_cbor( Data::to_cbor( hover ) );
}

u1 TextDocumentClientCapabilities::hasSignatureHelp( void ) const
{
    return find( SIGNATURE_HELP ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::signatureHelp( void ) const
{
    return at( SIGNATURE_HELP );
}

void TextDocumentClientCapabilities::setSignatureHelp(
    const DynamicRegistration& signatureHelp )
{
    operator[]( SIGNATURE_HELP )
        = Data::from_cbor( Data::to_cbor( signatureHelp ) );
}

u1 TextDocumentClientCapabilities::hasReferences( void ) const
{
    return find( REFERENCES ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::references( void ) const
{
    return at( REFERENCES );
}

void TextDocumentClientCapabilities::setReferences(
    const DynamicRegistration& references )
{
    operator[]( REFERENCES ) = Data::from_cbor( Data::to_cbor( references ) );
}

u1 TextDocumentClientCapabilities::hasDocumentHighlight( void ) const
{
    return find( DOCUMENT_HIGHLIGHT ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::documentHighlight(
    void ) const
{
    return at( DOCUMENT_HIGHLIGHT );
}

void TextDocumentClientCapabilities::setDocumentHighlight(
    const DynamicRegistration& documentHighlight )
{
    operator[]( DOCUMENT_HIGHLIGHT )
        = Data::from_cbor( Data::to_cbor( documentHighlight ) );
}

u1 TextDocumentClientCapabilities::hasDocumentSymbol( void ) const
{
    return find( DOCUMENT_SYMBOL ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::documentSymbol( void ) const
{
    return at( DOCUMENT_SYMBOL );
}

void TextDocumentClientCapabilities::setDocumentSymbol(
    const DynamicRegistration& documentSymbol )
{
    operator[]( DOCUMENT_SYMBOL )
        = Data::from_cbor( Data::to_cbor( documentSymbol ) );
}

u1 TextDocumentClientCapabilities::hasFormatting( void ) const
{
    return find( FORMATTING ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::formatting( void ) const
{
    return at( FORMATTING );
}

void TextDocumentClientCapabilities::setFormatting(
    const DynamicRegistration& formatting )
{
    operator[]( FORMATTING ) = Data::from_cbor( Data::to_cbor( formatting ) );
}

u1 TextDocumentClientCapabilities::hasRangeFormatting( void ) const
{
    return find( RANGE_FORMATTING ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::rangeFormatting(
    void ) const
{
    return at( RANGE_FORMATTING );
}

void TextDocumentClientCapabilities::setRangeFormatting(
    const DynamicRegistration& rangeFormatting )
{
    operator[]( RANGE_FORMATTING )
        = Data::from_cbor( Data::to_cbor( rangeFormatting ) );
}

u1 TextDocumentClientCapabilities::hasOnTypeFormatting( void ) const
{
    return find( ON_TYPE_FORMATTING ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::onTypeFormatting(
    void ) const
{
    return at( ON_TYPE_FORMATTING );
}

void TextDocumentClientCapabilities::setOnTypeFormatting(
    const DynamicRegistration& onTypeFormatting )
{
    operator[]( ON_TYPE_FORMATTING )
        = Data::from_cbor( Data::to_cbor( onTypeFormatting ) );
}

u1 TextDocumentClientCapabilities::hasDefinition( void ) const
{
    return find( DEFINITION ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::definition( void ) const
{
    return at( DEFINITION );
}

void TextDocumentClientCapabilities::setDefinition(
    const DynamicRegistration& definition )
{
    operator[]( DEFINITION ) = Data::from_cbor( Data::to_cbor( definition ) );
}

u1 TextDocumentClientCapabilities::hasCodeAction( void ) const
{
    return find( CODE_ACTION ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::codeAction( void ) const
{
    return at( CODE_ACTION );
}

void TextDocumentClientCapabilities::setCodeAction(
    const DynamicRegistration& codeAction )
{
    operator[]( CODE_ACTION ) = Data::from_cbor( Data::to_cbor( codeAction ) );
}

u1 TextDocumentClientCapabilities::hasCodeLens( void ) const
{
    return find( CODE_LENS ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::codeLens( void ) const
{
    return at( CODE_LENS );
}

void TextDocumentClientCapabilities::setCodeLens(
    const DynamicRegistration& codeLens )
{
    operator[]( CODE_LENS ) = Data::from_cbor( Data::to_cbor( codeLens ) );
}

u1 TextDocumentClientCapabilities::hasDocumentLink( void ) const
{
    return find( DOCUMENT_LINK ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::documentLink( void ) const
{
    return at( DOCUMENT_LINK );
}

void TextDocumentClientCapabilities::setDocumentLink(
    const DynamicRegistration& documentLink )
{
    operator[]( DOCUMENT_LINK )
        = Data::from_cbor( Data::to_cbor( documentLink ) );
}

u1 TextDocumentClientCapabilities::hasRename( void ) const
{
    return find( RENAME ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::rename( void ) const
{
    return at( RENAME );
}

void TextDocumentClientCapabilities::setRename(
    const DynamicRegistration& rename )
{
    operator[]( RENAME ) = Data::from_cbor( Data::to_cbor( rename ) );
}

u1 TextDocumentClientCapabilities::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( SYNCHRONIZATION ) != data.end()
            and not Synchronization::isValid( data[ SYNCHRONIZATION ] ) )
        {
            return false;
        }

        if( data.find( COMPLETION ) != data.end()
            and not Completion::isValid( data[ COMPLETION ] ) )
        {
            return false;
        }

        if( data.find( HOVER ) != data.end()
            and not DynamicRegistration::isValid( data[ HOVER ] ) )
        {
            return false;
        }

        if( data.find( SIGNATURE_HELP ) != data.end()
            and not DynamicRegistration::isValid( data[ SIGNATURE_HELP ] ) )
        {
            return false;
        }

        if( data.find( REFERENCES ) != data.end()
            and not DynamicRegistration::isValid( data[ REFERENCES ] ) )
        {
            return false;
        }

        if( data.find( DOCUMENT_HIGHLIGHT ) != data.end()
            and not DynamicRegistration::isValid( data[ DOCUMENT_HIGHLIGHT ] ) )
        {
            return false;
        }

        if( data.find( DOCUMENT_SYMBOL ) != data.end()
            and not DynamicRegistration::isValid( data[ DOCUMENT_SYMBOL ] ) )
        {
            return false;
        }

        if( data.find( FORMATTING ) != data.end()
            and not DynamicRegistration::isValid( data[ FORMATTING ] ) )
        {
            return false;
        }

        if( data.find( RANGE_FORMATTING ) != data.end()
            and not DynamicRegistration::isValid( data[ RANGE_FORMATTING ] ) )
        {
            return false;
        }

        if( data.find( ON_TYPE_FORMATTING ) != data.end()
            and not DynamicRegistration::isValid( data[ ON_TYPE_FORMATTING ] ) )
        {
            return false;
        }

        if( data.find( DEFINITION ) != data.end()
            and not DynamicRegistration::isValid( data[ DEFINITION ] ) )
        {
            return false;
        }

        if( data.find( CODE_ACTION ) != data.end()
            and not DynamicRegistration::isValid( data[ CODE_ACTION ] ) )
        {
            return false;
        }

        if( data.find( CODE_LENS ) != data.end()
            and not DynamicRegistration::isValid( data[ CODE_LENS ] ) )
        {
            return false;
        }

        if( data.find( DOCUMENT_LINK ) != data.end()
            and not DynamicRegistration::isValid( data[ DOCUMENT_LINK ] ) )
        {
            return false;
        }

        if( data.find( RENAME ) != data.end()
            and not DynamicRegistration::isValid( data[ RENAME ] ) )
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
    return find( WORKSPACE ) != end();
}

WorkspaceClientCapabilities ClientCapabilities::workspace( void ) const
{
    return at( WORKSPACE );
}

void ClientCapabilities::setWorkspace(
    const WorkspaceClientCapabilities& workspace )
{
    operator[]( WORKSPACE ) = Data::from_cbor( Data::to_cbor( workspace ) );
}

u1 ClientCapabilities::hasTextDocument( void ) const
{
    return find( TEXT_DOCUMENT ) != end();
}

TextDocumentClientCapabilities ClientCapabilities::textDocument( void ) const
{
    return at( TEXT_DOCUMENT );
}

void ClientCapabilities::setTextDocument(
    const TextDocumentClientCapabilities& textDocument )
{
    operator[]( TEXT_DOCUMENT )
        = Data::from_cbor( Data::to_cbor( textDocument ) );
}

u1 ClientCapabilities::hasExperimental( void ) const
{
    return find( EXPERIMENTAL ) != end();
}

Data ClientCapabilities::experimental( void ) const
{
    return at( EXPERIMENTAL );
}

void ClientCapabilities::setExperimental( const Data& experimental )
{
    operator[]( EXPERIMENTAL )
        = Data::from_cbor( Data::to_cbor( experimental ) );
}

u1 ClientCapabilities::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( WORKSPACE ) != data.end()
            and not WorkspaceClientCapabilities::isValid( data[ WORKSPACE ] ) )
        {
            return false;
        }

        if( data.find( TEXT_DOCUMENT ) != data.end()
            and not TextDocumentClientCapabilities::isValid(
                    data[ TEXT_DOCUMENT ] ) )
        {
            return false;
        }

        if( data.find( EXPERIMENTAL ) != data.end()
            and not data[ EXPERIMENTAL ].is_object() )
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
    return find( INCLUDE_TEXT ) != end();
}

u1 SaveOptions::includeText( void ) const
{
    return at( INCLUDE_TEXT ).get< u1 >();
}

void SaveOptions::setIncludeText( const u1 includeText )
{
    operator[]( INCLUDE_TEXT ) = includeText;
}

u1 SaveOptions::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( INCLUDE_TEXT ) != data.end()
            and not data[ INCLUDE_TEXT ].is_boolean() )
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
    return find( OPEN_CLOSE ) != end();
}

u1 TextDocumentSyncOptions::openClose( void ) const
{
    return at( OPEN_CLOSE ).get< u1 >();
}

void TextDocumentSyncOptions::setOpenClose( const u1 openClose )
{
    operator[]( OPEN_CLOSE ) = openClose;
}

u1 TextDocumentSyncOptions::hasChange( void ) const
{
    return find( CHANGE ) != end();
}

TextDocumentSyncKind TextDocumentSyncOptions::change( void ) const
{
    assert( hasChange() );
    return static_cast< TextDocumentSyncKind >(
        at( CHANGE ).get< std::size_t >() );
}

void TextDocumentSyncOptions::setChange( const TextDocumentSyncKind& change )
{
    operator[]( CHANGE ) = static_cast< std::size_t >( change );
}

u1 TextDocumentSyncOptions::hasWillSave( void ) const
{
    return find( WILL_SAVE ) != end();
}

u1 TextDocumentSyncOptions::willSave( void ) const
{
    return at( WILL_SAVE ).get< u1 >();
}

void TextDocumentSyncOptions::setWillSave( const u1 willSave )
{
    operator[]( WILL_SAVE ) = willSave;
}

u1 TextDocumentSyncOptions::hasWillSaveWaitUntil( void ) const
{
    return find( WILL_SAVE_WAIT_UNITL ) != end();
}

u1 TextDocumentSyncOptions::willSaveWaitUntil( void ) const
{
    return at( WILL_SAVE_WAIT_UNITL ).get< u1 >();
}

void TextDocumentSyncOptions::setWillSaveWaitUntil( const u1 willSaveWaitUntil )
{
    operator[]( WILL_SAVE_WAIT_UNITL ) = willSaveWaitUntil;
}

u1 TextDocumentSyncOptions::hasSave( void ) const
{
    return find( SAVE ) != end();
}

SaveOptions TextDocumentSyncOptions::save( void ) const
{
    return at( SAVE );
}

void TextDocumentSyncOptions::setSave( const SaveOptions& save )
{
    operator[]( SAVE ) = Data::from_cbor( Data::to_cbor( save ) );
}

u1 TextDocumentSyncOptions::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( OPEN_CLOSE ) != data.end()
            and not data[ OPEN_CLOSE ].is_boolean() )
        {
            return false;
        }

        if( data.find( CHANGE ) != data.end()
            and not data[ CHANGE ].is_number() )
        {
            return false;
        }

        if( data.find( WILL_SAVE ) != data.end()
            and not data[ WILL_SAVE ].is_boolean() )
        {
            return false;
        }

        if( data.find( WILL_SAVE_WAIT_UNITL ) != data.end()
            and not data[ WILL_SAVE_WAIT_UNITL ].is_boolean() )
        {
            return false;
        }

        if( data.find( SAVE ) != data.end()
            and not SaveOptions::isValid( data[ SAVE ] ) )
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
    operator[]( TRIGGER_CHARACTERS ) = Data::array();
}

u1 CompletionOptions::hasResolveProvider( void ) const
{
    return find( RESOLVE_PROVIDER ) != end();
}

u1 CompletionOptions::resolveProvider( void ) const
{
    return at( RESOLVE_PROVIDER ).get< u1 >();
}

void CompletionOptions::setResolveProvider( const u1 resolveProvider )
{
    operator[]( RESOLVE_PROVIDER ) = resolveProvider;
}

u1 CompletionOptions::hasTriggerCharacters( void ) const
{
    return find( TRIGGER_CHARACTERS ) != end();
}

Data CompletionOptions::triggerCharacters( void ) const
{
    return at( TRIGGER_CHARACTERS );
}

void CompletionOptions::addTriggerCharacters(
    const std::string& triggerCharacter )
{
    operator[]( TRIGGER_CHARACTERS ).push_back( triggerCharacter );
}

u1 CompletionOptions::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( RESOLVE_PROVIDER ) != data.end()
            and not data[ RESOLVE_PROVIDER ].is_boolean() )
        {
            return false;
        }

        if( data.find( TRIGGER_CHARACTERS ) != data.end()
            and data[ TRIGGER_CHARACTERS ].is_array() )
        {
            for( auto triggerCharacter : data[ TRIGGER_CHARACTERS ] )
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
    operator[]( MORE_TRIGGER_CHARACTER ) = Data::array();
}

u1 SignatureHelpOptions::hasTriggerCharacters( void ) const
{
    return find( TRIGGER_CHARACTERS ) != end();
}

Data SignatureHelpOptions::triggerCharacters( void ) const
{
    return at( TRIGGER_CHARACTERS );
}

void SignatureHelpOptions::addTriggerCharacters(
    const std::string& triggerCharacter )
{
    operator[]( TRIGGER_CHARACTERS ).push_back( triggerCharacter );
}

u1 SignatureHelpOptions::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( TRIGGER_CHARACTERS ) != data.end()
            and data[ TRIGGER_CHARACTERS ].is_array() )
        {
            for( auto triggerCharacter : data[ TRIGGER_CHARACTERS ] )
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
    return find( RESOLVE_PROVIDER ) != end();
}

u1 CodeLensOptions::resolveProvider( void ) const
{
    return at( RESOLVE_PROVIDER ).get< u1 >();
}

void CodeLensOptions::setResolveProvider( const u1 resolveProvider )
{
    operator[]( RESOLVE_PROVIDER ) = resolveProvider;
}

u1 CodeLensOptions::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( RESOLVE_PROVIDER ) != data.end()
            and not data[ RESOLVE_PROVIDER ].is_boolean() )
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
    operator[]( FIRST_TRIGGER_CHARACTER ) = firstTriggerCharacter;
    operator[]( MORE_TRIGGER_CHARACTER ) = Data::array();
}

std::string DocumentOnTypeFormattingOptions::firstTriggerCharacter( void ) const
{
    return at( FIRST_TRIGGER_CHARACTER ).get< std::string >();
}

u1 DocumentOnTypeFormattingOptions::hasMoreTriggerCharacter( void ) const
{
    return find( MORE_TRIGGER_CHARACTER ) != end();
}

Data DocumentOnTypeFormattingOptions::moreTriggerCharacter( void ) const
{
    return at( MORE_TRIGGER_CHARACTER );
}

void DocumentOnTypeFormattingOptions::addMoreTriggerCharacter(
    const Data& moreTriggerCharacter )
{
    operator[]( MORE_TRIGGER_CHARACTER ).push_back( moreTriggerCharacter );
}

u1 DocumentOnTypeFormattingOptions::isValid( const Data& data )
{
    if( data.is_object() and data.find( FIRST_TRIGGER_CHARACTER ) != data.end()
        and data[ FIRST_TRIGGER_CHARACTER ].is_string() )
    {
        if( data.find( MORE_TRIGGER_CHARACTER ) != data.end()
            and not data[ MORE_TRIGGER_CHARACTER ].is_array() )
        {
            for( auto moreTriggerCharacter : data[ MORE_TRIGGER_CHARACTER ] )
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
    operator[]( COMMANDS ) = Data::array();
}

u1 ExecuteCommandOptions::hasCommands( void ) const
{
    return find( COMMANDS ) != end();
}

Data ExecuteCommandOptions::commands( void ) const
{
    return at( COMMANDS );
}

void ExecuteCommandOptions::addCommand( const std::string& command )
{
    operator[]( COMMANDS ).push_back( command );
}

u1 ExecuteCommandOptions::isValid( const Data& data )
{
    if( data.is_object() and data.find( COMMANDS ) != data.end()
        and data[ COMMANDS ].is_array() )
    {
        for( auto command : data[ COMMANDS ] )
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
    return find( TEXT_DOCUMENT_SYNC ) != end();
}

TextDocumentSyncOptions ServerCapabilities::textDocumentSync( void ) const
{
    return at( TEXT_DOCUMENT_SYNC );
}

void ServerCapabilities::setTextDocumentSync(
    const TextDocumentSyncOptions& textDocumentSync )
{
    operator[]( TEXT_DOCUMENT_SYNC )
        = Data::from_cbor( Data::to_cbor( textDocumentSync ) );
}

void ServerCapabilities::setTextDocumentSync(
    const TextDocumentSyncKind& textDocumentSync )
{
    operator[]( TEXT_DOCUMENT_SYNC )
        = static_cast< std::size_t >( textDocumentSync );
}

u1 ServerCapabilities::hasHoverProvider( void ) const
{
    return find( HOVER_PROVIDER ) != end();
}

u1 ServerCapabilities::hoverProvider( void ) const
{
    return at( HOVER_PROVIDER ).get< u1 >();
}

void ServerCapabilities::setHoverProvider( const u1 hoverProvider )
{
    operator[]( HOVER_PROVIDER ) = hoverProvider;
}

u1 ServerCapabilities::hasCompletionProvider( void ) const
{
    return find( COMPLETION_PROVIDER ) != end();
}

CompletionOptions ServerCapabilities::completionProvider( void ) const
{
    return at( COMPLETION_PROVIDER );
}

void ServerCapabilities::setCompletionProvider(
    const CompletionOptions& completionProvider )
{
    operator[]( COMPLETION_PROVIDER ) = completionProvider;
}

u1 ServerCapabilities::hasSignatureHelpProvider( void ) const
{
    return find( SIGNATURE_HELP_PROVIDER ) != end();
}

SignatureHelpOptions ServerCapabilities::signatureHelpProvider( void ) const
{
    return at( SIGNATURE_HELP_PROVIDER );
}

void ServerCapabilities::setSignatureHelpProvider(
    const SignatureHelpOptions& signatureHelpProvider )
{
    operator[]( SIGNATURE_HELP_PROVIDER ) = signatureHelpProvider;
}

u1 ServerCapabilities::hasDefinitionProvider( void ) const
{
    return find( DEFINITION_PROVIDER ) != end();
}

u1 ServerCapabilities::definitionProvider( void ) const
{
    return at( DEFINITION_PROVIDER ).get< u1 >();
}

void ServerCapabilities::setDefinitionProvider( const u1 definitionProvider )
{
    operator[]( DEFINITION_PROVIDER ) = definitionProvider;
}

u1 ServerCapabilities::hasReferencesProvider( void ) const
{
    return find( REFERENCES_PROVIDER ) != end();
}

u1 ServerCapabilities::referencesProvider( void ) const
{
    return at( REFERENCES_PROVIDER ).get< u1 >();
}

void ServerCapabilities::setReferencesProvider( const u1 referencesProvider )
{
    operator[]( REFERENCES_PROVIDER ) = referencesProvider;
}

u1 ServerCapabilities::hasDocumentHighlightProvider( void ) const
{
    return find( DOCUMENT_HIGHLIGHT_PROVIDER ) != end();
}

u1 ServerCapabilities::documentHighlightProvider( void ) const
{
    return at( DOCUMENT_HIGHLIGHT_PROVIDER ).get< u1 >();
}

void ServerCapabilities::setDocumentHighlightProvider(
    const u1 documentHighlightProvider )
{
    operator[]( DOCUMENT_HIGHLIGHT_PROVIDER ) = documentHighlightProvider;
}

u1 ServerCapabilities::hasDocumentSymbolProvider( void ) const
{
    return find( DOCUMENT_SYMBOL_PROVIDER ) != end();
}

u1 ServerCapabilities::documentSymbolProvider( void ) const
{
    return at( DOCUMENT_SYMBOL_PROVIDER ).get< u1 >();
}

void ServerCapabilities::setDocumentSymbolProvider(
    const u1 documentSymbolProvider )
{
    operator[]( DOCUMENT_SYMBOL_PROVIDER ) = documentSymbolProvider;
}

u1 ServerCapabilities::hasWorkspaceSymbolProvider( void ) const
{
    return find( WORKSPACE_SYMBOL_PROVIDER ) != end();
}

u1 ServerCapabilities::workspaceSymbolProvider( void ) const
{
    return at( WORKSPACE_SYMBOL_PROVIDER ).get< u1 >();
}

void ServerCapabilities::setWorkspaceSymbolProvider(
    const u1 workspaceSymbolProvider )
{
    operator[]( WORKSPACE_SYMBOL_PROVIDER ) = workspaceSymbolProvider;
}

u1 ServerCapabilities::hasCodeActionProvider( void ) const
{
    return find( CODE_ACTION_PROVIDER ) != end();
}

u1 ServerCapabilities::codeActionProvider( void ) const
{
    return at( CODE_ACTION_PROVIDER ).get< u1 >();
}

void ServerCapabilities::setCodeActionProvider( const u1 codeActionProvider )
{
    operator[]( CODE_ACTION_PROVIDER ) = codeActionProvider;
}

u1 ServerCapabilities::hasCodeLensProvider( void ) const
{
    return find( CODE_LENS_PROVIDER ) != end();
}

CodeLensOptions ServerCapabilities::codeLensProvider( void ) const
{
    return at( CODE_LENS_PROVIDER );
}

void ServerCapabilities::setCodeLensProvider(
    const CodeLensOptions& codeLensProvider )
{
    operator[]( CODE_LENS_PROVIDER )
        = Data::from_cbor( Data::to_cbor( codeLensProvider ) );
}

u1 ServerCapabilities::hasDocumentFormattingProvider( void ) const
{
    return find( DOCUMENT_FORMATTING_PROVIDER ) != end();
}

u1 ServerCapabilities::documentFormattingProvider( void ) const
{
    return at( DOCUMENT_FORMATTING_PROVIDER ).get< u1 >();
}

void ServerCapabilities::setDocumentFormattingProvider(
    const u1 documentFormattingProvider )
{
    operator[]( DOCUMENT_FORMATTING_PROVIDER ) = documentFormattingProvider;
}

u1 ServerCapabilities::hasDocumentRangeFormattingProvider( void ) const
{
    return find( DOCUMENT_RANGE_FORMATTING_PROVIDER ) != end();
}

u1 ServerCapabilities::documentRangeFormattingProvider( void ) const
{
    return at( DOCUMENT_RANGE_FORMATTING_PROVIDER ).get< u1 >();
}

void ServerCapabilities::setDocumentRangeFormattingProvider(
    const u1 documentRangeFormattingProvider )
{
    operator[]( DOCUMENT_RANGE_FORMATTING_PROVIDER )
        = documentRangeFormattingProvider;
}

u1 ServerCapabilities::hasDocumentOnTypeFormattingProvider( void ) const
{
    return find( DOCUMENT_ON_TYPE_FORMATTING_PROVIDER ) != end();
}

DocumentOnTypeFormattingOptions
ServerCapabilities::documentOnTypeFormattingProvider( void ) const
{
    return at( DOCUMENT_ON_TYPE_FORMATTING_PROVIDER );
}

void ServerCapabilities::setDocumentOnTypeFormattingProvider(
    const DocumentOnTypeFormattingOptions& documentOnTypeFormattingProvider )
{
    operator[]( DOCUMENT_ON_TYPE_FORMATTING_PROVIDER )
        = Data::from_cbor( Data::to_cbor( documentOnTypeFormattingProvider ) );
}

u1 ServerCapabilities::hasRenameProvider( void ) const
{
    return find( RENAME_PROVIDER ) != end();
}

u1 ServerCapabilities::renameProvider( void ) const
{
    return at( RENAME_PROVIDER ).get< u1 >();
}

void ServerCapabilities::setRenameProvider( const u1 renameProvider )
{
    operator[]( RENAME_PROVIDER ) = renameProvider;
}

u1 ServerCapabilities::hasDocumentLinkProvider( void ) const
{
    return find( DOCUMENT_LINK_PROVIDER ) != end();
}

DocumentLinkOptions ServerCapabilities::documentLinkProvider( void ) const
{
    return at( DOCUMENT_LINK_PROVIDER );
}

void ServerCapabilities::setDocumentLinkProvider(
    const DocumentLinkOptions& documentLinkProvider )
{
    operator[]( DOCUMENT_LINK_PROVIDER ) = documentLinkProvider;
}

u1 ServerCapabilities::hasExecuteCommandProvider( void ) const
{
    return find( EXECUTE_COMMAND_PROVIDER ) != end();
}

ExecuteCommandOptions ServerCapabilities::executeCommandProvider( void ) const
{
    return at( EXECUTE_COMMAND_PROVIDER );
}

void ServerCapabilities::setExecuteCommandProvider(
    const ExecuteCommandOptions& executeCommandProvider )
{
    operator[]( EXECUTE_COMMAND_PROVIDER ) = executeCommandProvider;
}

u1 ServerCapabilities::hasExperimental( void ) const
{
    return find( EXPERIMENTAL ) != end();
}

Data ServerCapabilities::experimental( void ) const
{
    return at( EXPERIMENTAL );
}

void ServerCapabilities::setExperimental( const Data& experimental )
{
    operator[]( EXPERIMENTAL )
        = Data::from_cbor( Data::to_cbor( experimental ) );
}

u1 ServerCapabilities::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( TEXT_DOCUMENT_SYNC ) != data.end()
            and not TextDocumentSyncOptions::isValid(
                    data[ TEXT_DOCUMENT_SYNC ] ) )
        {
            return false;
        }

        if( data.find( HOVER_PROVIDER ) != data.end()
            and not data[ HOVER_PROVIDER ].is_boolean() )
        {
            return false;
        }

        if( data.find( COMPLETION_PROVIDER ) != data.end()
            and not CompletionOptions::isValid( data[ COMPLETION_PROVIDER ] ) )
        {
            return false;
        }

        if( data.find( SIGNATURE_HELP_PROVIDER ) != data.end()
            and not SignatureHelpOptions::isValid(
                    data[ SIGNATURE_HELP_PROVIDER ] ) )
        {
            return false;
        }

        if( data.find( DEFINITION_PROVIDER ) != data.end()
            and not data[ DEFINITION_PROVIDER ].is_boolean() )
        {
            return false;
        }

        if( data.find( REFERENCES_PROVIDER ) != data.end()
            and not data[ REFERENCES_PROVIDER ].is_boolean() )
        {
            return false;
        }

        if( data.find( DOCUMENT_HIGHLIGHT_PROVIDER ) != data.end()
            and not data[ DOCUMENT_HIGHLIGHT_PROVIDER ].is_boolean() )
        {
            return false;
        }

        if( data.find( DOCUMENT_SYMBOL_PROVIDER ) != data.end()
            and not data[ DOCUMENT_SYMBOL_PROVIDER ].is_boolean() )
        {
            return false;
        }

        if( data.find( WORKSPACE_SYMBOL_PROVIDER ) != data.end()
            and not data[ WORKSPACE_SYMBOL_PROVIDER ].is_boolean() )
        {
            return false;
        }

        if( data.find( CODE_ACTION_PROVIDER ) != data.end()
            and not data[ CODE_ACTION_PROVIDER ].is_boolean() )
        {
            return false;
        }

        if( data.find( CODE_LENS_PROVIDER ) != data.end()
            and not CodeLensOptions::isValid( data[ CODE_LENS_PROVIDER ] ) )
        {
            return false;
        }

        if( data.find( DOCUMENT_FORMATTING_PROVIDER ) != data.end()
            and not data[ DOCUMENT_FORMATTING_PROVIDER ].is_boolean() )
        {
            return false;
        }

        if( data.find( DOCUMENT_RANGE_FORMATTING_PROVIDER ) != data.end()
            and not data[ DOCUMENT_RANGE_FORMATTING_PROVIDER ].is_boolean() )
        {
            return false;
        }

        if( data.find( DOCUMENT_ON_TYPE_FORMATTING_PROVIDER ) != data.end()
            and not DocumentOnTypeFormattingOptions::isValid(
                    data[ DOCUMENT_ON_TYPE_FORMATTING_PROVIDER ] ) )
        {
            return false;
        }

        if( data.find( RENAME_PROVIDER ) != data.end()
            and not data[ RENAME_PROVIDER ].is_boolean() )
        {
            return false;
        }

        if( data.find( DOCUMENT_LINK_PROVIDER ) != data.end()
            and not DocumentLinkOptions::isValid(
                    data[ DOCUMENT_LINK_PROVIDER ] ) )
        {
            return false;
        }

        if( data.find( EXECUTE_COMMAND_PROVIDER ) != data.end()
            and not ExecuteCommandOptions::isValid(
                    data[ EXECUTE_COMMAND_PROVIDER ] ) )
        {
            return false;
        }

        if( data.find( EXPERIMENTAL ) != data.end()
            and not data[ EXPERIMENTAL ].is_object() )
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
    operator[]( PROCESS_ID ) = processId;
    operator[]( ROOT_URI ) = rootUri.toString();
    operator[]( CAPABILITIES )
        = Data::from_cbor( Data::to_cbor( capabilities ) );
}

std::size_t InitializeParams::processId( void ) const
{
    return at( PROCESS_ID ).get< std::size_t >();
}

DocumentUri InitializeParams::rootUri( void ) const
{
    return DocumentUri::fromString( at( PROCESS_ID ).get< std::string >() );
}

u1 InitializeParams::hasInitializationOptions( void ) const
{
    return find( INITIALIZATION_OPTIONS ) != end();
}

Data InitializeParams::initializationOptions( void ) const
{
    return at( INITIALIZATION_OPTIONS );
}

void InitializeParams::setInitializationOptions(
    const Data& initializationOptions )
{
    operator[]( INITIALIZATION_OPTIONS ) = initializationOptions;
}

ClientCapabilities InitializeParams::capabilities( void ) const
{
    return at( CAPABILITIES );
}

u1 InitializeParams::hasTrace( void ) const
{
    return find( TRACE ) != end();
}

std::string InitializeParams::trace( void ) const
{
    return at( TRACE );
}

void InitializeParams::setTrace( const std::string& trace )
{
    operator[]( TRACE ) = trace;
}

u1 InitializeParams::isValid( const Data& data )
{
    if( data.is_object() and data.find( ROOT_URI ) != data.end()
        and ( data[ ROOT_URI ].is_string() or data[ ROOT_URI ].is_null() )
        and data.find( CAPABILITIES ) != data.end()
        and ClientCapabilities::isValid( data[ CAPABILITIES ] ) )
    {
        if( data.find( PROCESS_ID ) != data.end()
            and not( data[ PROCESS_ID ].is_number()
                     or data[ PROCESS_ID ].is_null() ) )
        {
            return false;
        }

        if( not data[ ROOT_URI ].is_null() )
        {
            try
            {
                DocumentUri::fromString( data[ URI ].get< std::string >() );
            }
            catch( const std::exception& e )
            {
                return false;
            }
        }

        if( data.find( INITIALIZATION_OPTIONS ) != data.end()
            and not data[ INITIALIZATION_OPTIONS ].is_object() )
        {
            return false;
        }

        if( data.find( TRACE ) != data.end() and not data[ TRACE ].is_string() )
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
    operator[]( CAPABILITIES )
        = Data::from_cbor( Data::to_cbor( capabilities ) );
}

ServerCapabilities InitializeResult::capabilities( void ) const
{
    return at( CAPABILITIES );
}

u1 InitializeResult::isValid( const Data& data )
{
    if( data.is_object() )
    {
        if( data.find( CAPABILITIES ) != data.end()
            and not ServerCapabilities::isValid( data[ CAPABILITIES ] ) )
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
    operator[]( RETRY ) = retry;
}

u1 InitializeError::retry( void ) const
{
    return at( RETRY ).get< u1 >();
}

u1 InitializeError::isValid( const Data& data )
{
    if( data.is_object() and data.find( RETRY ) != data.end()
        and data[ RETRY ].is_boolean() )
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
    operator[]( TEXT_DOCUMENT )
        = Data::from_cbor( Data::to_cbor( textDocument ) );
}

TextDocumentItem DidOpenTextDocumentParams::textDocument( void ) const
{
    return at( TEXT_DOCUMENT );
}

u1 DidOpenTextDocumentParams::isValid( const Data& data )
{
    if( data.is_object() and data.find( TEXT_DOCUMENT ) != data.end()
        and TextDocumentItem::isValid( data[ TEXT_DOCUMENT ] ) )
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
    operator[]( TEXT ) = text;
}

u1 TextDocumentContentChangeEvent::hasRange( void ) const
{
    return find( RANGE ) != end();
}

Range TextDocumentContentChangeEvent::range( void ) const
{
    return at( RANGE );
}

void TextDocumentContentChangeEvent::setRange( const Range& range )
{
    operator[]( RANGE ) = Data::from_cbor( Data::to_cbor( range ) );
}

u1 TextDocumentContentChangeEvent::hasRangeLength( void ) const
{
    return find( RANGE_LENGTH ) != end();
}

std::size_t TextDocumentContentChangeEvent::rangeLength( void ) const
{
    return at( RANGE_LENGTH ).get< std::size_t >();
}

void TextDocumentContentChangeEvent::setRangeLength(
    const std::size_t rangeLength )
{
    operator[]( RANGE_LENGTH ) = rangeLength;
}

std::string TextDocumentContentChangeEvent::text( void ) const
{
    return at( TEXT ).get< std::string >();
}

u1 TextDocumentContentChangeEvent::isValid( const Data& data )
{
    if( data.is_object() and data.find( TEXT ) != data.end()
        and data[ TEXT ].is_string() )
    {
        if( data.find( RANGE ) != data.end()
            and not Range::isValid( data[ RANGE ] ) )
        {
            return false;
        }

        if( data.find( RANGE_LENGTH ) != data.end()
            and not data[ RANGE_LENGTH ].is_number() )
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
    operator[]( TEXT_DOCUMENT )
        = Data::from_cbor( Data::to_cbor( TEXT_DOCUMENT ) );
    operator[]( CONTENT_CHANGES ) = Data::array();

    for( auto contentChange : contentChanges )
    {
        operator[]( CONTENT_CHANGES ).push_back( contentChange );
    }
}

VersionedTextDocumentIdentifier DidChangeTextDocumentParams::textDocument(
    void ) const
{
    return at( TEXT_DOCUMENT );
}

Data DidChangeTextDocumentParams::contentChanges( void ) const
{
    return at( CONTENT_CHANGES );
}

u1 DidChangeTextDocumentParams::isValid( const Data& data )
{
    if( data.is_object() and data.find( TEXT_DOCUMENT ) != data.end()
        and data[ TEXT_DOCUMENT ].is_object()
        and data.find( CONTENT_CHANGES ) != data.end()
        and data[ CONTENT_CHANGES ].is_array() )
    {
        if( not VersionedTextDocumentIdentifier::isValid(
                data[ TEXT_DOCUMENT ] ) )
        {
            return false;
        }

        for( auto contentChange : data[ CONTENT_CHANGES ] )
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
    operator[]( DIAGNOSTICS ) = Data::array();

    for( auto diagnostic : diagnostics )
    {
        operator[]( DIAGNOSTICS ).push_back( diagnostic );
    }
}

Data CodeActionContext::diagnostics( void ) const
{
    return at( DIAGNOSTICS );
}

u1 CodeActionContext::isValid( const Data& data )
{
    if( data.is_object() and data.find( DIAGNOSTICS ) != data.end()
        and data[ DIAGNOSTICS ].is_array() )
    {
        for( auto diagnostic : data[ DIAGNOSTICS ] )
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
    operator[]( TEXT_DOCUMENT )
        = Data::from_cbor( Data::to_cbor( TEXT_DOCUMENT ) );
    operator[]( RANGE ) = Data::from_cbor( Data::to_cbor( RANGE ) );
    operator[]( CONTEXT ) = Data::from_cbor( Data::to_cbor( CONTEXT ) );
}

TextDocumentIdentifier CodeActionParams::textDocument( void ) const
{
    return at( TEXT_DOCUMENT );
}

Range CodeActionParams::range( void ) const
{
    return at( RANGE );
}

CodeActionContext CodeActionParams::context( void ) const
{
    return at( CONTEXT );
}

u1 CodeActionParams::isValid( const Data& data )
{
    if( data.is_object() and data.find( TEXT_DOCUMENT ) != data.end()
        and data[ TEXT_DOCUMENT ].is_object()
        and data.find( RANGE ) != data.end()
        and data[ RANGE ].is_object()
        and data.find( CONTEXT ) != data.end()
        and data[ CONTEXT ].is_object() )
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
    operator[]( URI ) = uri.toString();
    operator[]( DIAGNOSTICS ) = Data::array();

    for( auto diagnostic : diagnostics )
    {
        operator[]( DIAGNOSTICS ).push_back( diagnostic );
    }
}

DocumentUri PublishDiagnosticsParams::uri( void ) const
{
    return DocumentUri::fromString( at( URI ).get< std::string >() );
}

Data PublishDiagnosticsParams::diagnostics( void ) const
{
    return at( DIAGNOSTICS );
}

u1 PublishDiagnosticsParams::isValid( const Data& data )
{
    if( data.is_object() and data.find( URI ) != data.end()
        and data[ URI ].is_string()
        and data.find( DIAGNOSTICS ) != data.end()
        and data[ DIAGNOSTICS ].is_array() )
    {
        try
        {
            DocumentUri::fromString( data[ URI ].get< std::string >() );
        }
        catch( const std::exception& e )
        {
            return false;
        }

        for( auto diagnostic : data[ DIAGNOSTICS ] )
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
    operator[]( LANGUAGE ) = language;
    operator[]( VALUE ) = value;
}

std::string MarkedString::language( void ) const
{
    return at( LANGUAGE ).get< std::string >();
}

std::string MarkedString::value( void ) const
{
    return at( VALUE ).get< std::string >();
}

u1 MarkedString::isValid( const Data& data )
{
    if( data.is_string() )
    {
        return true;
    }
    else if( data.is_object() and data.find( LANGUAGE ) != data.end()
             and data[ LANGUAGE ].is_string()
             and data.find( VALUE ) != data.end()
             and data[ VALUE ].is_string() )
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
{
    operator[]( CONTENTS ) = Data::array();

    for( auto content : contents )
    {
        addContent( content );
    }
}

Data HoverResult::contents( void ) const
{
    return at( CONTENTS );
}

void HoverResult::addContent( const MarkedString& content )
{
    operator[]( CONTENTS ).push_back( content );
}

u1 HoverResult::hasRange( void ) const
{
    return find( RANGE ) != end();
}

Range HoverResult::range( void ) const
{
    return at( RANGE );
}

void HoverResult::setRange( const Range& range )
{
    operator[]( RANGE ) = Data::from_cbor( Data::to_cbor( range ) );
}

u1 HoverResult::isValid( const Data& data )
{
    if( data.is_object() and data.find( CONTENTS ) != data.end()
        and ( data[ CONTENTS ].is_object() or data[ CONTENTS ].is_array() ) )
    {
        if( data[ CONTENTS ].is_object()
            and not MarkedString::isValid( data[ CONTENTS ] ) )
        {
            return false;
        }

        if( data[ CONTENTS ].is_array() )
        {
            for( auto content : data[ CONTENTS ] )
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
//
//

//
//
//
//

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
