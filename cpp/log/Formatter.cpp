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

#include "Formatter.h"

#include "Data.h"

using namespace libstdhl;
using namespace Log;

//
// StringFormatter
//

std::string StringFormatter::visit( Timestamp& item )
{
    return item.local();
}

std::string StringFormatter::visit( Chronograph& item )
{
    return std::to_string( item.duration< std::chrono::nanoseconds >().count() )
           + "ns";
}

std::string StringFormatter::visit( Source& item )
{
    return item.name();
}

std::string StringFormatter::visit( Category& item )
{
    return item.name();
}

std::string StringFormatter::visit( Level& item )
{
    switch( item.id() )
    {
        case Level::EMERGENCY:
        {
            return "emergency";
        }
        case Level::ALERT:
        {
            return "alert";
        }
        case Level::CRITICAL:
        {
            return "critical";
        }
        case Level::ERROR:
        {
            return "error";
        }
        case Level::WARNING:
        {
            return "warning";
        }
        case Level::NOTICE:
        {
            return "";
        }
        case Level::INFORMATIONAL:
        {
            return "info";
        }
        case Level::DEBUG:
        {
            return "debug";
        }
    }
}

std::string StringFormatter::visit( Data& item )
{
    std::string tmp = "";

    tmp += "[" + item.timestamp().accept( *this ) + "] ";

    tmp += item.source()->accept( *this ) + ": ";

    tmp += item.category()->accept( *this ) + ": ";

    tmp += item.level().accept( *this ) + ": ";

    u1 first = true;

    for( auto i : item.items() )
    {
        tmp += ( first ? "" : ", " ) + i->accept( *this );

        first = false;
    }

    return tmp;
}

std::string StringFormatter::visit( TextItem& item )
{
    return item.text();
}

std::string StringFormatter::visit( PositionItem& item )
{
    return std::to_string( item.line() ) + ":"
           + std::to_string( item.column() );
}

std::string StringFormatter::visit( RangeItem& item )
{
    return item.begin().accept( *this ) + ".." + item.end().accept( *this );
}

std::string StringFormatter::visit( LocationItem& item )
{
    return item.filename().accept( *this ) + ":" + item.range().accept( *this );
}

//
// ConsoleFormatter
//

std::string ConsoleFormatter::visit( Data& item )
{
    std::string tmp = "";

    tmp += item.source()->accept( *this ) + ": ";

    tmp += item.level().accept( *this ) + ": ";

    u1 first = true;

    for( auto i : item.items() )
    {
        tmp += ( first ? "" : "\n" ) + i->accept( *this );

        first = false;
    }

    return tmp;
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
