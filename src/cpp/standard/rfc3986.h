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

#ifndef _LIBSTDHL_CPP_STANDARD_RFC3986_H_
#define _LIBSTDHL_CPP_STANDARD_RFC3986_H_

#include <libstdhl/Type>

/**
   @brief    TBD

   TBD

   http://tools.ietf.org/html/rfc3986

   ~~~
   URI         = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

   hier-part   = "//" authority path-abempty
               / path-absolute
               / path-rootless
               / path-empty

       foo://example.com:8042/over/there?name=ferret#nose
       \_/   \______________/\_________/ \_________/ \__/
        |           |            |            |        |
     scheme     authority       path        query   fragment
        |   _____________________|__
       / \ /                        \
       urn:example:animal:ferret:nose
*/

namespace libstdhl
{
    namespace Standard
    {
        /**
           @extends Standard
        */
        namespace RFC3986
        {
            /**
               @extends RFC3986
            */
            class UniformResourceIdentifier
            {
              public:
                UniformResourceIdentifier( const std::string& scheme,
                    const std::string& m_authority,
                    const std::string& m_path,
                    const std::string& m_query,
                    const std::string& m_fragment );

                const std::string& scheme( void ) const;

                const std::string& authority( void ) const;

                const std::string& path( void ) const;

                const std::string& query( void ) const;

                const std::string& fragment( void ) const;

                std::string toString( void ) const;

                static UniformResourceIdentifier fromString(
                    const std::string& uri );

              private:
                std::string m_scheme;
                std::string m_authority;
                std::string m_path;
                std::string m_query;
                std::string m_fragment;
            };

            using URI = UniformResourceIdentifier;
        }
    }
}

#endif // _LIBSTDHL_CPP_STANDARD_RFC3986_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
