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

#include "Json.h"

using namespace libstdhl;

u1 Json::hasProperty( const Object& object, const std::string& field )
{
    return object.find( field ) != object.end();
}

void Json::validateTypeIsObject( const std::string& context, const Object& object )
{
    if( not object.is_object() )
    {
        throw std::invalid_argument( context + " invalid data type, shall be 'object'" );
    }
}

void Json::validateTypeIsString( const std::string& context, const Object& object )
{
    if( not object.is_string() )
    {
        throw std::invalid_argument( context + " invalid data type, shall be 'string'" );
    }
}

void Json::validateTypeIsArray( const std::string& context, const Object& object )
{
    if( not object.is_array() )
    {
        throw std::invalid_argument( context + " invalid data type, shall be 'array'" );
    }
}

void Json::validatePropertyIs(
    const std::string& context,
    const Object& object,
    const std::string& field,
    const u1 required,
    const std::function< u1( const Object& property ) >& condition )
{
    if( hasProperty( object, field ) )
    {
        if( not condition( object[ field ] ) )
        {
            throw std::invalid_argument( context + " invalid property '" + field + "'" );
        }
    }
    else
    {
        if( required )
        {
            throw std::invalid_argument( context + " missing property '" + field + "'" );
        }
    }
}

void Json::validatePropertyIsObject(
    const std::string& context, const Object& object, const std::string& field, const u1 required )
{
    validatePropertyIs( context, object, field, required, []( const Object& property ) -> u1 {
        return property.is_object();
    } );
}

void Json::validatePropertyIsArray(
    const std::string& context, const Object& object, const std::string& field, const u1 required )
{
    validatePropertyIs( context, object, field, required, []( const Object& property ) -> u1 {
        return property.is_array();
    } );
}

void Json::validatePropertyIsString(
    const std::string& context, const Object& object, const std::string& field, const u1 required )
{
    validatePropertyIs( context, object, field, required, []( const Object& property ) -> u1 {
        return property.is_string();
    } );
}

void Json::validatePropertyIsNumber(
    const std::string& context, const Object& object, const std::string& field, const u1 required )
{
    validatePropertyIs( context, object, field, required, []( const Object& property ) -> u1 {
        return property.is_number();
    } );
}

void Json::validatePropertyIsNumberOrNull(
    const std::string& context, const Object& object, const std::string& field, const u1 required )
{
    validatePropertyIs( context, object, field, required, []( const Object& property ) -> u1 {
        return property.is_number() or property.is_null();
    } );
}

void Json::validatePropertyIsBoolean(
    const std::string& context, const Object& object, const std::string& field, const u1 required )
{
    validatePropertyIs( context, object, field, required, []( const Object& property ) -> u1 {
        return property.is_boolean();
    } );
}

void Json::validatePropertyIsUuid(
    const std::string& context, const Object& object, const std::string& field, const u1 required )
{
    validatePropertyIs( context, object, field, required, []( const Object& property ) -> u1 {
        return property.is_string() or property.is_number();
    } );
}

void Json::validatePropertyIsUri(
    const std::string& context, const Object& object, const std::string& field, const u1 required )
{
    validatePropertyIsString( context, object, field, required );
    try
    {
        libstdhl::Standard::RFC3986::URI::fromString( object[ field ].get< std::string >() );
    }
    catch( const std::exception& e )
    {
        throw std::invalid_argument( context + " DocumentUri: " + e.what() );
    }
}

void Json::validatePropertyIsUriOrNull(
    const std::string& context, const Object& object, const std::string& field, const u1 required )
{
    validatePropertyIs( context, object, field, required, []( const Object& property ) -> u1 {
        return property.is_string() or property.is_null();
    } );
    if( hasProperty( object, field ) and object[ field ].is_string() )
    {
        validatePropertyIsUri( context, object, field, required );
    }
}

void Json::validatePropertyIsArrayOfString(
    const std::string& context, const Object& object, const std::string& field, const u1 required )
{
    validatePropertyIsArray( context, object, field, required );
    if( hasProperty( object, field ) )
    {
        for( auto element : object[ field ] )
        {
            validateTypeIsString( context, element );
        }
    }
}

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
