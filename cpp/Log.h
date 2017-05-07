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

#ifndef _LIB_STDHL_CPP_LOG_H_
#define _LIB_STDHL_CPP_LOG_H_

#include "Default.h"
#include "Type.h"

/**
   @brief    TODO

   TODO
*/

#include "log/Category.h"
#include "log/Channel.h"
#include "log/Data.h"
#include "log/Filter.h"
#include "log/Formatter.h"
#include "log/Item.h"
#include "log/Level.h"
#include "log/Router.h"
#include "log/Sink.h"
#include "log/Source.h"
#include "log/Stream.h"
#include "log/Switch.h"
#include "log/Timestamp.h"

namespace libstdhl
{
    /**
       @extends Stdhl
    */

    namespace Log
    {
        Source::Ptr defaultSource( const Source::Ptr& source = nullptr );

        void log( Level::ID level, const std::string& text );

        void error( const char* format, ... );

        void warning( const char* format, ... );

        void info( const char* format, ... );
    }

    /**
       @extends Stdhl
    */

    class Logger
    {
      public:
        Logger( Log::Stream& stream );

        void output( const std::string& text );
        void output( const char* format, ... );

        void error( const std::string& text );
        void error( const char* format, ... );
        u64 errors( void ) const;

        void warning( const std::string& text );
        void warning( const char* format, ... );
        u64 warnings( void ) const;

        void info( const std::string& text );
        void info( const char* format, ... );

        void hint( const std::string& text );
        void hint( const char* format, ... );

#ifndef NDEBUG
        void debug( const std::string& text );
        void debug( const char* format, ... );
#else
        inline void debug( const std::string& text )
        {
        }
        inline void debug( const char* format, ... )
        {
        }
#endif

        void c_log( Log::Level::ID level, const char* format, va_list args );

        template < typename... Args >
        void log( Args&&... args )
        {
            m_stream.add( std::forward< Args >( args )... );
            diagnostic( m_stream.data().back() );
        }

        template < const Log::Level::ID LEVEL, typename... Args >
        void log( Args&&... args )
        {
            m_stream.add( LEVEL, source(), category(),
                Log::Items( { std::forward< Args >( args )... } ) );
            diagnostic( m_stream.data().back() );
        }

        Log::Stream& stream( void );

        void setSource( const Log::Source::Ptr& source );

        Log::Source::Ptr source( void ) const;

        void setCategory( const Log::Category::Ptr& category );

        Log::Category::Ptr category( void ) const;

      private:
        void diagnostic( const Log::Data& data );

        Log::Stream& m_stream;
        Log::Source::Ptr m_source;
        Log::Category::Ptr m_category;
        u64 m_errors;
        u64 m_warnings;
    };
}

#endif // _LIB_STDHL_CPP_LOG_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
