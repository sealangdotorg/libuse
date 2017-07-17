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

#ifndef _LIB_STDHL_CPP_RANDOM_H_
#define _LIB_STDHL_CPP_RANDOM_H_

#include "Default.h"
#include "Math.h"
#include "Type.h"
#include "Limits.h"

#include <random>

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    /**
       @extends Stdhl
    */
    namespace Random
    {
        template < typename T >
        T uniform( const T& from, const T& to )
        {
            if( from >= to )
            {
                std::domain_error( "invalid range" );
            }

            std::random_device device;
            static thread_local std::default_random_engine engine( device() );

            std::uniform_int_distribution< T > distribution( from, to );
            return distribution( engine );
        }

        template <>
        inline Integer uniform< Integer >( const Integer& from, const Integer& to )
        {
            return uniform< u64 >( from.word( 0 ), to.word( 0 ) );
        }

        template <>
        inline FloatingPoint uniform< FloatingPoint >(
            const FloatingPoint& from, const FloatingPoint& to )
        {
            if( from >= to )
            {
                std::domain_error( "invalid range" );
            }

            std::random_device device;
            static thread_local std::default_random_engine engine( device() );

            std::uniform_real_distribution< double > distribution(
                from.word( 0 ), to.word( 0 ) );
            return distribution( engine );
        }

        template < typename T >
        inline T uniform( void )
        {
            return uniform( Limits< T >::min(), Limits< T >::max() );
        }
    };
}

#endif // _LIB_STDHL_CPP_RANDOM_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
