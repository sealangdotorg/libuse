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

#include "Args.h"

using namespace libstdhl;

Args::Args( int argc, const char** argv,
    std::function< void( const char* ) > process_non_option )
: Args( argc, argv, DEFAULT, process_non_option )
{
}

Args::Args( int argc, const char** argv, Mode mode,
    std::function< void( const char* ) > process_non_option )
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

    m_usage = [this]() {
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

            fprintf( stderr, "  %-30.30s %s\n", str.c_str(),
                opt.second.description.c_str() );
        }
    };

    m_message = [this]( const char* kind, const char* msg ) {
        assert( kind );
        assert( msg );

        fprintf( stderr, "%s: %s: %s\n", this->m_argv[ 0 ], kind, msg );
    };

    m_info = [this]( const char* msg ) { this->m_message( "info", msg ); };

    m_warning
        = [this]( const char* msg ) { this->m_message( "warning", msg ); };

    m_error = [this]( int error_code, const char* msg ) {
        if( msg )
        {
            this->m_message( "error", msg );
        }

        if( error_code != 0 )
        {
            exit( error_code );
        }
    };

    m_error_arg_required = [this]( const char* arg ) {
        std::string tmp;
        tmp.append( "option '" );
        tmp.append( arg );
        tmp.append( "' requires an argument" );
        m_error( 0, tmp.c_str() );
    };

    m_error_arg_invalid = [this]( const char* arg ) {
        std::string tmp;
        tmp.append( "unrecognized option '" );
        tmp.append( arg );
        tmp.append( "'" );
        m_error( 0, tmp.c_str() );
    };
}

const char* Args::programName() const
{
    return m_argv[ 0 ];
}

int Args::parse( void )
{
    int err = 0;
    int pos = 0;
    int len = m_options.size();

    int getopt_ctrl;
    option getopt_options[ len + 1 ];

    for( auto& opt : m_options )
    {
        // printf( "%i: %s | %i:%s\n    %s | %s | %p\n"
        //         , pos, opt.first.c_str()
        //         , opt.second.field.val
        //         , opt.second.field.name
        //         , opt.second.description
        //         , opt.second.metatag
        //         , &opt.second.action
        //     );

        getopt_options[ pos ] = opt.second.field;
        pos++;
    }

    getopt_options[ pos ] = { 0, 0, 0, 0 };
    opterr = 0;
    optind = 0;

    // printf( "format_str = 0x'%p', str'%s', size = '%i'\n", &format_str,
    // format_str.c_str(), format_str.size() );

    while( true )
    {
        int getopt_index = 0;

        getopt_ctrl = m_getopt_func( m_argc, (char* const*)m_argv,
            (const char*)( m_format_str.c_str() ), getopt_options,
            &getopt_index );

        // printf( "::: %i, %i, %i, '%s', \n", getopt_ctrl, getopt_index,
        // optind, optarg );

        if( getopt_ctrl == -1 )
        {
            break;
        }

        if( getopt_ctrl == 0 )
        {
            if( getopt_options[ getopt_index ].flag == 0 )
            {
                if( optarg )
                {
                    m_options[ getopt_options[ getopt_index ].name ].action(
                        optarg );
                }
                else
                {
                    m_options[ getopt_options[ getopt_index ].name ].action(
                        "" );
                }
            }

            continue;
        }

        if( getopt_ctrl == ':' )
        {
            m_error_arg_required( m_argv[ optind - 1 ] );
            err++;
            continue;
        }

        if( optind <= 1 )
        {
            m_error_arg_invalid( m_argv[ optind ] );
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
                        // printf("not default\n");
                        m_error_arg_invalid( arg );
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
            // printf("not found '%s'\n", str);
            m_error_arg_invalid( arg );
            err++;
            continue;
        }

        if( optarg )
        {
            result->second.action( optarg );
        }
        else
        {
            if( result->second.field.has_arg == REQUIRED )
            {
                m_error_arg_required( arg );
                err++;
            }
            else
            {
                result->second.action( "" );
            }
        }
    }

    for( int index = optind; index < m_argc; index++ )
    {
        m_process_non_option( m_argv[ index ] );
    }

    if( err > 0 )
    {
        m_error( -1, 0 );
    }

    return err;
}

void Args::add( const char arg_char, Kind kind, const std::string& description,
    std::function< void( const char* ) > action, const std::string& metatag )
{
    add( arg_char, 0, kind, description, action, metatag );
}

void Args::add( const char* arg_str, Kind kind, const std::string& description,
    std::function< void( const char* ) > action, const std::string& metatag )
{
    add( 0, arg_str, kind, description, action, metatag );
}

void Args::add( const char arg_char, const char* arg_str, Kind kind,
    const std::string& description, std::function< void( const char* ) > action,
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
            fprintf( stderr,
                "%s: internal error: '%c' option argument is not unique\n",
                m_argv[ 0 ], arg_char );
            exit( -1 );
        }
    }

    if( arg_str )
    {
        if( m_options.find( arg_str ) != m_options.end() )
        {
            fprintf( stderr,
                "%s: internal error: '%s' option argument is not unique\n",
                m_argv[ 0 ], arg_str );
            exit( -1 );
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

    // printf( "format_str: '%s'\n", format_str.c_str() );
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
