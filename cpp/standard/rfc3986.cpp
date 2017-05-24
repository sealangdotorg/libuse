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

#include "rfc3986.h"

/**
   @brief    TBD

   TBD
*/

using namespace libstdhl;
using namespace Standard;
using namespace RFC3986;

UniformResourceIdentifier::UniformResourceIdentifier( const std::string& uri )
: std::string( uri )
{
    std::regex pattern(
        // 1: scheme
        "(?:([[:alpha:]](?:[[:alnum:]]|[\\+]|[\\-]|[\\.])*)\\:)?"
        // 2: authority
        "(?://([^/\\?\\#]*))?"
        // 3: path
        "([^\\?\\#]*)"
        // 4: query
        "(?:/[\\?]([^\\#]*))?"
        // 5: fragment
        "(?:[\\#]([\\S]*))?" );

    std::sregex_iterator start( uri.begin(), uri.end(), pattern );
    std::sregex_iterator end;

    if( start == end )
    {
        throw std::invalid_argument(
            "invalid URI to parse from string '" + uri + "'" );
    }

    auto match = *start;
    assert( match.size() >= 5 );

    m_scheme = match[ 1 ].str();
    m_authority = match[ 2 ].str();
    m_path = match[ 3 ].str();
    m_query = match[ 4 ].str();
    m_fragment = match[ 5 ].str();
}

std::string UniformResourceIdentifier::scheme( void ) const
{
    return m_scheme;
}

std::string UniformResourceIdentifier::authority( void ) const
{
    return m_authority;
}

std::string UniformResourceIdentifier::path( void ) const
{
    return m_path;
}

std::string UniformResourceIdentifier::query( void ) const
{
    return m_query;
}

std::string UniformResourceIdentifier::fragment( void ) const
{
    return m_fragment;
}

std::string UniformResourceIdentifier::uri( void ) const
{
    return *this;
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
