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

        //   private:
        //     Log();

        //     void log( Source& source, Channel& channel,
        //         const std::vector< Sink* >& sinks, const char* format,
        //         va_list args ) const
        //     {
        //         char buffer[ 1024 ];

        //         vsprintf( buffer, format, args );

        //         for( auto sink : sinks )
        //         {
        //             assert( sink );
        //             fprintf( sink->get(), "%s: %s: %s\n", source.get(),
        //                 channel.get(), buffer );
        //         }
        //     };

        //     static Log& instance( void )
        //     {
        //         static Log singelton;
        //         return singelton;
        //     };

        //   public:
        //     static void info( const char* format, ... )
        //     {
        //         va_list args;
        //         va_start( args, format );
        //         instance().log(
        //             DefaultSource, Info, { &DefaultSink }, format, args );
        //         va_end( args );
        //     };

        //     static void warning( const char* format, ... )
        //     {
        //         va_list args;
        //         va_start( args, format );
        //         instance().log(
        //             DefaultSource, Warning, { &DefaultSink }, format, args );
        //         va_end( args );
        //     };

        // };
        // }
    }
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
