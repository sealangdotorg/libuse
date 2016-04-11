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

#ifndef _LIB_STDHL_CPP_BINDING_H_
#define _LIB_STDHL_CPP_BINDING_H_

#include "Type.h"

/**
   @brief    TODO
   
   TODO
*/

namespace libstdhl 
{
    template< class C >
	class Binding
	{
	private:
		C* reference = 0;
		
	public:
		void bind( C* object )
		{
			assert( !reference && "already bounded to a reference" );
			assert( object && "invalid object reference to bind" );
			
			reference = object;
		}
		
		C* unbind( void )
		{
			assert( reference && "object reference was never bound" );
			
			C* tmp = reference;
			reference = 0;
			return tmp;
		}
		
	    C* getBound( void ) const
		{
			assert( reference && "object reference was never bound" );
			return reference;
		}

		const u1 isBound( void ) const
		{
			return reference != 0;
		}
	};
}

#endif /* _LIB_STDHL_CPP_BINDING_H_ */


//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
