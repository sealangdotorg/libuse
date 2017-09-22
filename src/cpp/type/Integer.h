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

#ifndef _LIBSTDHL_CPP_TYPE_INTEGER_H_
#define _LIBSTDHL_CPP_TYPE_INTEGER_H_

#include <libstdhl/type/Data>

#include <vector>

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    namespace Type
    {
        class Integer : public Data
        {
          public:
            using Ptr = std::shared_ptr< Integer >;

            using Data::Data;

            static Integer fromString(
                const std::string& value, const Radix radix );

            const u64 operator[]( std::size_t idx ) const;

            inline friend Integer operator-( Integer arg )
            {
                auto tmp = -static_cast< Data& >( arg );
                return static_cast< Integer& >( tmp );
            }

            Integer& operator<<=( const u64 rhs );

            inline friend Integer operator<<( Integer lhs, const u64 rhs )
            {
                lhs <<= rhs;
                return lhs;
            }

            Integer& operator++( void );

            Integer operator++( int );

            Integer& operator+=( const u64 rhs );

            inline friend Integer operator+( Integer lhs, const u64 rhs )
            {
                lhs += rhs;
                return lhs;
            }

            Integer& operator--( void );

            Integer operator--( int );

            Integer& operator-=( const u64 rhs );

            inline friend Integer operator-( Integer lhs, const u64 rhs )
            {
                lhs -= rhs;
                return lhs;
            }

            Integer& operator*=( const u64 rhs );

            inline friend Integer operator*( Integer lhs, const u64 rhs )
            {
                lhs *= rhs;
                return lhs;
            }

            u1 operator==( const u64& rhs ) const;

            Integer& operator+=( const Integer& rhs );

            inline friend Integer operator+( Integer lhs, const Integer& rhs )
            {
                lhs += rhs;
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

            Integer& operator%=( const Integer& rhs );

            inline friend Integer operator%( Integer lhs, const Integer& rhs )
            {
                lhs %= rhs;
                return lhs;
            }

            Integer& operator/=( const Integer& rhs );

            inline friend Integer operator/( Integer lhs, const Integer& rhs )
            {
                lhs /= rhs;
                return lhs;
            }

            Integer operator~(void)const;

            u1 operator==( const Integer& rhs ) const;

            inline u1 operator!=( const Integer& rhs ) const
            {
                return not( operator==( rhs ) );
            }

            u1 operator<( const Integer& rhs ) const;

            inline u1 operator>=( const Integer& rhs ) const
            {
                return not( operator<( rhs ) );
            }

            u1 operator>( const Integer& rhs ) const;

            inline u1 operator<=( const Integer& rhs ) const
            {
                return not( operator>( rhs ) );
            }
        };

        class IntegerLayout final : public Layout
        {
          public:
            using Ptr = std::unique_ptr< IntegerLayout >;

            IntegerLayout( const u64 low, const u64 high );

            Layout* clone( void ) const override;

            const u64 operator[]( std::size_t idx ) const;

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

            IntegerLayout& operator<<=( const u64 rhs );

            inline friend IntegerLayout operator<<(
                IntegerLayout lhs, const u64 rhs )
            {
                lhs <<= rhs;
                return lhs;
            }

            IntegerLayout& operator+=( const u64 rhs );

            inline friend IntegerLayout operator+(
                IntegerLayout lhs, const u64 rhs )
            {
                lhs += rhs;
                return lhs;
            }

            IntegerLayout& operator-=( const u64 rhs );

            inline friend IntegerLayout operator-(
                IntegerLayout lhs, const u64 rhs )
            {
                lhs -= rhs;
                return lhs;
            }

            IntegerLayout& operator*=( const u64 rhs );

            inline friend IntegerLayout operator*(
                IntegerLayout lhs, const u64 rhs )
            {
                lhs *= rhs;
                return lhs;
            }

            IntegerLayout& operator~( void );

          private:
            std::vector< u64 > m_word;
        };
    }
}

#endif // _LIBSTDHL_CPP_TYPE_INTEGER_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
