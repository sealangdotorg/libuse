//
//  Copyright (c) 2014-2017 CASM Organization https://casm-lang.org
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
//                https://github.com/casm-lang/libstdhl
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

#include "rfc3986.h"

#include "../String.h"

#include <cassert>
#include <regex>

/**
   @brief    TBD

   TBD
*/

using namespace libstdhl;
using namespace Standard;
using namespace RFC3986;

UniformResourceIdentifier::UniformResourceIdentifier(
    const std::string& scheme,
    const std::string& authority,
    const std::string& path,
    const std::string& query,
    const std::string& fragment )
: m_scheme( scheme )
, m_authority( authority )
, m_path( path )
, m_query( query )
, m_fragment( fragment )
{
}

const std::string& UniformResourceIdentifier::scheme( void ) const
{
    return m_scheme;
}

const std::string& UniformResourceIdentifier::authority( void ) const
{
    return m_authority;
}

const std::string& UniformResourceIdentifier::path( void ) const
{
    return m_path;
}

const std::string& UniformResourceIdentifier::query( void ) const
{
    return m_query;
}

const std::string& UniformResourceIdentifier::fragment( void ) const
{
    return m_fragment;
}

std::string UniformResourceIdentifier::toString( void ) const
{
    std::string tmp = "";

    if( scheme().size() > 0 )
    {
        tmp += scheme() + "://" + authority();
    }

    tmp += path();

    if( query().size() > 0 )
    {
        tmp += "?" + query();
    }

    if( fragment().size() > 0 )
    {
        tmp += "#" + fragment();
    }

    return tmp;
}

UniformResourceIdentifier UniformResourceIdentifier::fromString( const std::string& uri )
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
        throw std::invalid_argument( "invalid URI to parse from string '" + uri + "'" );
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

    return UniformResourceIdentifier( scheme, authority, path, query, fragment );
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
