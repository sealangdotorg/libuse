//  
//  Copyright (c) 2016 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libstdhl
//  
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program. If not, see <http://www.gnu.org/licenses/>.
//  

#ifndef _LIB_STDHL_CPP_LABELING_H_
#define _LIB_STDHL_CPP_LABELING_H_

#include "Type.h"
#include "Allocator.h"

/**
   @brief    TODO
   
   TODO
*/

namespace libstdhl 
{
    class Labeling
	{
	private:
		char* label;
		
	public:
		Labeling()
		: label( 0 )
		{
		}
		
		~Labeling()
		{
			if( label )
			{
				free( label );
			}
		}
		
	    const char* getLabel( void )
		{
			if( label == 0 )
			{
				std::string s( "lbl" + std::to_string( getLabelID() ) );
				label = (char*)malloc( s.size() );
				assert( label );
				strcpy( label, s.c_str() );
			}
			
			return label;
		}

		virtual u64 getLabelID( void )
		{
			return (u64)this;
		}
	};
}

#endif /* _LIB_STDHL_CPP_LABELING_H_ */


//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
