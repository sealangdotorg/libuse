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

#ifndef _LIB_STDHL_CPP_TYPE_H_
#define _LIB_STDHL_CPP_TYPE_H_

#include <set>
#include <map>
#include <list>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <type_traits>

using namespace std;

#include "stdhl/c/type.h"
#include "Default.h"

/**
   @brief    TODO
   
   TODO
*/

namespace libstdhl 
{

	class Hash
	{
	public:
		size_t operator()( const char *val ) const
		{
			return std::hash< std::string >()( val );
		}
	};

	class Equal
	{
	public:
		size_t operator()( const char *val1, const char *val2 ) const
		{
			return std::string( val1 ) == std::string( val2 );
		}
	};
}

#endif /* _LIB_STDHL_CPP_TYPE_H_ */


//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
