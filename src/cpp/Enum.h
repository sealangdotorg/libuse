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

#pragma once
#ifndef _LIBSTDHL_CPP_ENUM_H_
#define _LIBSTDHL_CPP_ENUM_H_

#include <libstdhl/Type>

#include <cassert>
#include <functional>
#include <initializer_list>
#include <type_traits>

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    /**
       @extends Stdhl
     */
    namespace Enum
    {
        /**
           @brief Provides a type-safe way of storing enum value combinations.
         */
        template < typename Enum >
        class Flags
        {
            using UnderlyingType = typename std::underlying_type< Enum >::type;

          public:
            static_assert( std::is_enum< Enum >::value, "Template parameter must be an enum" );

            /**
               No flags will be set initially.
             */
            Flags( void )
            : m_flags( 0 )
            {
            }

            /**
               Init flags from initializer list
             */
            Flags( std::initializer_list< Enum > flags )
            : Flags()
            {
                for( const auto flag : flags )
                {
                    set( flag );
                }
            }

            /**
               Sets the flag \a e on initialization, all other flags will be
               cleared.
             */
            explicit Flags( Enum e )
            : m_flags( asBitValue( e ) )
            {
            }

            /**
               Sets the flag \a e if it's not already set.
             */
            void set( Enum e )
            {
                m_flags |= asBitValue( e );
            }

            /**
               Clears the flag \a e if it's set.
             */
            void unset( Enum e )
            {
                m_flags &= ~asBitValue( e );
            }

            /**
               Sets the flag \a e if it's not already set, or clears it if it's
               set.
             */
            void flip( Enum e )
            {
                m_flags ^= asBitValue( e );
            }

            /**
               Checks whether the flag \a e is set.

               @return True if the flag is set, false otherwise.
             */
            u1 isSet( Enum e ) const
            {
                return ( m_flags & asBitValue( e ) ) != 0;
            }

            /**
               Clears all flags.
             */
            void clear( void )
            {
                m_flags = 0;
            }

            /**
               Checks whether one or more flags are set.

               @return True if no flag is set, false otherwise.
             */
            u1 empty( void ) const
            {
                return m_flags == 0;
            }

            /**
               Provides the ability to iterate and process each flag through callback
               function \a processFlag which can abort the iteration if it returns `false`.
             */
            void foreach( std::function< u1( const Enum ) > processFlag ) const
            {
                std::size_t position = 0;
                auto flags = m_flags;
                while( flags != 0 )
                {
                    if( flags & 0x1 )
                    {
                        if( not processFlag( static_cast< const Enum >( position ) ) )
                        {
                            break;
                        }
                    }
                    position++;
                    flags >>= 1;
                }
            }

            /**
               Checks whether the flag \a e is set.

               @return True if the flag is set, false otherwise.
             */
            u1 operator&( Enum e ) const
            {
                return isSet( e );
            }

            /**
               Sets the flag \a e if it's not already set.
             */
            Flags& operator|=( Enum e )
            {
                set( e );
                return *this;
            }

            /**
               Sets the flag \a e in \a flags if it's not already set.
             */
            friend Flags operator|( Flags flags, Enum e )
            {
                flags.set( e );
                return flags;
            }

            /**
               Combines \a lhs and \a rhs
             */
            friend Flags operator|( Flags lhs, Flags rhs )
            {
                lhs.m_flags |= rhs.m_flags;
                return lhs;
            }

            /**
               Intersection \a lhs and \a rhs
             */
            friend Flags operator*( Flags lhs, Flags rhs )
            {
                lhs.m_flags &= rhs.m_flags;
                return lhs;
            }

            /**
               Sets the flag \a e if it's not already set, or clears it if it's
               set.
             */
            Flags& operator^=( Enum e )
            {
                flip( e );
                return *this;
            }

            /**
               Sets the flag \a e in \a flags if it's not already set, or
               clears it in \a flags if it's set.
             */
            friend Flags operator^( Flags flags, Enum e )
            {
                flags.flip( e );
                return flags;
            }

          private:
            static UnderlyingType asBitValue( Enum e )
            {
                assert( static_cast< UnderlyingType >( e ) < ( sizeof( UnderlyingType ) * 8 ) );
                return 1 << static_cast< UnderlyingType >( e );
            }

          private:
            UnderlyingType m_flags;
        };
    }
}

template < typename T >
typename std::enable_if< std::is_enum< T >::value, libstdhl::Enum::Flags< T > >::type operator|(
    T lhs, T rhs )
{
    return libstdhl::Enum::Flags< T >( lhs ) | libstdhl::Enum::Flags< T >( rhs );
}

template < typename T >
typename std::enable_if< std::is_enum< T >::value, libstdhl::Enum::Flags< T > >::type operator*(
    T lhs, T rhs )
{
    return libstdhl::Enum::Flags< T >( lhs ) * libstdhl::Enum::Flags< T >( rhs );
}

#endif  // _LIBSTDHL_CPP_ENUM_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
