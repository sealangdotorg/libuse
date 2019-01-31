//
//  Copyright (C) 2014-2018 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
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
#ifndef _LIBSTDHL_CPP_JSON_H_
#define _LIBSTDHL_CPP_JSON_H_

#include <libstdhl/Type>
#include <libstdhl/std/rfc3986>
#include <libstdhl/vendor/json/json>

/**
   @brief    C++ JSON Wrapper

   https://github.com/nlohmann/json
*/

namespace libstdhl
{
    /**
       @extends Stdhl
    */
    namespace Json
    {
        using Object = nlohmann::json;

        u1 hasProperty( const Object& object, const std::string& field );

        void validateTypeIsObject( const std::string& context, const Object& object );

        void validateTypeIsString( const std::string& context, const Object& object );

        void validateTypeIsArray( const std::string& context, const Object& object );

        void validatePropertyIs(
            const std::string& context,
            const Object& object,
            const std::string& field,
            const u1 required,
            const std::function< u1( const Object& property ) >& condition );

        void validatePropertyIsObject(
            const std::string& context,
            const Object& object,
            const std::string& field,
            const u1 required );

        void validatePropertyIsArray(
            const std::string& context,
            const Object& object,
            const std::string& field,
            const u1 required );

        void validatePropertyIsString(
            const std::string& context,
            const Object& object,
            const std::string& field,
            const u1 required );

        void validatePropertyIsNumber(
            const std::string& context,
            const Object& object,
            const std::string& field,
            const u1 required );

        void validatePropertyIsNumberOrNull(
            const std::string& context,
            const Object& object,
            const std::string& field,
            const u1 required );

        void validatePropertyIsBoolean(
            const std::string& context,
            const Object& object,
            const std::string& field,
            const u1 required );

        void validatePropertyIsUuid(
            const std::string& context,
            const Object& object,
            const std::string& field,
            const u1 required );

        void validatePropertyIsUri(
            const std::string& context,
            const Object& object,
            const std::string& field,
            const u1 required );

        void validatePropertyIsUriOrNull(
            const std::string& context,
            const Object& object,
            const std::string& field,
            const u1 required );

        template < class T >
        void validatePropertyIs(
            const std::string& context,
            const Object& object,
            const std::string& field,
            const u1 required )
        {
            validatePropertyIs(
                context, object, field, required, []( const Object& property ) -> u1 {
                    T::validate( property );
                    return true;
                } );
        };

        template < class T >
        void validatePropertyIsArrayOf(
            const std::string& context,
            const Object& object,
            const std::string& field,
            const u1 required )
        {
            validatePropertyIsArray( context, object, field, required );
            if( hasProperty( object, field ) )
            {
                for( auto element : object[ field ] )
                {
                    T::validate( element );
                }
            }
        };

        void validatePropertyIsArrayOfString(
            const std::string& context,
            const Object& object,
            const std::string& field,
            const u1 required );

        template < class T >
        void validateTypeIsArrayOf( const std::string& context, const Object& object )
        {
            validateTypeIsArray( context, object );
            for( auto element : object )
            {
                T::validate( element );
            }
        };

        template < class T, class U >
        void validateTypeIsMixedArrayOf( const std::string& context, const Object& object )
        {
            validateTypeIsArray( context, object );
            for( auto element : object )
            {
                try
                {
                    T::validate( element );
                }
                catch( std::invalid_argument a )
                {
                    U::validate( element );
                }
            }
        };

        template < class T, class U >
        void validateTypeIsArrayOf( const std::string& context, const Object& object )
        {
            try
            {
                validateTypeIsArrayOf< T >( context, object );
            }
            catch( std::invalid_argument a )
            {
                validateTypeIsArrayOf< U >( context, object );
            }
        };
    };
}

#endif  // _LIBSTDHL_CPP_JSON_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
