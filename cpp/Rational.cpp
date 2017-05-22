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

#include "Rational.h"

#include "Integer.h"
#include "String.h"

using namespace libstdhl;

Rational::Rational( const std::string& value, const Type::Radix radix )
: Type()
{
    std::vector< std::string > parts;

    String::split( value, "/", parts );

    if( parts.size() == 0 )
    {
        throw std::invalid_argument(
            "value '" + value + "' seems to be an invalid Rational literal" );
    }
    else if( parts.size() > 2 )
    {
        throw std::domain_error(
            "value '" + value
            + "' too many Rational '/' characters found in literal" );
    }

    const auto numerator = Integer( parts[ 0 ], radix );

    m_meta = numerator.size();

    numerator.foreach( [&]( const std::size_t index, const u64 value ) {
        if( index < 2 )
        {
            m_words[ index ] = value;
        }
        else
        {
            m_words_ext.emplace_back( value );
        }
    } );

    if( parts.size() > 1 )
    {
        const auto denominator = Integer( parts[ 1 ], radix );

        denominator.foreach( [&]( const std::size_t index, const u64 value ) {
            const auto pos = m_meta + index;

            if( pos < 2 )
            {
                m_words[ pos ] = value;
            }
            else
            {
                m_words_ext.emplace_back( value );
            }
        } );
    }
}

Rational::Rational( const Integer& numerator, const Integer& denominator )
: Type()
{
    if( static_cast< const Type& >( denominator ) == 0 )
    {
        throw std::domain_error( "denominator of Rational is zero" );
    }

    m_meta = numerator.size();

    numerator.foreach( [&]( const std::size_t index, const u64 value ) {
        if( index < 2 )
        {
            m_words[ index ] = value;
        }
        else
        {
            m_words_ext.emplace_back( value );
        }
    } );

    denominator.foreach( [&]( const std::size_t index, const u64 value ) {

        const auto pos = m_meta + index;

        if( pos < 2 )
        {
            m_words[ pos ] = value;
        }
        else
        {
            m_words_ext.emplace_back( value );
        }
    } );
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
