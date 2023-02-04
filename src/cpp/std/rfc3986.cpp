//
//  Copyright (C) 2014-2023 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber et al.
//  <https://github.com/casm-lang/libstdhl/graphs/contributors>
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

#include <libstdhl/String>

#include <algorithm>
#include <functional>

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
    const std::string delimiter( "://" );

    std::string scheme;
    std::string authority;
    std::string path;
    std::string query;
    std::string fragment;

    std::string::const_iterator scheme_iterator =
        search( uri.begin(), uri.end(), delimiter.begin(), delimiter.end() );

    scheme.reserve( distance( uri.begin(), scheme_iterator ) );

    transform(
        uri.begin(),
        scheme_iterator,
        back_inserter( scheme ),
        std::ptr_fun< int, int >( tolower ) );

    if( scheme_iterator == uri.end() )
    {
        throw std::invalid_argument( "invalid URI '" + uri + "' to parse" );
    }

    advance( scheme_iterator, delimiter.length() );

    std::string::const_iterator path_iterator = find( scheme_iterator, uri.end(), '/' );

    authority.reserve( distance( scheme_iterator, path_iterator ) );

    transform(
        scheme_iterator,
        path_iterator,
        back_inserter( authority ),
        std::ptr_fun< int, int >( tolower ) );

    std::string::const_iterator query_iterator = find( path_iterator, uri.end(), '?' );

    path.assign( path_iterator, query_iterator );

    std::string::const_iterator fragment_iterator = find( query_iterator, uri.end(), '#' );

    if( query_iterator != uri.end() )
    {
        query_iterator++;
    }

    query.assign( query_iterator, fragment_iterator );

    if( fragment_iterator != uri.end() )
    {
        fragment_iterator++;
    }

    fragment.assign( fragment_iterator, uri.end() );

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
