//
//  Copyright (C) 2014-2021 CASM Organization <https://casm-lang.org>
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

#ifndef _LIBSTDHL_SOURCE_LOCATION_H_
#define _LIBSTDHL_SOURCE_LOCATION_H_

#include <libstdhl/Type>

#include <memory>

namespace libstdhl
{
    class SourcePosition
    {
      public:
        using value_type = u32;
        using difference_type = i32;

      public:
        explicit SourcePosition( void );

        SourcePosition(
            const std::shared_ptr< std::string >& fileName, value_type line, value_type column );

        void lines( difference_type count );

        void columns( difference_type count );

        SourcePosition& operator+=( difference_type width );

        friend SourcePosition operator+( SourcePosition lhs, difference_type width )
        {
            return lhs += width;
        }

        SourcePosition& operator-=( difference_type width );

        friend SourcePosition operator-( SourcePosition lhs, difference_type width )
        {
            return lhs += -width;
        }

        friend bool operator==( const SourcePosition& lhs, const SourcePosition& rhs )
        {
            return ( lhs.line == rhs.line ) and ( lhs.column == rhs.column ) and
                   ( ( lhs.fileName == rhs.fileName ) or
                     ( lhs.fileName and rhs.fileName and *lhs.fileName == *rhs.fileName ) );
        }

        friend bool operator!=( const SourcePosition& lhs, const SourcePosition& rhs )
        {
            return not( lhs == rhs );
        }

        friend std::ostream& operator<<( std::ostream& stream, const SourcePosition& position )
        {
            return stream << std::to_string( position.line ) << ":"
                          << std::to_string( position.column );
        }

      public:
        std::shared_ptr< std::string > fileName;
        value_type line;
        value_type column;
    };

    class SourceLocation
    {
      public:
        explicit SourceLocation( const SourcePosition& position = SourcePosition() );

        SourceLocation( const SourcePosition& begin, const SourcePosition& end );

        void step( void );

        void columns( SourcePosition::difference_type count );

        void lines( SourcePosition::difference_type count );

        const std::shared_ptr< std::string >& fileName( void ) const;

        SourceLocation& operator+=( const SourceLocation& rhs );

        friend SourceLocation operator+( SourceLocation lhs, const SourceLocation& rhs )
        {
            return lhs += rhs;
        }

        SourceLocation& operator+=( SourcePosition::difference_type width );

        friend SourceLocation operator+( SourceLocation lhs, SourcePosition::difference_type width )
        {
            return lhs += width;
        }

        SourceLocation& operator-=( SourcePosition::difference_type width );

        friend SourceLocation operator-( SourceLocation lhs, SourcePosition::difference_type width )
        {
            return lhs += -width;
        }

        friend bool operator==( const SourceLocation& lhs, const SourceLocation& rhs )
        {
            return lhs.begin == rhs.begin and lhs.end == rhs.end;
        }

        friend bool operator!=( const SourceLocation& lhs, const SourceLocation& rhs )
        {
            return not( lhs == rhs );
        }

        friend std::ostream& operator<<( std::ostream& stream, const SourceLocation& location )
        {
            if( location.begin != location.end )
            {
                return stream << location.begin << ".." << location.end;
            }
            else
            {
                return stream << location.begin;
            }
        }

        std::string read( void ) const;

      public:
        SourcePosition begin;
        SourcePosition end;
    };
}

#endif  // _LIBSTDHL_SOURCE_LOCATION_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
