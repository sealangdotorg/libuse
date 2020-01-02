//
//  Copyright (C) 2014-2020 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
//                Christian Lascsak
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

#include "Yaml.h"

#include <libstdhl/data/type/Integer>

#include <cassert>
#include <vector>  // missing include in mini-yaml/Yaml.h

#include "vendor/mini-yaml/Yaml.cpp"

//
//
// Yaml::Content
//

libstdhl::Yaml::Content::Content( void )
: ::Yaml::Node()
{
}

libstdhl::Yaml::Content& libstdhl::Yaml::Content::operator[]( const std::string& mapKey ) const
{
    if( not has( mapKey ) )
    {
        throw libstdhl::Yaml::Exception( "YAML map has no key '" + mapKey + "'" );
    }
    auto& map = ( ::Yaml::Node&)( *this );
    auto& mapValue = map[ mapKey ];
    return static_cast< Content& >( mapValue );
}

libstdhl::Yaml::Content& libstdhl::Yaml::Content::operator[](
    const std::size_t sequenceIndex ) const
{
    if( not has( sequenceIndex ) )
    {
        throw libstdhl::Yaml::Exception(
            "YAML sequence has no index '" + std::to_string( sequenceIndex ) + "'" );
    }
    auto& sequence = ( ::Yaml::Node&)( *this );
    auto& sequenceValue = sequence[ sequenceIndex ];
    return static_cast< Content& >( sequenceValue );
}

libstdhl::u1 libstdhl::Yaml::Content::has( const std::string& mapKey ) const
{
    if( type() != libstdhl::Yaml::Type::MAP and type() != libstdhl::Yaml::Type::NONE )
    {
        throw libstdhl::Yaml::Exception(
            "YAML content '" + description() + "' does not support map-based operation" );
    }

    u1 found = false;
    foreach( [&]( const std::string& key, const Content&, u1& abort ) {
        if( mapKey == key )
        {
            found = true;
            abort = true;
        }
    } )
        ;
    return found;
}

libstdhl::u1 libstdhl::Yaml::Content::has( const std::size_t sequenceIndex ) const
{
    if( type() != libstdhl::Yaml::Type::SEQUENCE and type() != libstdhl::Yaml::Type::NONE )
    {
        throw libstdhl::Yaml::Exception(
            "YAML content '" + description() + "' does not support sequence-based operation" );
    }

    return sequenceIndex >= 0 and sequenceIndex < size();
}

libstdhl::Yaml::Content::Result libstdhl::Yaml::Content::find( const std::string& mapKey ) const
{
    if( has( mapKey ) )
    {
        auto& self = *const_cast< Content* >( this );
        return Result{ &self[ mapKey ] };
    }

    return Result{ libstdhl::nullopt };
}

libstdhl::Yaml::Content::Result libstdhl::Yaml::Content::find(
    const std::size_t sequenceIndex ) const
{
    if( has( sequenceIndex ) )
    {
        auto& self = *const_cast< Content* >( this );
        return Result{ &self[ sequenceIndex ] };
    }

    return Result{ libstdhl::nullopt };
}

libstdhl::Yaml::Content::Value libstdhl::Yaml::Content::emplace(
    const std::string& mapKey, const Content& mapValue )
{
    if( has( mapKey ) )
    {
        auto existingMapValue = this->operator[]( mapKey );
        this->operator[]( mapKey ) = mapValue;
        return Value{ existingMapValue };
    }

    auto& map = ( ::Yaml::Node&)( *this );
    auto& content = map[ mapKey ];
    content = mapValue;
    return Value{ libstdhl::nullopt };
}

libstdhl::Yaml::Content::Value libstdhl::Yaml::Content::emplace(
    const Content& sequenceValue, const std::size_t sequenceIndex )
{
    if( has( sequenceIndex ) )
    {
        auto existingSequenceValue = this->operator[]( sequenceIndex );
        this->operator[]( sequenceIndex ) = sequenceValue;
        return Value{ existingSequenceValue };
    }

    emplace_back( sequenceValue );
    return Value{ libstdhl::nullopt };
}

libstdhl::Yaml::Content& libstdhl::Yaml::Content::emplace_back( const Content& sequenceValue )
{
    if( type() != libstdhl::Yaml::Type::SEQUENCE and type() != libstdhl::Yaml::Type::NONE )
    {
        throw libstdhl::Yaml::Exception(
            "YAML content '" + description() + "' does not support sequence-based operation" );
    }

    auto& content = static_cast< Content& >( PushBack() );
    assert( size() > 0 and has( size() - 1 ) );
    content = sequenceValue;
    return content;
}

