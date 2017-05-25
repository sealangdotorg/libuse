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

#include "../String.h"

/**
   @brief    TBD

   TBD
*/

using namespace libstdhl;
using namespace Standard;
using namespace RFC3986;

UniformResourceIdentifier UniformResourceIdentifier::parse(
    const std::string& uri )
{
    std::regex uri_regex(
        // 1: scheme
        "(?:([[:alpha:]](?:[[:alnum:]]|[\\+]|[\\-]|[\\.])*)\\:)?"
        // 2: authority
        "(?://([^/\\?\\#]*))?"
        // 3: path
        "([\\S][^\\?\\#]*)"
        // 4: query
        "(?:/[\\?]([^\\#]*))?"
        // 5: fragment
        "(?:[\\#]([\\S]*))?" );

    std::sregex_iterator uri_start( uri.begin(), uri.end(), uri_regex );
    std::sregex_iterator uri_end;

    if( uri_start == uri_end )
    {
        throw std::invalid_argument(
            "invalid URI to parse from string '" + uri + "'" );
    }

    auto match = *uri_start;
    assert( match.size() >= 5 );

    const auto& scheme = match[ 1 ].str();
    const auto& authority = match[ 2 ].str();
    const auto& path = match[ 3 ].str();
    const auto& query = match[ 4 ].str();
    const auto& fragment = match[ 5 ].str();

    std::regex path_regex( ".*([\\s]+).*" );
    std::sregex_iterator path_start( path.begin(), path.end(), path_regex );
    std::sregex_iterator path_end;

    if( path_start != path_end )
    {
        throw std::invalid_argument(
            "invalid URI path '" + path + "' found in string '" + uri + "'" );
    }

    return UniformResourceIdentifier(
        scheme, authority, path, query, fragment );
}

UniformResourceIdentifier::UniformResourceIdentifier( const std::string& scheme,
    const std::string& authority,
    const std::string& path,
    const std::string& query,
    const std::string& fragment )
: std::string( "" )
, m_scheme( scheme )
, m_authority( authority )
, m_path( path )
, m_query( query )
, m_fragment( fragment )
{
    if( scheme.size() > 0 )
    {
        *this += scheme + "://" + authority;
    }

    *this += path;

    if( query.size() > 0 )
    {
        *this += "?" + query;
    }

    if( fragment.size() > 0 )
    {
        *this += "#" + fragment;
    }
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
