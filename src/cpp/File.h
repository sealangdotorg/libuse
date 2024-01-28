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
#ifndef _LIBSTDHL_CPP_FILE_H_
#define _LIBSTDHL_CPP_FILE_H_

#include <libstdhl/Exception>
#include <libstdhl/Type>

#include <cassert>
#include <fstream>
#include <functional>
#include <limits>

#include <sys/stat.h>
#include <unistd.h>

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    /**
       @extends Stdhl
    */

    class FileNumberOutOfRangeException : public Exception
    {
      public:
        using Exception::Exception;
    };

    namespace File
    {
        std::fstream open(
            const std::string& filename, const std::ios_base::openmode mode = std::fstream::in );

        u1 exists( const std::string& filename );

        u1 exists( const std::fstream& file );

        void remove( const std::string& filename );

        u8 readLines(
            const std::string& filename,
            std::function< void( u32, const std::string& ) > process_line );

        std::string readLine( const std::string& filename, const u32 num );

        std::fstream& gotoLine( std::fstream& file, const std::size_t num );

        namespace Path
        {
            void create( const std::string& path );

            u1 exists( const std::string& path );

            void remove( const std::string& path );

            std::string temporary( void );
        }
    }
}

#endif  // _LIBSTDHL_CPP_FILE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
