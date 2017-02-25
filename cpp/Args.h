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

#ifndef _LIB_STDHL_CPP_ARGS_H_
#define _LIB_STDHL_CPP_ARGS_H_

#include "c/args.h"

#include "Type.h"

/**
   @brief    TODO

   TODO
*/

namespace libstdhl
{
    class Args
    {
      public:
        enum Mode
        {
            DEFAULT = 0,
            ALTERNATE = 1
        };

        enum Kind
        {
            NONE = 0,
            REQUIRED = 1,
            OPTIONAL = 2
        };

        std::function< void( void ) > m_usage;
        std::function< void( const char*, const char* ) > m_message;
        std::function< void( const char* ) > m_info;
        std::function< void( const char* ) > m_warning;
        std::function< void( int, const char* ) > m_error;

        std::function< void( const char* ) > m_error_arg_required;
        std::function< void( const char* ) > m_error_arg_invalid;

      private:
        struct Option
        {
            option field;
            const char* description;
            const char* metatag;
            std::function< void( const char* ) > action;
        };

        int m_argc;

        const char** m_argv;

        Mode m_mode;

        std::function< void( const char* ) > m_process_non_option;

        std::string m_format_str;

        std::unordered_map< std::string, Option > m_options;

        int ( *m_getopt_func )(
            int, char* const*, const char*, const option*, int* );

      public:
        Args( int argc, const char** argv,
            std::function< void( const char* ) > process_non_option
            = []( const char* arg ) {} );

        Args( int argc, const char** argv, Mode mode,
            std::function< void( const char* ) > process_non_option
            = []( const char* arg ) {} );

        const char* programName() const;

        int parse( void );

        void add( const char arg_char, Kind kind, const char* description,
            std::function< void( const char* ) > process_option,
            const char* metatag = "arg" );

        void add( const char* arg_str, Kind kind, const char* description,
            std::function< void( const char* ) > process_option,
            const char* metatag = "arg" );

        void add( const char arg_char, const char* arg_str, Kind kind,
            const char* description,
            std::function< void( const char* ) > process_option,
            const char* metatag = "arg" );
    };
}

#endif // _LIB_STDHL_CPP_ARGS_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
