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

#ifndef _LIBSTDHL_CPP_TYPE_H_
#define _LIBSTDHL_CPP_TYPE_H_

#include <cstdint>
#include <string>

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    using u1 = bool;
    using u8 = std::uint8_t;
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;
    using i8 = std::int8_t;
    using i16 = std::int16_t;
    using i32 = std::int32_t;
    using i64 = std::int64_t;

    /**
       @extends Stdhl
    */
    namespace Type
    {
        enum Radix : u8
        {
            BINARY = 2,
            OCTAL = 8,
            DECIMAL = 10,
            HEXADECIMAL = 16,
            SEXAGESIMAL = 60,
            RADIX64 = 64,
        };

        enum Literal : u8
        {
            NONE = 0,
            STDHL = 1,
            C = 2,
            CPP14 = 3,
            BASE64 = 10,
            UNIX = 20
        };

        //
        // Boolean
        //

        class Boolean;

        Boolean createBoolean( const std::string& value );

        Boolean createBoolean( const u1 value );

        //
        // Integer
        //

        class Integer;

        Integer createInteger(
            const std::string& value, const Radix radix = DECIMAL );

        Integer createInteger( const u64 value );

        Integer createInteger( const i64 value );

        //
        // Natural
        //

        class Natural;

        Natural createNatural(
            const std::string& value, const Radix radix = DECIMAL );

        Natural createNatural( const Integer& value );

        Natural createNatural( const u64 value );

        //
        // Rational
        //

        class Rational;

        Rational createRational(
            const std::string& value, const Radix radix = DECIMAL );

        Rational createRational(
            const Integer& numerator, const Integer& denominator );

        //
        // Decimal
        //

        class Decimal;

        Decimal createDecimal(
            const std::string& value, const Radix radix = DECIMAL );

        Decimal createDecimal( const double value );

        Decimal createDecimal( const Integer& value );

        Decimal createDecimal( const Natural& value );

        //
        // String
        //

        class String;

        String createString( const std::string& value );
    }
}

#endif // _LIBSTDHL_CPP_TYPE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
