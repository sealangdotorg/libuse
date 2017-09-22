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

#include "main.h"

using namespace libstdhl;

#define LIBSTDHL_CPP_MATH_POW_TEST( NAME, TYPE, BASE, EXPONENT )               \
    TEST( libstdhl_cpp_math, pow_##NAME )                                      \
    {                                                                          \
        auto p0 = Math::pow< TYPE >( BASE, EXPONENT );                         \
                                                                               \
        EXPECT_EQ( p0, std::pow( BASE, EXPONENT ) );                           \
    }

LIBSTDHL_CPP_MATH_POW_TEST( f_l_0, long double, 2, 0 );
LIBSTDHL_CPP_MATH_POW_TEST( f_l_1, long double, 2, 15 );
LIBSTDHL_CPP_MATH_POW_TEST( f_l_2, long double, 2, 16 );
LIBSTDHL_CPP_MATH_POW_TEST( f_l_3, long double, 2, 17 );
LIBSTDHL_CPP_MATH_POW_TEST( f_l_4, long double, 2, 31 );
LIBSTDHL_CPP_MATH_POW_TEST( f_l_5, long double, 2, 32 );
LIBSTDHL_CPP_MATH_POW_TEST( f_l_6, long double, 2, 33 );
LIBSTDHL_CPP_MATH_POW_TEST( f_l_7, long double, 2, 63 );
LIBSTDHL_CPP_MATH_POW_TEST( f_l_8, long double, 2, 64 );
LIBSTDHL_CPP_MATH_POW_TEST( f_l_9, long double, 2, 65 );

LIBSTDHL_CPP_MATH_POW_TEST( f_d_0, double, 2, 0 );
LIBSTDHL_CPP_MATH_POW_TEST( f_d_1, double, 2, 15 );
LIBSTDHL_CPP_MATH_POW_TEST( f_d_2, double, 2, 16 );
LIBSTDHL_CPP_MATH_POW_TEST( f_d_3, double, 2, 17 );
LIBSTDHL_CPP_MATH_POW_TEST( f_d_4, double, 2, 31 );
LIBSTDHL_CPP_MATH_POW_TEST( f_d_5, double, 2, 32 );
LIBSTDHL_CPP_MATH_POW_TEST( f_d_6, double, 2, 33 );
LIBSTDHL_CPP_MATH_POW_TEST( f_d_7, double, 2, 63 );
LIBSTDHL_CPP_MATH_POW_TEST( f_d_8, double, 2, 64 );
LIBSTDHL_CPP_MATH_POW_TEST( f_d_9, double, 2, 65 );

LIBSTDHL_CPP_MATH_POW_TEST( f_f_0, float, 2, 0 );
LIBSTDHL_CPP_MATH_POW_TEST( f_f_1, float, 2, 15 );
LIBSTDHL_CPP_MATH_POW_TEST( f_f_2, float, 2, 16 );
LIBSTDHL_CPP_MATH_POW_TEST( f_f_3, float, 2, 17 );
LIBSTDHL_CPP_MATH_POW_TEST( f_f_4, float, 2, 31 );
LIBSTDHL_CPP_MATH_POW_TEST( f_f_5, float, 2, 32 );
LIBSTDHL_CPP_MATH_POW_TEST( f_f_6, float, 2, 33 );
LIBSTDHL_CPP_MATH_POW_TEST( f_f_7, float, 2, 63 );
LIBSTDHL_CPP_MATH_POW_TEST( f_f_8, float, 2, 64 );
LIBSTDHL_CPP_MATH_POW_TEST( f_f_9, float, 2, 65 );

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
