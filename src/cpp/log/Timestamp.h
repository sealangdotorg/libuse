//
//  Copyright (c) 2014-2017 CASM Organization https://casm-lang.org
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
//                https://github.com/casm-lang/libstdhl
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

#ifndef _LIBSTDHL_CPP_LOG_TIMESTAMP_H_
#define _LIBSTDHL_CPP_LOG_TIMESTAMP_H_

#include <libstdhl/log/Item>

#include <chrono>

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
        class Timestamp final : public Item
        {
          public:
            using Ptr = std::shared_ptr< Timestamp >;

            Timestamp( void );

            std::chrono::system_clock::time_point timestamp( void ) const;

            std::time_t c_timestamp( void ) const;

            // format parameter
            // http://en.cppreference.com/w/cpp/io/manip/put_time
            std::string local( const std::string& format = "%c %Z" ) const;

            std::string utc( const std::string& format = "%c %Z" ) const;

            inline u1 operator==( const Timestamp& rhs ) const
            {
                if( this != &rhs )
                {
                    if( this->timestamp() != rhs.timestamp() )
                    {
                        return false;
                    }
                }
                return true;
            }

            inline u1 operator!=( const Timestamp& rhs ) const
            {
                return !operator==( rhs );
            }

            inline u1 operator<=( const Timestamp& rhs ) const
            {
                return this->timestamp() <= rhs.timestamp();
            }

            inline u1 operator<( const Timestamp& rhs ) const
            {
                return this->timestamp() < rhs.timestamp();
            }

            inline u1 operator>=( const Timestamp& rhs ) const
            {
                return not( *this < rhs );
            }

            inline u1 operator>( const Timestamp& rhs ) const
            {
                return not( *this <= rhs );
            }

          private:
            std::chrono::system_clock::time_point m_timestamp;

          public:
            std::string accept( Formatter& formatter ) override;
        };
    }
}

#endif  // _LIBSTDHL_CPP_LOG_TIMESTAMP_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
