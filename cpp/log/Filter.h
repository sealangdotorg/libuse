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

#ifndef _LIB_STDHL_CPP_LOG_FILTER_H_
#define _LIB_STDHL_CPP_LOG_FILTER_H_

#include "Channel.h"

#include "Category.h"
#include "Level.h"
#include "Source.h"

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
        class Filter final : public Channel
        {
          public:
            Filter( Channel& channel );

            void clear( void );

            void setInverse( u1 inverse );

            u1 inverse( void ) const;

            void setSource( const Source::Ptr& source );

            void setCategory( const Category::Ptr& category );

            void setLevel( Level level );

            void setLevel( Level::ID level );

          private:
            Channel& m_channel;

            u1 m_inverse;

            Sources m_sources;

            Categories m_categories;

            Levels m_levels;

          public:
            void process( Stream& stream ) override;
        };
    }
}

#endif // _LIB_STDHL_CPP_LOG_FILTER_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
