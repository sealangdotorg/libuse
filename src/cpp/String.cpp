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

#include "String.h"

#include <libstdhl/Unicode>

#include <cstring>
#include <sstream>

using namespace libstdhl;

void String::split(
    const std::string& str, const std::string& delimiter, std::vector< std::string >& result )
{
    std::size_t start = 0;
    std::size_t end = std::string::npos;
    while( ( end = str.find( delimiter, start ) ) != std::string::npos )
    {
        result.emplace_back( str.substr( start, end - start ) );
        start = end + delimiter.length();
    }
    result.emplace_back( str.substr( start, end ) );
}

std::vector< std::string > String::split( const std::string& str, const std::string& delimiter )
{
    std::vector< std::string > result;

    split( str, delimiter, result );

    return result;
}

std::string String::join( const std::vector< std::string >& elements, const std::string& delimiter )
{
    std::stringstream result;

    u1 first = true;
    for( const auto& element : elements )
    {
        result << ( first ? "" : delimiter ) << element;
        first = false;
    }

    return result.str();
}

u1 String::startsWith( const std::string& str, const std::string& pattern )
{
    if( strncmp( str.c_str(), pattern.c_str(), pattern.size() ) == 0 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

u1 String::endsWith( const std::string& str, const std::string& pattern )
{
    const i64 pos = str.size() - pattern.size();

    if( pos < 0 )
    {
        return false;
    }

    if( strcmp( &str.c_str()[ pos ], pattern.c_str() ) == 0 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
   adopted from:
   https://stackoverflow.com/questions/2896600/how-to-replace-all-occurrences-of-a-character-in-string#answer-24315631
*/

std::string String::replaceAll(
    const std::string& str, const std::string& from, const std::string& to )
{
    auto tmp = str;
    size_t start_pos = 0;
    while( ( start_pos = tmp.find( from, start_pos ) ) != std::string::npos )
    {
        tmp.replace( start_pos, from.length(), to );
        start_pos += to.length();
    }
    return tmp;
}

std::string String::expansion(
    const std::string& str,
    const std::size_t start,
    const std::size_t length,
    const std::size_t tabSize,
    const char asciiSymbol,
    std::string utf8Symbol )
{
    if( utf8Symbol == "" )
    {
        utf8Symbol = std::string( 2, asciiSymbol );
    }

    auto begin = start;

    for( auto position = 0; position < begin; position++ )
    {
        const auto character = str[ position ];
        const auto utf8 = Standard::RFC3629::UTF8::byteSequenceLengthIndication( character );

        if( utf8 > 1 )
        {
            position += utf8 - 1;
            begin += utf8 - 1;
        }
    }

    auto end = begin + length;
    std::stringstream tmp;

    for( auto position = begin; position < end; position++ )
    {
        const auto character = str[ position ];
        const auto utf8 = Standard::RFC3629::UTF8::byteSequenceLengthIndication( character );

        if( character == '\t' )
        {
            tmp << std::string( tabSize, asciiSymbol );
        }
        else if( utf8 > 1 )
        {
            tmp << utf8Symbol;
            position += utf8 - 1;
            end += utf8 - 1;
        }
        else
        {
            tmp << asciiSymbol;
        }
    }

    return tmp.str();
}

/**
   adopted from: https://stackoverflow.com/a/29962178
*/

std::string String::urlEncode( std::string str )
{
    std::string new_str = "";
    char c;
    int ic;
    const char* chars = str.c_str();
    char bufHex[ 10 ];
    int len = strlen( chars );

    for( int i = 0; i < len; i++ )
    {
        c = chars[ i ];
        ic = c;

        if( isalnum( c ) or c == '-' or c == '_' or c == '.' or c == '~' )
        {
            new_str += c;
        }
        else
        {
            sprintf( bufHex, "%X", c );
            if( ic < 16 )
            {
                new_str += "%0";
            }
            else
            {
                new_str += "%";
            }
            new_str += bufHex;
        }
    }
    return new_str;
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
