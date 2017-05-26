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
    const auto c = operator[]( CODE ).get< i32 >();
    return static_cast< ErrorCode >( c );
}

std::string ResponseError::message( void ) const
{
    return operator[]( MESSAGE );
}

u1 ResponseError::hasData( void ) const
{
    return find( DATA ) != end();
}

Data ResponseError::data( void ) const
{
    assert( hasData() );
    return operator[]( DATA );
}

void ResponseError::setData( const Data& data )
{
    operator[]( DATA ) = data;
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
    return operator[]( LINE ).get< std::size_t >();
}

std::size_t Position::character( void ) const
{
    return operator[]( CHARACTER ).get< std::size_t >();
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
    operator[]( START ) = start;
    operator[]( END ) = end;
}

Position Range::start( void ) const
{
    return Position( operator[]( START ) );
}

Position Range::end( void ) const
{
    return Position( operator[]( END ) );
}

u1 Range::isValid( const Data& data )
{
    return data.is_object() and data.find( START ) != data.end()
           and data[ START ].is_object() and data.find( END ) != data.end()
           and data[ END ].is_object() and Position::isValid( data[ START ] )
           and Position::isValid( data[ END ] );
}

static void to_json( Data& data, const Range& range )
{
    data = Data{
        { START, range.start() }, { END, range.start() },
    };
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
    operator[]( URI ) = uri;
    auto& r = operator[]( RANGE );
    to_json( r, range );
}

DocumentUri Location::uri( void ) const
{
    return DocumentUri::parse( operator[]( URI ).get< std::string >() );
}

Range Location::range( void ) const
{
    return Range( operator[]( RANGE ) );
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
            DocumentUri::parse( data[ URI ].get< std::string >() );
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
    auto& r = operator[]( RANGE );
    to_json( r, range );
    operator[]( MESSAGE ) = message;
}

Range Diagnostic::range( void ) const
{
    return Range( operator[]( RANGE ) );
}

std::string Diagnostic::message( void ) const
{
    return operator[]( MESSAGE ).get< std::string >();
}

u1 Diagnostic::hasSeverity( void ) const
{
    return find( SEVERITY ) != end();
}

DiagnosticSeverity Diagnostic::severity( void ) const
{
    assert( hasSeverity() );
    return static_cast< DiagnosticSeverity >( operator[](
        SEVERITY ).get< std::size_t >() );
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
    assert( hasCode() );
    return operator[]( CODE ).get< std::string >();
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
    assert( hasSource() );
    return operator[]( SOURCE ).get< std::string >();
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
        if( not Position::isValid( data[ RANGE ] ) )
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
    return operator[]( TITLE ).get< std::string >();
}

std::string Command::command( void ) const
{
    return operator[]( COMMAND ).get< std::string >();
}

u1 Command::hasArguments( void ) const
{
    return find( ARGUMENTS ) != end();
}

Data Command::arguments( void ) const
{
    assert( hasArguments() );
    return operator[]( ARGUMENTS );
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
    auto& r = operator[]( RANGE );
    to_json( r, range );
    operator[]( NEW_TEXT ) = newText;
}

Range TextEdit::range( void ) const
{
    return Range( operator[]( RANGE ) );
}

std::string TextEdit::newText( void ) const
{
    return operator[]( NEW_TEXT ).get< std::string >();
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
    operator[]( URI ) = uri;
}

DocumentUri TextDocumentIdentifier::uri( void ) const
{
    return DocumentUri::parse( operator[]( URI ).get< std::string >() );
}

