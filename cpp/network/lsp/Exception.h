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

#ifndef _LIB_STDHL_CPP_NETWORK_LSP_EXCEPTION_H_
#define _LIB_STDHL_CPP_NETWORK_LSP_EXCEPTION_H_

#include "Content.h"

/**
   @brief    TBD

   TBD
*/

namespace libstdhl
{
    namespace Network
    {
        namespace LSP
        {
            class Exception : public Data, public std::exception
            {
              public:
                Exception( const ErrorCode code, const std::string& message,
                    const Data& data )
                : m_code( code )
                , m_message( message )
                , m_data( data )
                {
                }

                inline Exception(
                    const ErrorCode code, const std::string& message )
                : Exception( code, message, nullptr )
                {
                }

                inline Exception( const std::string& message, const Data& data )
                : Exception( ErrorCode::UnknownErrorCode, message, data )
                {
                }

                inline Exception( const std::string& message )
                : Exception( ErrorCode::UnknownErrorCode, message, nullptr )
                {
                }

                ErrorCode code( void ) const
                {
                    return m_code;
                }

                std::string message( void ) const
                {
                    return m_message;
                }

                Data data( void ) const
                {
                    return m_data;
                }

                const char* what( void ) const noexcept override
                {
                    return m_message.c_str();
                }

              private:
                ErrorCode m_code;
                std::string m_message;
                Data m_data;
            };
        }
    }
}

#endif // _LIB_STDHL_CPP_NETWORK_LSP_EXCEPTION_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
