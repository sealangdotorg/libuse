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

#ifndef _LIB_STDHL_CPP_STANDARD_RFC3986_H_
#define _LIB_STDHL_CPP_STANDARD_RFC3986_H_

#include "../Type.h"

/**
   @brief    TBD

   TBD
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

            class UniformResourceIdentifier : public std::string
            {
              public:
                UniformResourceIdentifier( const std::string& uri );

                std::string scheme( void ) const;

                std::string authority( void ) const;

                std::string path( void ) const;

                std::string query( void ) const;

                std::string fragment( void ) const;

                std::string uri( void ) const;

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

#endif // _LIB_STDHL_CPP_STANDARD_RFC3986_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