u1 TextDocumentIdentifier::isValid( const Data& data )
{
    if( data.is_object() and data.find( URI ) != data.end()
        and data[ URI ].is_string() )
    {
        try
        {
            DocumentUri::parse( data[ URI ].get< std::string >() );
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
    return operator[]( VERSION ).get< std::size_t >();
}

u1 VersionedTextDocumentIdentifier::isValid( const Data& data )
{
    if( TextDocumentIdentifier( data ) and data.find( VERSION ) != data.end()
        and data[ VERSION ].is_number() )
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
    operator[]( TEXT_DOCUMENT ) = textDocument;
    operator[]( EDITS ) = Data::array();

    for( auto edit : edits )
    {
        operator[]( EDITS ).push_back( edit );
    }
}

VersionedTextDocumentIdentifier TextDocumentEdit::textDocument( void ) const
{
    return operator[]( TEXT_DOCUMENT );
}

Data TextDocumentEdit::edits( void ) const
{
    return operator[]( EDITS );
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
    assert( hasDocumentChanges() );
    return operator[]( DOCUMENT_CHANGES );
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
    operator[]( URI ) = uri;
    operator[]( LANGUAGE_ID ) = languageId;
    operator[]( VERSION ) = version;
    operator[]( TEXT ) = text;
}

DocumentUri TextDocumentItem::uri( void ) const
{
    return DocumentUri::parse( operator[]( URI ).get< std::string >() );
}

std::string TextDocumentItem::languageId( void ) const
{
    return operator[]( LANGUAGE_ID ).get< std::string >();
}

std::size_t TextDocumentItem::version( void ) const
{
    return operator[]( VERSION ).get< std::size_t >();
}

std::string TextDocumentItem::text( void ) const
{
    return operator[]( TEXT ).get< std::string >();
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
            DocumentUri::parse( data[ URI ].get< std::string >() );
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
    operator[]( TEXT_DOCUMENT ) = textDocument;
    operator[]( POSITION ) = position;
}

TextDocumentIdentifier TextDocumentPositionParams::textDocument( void ) const
{
    return operator[]( TEXT_DOCUMENT );
}

Position TextDocumentPositionParams::position( void ) const
{
    return operator[]( POSITION );
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
    assert( hasLanguage() );
    return operator[]( LANGUAGE ).get< std::string >();
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
    assert( hasScheme() );
    return operator[]( SCHEME ).get< std::string >();
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
    assert( hasPattern() );
    return operator[]( PATTERN ).get< std::string >();
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
    assert( hasDynamicRegistration() );
    return operator[]( DYNAMIC_REGISTRATION ).get< u1 >();
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
    assert( hasApplyEdit() );
    return operator[]( APPLY_EDIT ).get< u1 >();
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
    assert( hasWorkspaceEdit() );
    return operator[]( WORKSPACE_EDIT );
}

void WorkspaceClientCapabilities::setWorkspaceEdit(
    const WorkspaceEdit& workspaceEdit )
{
    operator[]( WORKSPACE_EDIT ) = workspaceEdit;
}

u1 WorkspaceClientCapabilities::hasDidChangeConfiguration( void ) const
{
    return find( DID_CHANGE_CONFIGURATION ) != end();
}

DynamicRegistration WorkspaceClientCapabilities::didChangeConfiguration(
    void ) const
{
    assert( hasDidChangeConfiguration() );
    return operator[]( DID_CHANGE_CONFIGURATION );
}

void WorkspaceClientCapabilities::setDidChangeConfiguration(
    const DynamicRegistration& didChangeConfiguration )
{
    operator[]( DID_CHANGE_CONFIGURATION ) = didChangeConfiguration;
}

u1 WorkspaceClientCapabilities::hasDidChangeWatchedFiles( void ) const
{
    return find( DID_CHANGE_WATCHED_FILES ) != end();
}

DynamicRegistration WorkspaceClientCapabilities::didChangeWatchedFiles(
    void ) const
{
    assert( hasDidChangeWatchedFiles() );
    return operator[]( DID_CHANGE_WATCHED_FILES );
}

void WorkspaceClientCapabilities::didChangeWatchedFiles(
    const DynamicRegistration& didChangeWatchedFiles )
{
    operator[]( DID_CHANGE_WATCHED_FILES ) = didChangeWatchedFiles;
}

u1 WorkspaceClientCapabilities::hasSymbol( void ) const
{
    return find( SYMBOL ) != end();
}

DynamicRegistration WorkspaceClientCapabilities::symbol( void ) const
{
    assert( hasSymbol() );
    return operator[]( SYMBOL );
}

void WorkspaceClientCapabilities::setSymbol( const DynamicRegistration& symbol )
{
    operator[]( SYMBOL ) = symbol;
}

u1 WorkspaceClientCapabilities::hasExecuteCommand( void ) const
{
    return find( EXECUTE_COMMAND ) != end();
}

DynamicRegistration WorkspaceClientCapabilities::executeCommand( void ) const
{
    assert( hasExecuteCommand() );
    return operator[]( EXECUTE_COMMAND );
}

void WorkspaceClientCapabilities::executeCommand(
    const DynamicRegistration& executeCommand )
{
    operator[]( EXECUTE_COMMAND ) = executeCommand;
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
    assert( hasDynamicRegistration() );
    return operator[]( DYNAMIC_REGISTRATION ).get< u1 >();
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
    assert( hasWillSave() );
    return operator[]( WILL_SAVE ).get< u1 >();
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
    assert( hasWillSaveWaitUntil() );
    return operator[]( WILL_SAVE_WAIT_UNITL ).get< u1 >();
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
    assert( hasDidSave() );
    return operator[]( DID_SAVE ).get< u1 >();
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
    assert( hasSnippetSupport() );
    return operator[]( SNIPPET_SUPPORT ).get< u1 >();
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
    assert( hasDynamicRegistration() );
    return operator[]( DYNAMIC_REGISTRATION ).get< u1 >();
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
    assert( hasCompletionItem() );
    return operator[]( COMPLETION_ITEM );
}

void Completion::completionItem( const CompletionItem& completionItem )
{
    operator[]( COMPLETION_ITEM ) = completionItem;
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
    assert( hasSynchronization() );
    return operator[]( SYNCHRONIZATION );
}

void TextDocumentClientCapabilities::setSynchronization(
    const Synchronization& synchronization )
{
    operator[]( SYNCHRONIZATION ) = synchronization;
}

u1 TextDocumentClientCapabilities::hasCompletion( void ) const
{
    return find( COMPLETION ) != end();
}

Completion TextDocumentClientCapabilities::completion( void ) const
{
    assert( hasCompletion() );
    return operator[]( COMPLETION );
}

void TextDocumentClientCapabilities::setCompletion(
    const Completion& completion )
{
    operator[]( COMPLETION ) = completion;
}

u1 TextDocumentClientCapabilities::hasHover( void ) const
{
    return find( HOVER ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::hover( void ) const
{
    assert( hasHover() );
    return operator[]( HOVER );
}

void TextDocumentClientCapabilities::setHover(
    const DynamicRegistration& hover )
{
    operator[]( HOVER ) = hover;
}

u1 TextDocumentClientCapabilities::hasSignatureHelp( void ) const
{
    return find( SIGNATURE_HELP ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::signatureHelp( void ) const
{
    assert( hasSignatureHelp() );
    return operator[]( SIGNATURE_HELP );
}

void TextDocumentClientCapabilities::setSignatureHelp(
    const DynamicRegistration& signatureHelp )
{
    operator[]( SIGNATURE_HELP ) = signatureHelp;
}

u1 TextDocumentClientCapabilities::hasReferences( void ) const
{
    return find( REFERENCES ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::references( void ) const
{
    assert( hasReferences() );
    return operator[]( REFERENCES );
}

void TextDocumentClientCapabilities::setReferences(
    const DynamicRegistration& references )
{
    operator[]( REFERENCES );
}

u1 TextDocumentClientCapabilities::hasDocumentHighlight( void ) const
{
    return find( DOCUMENT_HIGHLIGHT ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::documentHighlight(
    void ) const
{
    assert( hasDocumentHighlight() );
    return operator[]( DOCUMENT_HIGHLIGHT );
}

void TextDocumentClientCapabilities::setDocumentHighlight(
    const DynamicRegistration& documentHighlight )
{
    operator[]( DOCUMENT_HIGHLIGHT ) = documentHighlight;
}

u1 TextDocumentClientCapabilities::hasDocumentSymbol( void ) const
{
    return find( DOCUMENT_SYMBOL ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::documentSymbol( void ) const
{
    assert( hasDocumentSymbol() );
    return operator[]( DOCUMENT_SYMBOL );
}

void TextDocumentClientCapabilities::setDocumentSymbol(
    const DynamicRegistration& documentSymbol )
{
    operator[]( DOCUMENT_SYMBOL ) = documentSymbol;
}

u1 TextDocumentClientCapabilities::hasFormatting( void ) const
{
    return find( FORMATTING ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::formatting( void ) const
{
    assert( hasFormatting() );
    return operator[]( FORMATTING );
}

void TextDocumentClientCapabilities::setFormatting(
    const DynamicRegistration& formatting )
{
    operator[]( FORMATTING ) = formatting;
}

u1 TextDocumentClientCapabilities::hasRangeFormatting( void ) const
{
    return find( RANGE_FORMATTING ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::rangeFormatting(
    void ) const
{
    assert( hasRangeFormatting() );
    return operator[]( RANGE_FORMATTING );
}

void TextDocumentClientCapabilities::setRangeFormatting(
    const DynamicRegistration& rangeFormatting )
{
    operator[]( RANGE_FORMATTING ) = rangeFormatting;
}

u1 TextDocumentClientCapabilities::hasOnTypeFormatting( void ) const
{
    return find( ON_TYPE_FORMATTING ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::onTypeFormatting(
    void ) const
{
    assert( hasOnTypeFormatting() );
    return operator[]( ON_TYPE_FORMATTING );
}

void TextDocumentClientCapabilities::setOnTypeFormatting(
    const DynamicRegistration& onTypeFormatting )
{
    operator[]( ON_TYPE_FORMATTING ) = onTypeFormatting;
}

u1 TextDocumentClientCapabilities::hasDefinition( void ) const
{
    return find( DEFINITION ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::definition( void ) const
{
    assert( hasDefinition() );
    return operator[]( DEFINITION );
}

void TextDocumentClientCapabilities::setDefinition(
    const DynamicRegistration& definition )
{
    operator[]( DEFINITION ) = definition;
}

u1 TextDocumentClientCapabilities::hasCodeAction( void ) const
{
    return find( CODE_ACTION ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::codeAction( void ) const
{
    assert( hasCodeAction() );
    return operator[]( CODE_ACTION );
}

void TextDocumentClientCapabilities::setCodeAction(
    const DynamicRegistration& codeAction )
{
    operator[]( CODE_ACTION ) = codeAction;
}

u1 TextDocumentClientCapabilities::hasCodeLens( void ) const
{
    return find( CODE_LENS ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::codeLens( void ) const
{
    assert( hasCodeLens() );
    return operator[]( CODE_LENS );
}

void TextDocumentClientCapabilities::setCodeLens(
    const DynamicRegistration& codeLens )
{
    operator[]( CODE_LENS ) = codeLens;
}

u1 TextDocumentClientCapabilities::hasDocumentLink( void ) const
{
    return find( DOCUMENT_LINK ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::documentLink( void ) const
{
    assert( hasDocumentLink() );
    return operator[]( DOCUMENT_LINK );
}

void TextDocumentClientCapabilities::setDocumentLink(
    const DynamicRegistration& documentLink )
{
    operator[]( DOCUMENT_LINK ) = documentLink;
}

u1 TextDocumentClientCapabilities::hasRename( void ) const
{
    return find( RENAME ) != end();
}

DynamicRegistration TextDocumentClientCapabilities::rename( void ) const
{
    assert( hasRename() );
    return operator[]( RENAME );
}

void TextDocumentClientCapabilities::setRename(
    const DynamicRegistration& rename )
{
    operator[]( RENAME ) = rename;
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
    assert( hasWorkspace() );
    return operator[]( WORKSPACE );
}

void ClientCapabilities::setWorkspace(
    const WorkspaceClientCapabilities& workspace )
{
    operator[]( WORKSPACE ) = workspace;
}

u1 ClientCapabilities::hasTextDocument( void ) const
{
    return find( TEXT_DOCUMENT ) != end();
}

TextDocumentClientCapabilities ClientCapabilities::textDocument( void ) const
{
    assert( hasTextDocument() );
    return operator[]( TEXT_DOCUMENT );
}

void ClientCapabilities::setTextDocument(
    const TextDocumentClientCapabilities& textDocument )
{
    operator[]( TEXT_DOCUMENT ) = textDocument;
}

u1 ClientCapabilities::hasExperimental( void ) const
{
    return find( EXPERIMENTAL ) != end();
}

Data ClientCapabilities::experimental( void ) const
{
    assert( hasExperimental() );
    return operator[]( EXPERIMENTAL );
}

void ClientCapabilities::setExperimental( const Data& experimental )
{
    operator[]( EXPERIMENTAL ) = experimental;
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
    assert( hasIncludeText() );
    return operator[]( INCLUDE_TEXT ).get< u1 >();
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
    assert( hasOpenClose() );
    return operator[]( OPEN_CLOSE ).get< u1 >();
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
    return static_cast< TextDocumentSyncKind >( operator[](
        CHANGE ).get< std::size_t >() );
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
    assert( hasWillSave() );
    return operator[]( WILL_SAVE ).get< u1 >();
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
    assert( hasWillSaveWaitUntil() );
    return operator[]( WILL_SAVE_WAIT_UNITL ).get< u1 >();
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
    assert( hasSave() );
    return operator[]( SAVE );
}

void TextDocumentSyncOptions::setSave( const SaveOptions& save )
{
    operator[]( SAVE ) = save;
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
    assert( hasResolveProvider() );
    return operator[]( RESOLVE_PROVIDER ).get< u1 >();
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
    assert( hasTriggerCharacters() );
    return operator[]( TRIGGER_CHARACTERS );
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
    assert( hasTriggerCharacters() );
    return operator[]( TRIGGER_CHARACTERS );
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
    assert( hasResolveProvider() );
    return operator[]( RESOLVE_PROVIDER ).get< u1 >();
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
    return operator[]( FIRST_TRIGGER_CHARACTER ).get< std::string >();
}

u1 DocumentOnTypeFormattingOptions::hasMoreTriggerCharacter( void ) const
{
    return find( MORE_TRIGGER_CHARACTER ) != end();
}

Data DocumentOnTypeFormattingOptions::moreTriggerCharacter( void ) const
{
    assert( hasMoreTriggerCharacter() );
    return operator[]( MORE_TRIGGER_CHARACTER );
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
    assert( hasCommands() );
    return operator[]( COMMANDS );
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
    assert( hasTextDocumentSync() );
    return operator[]( TEXT_DOCUMENT_SYNC );
}

void ServerCapabilities::setTextDocumentSync(
    const TextDocumentSyncOptions& textDocumentSync )
{
    operator[]( TEXT_DOCUMENT_SYNC ) = textDocumentSync;
}

u1 ServerCapabilities::hasHoverProvider( void ) const
{
    return find( HOVER_PROVIDER ) != end();
}

u1 ServerCapabilities::hoverProvider( void ) const
{
    assert( hasHoverProvider() );
    return operator[]( HOVER_PROVIDER ).get< u1 >();
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
    assert( hasCompletionProvider() );
    return operator[]( COMPLETION_PROVIDER );
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
    assert( hasSignatureHelpProvider() );
    return operator[]( SIGNATURE_HELP_PROVIDER );
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
    assert( hasDefinitionProvider() );
    return operator[]( DEFINITION_PROVIDER ).get< u1 >();
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
    assert( hasReferencesProvider() );
    return operator[]( REFERENCES_PROVIDER ).get< u1 >();
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
    assert( hasDocumentHighlightProvider() );
    return operator[]( DOCUMENT_HIGHLIGHT_PROVIDER ).get< u1 >();
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
    assert( hasDocumentSymbolProvider() );
    return operator[]( DOCUMENT_SYMBOL_PROVIDER ).get< u1 >();
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
    assert( hasWorkspaceSymbolProvider() );
    return operator[]( WORKSPACE_SYMBOL_PROVIDER ).get< u1 >();
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
    assert( hasCodeActionProvider() );
    return operator[]( CODE_ACTION_PROVIDER ).get< u1 >();
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
    assert( hasCodeLensProvider() );
    return operator[]( CODE_LENS_PROVIDER );
}

void ServerCapabilities::setCodeLensProvider(
    const CodeLensOptions& codeLensProvider )
{
    operator[]( CODE_LENS_PROVIDER ) = codeLensProvider;
}

u1 ServerCapabilities::hasDocumentFormattingProvider( void ) const
{
    return find( DOCUMENT_FORMATTING_PROVIDER ) != end();
}

u1 ServerCapabilities::documentFormattingProvider( void ) const
{
    assert( hasDocumentFormattingProvider() );
    return operator[]( DOCUMENT_FORMATTING_PROVIDER ).get< u1 >();
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
    assert( hasDocumentRangeFormattingProvider() );
    return operator[]( DOCUMENT_RANGE_FORMATTING_PROVIDER ).get< u1 >();
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
    assert( hasDocumentOnTypeFormattingProvider() );
    return operator[]( DOCUMENT_ON_TYPE_FORMATTING_PROVIDER );
}

void ServerCapabilities::setDocumentOnTypeFormattingProvider(
    const DocumentOnTypeFormattingOptions& documentOnTypeFormattingProvider )
{
    operator[]( DOCUMENT_ON_TYPE_FORMATTING_PROVIDER )
        = documentOnTypeFormattingProvider;
}

u1 ServerCapabilities::hasRenameProvider( void ) const
{
    return find( RENAME_PROVIDER ) != end();
}

u1 ServerCapabilities::renameProvider( void ) const
{
    assert( hasRenameProvider() );
    return operator[]( RENAME_PROVIDER ).get< u1 >();
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
    assert( hasDocumentLinkProvider() );
    return operator[]( DOCUMENT_LINK_PROVIDER );
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
    assert( hasExecuteCommandProvider() );
    return operator[]( EXECUTE_COMMAND_PROVIDER );
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
    assert( hasExperimental() );
    return operator[]( EXPERIMENTAL );
}

void ServerCapabilities::setExperimental( const Data& experimental )
{
    operator[]( EXPERIMENTAL ) = experimental;
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
    operator[]( ROOT_URI ) = rootUri;
    operator[]( CAPABILITIES ) = capabilities;
}

std::size_t InitializeParams::processId( void ) const
{
    return operator[]( PROCESS_ID ).get< std::size_t >();
}

DocumentUri InitializeParams::rootUri( void ) const
{
    return DocumentUri::parse( operator[]( PROCESS_ID ).get< std::string >() );
}

u1 InitializeParams::hasInitializationOptions( void ) const
{
    return find( INITIALIZATION_OPTIONS ) != end();
}

Data InitializeParams::initializationOptions( void ) const
{
    assert( hasInitializationOptions() );
    return operator[]( INITIALIZATION_OPTIONS );
}

void InitializeParams::setInitializationOptions(
    const Data& initializationOptions )
{
    operator[]( INITIALIZATION_OPTIONS ) = initializationOptions;
}

ClientCapabilities InitializeParams::capabilities( void ) const
{
    return operator[]( CAPABILITIES );
}

u1 InitializeParams::hasTrace( void ) const
{
    return find( TRACE ) != end();
}

std::string InitializeParams::trace( void ) const
{
    assert( hasTrace() );
    return operator[]( TRACE );
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
                DocumentUri::parse( data[ URI ].get< std::string >() );
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
    operator[]( CAPABILITIES ) = capabilities;
}

ServerCapabilities InitializeResult::capabilities( void ) const
{
    return operator[]( CAPABILITIES );
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
    return operator[]( RETRY ).get< u1 >();
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
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
