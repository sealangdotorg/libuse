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

#ifndef _LIB_STDHL_CPP_TYPE_H_
#define _LIB_STDHL_CPP_TYPE_H_

#include "cpp/Default.h"

#include "c/type.h"

#include <bitset>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    /**
       @extends Stdhl
    */
    class Type
    {
      public:
        using Ptr = std::shared_ptr< Type >;

        enum Radix : u8
        {
            BINARY = 2,
            OCTAL = 8,
            DECIMAL = 10,
            HEXADECIMAL = 16,
            SEXAGESIMAL = 60,
            RADIX64 = 64,
        };

        enum Literal : u8
        {
            NONE = 0,
            STDHL = 1,
            C = 2,
            CPP14 = 3,
            BASE64 = 10,
            UNIX = 20
        };

        explicit Type( const std::string& data, u64 precision,
            const u1 sign = false, const Radix radix = BINARY );

        explicit Type( const std::vector< u64 >& words, const u1 sign = false );

        explicit Type( u64 word, u64 precision, const u1 sign = false );

        Type( void );

        ~Type( void ) = default;

        void add( u64 word );

        void set( std::size_t index, u64 word );

        inline void setToZero( void )
        {
            m_words.assign( m_words.size(), 0 );
            m_carry = 0;
            m_sign = false;
        }

        u64 word( std::size_t index ) const;

        u64 carry( void ) const;

        u1 sign( void ) const;

        inline const u64* data( void ) const
        {
            return m_words.data();
        }

        const std::vector< u64 >& words( void ) const;

        void shrink( void );

        inline u1 operator==( const u64 rhs ) const
        {
            auto const size = this->words().size();

            for( std::size_t c = 1; c < size; c++ )
            {
                if( this->words()[ c ] != 0 )
                {
                    return false;
                }
            }

            return this->words()[ 0 ] == rhs;
        }

        inline u1 operator!=( const u64 rhs ) const
        {
            return !operator==( rhs );
        }

        inline u1 operator==( const std::vector< u64 >& rhs ) const
        {
            Type t( rhs );
            return *this == t;
        }

        inline u1 operator!=( const std::vector< u64 >& rhs ) const
        {
            return !operator==( rhs );
        }

        inline u1 operator==( const Type& rhs ) const
        {
            auto const size = this->words().size();

            if( size != rhs.words().size() )
            {
                return false;
            }

            for( std::size_t c = 0; c < size; c++ )
            {
                if( this->words()[ c ] != rhs.words()[ c ] )
                {
                    return false;
                }
            }

            return true;
        }

        inline u1 operator!=( const Type& rhs ) const
        {
            return !operator==( rhs );
        }

        u1 operator>( const u64 rhs ) const;

        Type& operator+=( const u64 rhs );

        friend Type operator+( Type lhs, const u64 rhs )
        {
            lhs += rhs;
            return lhs;
        }

        Type& operator-=( const u64 rhs );

        friend Type operator-( Type lhs, const u64 rhs )
        {
            lhs -= rhs;
            return lhs;
        }

        Type& operator*=( const u64 rhs );

        friend inline Type operator*( Type lhs, const u64 rhs )
        {
            lhs *= rhs;
            return lhs;
        }

        Type& operator/=( const u64 rhs );

        friend Type operator/( Type lhs, const u64 rhs )
        {
            lhs /= rhs;
            return lhs;
        }

        Type& operator%=( const u64 rhs );

        friend Type operator%( Type lhs, const u64 rhs )
        {
            lhs %= rhs;
            return lhs;
        }

        Type& operator<<=( const u64 rhs );

        friend Type operator<<( Type lhs, const u64 rhs )
        {
            lhs <<= rhs;
            return lhs;
        }

        template < const Radix RADIX, const Literal LITERAL = STDHL >
        inline std::string to( void ) const
        {
            return to_string( RADIX, LITERAL );
        }

        std::string to_string(
            const Radix radix = DECIMAL, const Literal literal = NONE ) const;

        static u64 to_digit( char character, const Radix radix = DECIMAL,
            const Literal literal = NONE );

      protected:
        std::vector< u64 > m_words;
        u64 m_carry;
        u1 m_sign;
        u64 m_meta;
    };
}

#endif // _LIB_STDHL_CPP_TYPE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
