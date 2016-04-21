//  
//  Copyright (c) 2014-2016 Philipp Paulweber
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

#ifndef _LIB_STDHL_C_TYPE_H_
#define _LIB_STDHL_C_TYPE_H_

#include <stdint.h>
#include <stdbool.h>

#include "default.h"

/**
   @brief    TODO
   
   TODO
*/

#ifdef __cplusplus
extern "C"
{
#endif

	typedef bool     u1;
	typedef uint8_t  u8;
	typedef uint16_t u16;
	typedef uint16_t u32;
	typedef uint64_t u64;

	typedef bool     i1;
	typedef int8_t   i8;
	typedef int16_t  i16;
	typedef int16_t  i32;
	typedef int64_t  i64;

#ifdef __cplusplus
}
#endif

#endif /* _LIB_STDHL_C_TYPE_H_ */


//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
