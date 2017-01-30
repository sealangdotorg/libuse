//
//  Copyright (c) 2014-2017 Philipp Paulweber
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
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

Log::Channel::Channel( std::function< const char*( void* ) > description )
: m_description( description ){};

Log::Channel Log::Info
    = Log::Channel( []( void* arg ) -> const char* { return "info"; } );
Log::Channel Log::Warning
    = Log::Channel( []( void* arg ) -> const char* { return "warning"; } );
Log::Channel Log::Error
    = Log::Channel( []( void* arg ) -> const char* { return "error"; } );

Log::Source::Source( std::function< const char*( void* ) > description )
: m_description( description ){};

Log::Source Log::None
    = Log::Source( []( void* arg ) -> const char* { return 0; } );
Log::Source Log::DefaultSource
    = Log::Source( []( void* arg ) -> const char* { return "Log"; } );

Log::Sink::Sink( std::function< FILE*( void* ) > stream )
: m_stream( stream ){};

Log::Sink Log::StdOut
    = Log::Sink( []( void* arg ) -> FILE* { return stdout; } );
Log::Sink Log::StdErr
    = Log::Sink( []( void* arg ) -> FILE* { return stderr; } );
Log::Sink Log::DefaultSink = StdErr;

Log::Log()
{
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
