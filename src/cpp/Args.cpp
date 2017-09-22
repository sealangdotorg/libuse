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

#include "Args.h"

#include <cassert>
#include <cstring>
#include <map>
#include <sstream>

using namespace libstdhl;

Args::Args( int argc, const char** argv,
    std::function< i32( const char* ) > process_non_option )
: Args( argc, argv, DEFAULT, process_non_option )
{
}

Args::Args( int argc, const char** argv, Mode mode,
    std::function< i32( const char* ) > process_non_option )
: m_argc( argc )
, m_argv( argv )
, m_mode( mode )
, m_process_non_option( process_non_option )
{
    m_format_str = "";

    if( m_mode == DEFAULT )
    {
        m_getopt_func = &getopt_long;
    }
    else
    {
        m_getopt_func = &getopt_long_only;
    }
}

int Args::parse( Logger& log )
{
    int err = 0;
    int pos = 0;
    int len = m_options.size();

    int getopt_ctrl;
    option getopt_options[ len + 1 ];

    for( auto& opt : m_options )
    {
        getopt_options[ pos ] = opt.second.field;
        pos++;
    }

    getopt_options[ pos ] = { 0, 0, 0, 0 };
    opterr = 0;
    optind = 0;

    while( true )
    {
        int getopt_index = 0;

        getopt_ctrl = m_getopt_func( m_argc, (char* const*)m_argv,
            (const char*)( m_format_str.c_str() ), getopt_options,
            &getopt_index );

        if( getopt_ctrl == -1 )
        {
            break;
        }

        if( getopt_ctrl == 0 )
        {
            if( getopt_options[ getopt_index ].flag == 0 )
            {
                i32 ret = 0;

                if( optarg )
                {
                    ret = m_options[ getopt_options[ getopt_index ].name ]
                              .action( optarg );
                }
                else
                {
                    ret = m_options[ getopt_options[ getopt_index ].name ]
                              .action( "" );
                }

                if( ret >= 0 )
                {
                    err += ret;
                }
                else
                {
                    return -1;
                }
            }
            continue;
        }

        if( getopt_ctrl == ':' )
        {
            log.error(
                "option '%s' requires an argument", m_argv[ optind - 1 ] );
            err++;
            continue;
        }

        if( optind <= 1 )
        {
            log.error( "unrecognized option '%s'", m_argv[ optind ] );
            err++;
            break;
        }

        const char* arg = m_argv[ optind - 1 - ( optarg ? 1 : 0 ) ];
        const char* str = "";

        if( getopt_ctrl == '?' )
        {
            str = &arg[ 2 ];

            if( m_mode == DEFAULT )
            {
                if( strlen( arg ) <= 2 )
                {
                    str = &arg[ 1 ];
                }
                else
                {
                    if( arg[ 1 ] != '-' )
                    {
                        log.error( "unrecognized option '%s'", arg );
                        err++;
                        continue;
                    }
                }
            }
            else
            {
                if( arg[ 1 ] != '-' )
                {
                    str = &arg[ 1 ];
                }
            }
        }
        else
        {
            str = &arg[ 1 ];
        }

        auto result = m_options.find( str );
        if( result == m_options.end() )
        {
            if( str[ 0 ] == '-' )
            {
                result = m_options.find( &str[ 1 ] );
            }
        }
        if( result == m_options.end() )
        {
            log.error( "unrecognized option '%s'", arg );
            err++;
            continue;
        }

        if( optarg )
        {
            i32 ret = result->second.action( optarg );
            if( ret >= 0 )
            {
                err += ret;
            }
            else
            {
                return -1;
            }
        }
        else
        {
            if( result->second.field.has_arg == REQUIRED )
            {
                log.error( "option '%s' requires an argument", arg );
                err++;
            }
            else
            {
                i32 ret = result->second.action( "" );
                if( ret >= 0 )
                {
                    err += ret;
                }
                else
                {
                    return -1;
                }
            }
        }
    }

    for( int index = optind; index < m_argc; index++ )
    {
        i32 ret = m_process_non_option( m_argv[ index ] );
        if( ret >= 0 )
        {
            err += ret;
        }
        else
        {
            return -1;
        }
    }

    return err;
}

