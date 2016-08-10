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

#include "Args.h"


Args::Args
( int argc
, const char** argv
, function< void( const char* ) > process_non_option 
)
: Args( argc, argv, mode, process_non_option )
{
}

Args::Args
( int argc
, const char** argv
, Mode mode
, function< void( const char* ) > process_non_option 
)
: argc( argc )
, argv( argv )
, mode( mode )
, process_non_option( process_non_option )
{
    format_str = "";
	
	for( int i = 0; i < argc; i++ )
	{
		if( std::string( argv[i] ).find( " " ) != std::string::npos )
		{
			fprintf
			( stderr
			, "%s: internal error: '%s' is a invalid argument element at argv[%i] to parse\n"
			, argv[0]
			, argv[i]
			, i
			);
			exit( -1 );		
		}
	}
	
	
	if( mode == DEFAULT )
	{
		getopt_func = &getopt_long;
	}
	else
	{
		getopt_func = &getopt_long_only;
	}
	
	usage = [this]() 
	{
		map< string, Option > sorted_options;
		
		for( auto& opt : this->options )
		{
			string key;
			
			if( opt.second.field.val )
			{
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
			string str;
							
			if( opt.second.field.val < 256 )
			{
				str.append( "-" );
				str.push_back( opt.second.field.val );
				
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
						str.append("]");
					}
				}
				
				str.append( " " );
			}
			
			if( opt.second.field.name )
			{
				if( this->mode == DEFAULT )
				{
					str.append( "--" );
				}
				else
				{
					str.append( "-" );
				}
				
				str.append( opt.second.field.name );

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
			}
			
			fprintf( stderr, "  %-30.30s %s\n"
					 , str.c_str()
				, opt.second.description );
		}		
	};
	
	error = [this]( int error_code, const char* msg ) 
	{
		if( msg )
		{
			fprintf( stderr, "%s: error: %s\n", this->argv[0], msg );				
		}

		if( error_code != 0 )
		{
			exit( error_code );
		}
	};
	
	error_arg_required = [this]( const char* arg ) 
	{
		std::string tmp;
	    tmp.append( "option '" );
		tmp.append( arg );
		tmp.append( "' requires an argument" );
		error( 0, tmp.c_str() );
	};
	
	error_arg_invalid = [this]( const char* arg ) 
	{
		std::string tmp;
		tmp.append( "unrecognized option '" );
		tmp.append( arg );
		tmp.append( "'" );
		error( 0, tmp.c_str() );
	};
}

const char* Args::getProgramName() const
{
	return argv[ 0 ];
}

int Args::parse( void )
{
	int err = 0;
	int pos = 0;
	int len = options.size();
	
	int getopt_ctrl;
	option getopt_options[ len + 1 ];
	
	for( auto& opt : options )
	{
		// printf( "%i: %c (%i), %i:%s\n"
		// 		, pos, opt.first, opt.first
		// 		, opt.second.field.val, opt.second.field.name );
		
		getopt_options[ pos ] = opt.second.field;
		pos++;
	}
	
	getopt_options[ pos ] = { 0, 0, 0, 0 };
    opterr = 0;
	optind = 0;
	
	//printf( "format_str = 0x'%p', str'%s', size = '%i'\n", &format_str, format_str.c_str(), format_str.size() );
	
	while( true )
	{
		int getopt_index = 0;

		getopt_ctrl = getopt_func
		( argc
		, (char* const*)argv
		, (const char *)(format_str.c_str())
		, getopt_options
		, &getopt_index
		);
		
		//printf( "::: %i, %i, %i, '%s', \n", getopt_ctrl, getopt_index, optind, optarg );
		
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
					options[ getopt_options[ getopt_index ].name ].action( optarg );
				}
				else
				{
					options[ getopt_options[ getopt_index ].name ].action( "" );
				}
			}
			
			continue;
		}
		
		if( getopt_ctrl == ':' )
		{
			error_arg_required( argv[ optind - 1 ] );
			err++;
			continue;
		}

		if( optind <= 1 )
		{
			error_arg_invalid( argv[ optind ] );
			err++;
		    break;
		}
		
		const char* arg = argv[ optind - 1 - ( optarg ? 1 : 0 ) ];
	    const char* str = "";
		
		if( getopt_ctrl == '?' )
		{
			str = &arg[2];
			
			if( mode == DEFAULT )
			{
				if( strlen( arg ) <= 2 )
				{
					str = &arg[1];
				}
				else
				{
					if( arg[1] != '-' )
					{
						//printf("not default\n");
						error_arg_invalid( arg );
						err++;
						continue;
					}
				}
			}
			else
			{
				if( arg[1] != '-' )
				{
					str = &arg[1];
				}
			}
		}
		else
		{
		    str = &arg[1];
		}
		
		auto result  = options.find( str );
	    if( result == options.end() )
		{
			if( str[0] == '-' )
			{
				result = options.find( &str[1] );
				
			}
		}
		if( result == options.end() )
		{
			//printf("not found '%s'\n", str);
			error_arg_invalid( arg );
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
				error_arg_required( arg );
				err++;
			}
			else
			{
				result->second.action( "" );
			}
		}
	}
	
	for( int index = optind; index < argc; index++ )
	{
		process_non_option( argv[ index ] );
	}
	
	if( err > 0 )
	{
		error( -1, 0 );
	}

	return err;
}

void Args::add
( const char arg_char
, Kind kind
, const char* description
, function< void( const char* ) > action
, const char* metatag
)
{
	add( arg_char, 0, kind, description, action, metatag );
}

void Args::add
( const char* arg_str
, Kind kind
, const char* description
, function< void( const char* ) > action
, const char* metatag
)
{
	add( 0, arg_str, kind, description, action, metatag );
}

void Args::add
( const char arg_char
, const char* arg_str
, Kind kind
, const char* description
, function< void( const char* ) > action
, const char* metatag
)
{
	assert( ( ( arg_char == 0 ) ||
			  ( arg_char == '+' ) ||
			  ( arg_char >= 'a' && arg_char <= 'z' ) ||
			  ( arg_char >= 'A' && arg_char <= 'Z' ) ||
			  ( arg_char >= '0' && arg_char <= '9' ) ) &&
			"invalid 'arg_char' value" );

	std::string key;
	
	if( arg_char > 0 )
	{
	    key.push_back( arg_char );
		
		if( options.find( key ) != options.end() )
		{
			fprintf( stderr
					 , "%s: internal error: '%c' option argument is not unique\n"
					 , argv[0], arg_char );
			exit( -1 );		
		}

	}
	
	if( arg_str )
	{		
		if( options.find( arg_str ) != options.end() )
		{
			fprintf( stderr
					 , "%s: internal error: '%s' option argument is not unique\n"
					 , argv[0], arg_str );
			exit( -1 );
		}

	    key = std::string( arg_str );
	}
    
	options[ key ] = Option();
	options[ key ].field.name    = arg_str;
	options[ key ].field.has_arg = kind;
	options[ key ].field.flag    = 0;
	options[ key ].field.val     = arg_char;
	options[ key ].action        = action;
	options[ key ].description   = description;
	options[ key ].metatag       = metatag;
	
	if( arg_str and arg_char )
	{
		std::string tmp;
		tmp.push_back( arg_char );
		options[ tmp ] = options[ key ];
	}
	
	if( arg_char > 0 )
	{
	    format_str.push_back( arg_char );
		
		if( kind == REQUIRED or kind == OPTIONAL )
		{
			format_str.push_back( ':' );
		}
	}
	
	//printf( "format_str: '%s'\n", format_str.c_str() );
}


//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
