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

#ifndef _LIB_STDHL_CPP_ARGS_H_
#define _LIB_STDHL_CPP_ARGS_H_

#include "c/args.h"

#include "Type.h"

/**
   @brief    TODO

   TODO
*/

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

    function< void( void ) > usage;
    function< void( const char*, const char* ) > message;
    function< void( const char* ) > info;
    function< void( const char* ) > warning;
    function< void( int, const char* ) > error;

    function< void( const char* ) > error_arg_required;
    function< void( const char* ) > error_arg_invalid;

  private:
    struct Option
    {
        option field;
        const char* description;
        const char* metatag;
        function< void( const char* ) > action;
    };

    int argc;

    const char** argv;

    Mode mode;

    function< void( const char* ) > process_non_option;

    std::string format_str;

    std::unordered_map< std::string, Option > options;

    int ( *getopt_func )( int, char* const*, const char*, const option*, int* );

  public:
    Args( int argc, const char** argv,
        function< void( const char* ) > process_non_option
        = []( const char* arg ) {} );

    Args( int argc, const char** argv, Mode mode,
        function< void( const char* ) > process_non_option
        = []( const char* arg ) {} );

    const char* getProgramName() const;

    int parse( void );

    void add( const char arg_char, Kind kind, const char* description,
        function< void( const char* ) > process_option,
        const char* metatag = "arg" );

    void add( const char* arg_str, Kind kind, const char* description,
        function< void( const char* ) > process_option,
        const char* metatag = "arg" );

    void add( const char arg_char, const char* arg_str, Kind kind,
        const char* description, function< void( const char* ) > process_option,
        const char* metatag = "arg" );
};

#endif /* _LIB_STDHL_CPP_ARGS_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
