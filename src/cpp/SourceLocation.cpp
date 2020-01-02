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

#include "SourceLocation.h"

#include <libstdhl/File>
#include <libstdhl/Unicode>

using namespace libstdhl;

static SourcePosition::value_type add_(
    SourcePosition::value_type lhs,
    SourcePosition::difference_type rhs,
    SourcePosition::difference_type min )
{
    return ( 0 < rhs || -static_cast< SourcePosition::value_type >( rhs ) < lhs ? rhs + lhs : min );
}

//
//
// SourcePosition
//

SourcePosition::SourcePosition( void )
: SourcePosition( nullptr, 1u, 1u )
{
}

SourcePosition::SourcePosition(
    const std::shared_ptr< std::string >& fileName, value_type line, value_type column )
: fileName( fileName )
, line( line )
, column( column )
{
}

void SourcePosition::lines( difference_type count )
{
    if( count != 0 )
    {
        column = 1u;
        line = add_( line, count, 1 );
    }
}

void SourcePosition::columns( difference_type count )
{
    column = add_( column, count, 1 );
}

SourcePosition& SourcePosition::operator+=( difference_type width )
{
    columns( width );
    return *this;
}

SourcePosition& SourcePosition::operator-=( difference_type width )
{
    return operator+=( -width );
}

//
//
// SourceLocation
//

SourceLocation::SourceLocation( const SourcePosition& position )
: SourceLocation( position, position )
{
}

SourceLocation::SourceLocation( const SourcePosition& begin, const SourcePosition& end )
: begin( begin )
, end( end )
{
}

void SourceLocation::step( void )
{
    begin = end;
}

void SourceLocation::columns( SourcePosition::difference_type count )
{
    end += count;
}

void SourceLocation::lines( SourcePosition::difference_type count )
{
    end.lines( count );
}

const std::shared_ptr< std::string >& SourceLocation::fileName( void ) const
{
    return begin.fileName;
}

SourceLocation& SourceLocation::operator+=( const SourceLocation& rhs )
{
    end = rhs.end;
    return *this;
}

SourceLocation& SourceLocation::operator+=( SourcePosition::difference_type width )
{
    columns( width );
    return *this;
}

SourceLocation& SourceLocation::operator-=( SourcePosition::difference_type width )
{
    return operator+=( -width );
}

static inline std::string slice(
    const std::string& line, const std::size_t index, const std::size_t length )
{
    auto begin = index;
    for( auto position = 0; position < begin; position++ )
    {
        const auto character = line[ position ];
        const auto utf8 = Standard::RFC3629::UTF8::byteSequenceLengthIndication( character );

        if( utf8 > 1 )
        {
            const auto delta = ( utf8 - 1 );
            position += delta;
            begin += delta;
        }
    }

    if( length == 0 )
    {
        return line.substr( begin );
    }

    auto end = begin + length;
    for( auto position = begin; position < end; position++ )
    {
        assert( position < line.size() );
        const auto character = line[ position ];
        const auto utf8 = libstdhl::Unicode::UTF8::byteSequenceLengthIndication( character );
        if( utf8 > 1 )
        {
            const auto delta = ( utf8 - 1 );
            position += delta;
            end += delta;
        }
    }

    assert( end <= line.size() );
    return line.substr( begin, end - begin );
}

std::string SourceLocation::read( void ) const
{
    std::string range = "";
    const auto beginL = begin.line;
    const auto endL = end.line;

    if( not fileName() )
    {
        throw std::domain_error( "source location has no file name" );
    }

    for( auto pos = beginL; pos <= endL; pos++ )
    {
        auto line = libstdhl::File::readLine( *fileName(), pos );

        if( pos == beginL and pos == endL )
        {
            line = slice( line, begin.column - 1, end.column - begin.column );
        }
        else if( pos == beginL )
        {
            line = slice( line, begin.column - 1, 0 );
        }
        else if( pos == endL )
        {
            if( end.column != 1 )
            {
                line = slice( line, 0, end.column - 1 );
            }
            else
            {
                line = "";
            }
        }

        range += line;

        if( pos != endL )
        {
            range += "\n";
        }
    }

    return range;
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
