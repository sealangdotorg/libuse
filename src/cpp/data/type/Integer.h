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

#pragma once
#ifndef _LIBSTDHL_CPP_TYPE_INTEGER_H_
#define _LIBSTDHL_CPP_TYPE_INTEGER_H_

#include <libstdhl/data/type/Data>

#include <vector>

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    namespace Type
    {
        class Natural;

        class Integer : public Data
        {
          public:
            using Ptr = std::shared_ptr< Integer >;

            using Data::Data;

            static Integer fromString( const std::string& value, const Radix radix );

            const u64 operator[]( std::size_t idx ) const;

            //
            // operator '==' and '!='
            //

            u1 operator==( const u64 rhs ) const;

            inline u1 operator!=( const u64 rhs ) const
            {
                return not( operator==( rhs ) );
            }

            u1 operator==( const Integer& rhs ) const;

            inline u1 operator!=( const Integer& rhs ) const
            {
                return not( operator==( rhs ) );
            }

            //
            // operator '<' and '>='
            //

            u1 operator<( const u64 rhs ) const;

            inline u1 operator>=( const u64 rhs ) const
            {
                return not( operator<( rhs ) );
            }

            inline friend u1 operator<( const u64 lhs, const Integer& rhs )
            {
                return rhs >= lhs;
            }

            u1 operator<( const Integer& rhs ) const;

            inline u1 operator>=( const Integer& rhs ) const
            {
                return not( operator<( rhs ) );
            }

            //
            // operator '>' and '<='
            //

            u1 operator>( const u64 rhs ) const;

            inline u1 operator<=( const u64 rhs ) const
            {
                return not( operator>( rhs ) );
            }

            inline friend u1 operator>( const u64 lhs, const Integer& rhs )
            {
                return rhs <= lhs;
            }

            u1 operator>( const Integer& rhs ) const;

            inline u1 operator<=( const Integer& rhs ) const
            {
                return not( operator>( rhs ) );
            }

            //
            // operator '+=' and '+'
            //

            Integer& operator++( void );

            Integer operator++( const int increment );

            Integer& operator+=( const u64 rhs );

            inline friend Integer operator+( Integer lhs, const u64 rhs )
            {
                lhs += rhs;
                return lhs;
            }

            Integer& operator+=( const Integer& rhs );

            inline friend Integer operator+( Integer lhs, const Integer& rhs )
            {
                lhs += rhs;
                return lhs;
            }

            //
            // operator '--', '-=' and '-'
            //

            Integer& operator--( void );

            Integer operator--( const int decrement );

            Integer& operator-=( const u64 rhs );

            inline friend Integer operator-( Integer lhs, const u64 rhs )
            {
                lhs -= rhs;
                return lhs;
            }

            Integer& operator-=( const Integer& rhs );

            inline friend Integer operator-( Integer lhs, const Integer& rhs )
            {
                lhs -= rhs;
                return lhs;
            }

            Integer& operator*=( const Integer& rhs );

            inline friend Integer operator*( Integer lhs, const Integer& rhs )
            {
                lhs *= rhs;
                return lhs;
            }

            //
            // operator '-' (NEGATE)
            //

            inline friend Integer operator-( Integer arg )
            {
                auto tmp = -static_cast< Data& >( arg );
                return static_cast< Integer& >( tmp );
            }

            //
            // operator '*=' and '*'
            //

            Integer& operator*=( const u64 rhs );

            inline friend Integer operator*( Integer lhs, const u64 rhs )
            {
                lhs *= rhs;
                return lhs;
            }

            //
            // operator '%=' and '%'
            //

            Integer& operator%=( const u64 rhs );

            inline friend Integer operator%( Integer lhs, const u64 rhs )
            {
                lhs %= rhs;
                return lhs;
            }

            Integer& operator%=( const Integer& rhs );

            inline friend Integer operator%( Integer lhs, const Integer& rhs )
            {
                lhs %= rhs;
                return lhs;
            }

            //
            // operator '/=' and '/'
            //

            Integer& operator/=( const Integer& rhs );

            inline friend Integer operator/( Integer lhs, const Integer& rhs )
            {
                lhs /= rhs;
                return lhs;
            }

            //
            // operator '^=' and '^'
            //

            Integer& operator^=( const Natural& rhs );

            inline friend Integer operator^( Integer lhs, const Natural& rhs )
            {
                lhs ^= rhs;
                return lhs;
            }

            //
            // operator '~' (INVERSE)
            //

            Integer operator~( void ) const;

            //
            // operator '<<=' and '<<'
            //

            Integer& operator<<=( const u64 rhs );

            inline friend Integer operator<<( Integer lhs, const u64 rhs )
            {
                lhs <<= rhs;
                return lhs;
            }

            Integer& operator<<=( const Integer& rhs );

            inline friend Integer operator<<( Integer lhs, const Integer rhs )
            {
                lhs <<= rhs;
                return lhs;
            }

            //
            // operator '>>=' and '>>'
            //

            Integer& operator>>=( const u64 rhs );

            inline friend Integer operator>>( Integer lhs, const u64 rhs )
            {
                lhs >>= rhs;
                return lhs;
            }

            Integer& operator>>=( const Integer& rhs );

            inline friend Integer operator>>( Integer lhs, const Integer rhs )
            {
                lhs >>= rhs;
                return lhs;
            }
        };

        class IntegerLayout final : public Layout
        {
          public:
            using Ptr = std::unique_ptr< IntegerLayout >;

            IntegerLayout( const u64 low, const u64 high );

            Layout* clone( void ) const override;

            std::size_t hash( void ) const override;

            const u64 operator[]( std::size_t idx ) const;

            const std::vector< u64 >& word( void ) const;

            //
            // operator '==' and '!='
            //

            u1 operator==( const u64 rhs ) const;

            inline u1 operator!=( const u64 rhs ) const
            {
                return not( operator==( rhs ) );
            }

            u1 operator==( const IntegerLayout& rhs ) const;

            inline u1 operator!=( const IntegerLayout& rhs ) const
            {
                return not( operator==( rhs ) );
            }

            //
            // operator '+=' and '+'
            //

            IntegerLayout& operator+=( const u64 rhs );

            inline friend IntegerLayout operator+( IntegerLayout lhs, const u64 rhs )
            {
                lhs += rhs;
                return lhs;
            }

            //
            // operator '-=' and '-'
            //

            IntegerLayout& operator-=( const u64 rhs );

            inline friend IntegerLayout operator-( IntegerLayout lhs, const u64 rhs )
            {
                lhs -= rhs;
                return lhs;
            }

            //
            // operator '*=' and '*'
            //

            IntegerLayout& operator*=( const u64 rhs );

            inline friend IntegerLayout operator*( IntegerLayout lhs, const u64 rhs )
            {
                lhs *= rhs;
                return lhs;
            }

            //
            // operator '~' (INVERSE)
            //

            IntegerLayout& operator~( void );

            //
            // operator '<<=' and '<<'
            //

            IntegerLayout& operator<<=( const u64 rhs );

            inline friend IntegerLayout operator<<( IntegerLayout lhs, const u64 rhs )
            {
                lhs <<= rhs;
                return lhs;
            }

            //
            // operator '>>=' and '>>'
            //

            IntegerLayout& operator>>=( const u64 rhs );

            inline friend IntegerLayout operator>>( IntegerLayout lhs, const u64 rhs )
            {
                lhs >>= rhs;
                return lhs;
            }

          private:
            std::vector< u64 > m_word;
        };
    }
}

#endif  // _LIBSTDHL_CPP_TYPE_INTEGER_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