void libstdhl::Yaml::Content::erase( const std::string& mapKey )
{
    if( not has( mapKey ) )
    {
        throw libstdhl::Yaml::Exception(
            "unable to erase YAML content at map key '" + mapKey + "', does not exist" );
    }
    Erase( mapKey );
}

void libstdhl::Yaml::Content::erase( const std::size_t sequenceIndex )
{
    if( not has( sequenceIndex ) )
    {
        throw libstdhl::Yaml::Exception(
            "unable to erase YAML content at sequence index '" + std::to_string( sequenceIndex ) +
            "', does not exist" );
    }
    Erase( sequenceIndex );
}

libstdhl::Yaml::Content::iterator libstdhl::Yaml::Content::begin( void )
{
    return Begin();
}

libstdhl::Yaml::Content::const_iterator libstdhl::Yaml::Content::begin( void ) const
{
    return Begin();
}

libstdhl::Yaml::Content::const_iterator libstdhl::Yaml::Content::cbegin( void ) const
{
    return begin();
}

libstdhl::Yaml::Content::iterator libstdhl::Yaml::Content::end( void )
{
    return End();
}

libstdhl::Yaml::Content::const_iterator libstdhl::Yaml::Content::end( void ) const
{
    return End();
}

libstdhl::Yaml::Content::const_iterator libstdhl::Yaml::Content::cend( void ) const
{
    return end();
}

void libstdhl::Yaml::Content::foreach(
    const std::function< void( const std::string& mapKey, const Content& mapValue, u1& abort ) >
        process ) const
{
    if( type() != libstdhl::Yaml::Type::MAP and type() != libstdhl::Yaml::Type::NONE )
    {
        throw libstdhl::Yaml::Exception(
            "YAML content '" + description() + "' does not support map-based operation" );
    }

    if( size() > 0 )
    {
        for( auto it = begin(); it != end(); it++ )
        {
            const auto& mapKey = ( *it ).first;
            const auto& mapValue = static_cast< const Content& >( ( *it ).second );
            u1 abort = false;
            process( mapKey, mapValue, abort );
            if( abort )
            {
                break;
            }
        }
    }
}

libstdhl::Yaml::Type libstdhl::Yaml::Content::type( void ) const
{
    if( IsNone() )
    {
        return libstdhl::Yaml::Type::NONE;
    }
    else if( IsMap() )
    {
        return libstdhl::Yaml::Type::MAP;
    }
    else if( IsSequence() )
    {
        return libstdhl::Yaml::Type::SEQUENCE;
    }
    else
    {
        assert( IsScalar() );
        const auto scalar = As< std::string >();
        if( scalar == "true" or scalar == "false" or scalar == "yes" or scalar == "no" or
            scalar == "1" or scalar == "0" )
        {
            return libstdhl::Yaml::Type::BOOLEAN;
        }

        try
        {
            libstdhl::Type::createInteger( scalar );
        }
        catch( const std::domain_error& )
        {
            return libstdhl::Yaml::Type::STRING;
        }

        return libstdhl::Yaml::Type::INTEGER;
    }
}

std::string libstdhl::Yaml::Content::description( void ) const
{
    switch( type() )
    {
        case libstdhl::Yaml::Type::NONE:
        {
            return "None";
        }
        case libstdhl::Yaml::Type::MAP:
        {
            return "Map";
        }
        case libstdhl::Yaml::Type::SEQUENCE:
        {
            return "Sequence";
        }
        case libstdhl::Yaml::Type::BOOLEAN:
        {
            return "Boolean";
        }
        case libstdhl::Yaml::Type::INTEGER:
        {
            return "Integer";
        }
        case libstdhl::Yaml::Type::STRING:
        {
            return "String";
        }
    }
    assert( !"unreachable" );
    return std::string();
}

std::size_t libstdhl::Yaml::Content::size( void ) const
{
    return Size();
}

std::string libstdhl::Yaml::Content::dump( void ) const
{
    std::stringstream stream;
    dump( stream );
    return stream.str();
}

void libstdhl::Yaml::Content::dump( std::iostream& stream ) const
{
    ::Yaml::Serialize( *this, stream );
}

libstdhl::Yaml::Content libstdhl::Yaml::Content::fromString( const std::string& text )
{
    std::stringstream stream( text );
    return fromStream( stream );
}

libstdhl::Yaml::Content libstdhl::Yaml::Content::fromStream( std::iostream& stream )
{
    libstdhl::Yaml::Content yaml;
    try
    {
        ::Yaml::Parse( yaml, stream );
    }
    catch( const ::Yaml::Exception& e )
    {
        throw libstdhl::Yaml::Exception( e.what() );
    }
    return yaml;
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
