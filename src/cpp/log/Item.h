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

#ifndef _LIBSTDHL_CPP_LOG_ITEM_H_
#define _LIBSTDHL_CPP_LOG_ITEM_H_

#include <libstdhl/List>
#include <libstdhl/Type>

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    /**
       @extends Stdhl
    */
    namespace Log
    {
        class Formatter;

        /**
           @extends Log
        */
        class Item
        {
          public:
            using Ptr = std::shared_ptr< Item >;

            enum class ID
            {
                TEXT,
                POSITION,
                RANGE,
                LOCATION,

                DATA,
                LEVEL,
                SOURCE,
                CATEGORY,
                TIMESTAMP,
                CHRONOGRAPH,
            };

            Item( const ID id );

            ID id( void ) const;

            virtual std::string accept( Formatter& formatter ) = 0;

          private:
            ID m_id;
        };

        using Items = List< Item >;

        class TextItem : public Item
        {
          public:
            TextItem( const std::string& text );

            std::string text( void ) const;

          private:
            std::string m_text;

          public:
            std::string accept( Formatter& formatter ) override;
        };

        class PositionItem : public Item
        {
          public:
            PositionItem( const u64 line, const u64 column );

            u64 line( void ) const;

            u64 column( void ) const;

          private:
            u64 m_line;
            u64 m_column;

          public:
            std::string accept( Formatter& formatter ) override;
        };

        class RangeItem : public Item
        {
          public:
            RangeItem( const PositionItem& begin, const PositionItem& end );

            PositionItem begin( void ) const;

            PositionItem end( void ) const;

          private:
            PositionItem m_begin;
            PositionItem m_end;

          public:
            std::string accept( Formatter& formatter ) override;
        };

        class LocationItem : public Item
        {
          public:
            LocationItem( const TextItem& filename, const RangeItem& range );

            LocationItem(
                const std::string& filename,
                const u64 beginLine,
                const u64 beginColumn,
                const u64 endLine,
                const u64 endColumn );

            TextItem filename( void ) const;

            RangeItem range( void ) const;

          private:
            TextItem m_filename;
            RangeItem m_range;

          public:
            std::string accept( Formatter& formatter ) override;
        };
    }
}

#endif  // _LIBSTDHL_CPP_LOG_ITEM_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