void Args::add( const char arg_char, Kind kind, const std::string& description,
    std::function< i32( const char* ) > action, const std::string& metatag )
{
    add( arg_char, 0, kind, description, action, metatag );
}

void Args::add( const char* arg_str, Kind kind, const std::string& description,
    std::function< i32( const char* ) > action, const std::string& metatag )
{
    add( 0, arg_str, kind, description, action, metatag );
}

void Args::add( const char arg_char, const char* arg_str, Kind kind,
    const std::string& description, std::function< i32( const char* ) > action,
    const std::string& metatag )
{
    assert( ( ( arg_char == 0 ) || ( arg_char == '+' )
                || ( arg_char >= 'a' && arg_char <= 'z' )
                || ( arg_char >= 'A' && arg_char <= 'Z' )
                || ( arg_char >= '0' && arg_char <= '9' ) )
            && "invalid 'arg_char' value" );

    std::string key;

    if( arg_char > 0 )
    {
        key.push_back( arg_char );

        if( m_options.find( key ) != m_options.end() )
        {
            throw std::domain_error( "option argument '"
                                     + std::string( 1, arg_char )
                                     + "' is not unique" );
        }
    }

    if( arg_str )
    {
        if( m_options.find( arg_str ) != m_options.end() )
        {
            throw std::domain_error( "option argument '"
                                     + std::string( arg_str )
                                     + "' is not unique" );
        }

        key = std::string( arg_str );
    }

    m_options[ key ] = Option();
    m_options[ key ].field.name = arg_str;
    m_options[ key ].field.has_arg = kind;
    m_options[ key ].field.flag = 0;
    m_options[ key ].field.val = arg_char;
    m_options[ key ].action = action;
    m_options[ key ].description = description;
    m_options[ key ].metatag = metatag;

    if( arg_str and arg_char )
    {
        std::string tmp;
        tmp.push_back( arg_char );
        m_options[ tmp ] = m_options[ key ];
    }

    if( arg_char > 0 )
    {
        m_format_str.push_back( arg_char );

        if( kind == REQUIRED or kind == OPTIONAL )
        {
            m_format_str.push_back( ':' );
        }
    }
}

std::string Args::usage( void ) const
{
    std::stringstream stream;

    std::map< std::string, Option > sorted_options;

    for( auto& opt : this->m_options )
    {
        std::string key;

        if( opt.second.field.val )
        {
            key.push_back( '.' );
            key.push_back( opt.second.field.val );
        }
        if( opt.second.field.name )
        {
            key.append( opt.second.field.name );
        }

        sorted_options[ key ] = opt.second;
    }

    for( auto& opt : sorted_options )
    {
        std::string str = "-";

        if( opt.second.field.val and ( not opt.second.field.name ) )
        {
            // only short option provided
            str.push_back( opt.second.field.val );
        }
        else
        {
            // long option provided
            if( opt.second.field.val )
            {
                // additionally short option provided
                str.push_back( opt.second.field.val );
                str.append( ", -" );
            }

            str.append( "-" );
            str.append( opt.second.field.name );
        }

        if( opt.second.field.has_arg )
        {
            if( opt.second.field.has_arg == OPTIONAL )
            {
                str.append( "[=" );
            }
            else
            {
                str.append( " " );
            }

            str.append( "<" );
            str.append( opt.second.metatag );
            str.append( ">" );

            if( opt.second.field.has_arg == OPTIONAL )
            {
                str.append( "]" );
            }
        }

        char buffer[ 256 ];

        sprintf( buffer, "  %-30.30s %s\n", str.c_str(),
            opt.second.description.c_str() );

        stream << buffer;
    }

    return stream.str();
}

std::string Args::programName( void ) const
{
    return m_argv[ 0 ];
}

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
