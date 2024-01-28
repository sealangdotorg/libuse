//
//  Copyright (C) 2014-2024 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber et al.
//  <https://github.com/casm-lang/libstdhl/graphs/contributors>
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

#include <libstdhl/Test>

using namespace libstdhl;

#define LIBSTDHL_CPP_MATH_POW_TEST( NAME, TYPE, BASE, EXPONENT ) \
    TEST( libstdhl_cpp_math, pow_##NAME )                        \
    {                                                            \
        auto p0 = Math::pow< TYPE >( BASE, EXPONENT );           \
                                                                 \
        EXPECT_EQ( p0, std::pow( BASE, EXPONENT ) );             \
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

#define LIBSTDHL_CPP_MATH_MIN_TEST( NAME, TYPE, A, B )         \
    TEST( libstdhl_cpp_math, min_##NAME##_##TYPE )             \
    {                                                          \
        EXPECT_EQ( Math::min< TYPE >( A, B ), A < B ? A : B ); \
    }

LIBSTDHL_CPP_MATH_MIN_TEST( 0, i32, 0, 0 );
LIBSTDHL_CPP_MATH_MIN_TEST( 1, i32, 0, 1 );
LIBSTDHL_CPP_MATH_MIN_TEST( 2, i32, 1, 0 );
LIBSTDHL_CPP_MATH_MIN_TEST( 3, i32, -1, 0 );
LIBSTDHL_CPP_MATH_MIN_TEST( 4, i32, -123456789, 123456789 );
LIBSTDHL_CPP_MATH_MIN_TEST( 5, i32, 123456789, -123456789 );

#define LIBSTDHL_CPP_MATH_MAX_TEST( NAME, TYPE, A, B )         \
    TEST( libstdhl_cpp_math, max_##NAME##_##TYPE )             \
    {                                                          \
        EXPECT_EQ( Math::max< TYPE >( A, B ), A < B ? B : A ); \
    }

LIBSTDHL_CPP_MATH_MAX_TEST( 0, i32, 0, 0 );
LIBSTDHL_CPP_MATH_MAX_TEST( 1, i32, 0, 1 );
LIBSTDHL_CPP_MATH_MAX_TEST( 2, i32, 1, 0 );
LIBSTDHL_CPP_MATH_MAX_TEST( 3, i32, -1, 0 );
LIBSTDHL_CPP_MATH_MAX_TEST( 4, i32, -123456789, 123456789 );
LIBSTDHL_CPP_MATH_MAX_TEST( 5, i32, 123456789, -123456789 );

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
