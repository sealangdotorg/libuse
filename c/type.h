//  
//  Copyright (c) 2016 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libstdhl
//  
//  This software is licensed under the Open Software License v. 3.0 (OSL-3.0).
//  You either find the license text below or in the root directory of this 
//  project in the LICENSE.txt file. If you cannot find the LICENSE.txt file,
//  please refer to https://opensource.org/licenses/OSL-3.0.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
//  CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
//  WITH THE SOFTWARE.
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
