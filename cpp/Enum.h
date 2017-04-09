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

#ifndef _LIB_STDHL_CPP_ENUM_H_
#define _LIB_STDHL_CPP_ENUM_H_

#include <type_traits>

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    /**
     * @extends Stdhl
     */
    namespace Enum
    {
        /**
         * @brief Provides a type-safe way of storing enum value combinations.
         */
        template < typename Enum >
        class Flags
        {
            using UnderlyingType = typename std::underlying_type< Enum >::type;

          public:
            static_assert( std::is_enum< Enum >::value,
                "Template parameter must be an enum" );

            /**
             * No flags will be set initially.
             */
            Flags( void )
            : m_flags( 0 )
            {
            }

            /**
             * Sets the flag \a e on initialization, all other flags will be
             * cleared.
             */
            Flags( Enum e )
            : m_flags( asBitValue( e ) )
            {
            }

            /**
             * Sets the flag \a e if it's not already set.
             */
            void set( Enum e )
            {
                m_flags |= asBitValue( e );
            }

            /**
             * Clears the flag \a e if it's set.
             */
            void unset( Enum e )
            {
                m_flags &= ~asBitValue( e );
            }

            /**
             * Sets the flag \a e if it's not already set, or clears it if it's
             * set.
             */
            void flip( Enum e )
            {
                m_flags ^= asBitValue( e );
            }

            /**
             * Checks whether the flag \a e is set.
             *
             * @return True if the flag is set, false otherwise.
             */
            bool isSet( Enum e ) const
            {
                return ( m_flags & asBitValue( e ) ) != 0;
            }

            /**
             * Clears all flags.
             */
            void clear( void )
            {
                m_flags = 0;
            }

            /**
             * Checks whether one or more flags are set.
             *
             * @return True if no flag is set, false otherwise.
             */
            bool empty( void ) const
            {
                return m_flags == 0;
            }

            /**
             * Checks whether the flag \a e is set.
             *
             * @return True if the flag is set, false otherwise.
             */
            bool operator&( Enum e ) const
            {
                return isSet( e );
            }

            /**
             * Sets the flag \a e if it's not already set.
             */
            Flags& operator|=( Enum e )
            {
                set( e );
                return *this;
            }

            /**
             * Sets the flag \a e in \a flags if it's not already set.
             */
            friend Flags operator|( Flags flags, Enum e )
            {
                flags.set( e );
                return flags;
            }

            /**
             * Combines \a lhs and \a rhs
             */
            friend Flags operator|( Flags lhs, Flags rhs )
            {
                lhs.m_flags |= rhs.m_flags;
                return lhs;
            }

            /**
             * Sets the flag \a e if it's not already set, or clears it if it's
             * set.
             */
            Flags& operator^=( Enum e )
            {
                flip( e );
                return *this;
            }

            /**
             * Sets the flag \a e in \a flags if it's not already set, or
             * clears it in \a flags if it's set.
             */
            friend Flags operator^( Flags flags, Enum e )
            {
                flags.flip( e );
                return flags;
            }

          private:
            static UnderlyingType asBitValue( Enum e )
            {
                assert( static_cast< UnderlyingType >( e )
                        < ( sizeof( UnderlyingType ) * 8 ) );
                return 1 << static_cast< UnderlyingType >( e );
            }

          private:
            UnderlyingType m_flags;
        };
    }
}

template < typename T >
typename std::enable_if< std::is_enum< T >::value,
    libstdhl::Enum::Flags< T > >::type
operator|( T lhs, T rhs )
{
    return libstdhl::Enum::Flags< T >( lhs )
           | libstdhl::Enum::Flags< T >( rhs );
}

#endif // _LIB_STDHL_CPP_ENUM_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
