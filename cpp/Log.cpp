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

#include "Log.h"

using namespace libstdhl;
using namespace Log;

Source::Ptr libstdhl::Log::defaultSource( const Source::Ptr& source )
{
    static Source::Ptr cache = nullptr;

    if( source )
    {
        cache = source;
    }

    if( not cache )
    {
        cache = Source::defaultSource();
    }

    return cache;
}

void libstdhl::Log::log( Level::ID level, const std::string& text )
{
    Log::Stream c;

    c.add( level, text );

    Log::ConsoleFormatter f;

    Log::OutputStreamSink s( std::cerr, f );

    c.flush( s );
}

static void va_log( Level::ID level, const char* format, va_list args )
{
    char buffer[ 4096 ];
    vsprintf( buffer, format, args );
    log( level, std::string( buffer ) );
}

void libstdhl::Log::error( const char* format, ... )
{
    va_list args;
    va_start( args, format );
    va_log( Level::ERROR, format, args );
    va_end( args );
}

void libstdhl::Log::warning( const char* format, ... )
{
    va_list args;
    va_start( args, format );
    va_log( Level::WARNING, format, args );
    va_end( args );
}

void libstdhl::Log::info( const char* format, ... )
{
    va_list args;
    va_start( args, format );
    va_log( Level::INFORMATIONAL, format, args );
    va_end( args );
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
