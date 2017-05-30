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

#include "../Ansi.h"
#include "../File.h"

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
        case Level::ID::EMERGENCY:
        {
            return "emergency";
        }
        case Level::ID::ALERT:
        {
            return "alert";
        }
        case Level::ID::CRITICAL:
        {
            return "critical";
        }
        case Level::ID::ERROR:
        {
            return "error";
        }
        case Level::ID::WARNING:
        {
            return "warning";
        }
        case Level::ID::NOTICE:
        {
            return "notice";
        }
        case Level::ID::INFORMATIONAL:
        {
            return "info";
        }
        case Level::ID::DEBUG:
        {
            return "debug";
        }
        case Level::ID::OUTPUT:
        {
            return "output";
        }
    }

    assert( !" internal error" );
    return "";
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
// ApplicationFormatter
//

ApplicationFormatter::ApplicationFormatter( const std::string& name )
: m_name( name )
, m_rawOutput( true )
, m_detailedLocation( true )
{
}

void ApplicationFormatter::setRawOutput( const u1 enable )
{
    m_rawOutput = enable;
}

void ApplicationFormatter::setDetailedLocation( const u1 enable )
{
    m_detailedLocation = enable;
}

std::string ApplicationFormatter::visit( Level& item )
{
    switch( item.id() )
    {
        case Level::ID::EMERGENCY:
        {
            return "emergency:";
        }
        case Level::ID::ALERT:
        {
            return "alert:";
        }
        case Level::ID::CRITICAL:
        {
            return "critical:";
        }
        case Level::ID::ERROR:
        {
            return Ansi::format< Ansi::Color::RED >( "error:" );
        }
        case Level::ID::WARNING:
        {
            return Ansi::format< Ansi::Color::MAGENTA >( "warning:" );
        }
        case Level::ID::NOTICE:
        {
            return "notice";
        }
        case Level::ID::INFORMATIONAL:
        {
            return Ansi::format< Ansi::Color::YELLOW >( "info:" );
        }
        case Level::ID::DEBUG:
        {
            return Ansi::format< Ansi::Color::CYAN >( "debug:" );
        }
        case Level::ID::OUTPUT:
        {
            return "output";
        }
    }

    assert( !" internal error" );
    return "";
}

std::string ApplicationFormatter::visit( Data& item )
{
    std::string tmp = m_name + ": ";

    tmp = Ansi::format< Ansi::Style::BOLD >(
              tmp + item.level().accept( *this ) )
          + " ";

    if( item.level() == Level::ID::OUTPUT and m_rawOutput )
    {
        tmp = "";
    }

    u1 first = true;

    for( auto i : item.items() )
    {
        if( i->id() == Item::ID::LOCATION )
        {
            continue;
        }

        tmp += ( first ? "" : ", " ) + i->accept( *this );

        first = false;
    }

#ifndef NDEBUG
    // auto tsp = item.timestamp().accept( *this );
    auto src = item.source()->accept( *this );
    auto cat = item.category()->accept( *this );

    tmp += Ansi::format< Ansi::Style::FAINT >(
        " [" + /*tsp +*/ src + ", " + cat + "]" );
#endif

    for( auto i : item.items() )
    {
        if( i->id() == Item::ID::LOCATION )
        {
            tmp += "\n" + i->accept( *this );

            if( not m_detailedLocation )
            {
                continue;
            }

            const auto& location = static_cast< const LocationItem& >( *i );

            std::string line = File::readLine(
                location.filename().text(), location.range().begin().line() );

            tmp += "\n" + Ansi::format< 192, 192, 192 >( line ) + "\n";
            tmp += std::string( location.range().begin().column() - 1, ' ' );

            std::string underline;
            if( ( location.range().begin().line()
                    == location.range().end().line() )
                and ( location.range().end().column()
                        > location.range().begin().column() ) )
            {
                underline = std::string( location.range().end().column()
                                             - location.range().begin().column()
                                             - 1,
                    '-' );
            }
            else
            {
                underline = std::string( line.size(), '-' ) + "...";
            }

            tmp += Ansi::format< Ansi::Color::GREEN >(
                Ansi::format< Ansi::Style::BOLD >( "^" ) + underline );
        }
    }

    return tmp;
}

std::string ApplicationFormatter::visit( LocationItem& item )
{
    return Ansi::format< Ansi::Style::BOLD >( StringFormatter::visit( item ) );
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
