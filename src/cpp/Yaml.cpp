//
//  Copyright (C) 2014-2019 CASM Organization <https://casm-lang.org>
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

libstdhl::u1 libstdhl::Yaml::Content::has( const std::string& mapKey ) const
{
    if( type() != libstdhl::Yaml::Type::MAP )
    {
        return false;
    }

    for( auto it = this->Begin(); it != this->End(); it++ )
    {
        if( ( *it ).first == mapKey )
        {
            return true;
        }
    }

    return false;
}

libstdhl::u1 libstdhl::Yaml::Content::has( const std::size_t sequenceIndex ) const
{
    if( type() != libstdhl::Yaml::Type::SEQUENCE )
    {
        return false;
    }

    return sequenceIndex >= 0 and sequenceIndex < size();
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

libstdhl::Yaml::Content& libstdhl::Yaml::Content::operator[]( const std::size_t sequenceIndex )
{
    assert( has( sequenceIndex ) );
    return static_cast< Content& >( ::Yaml::Node::operator[]( sequenceIndex ) );
}

libstdhl::Yaml::Content& libstdhl::Yaml::Content::operator[]( const std::string& mapKey )
{
    assert( has( mapKey ) );
    return static_cast< Content& >( ::Yaml::Node::operator[]( mapKey ) );
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
    ::Yaml::Parse( yaml, stream );
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
