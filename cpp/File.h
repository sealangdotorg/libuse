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
    class File
    {
      public:
        static u1 exists( const std::string& filename )
        {
            std::ifstream fd( filename );
            return (u1)fd;
        };

        static u8 readLines( const std::string& filename,
            std::function< void( u32, const std::string& ) >
                process_line )
        {
            u32 cnt = 0;
            std::string line;
            std::ifstream fd( filename );

            if( not fd )
            {
                return -1;
            }

            while( getline( fd, line ) )
            {
                process_line( cnt, line );
                cnt++;
            }

            fd.close();
            return 0;
        };
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
