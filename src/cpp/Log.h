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
#ifndef _LIBSTDHL_CPP_LOG_H_
#define _LIBSTDHL_CPP_LOG_H_

#include <libstdhl/Type>

#include <libstdhl/data/log/Category>
#include <libstdhl/data/log/Channel>
#include <libstdhl/data/log/Data>
#include <libstdhl/data/log/Filter>
#include <libstdhl/data/log/Formatter>
#include <libstdhl/data/log/Item>
#include <libstdhl/data/log/Level>
#include <libstdhl/data/log/Logger>
#include <libstdhl/data/log/Router>
#include <libstdhl/data/log/Sink>
#include <libstdhl/data/log/Source>
#include <libstdhl/data/log/Stream>
#include <libstdhl/data/log/Switch>
#include <libstdhl/data/log/Timestamp>

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
        Source::Ptr defaultSource( const Source::Ptr& source = nullptr );

        void log( Level::ID level, const std::string& text );

        void error( const char* format, ... );

        void warning( const char* format, ... );

        void info( const char* format, ... );
    }
}

#endif  // _LIBSTDHL_CPP_LOG_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
