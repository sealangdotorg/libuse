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

#ifndef _LIB_STDHL_CPP_LOG_FORMATTER_H_
#define _LIB_STDHL_CPP_LOG_FORMATTER_H_

#include "../Default.h"
#include "../Type.h"

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
        class Timestamp;
        class Source;
        class Category;
        class Level;
        class Data;

        class TextItem;
        class PositionItem;
        class RangeItem;
        class LocationItem;

        /**
           @extends Log
        */
        class Formatter
        {
          public:
            virtual std::string visit( Timestamp& item ) = 0;
            virtual std::string visit( Source& item ) = 0;
            virtual std::string visit( Category& item ) = 0;
            virtual std::string visit( Level& item ) = 0;
            virtual std::string visit( Data& item ) = 0;

            virtual std::string visit( TextItem& item ) = 0;
            virtual std::string visit( PositionItem& item ) = 0;
            virtual std::string visit( RangeItem& item ) = 0;
            virtual std::string visit( LocationItem& item ) = 0;
        };

        class StringFormatter : public Formatter
        {
          public:
            std::string visit( Timestamp& item ) override;
            std::string visit( Source& item ) override;
            std::string visit( Category& item ) override;
            std::string visit( Level& item ) override;
            std::string visit( Data& item ) override;

            std::string visit( TextItem& item ) override;
            std::string visit( PositionItem& item ) override;
            std::string visit( RangeItem& item ) override;
            std::string visit( LocationItem& item ) override;
        };

        class ConsoleFormatter : public StringFormatter
        {
          public:
            std::string visit( Data& item ) override;
        };
    }
}

#endif // _LIB_STDHL_CPP_LOG_FORMATTER_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
