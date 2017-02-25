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

#ifndef _LIB_STDHL_CPP_LABELING_H_
#define _LIB_STDHL_CPP_LABELING_H_

#include "Type.h"

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    class Labeling
    {
      private:
        std::shared_ptr< std::string > m_label;

      public:
        Labeling()
        : m_label( 0 )
        {
        }

        virtual ~Labeling() = default;

        virtual const char* label( void )
        {
            if( m_label == 0 )
            {
                m_label = std::make_shared< std::string >(
                    labelName() + std::to_string( labelId() ) );
                assert( m_label );
            }

            return m_label->c_str();
        }

        virtual const char* labelName( void )
        {
            return "lbl";
        }

        virtual u64 labelId( void )
        {
            return (u64)this;
        }
    };
}

#endif // _LIB_STDHL_CPP_LABELING_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
