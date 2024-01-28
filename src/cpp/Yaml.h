//
//  Copyright (C) 2014-2024 CASM Organization <https://casm-lang.org>
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

#pragma once
#ifndef _LIBSTDHL_CPP_YAML_H_
#define _LIBSTDHL_CPP_YAML_H_

#include <libstdhl/Exception>
#include <libstdhl/Optional>
#include <libstdhl/Type>
#include <libstdhl/vendor/mini-yaml/Yaml>

#include <functional>
#include <memory>

/**
   @brief    C++ YAML Wrapper

   https://github.com/jimmiebergmann/mini-yaml
*/

namespace libstdhl
{
    /**
       @extends Stdhl
    */
    namespace Yaml
    {
        enum class Type
        {
            NONE,
            MAP,
            SEQUENCE,
            BOOLEAN,
            INTEGER,
            STRING,
        };

        class Exception : public libstdhl::Exception
        {
          public:
            using libstdhl::Exception::Exception;
        };

        class MapHasNoKeyException : public Exception
        {
          public:
            using Exception::Exception;
        };

        class SequenceHasNoIndexException : public Exception
        {
          public:
            using Exception::Exception;
        };

        class Content : private ::Yaml::Node
        {
          public:
            using Ptr = std::shared_ptr< Content >;

            using Result = libstdhl::Optional< Content* >;

            using Value = libstdhl::Optional< Content >;

            using iterator = ::Yaml::Iterator;

            using const_iterator = ::Yaml::ConstIterator;

            explicit Content( void );

            Content( const Content& ) = default;

            Content& operator[]( const std::string& mapKey ) const;

            Content& operator[]( const std::size_t sequenceIndex ) const;

            u1 has( const std::string& mapKey ) const;

            u1 has( const std::size_t sequenceIndex ) const;

            Result find( const std::string& mapKey ) const;

            Result find( const std::size_t sequenceIndex ) const;

            Value emplace(
                const std::string& mapKey, const Content& mapValue = libstdhl::Yaml::Content() );

            Value emplace(
                const Content& sequenceValue = libstdhl::Yaml::Content(),
                const std::size_t sequenceIndex = 0 );

            Content& emplace_back( const Content& sequenceValue = libstdhl::Yaml::Content() );

            void erase( const std::string& mapKey );

            void erase( const std::size_t sequenceIndex );

            void foreach( const std::function<
                          void( const std::string& mapKey, const Content& mapValue, u1& abort ) >
                              process ) const;

            template < typename T >
            T as() const
            {
                return As< T >();
            }

            iterator begin( void );

            const_iterator begin( void ) const;

            const_iterator cbegin( void ) const;

            iterator end( void );

            const_iterator end( void ) const;

            const_iterator cend( void ) const;

            libstdhl::Yaml::Type type( void ) const;

            std::string description( void ) const;

            std::size_t size( void ) const;

            std::string dump( void ) const;

            void dump( std::iostream& stream ) const;

            static Content fromString( const std::string& text );

            static Content fromStream( std::iostream& stream );
        };
    }
}

#endif  // _LIBSTDHL_CPP_YAML_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
