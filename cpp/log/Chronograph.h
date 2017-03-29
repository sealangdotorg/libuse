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

#ifndef _LIB_STDHL_CPP_LOG_CHRONOGRAPH_H_
#define _LIB_STDHL_CPP_LOG_CHRONOGRAPH_H_

#include "Item.h"

#include "Formatter.h"

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
        class Chronograph final : public Item
        {
          public:
            using Ptr = std::shared_ptr< Chronograph >;

            Chronograph( u1 autostart = false );

            u1 running( void ) const;

            void start( void );

            void stop( void );

            void cont( void );

            template < typename T = std::chrono::nanoseconds >
            inline T duration( void ) const
            {
                if( running() )
                {
                    return std::chrono::high_resolution_clock::now() - m_start;
                }
                else
                {
                    return m_stop - m_start;
                }
            }

            operator std::string()
            {
                StringFormatter f;
                return this->accept( f );
            }

          private:
            u1 m_running;
            std::chrono::high_resolution_clock::time_point m_start;
            std::chrono::high_resolution_clock::time_point m_stop;

          public:
            std::string accept( Formatter& formatter ) override;

            friend std::ostream& operator<<(
                std::ostream& stream, Chronograph& obj )
            {
                stream << std::string( obj );
                return stream;
            }
        };
    }
}

#endif // _LIB_STDHL_CPP_LOG_CHRONOGRAPH_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
