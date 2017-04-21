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

#ifndef _LIB_STDHL_CPP_FILE_H_
#define _LIB_STDHL_CPP_FILE_H_

#include "Default.h"
#include "Type.h"

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    /**
       @extends Stdhl
    */
    class File
    {
      public:
        static std::fstream open( const std::string& filename,
            const std::ios_base::openmode mode = std::fstream::in )
        {
            std::fstream file;

            file.open( filename, mode );

            if( not exists( file ) )
            {
                throw std::invalid_argument(
                    "filename '" + filename + "' does not exist" );
            }

            return file;
        }

        static void remove( const std::string& filename )
        {
            if( not exists( filename ) )
            {
                throw std::invalid_argument(
                    "removing file '" + filename
                    + "' failed, because it does not exist" );
            }

            auto result = std::remove( filename.c_str() );

            if( result != 0 )
            {
                throw std::invalid_argument( "removing file '" + filename
                                             + "' failed (error "
                                             + std::to_string( result )
                                             + ")" );
            }

            assert( not exists( filename ) );
        }

        static u1 exists( const std::string& filename )
        {
            try
            {
                open( filename );
            }
            catch( const std::invalid_argument& e )
            {
                return false;
            }

            return true;
        }

        static u1 exists( const std::fstream& file )
        {
            return file.is_open();
        }

        static u8 readLines( const std::string& filename,
            std::function< void( u32, const std::string& ) >
                process_line )
        {
            u32 cnt = 0;
            std::string line;
            std::fstream fd( filename );

            if( not exists( fd ) )
            {
                return -1;
            }

            while( std::getline( fd, line ) )
            {
                process_line( cnt, line );
                cnt++;
            }

            fd.close();
            return 0;
        }

        static std::string readLine(
            const std::string& filename, const u32 num )
        {
            std::string line;

            auto file = open( filename );

            gotoLine( file, num );

            std::getline( file, line );

            return line;
        }

        static std::fstream& gotoLine(
            std::fstream& file, const std::size_t num )
        {
            file.seekg( std::ios::beg );

            for( std::size_t c = 0; c < ( num - 1 ); c++ )
            {
                file.ignore(
                    std::numeric_limits< std::streamsize >::max(), '\n' );
            }

            return file;
        }
    };
}

#endif // _LIB_STDHL_CPP_FILE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
