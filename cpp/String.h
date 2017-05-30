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

#ifndef _LIB_STDHL_CPP_STRING_H_
#define _LIB_STDHL_CPP_STRING_H_

#include "Default.h"
#include "Type.h"

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    /**
       @extends Stdhl
    */
    namespace String
    {
        inline void split( const std::string& str, const std::string& delimiter,
            std::vector< std::string >& result )
        {
            std::string input( str );

            char* token = std::strtok( &input[ 0 ], delimiter.c_str() );

            while( token != NULL )
            {
                result.emplace_back( token );

                token = std::strtok( NULL, delimiter.c_str() );
            }
        }

        inline std::vector< std::string > split(
            const std::string& str, const std::string& delimiter )
        {
            std::vector< std::string > result;

            split( str, delimiter, result );

            return result;
        }

        inline std::string join( const std::vector< std::string >& elements,
            const std::string& delimiter )
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

        inline u1 startsWith(
            const std::string& str, const std::string& pattern )
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

        inline u1 endsWith( const std::string& str, const std::string& pattern )
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
           adopted from: https://stackoverflow.com/a/29962178
        */

        inline std::string urlEncode( std::string str )
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

                if( isalnum( c ) or c == '-' or c == '_' or c == '.'
                    or c == '~' )
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

        /**
           adopted from https://stackoverflow.com/a/16388610
        */
        inline constexpr std::size_t value( const char* str, int h = 0 )
        {
            return !str[ h ] ? 5381 : ( value( str, h + 1 ) * 33 ) ^ str[ h ];
        }

        inline std::size_t value( const std::string str )
        {
            return value( str.c_str(), 0 );
        }

        /**
           adopted from https://stackoverflow.com/a/33122042
         */
        inline std::string trim( const std::string& str )
        {
            std::size_t first = str.find_first_not_of( ' ' );

            if( first == std::string::npos )
            {
                return "";
            }

            std::size_t last = str.find_last_not_of( ' ' );

            return str.substr( first, ( last - first + 1 ) );
        }

        /**
           adopted from http://stackoverflow.com/a/26221725
         */
        template < typename... Args >
        inline std::string format( const std::string& format, Args... args )
        {
            std::size_t size
                = std::snprintf( nullptr, 0, format.c_str(), args... ) + 1;
            std::unique_ptr< char[] > buf( new char[ size ] );
            std::snprintf( buf.get(), size, format.c_str(), args... );
            return std::string( buf.get(), buf.get() + size - 1 );
        }
    };
}

#endif // _LIB_STDHL_CPP_STRING_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
