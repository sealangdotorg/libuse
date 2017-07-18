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

#ifndef _LIB_STDHL_CPP_TYPE_RATIONAL_H_
#define _LIB_STDHL_CPP_TYPE_RATIONAL_H_

#include "Integer.h"

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    namespace Type
    {
        class Rational : public Data
        {
          public:
            using Ptr = std::shared_ptr< Rational >;

            using Data::Data;

            Rational( const Integer& numerator, const Integer& denominator );

            static Rational fromString(
                const std::string& value, const Radix radix );

            const Integer& numerator( void ) const;

            const Integer& denominator( void ) const;

            inline friend Rational operator-( Rational arg )
            {
                auto tmp = -static_cast< Data& >( arg );
                return static_cast< Rational& >( tmp );
            }

            u1 operator==( const u64 rhs ) const;

            inline u1 operator!=( const u64 rhs ) const
            {
                return not( operator==( rhs ) );
            }

            u1 operator==( const Rational& rhs ) const;

            inline u1 operator!=( const Rational& rhs ) const
            {
                return not( operator==( rhs ) );
            }
        };

        class RationalLayout final : public Layout
        {
          public:
            using Ptr = std::unique_ptr< RationalLayout >;

            RationalLayout(
                const Integer& numerator, const Integer& denominator );

            Layout* clone( void ) const override;

            const Integer& numerator( void ) const;

            const Integer& denominator( void ) const;

            u1 operator==( const u64 rhs ) const;

            inline u1 operator!=( const u64 rhs ) const
            {
                return not( operator==( rhs ) );
            }

            u1 operator==( const RationalLayout& rhs ) const;

            inline u1 operator!=( const RationalLayout& rhs ) const
            {
                return not( operator==( rhs ) );
            }

          private:
            Integer m_numerator;
            Integer m_denominator;
        };
    }
}

#endif // _LIB_STDHL_CPP_RATIONAL_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
