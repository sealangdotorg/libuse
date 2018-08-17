//
//  Copyright (C) 2014-2018 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
//                <https://github.com/casm-lang/libstdhl>
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

#include "Logger.h"

#include <libstdhl/Log>

#include <cassert>
#include <cstdarg>
#include <iostream>

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

    Log::StringFormatter f;
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
    va_log( Level::ID::ERROR, format, args );
    va_end( args );
}

void libstdhl::Log::warning( const char* format, ... )
{
    va_list args;
    va_start( args, format );
    va_log( Level::ID::WARNING, format, args );
    va_end( args );
}

void libstdhl::Log::info( const char* format, ... )
{
    va_list args;
    va_start( args, format );
    va_log( Level::ID::INFORMATIONAL, format, args );
    va_end( args );
}

//
// Logger
//

Logger::Logger( libstdhl::Log::Stream& stream )
: m_stream( stream )
, m_source( Source::defaultSource() )
, m_category( Category::defaultCategory() )
, m_errors( 0 )
, m_warnings( 0 )
{
}

void Logger::output( const std::string& text )
{
    log( Log::Level::ID::OUTPUT, m_source, m_category, text );
}

void Logger::output( const char* format, ... )
{
    va_list args;
    va_start( args, format );
    c_log( Log::Level::ID::OUTPUT, format, args );
    va_end( args );
}

void Logger::error( const std::string& text )
{
    log( Log::Level::ID::ERROR, m_source, m_category, text );
}

void Logger::error( const char* format, ... )
{
    va_list args;
    va_start( args, format );
    c_log( Log::Level::ID::ERROR, format, args );
    va_end( args );
}

u64 Logger::errors( void ) const
{
    return m_errors;
}

void Logger::warning( const std::string& text )
{
    log( Log::Level::ID::WARNING, m_source, m_category, text );
}

void Logger::warning( const char* format, ... )
{
    va_list args;
    va_start( args, format );
    c_log( Log::Level::ID::WARNING, format, args );
    va_end( args );
}

u64 Logger::warnings( void ) const
{
    return m_warnings;
}

void Logger::info( const std::string& text )
{
    log( Log::Level::ID::INFORMATIONAL, m_source, m_category, text );
}

void Logger::info( const char* format, ... )
{
    va_list args;
    va_start( args, format );
    c_log( Log::Level::ID::INFORMATIONAL, format, args );
    va_end( args );
}

void Logger::hint( const std::string& text )
{
    log( Log::Level::ID::NOTICE, m_source, m_category, text );
}

void Logger::hint( const char* format, ... )
{
    va_list args;
    va_start( args, format );
    c_log( Log::Level::ID::NOTICE, format, args );
    va_end( args );
}

#ifndef NDEBUG
void Logger::debug( const std::string& text )
{
    log( Log::Level::ID::DEBUG, m_source, m_category, text );
}

void Logger::debug( const char* format, ... )
{
    va_list args;
    va_start( args, format );
    c_log( Log::Level::ID::DEBUG, format, args );
    va_end( args );
}
#endif

void Logger::c_log( Log::Level::ID level, const char* format, va_list args )
{
    char buffer[ 4096 ];
    vsprintf( buffer, format, args );
    log( level, m_source, m_category, std::string( buffer ) );
}

Log::Stream& Logger::stream( void )
{
    return m_stream;
}

void Logger::setSource( const Log::Source::Ptr& source )
{
    assert( source );
    m_source = source;
}

Source::Ptr Logger::source( void ) const
{
    return m_source;
}

void Logger::setCategory( const Log::Category::Ptr& category )
{
    m_category = category;
}

Category::Ptr Logger::category( void ) const
{
    return m_category;
}

void Logger::diagnostic( const Log::Data& data )
{
    if( data.level().id() == Log::Level::ID::ERROR )
    {
        m_errors++;
    }
    else if( data.level().id() == Log::Level::ID::WARNING )
    {
        m_warnings++;
    }
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
