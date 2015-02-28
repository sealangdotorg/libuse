
/*
  Copyright (C) 2015 Philipp Paulweber
  
  This file is part of the 'libstdhl' project which is released under a NSCA
  open source software license. For more information, see the LICENSE.txt
  file in the project root directory.
*/

#ifndef _LIB_STDHL_CPP_ARGS_H_
#define _LIB_STDHL_CPP_ARGS_H_

#include "stdhl/c/args.h"

#include "Type.h"

/**
   @file     Args.h
   @class    Args
   
   @brief    TODO
   
   TODO
   
   @author   Philipp Paulweber
   @date     2015-02-16
*/

class Args  
{
public:
	enum Mode
	{ DEFAULT   = 0
	, ALTERNATE = 1
	};
	
	enum Kind
	{ NONE     = 0
	, REQUIRED = 1
	, OPTIONAL = 2
	};
	
	function< void( void ) > usage;
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
	
    unordered_map< const char*, Option > options;
	
	string format_str;
	
	int (*getopt_func)( int, char* const*, const char*, const option*, int* );
	
public:
	
	Args
	( int argc
	, const char** argv
	, function< void( const char* ) > process_non_option = []( const char* arg ) { }
	) 
	: Args( argc, argv, ALTERNATE, process_non_option )
	{
	}
	
	Args
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
								
				if( opt.second.field.val )
				{
					str.append( "-" );
					str.push_back( opt.second.field.val );
					
					if( opt.second.field.has_arg )
					{
						if( opt.second.field.has_arg == OPTIONAL )
						{
							str.append("[=");
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
							str.append("[=");
						}
						else
						{
							str.append(" ");
						}
						
						str.append( "<" );
						str.append( opt.second.metatag );
						str.append( ">" );
						
						if( opt.second.field.has_arg == OPTIONAL )
						{
							str.append("]");
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
			
			exit( error_code );
		};
		
		error_arg_required = [this]( const char* arg ) 
		{
			fprintf( stderr
				   , "%s: error: option '%s' requires an argument\n"
				   , this->argv[0], arg );
		};
		
		error_arg_invalid = [this]( const char* arg ) 
		{
			fprintf( stderr
				   , "%s: error: unrecognized option '%s'\n"
				   , this->argv[0], arg );
		};
	}
	
	const char* getProgramName() const
	{
		return argv[ 0 ];
	}
	
	void parse( void )
	{
		int err = 0;
		int pos = 0;
		int len = options.size();
		
		int getopt_ctrl;
		option getopt_options[ len + 1 ];
		
		for( pair< const char*, Option > opt : options )
		{
			getopt_options[ pos ] = opt.second.field;
			pos++;
		}
		
		getopt_options[ pos ] = { 0, 0, 0, 0 };
		
		opterr = 0;
		
		while( true )
		{ 
			int getopt_index = 0;
			
			getopt_ctrl = getopt_func
				( argc
				, (char* const*)argv
				, format_str.c_str()
				, getopt_options
				, &getopt_index
				);
			
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
			
			bool found = false;
			
			for( pair< const char*, Option > opt : options )
			{
				size_t optc = (size_t)opt.first;
				
				if( optc >= 256 )
				{
					continue;
				}
				
				if( (int)optc == (int)getopt_ctrl )
				{
					if( optarg )
					{
						opt.second.action( optarg );
					}
					else
					{
						opt.second.action( "" );
					}
					
					found = true;
					break;
				}
			}
			
			if( found )
			{
				continue;
			}
			
			if( getopt_ctrl == ':' )
			{
				error_arg_required( argv[ optind - 1 ] );
				err++;
				continue;
			}
			
			if( getopt_ctrl == '?' )
			{
				error_arg_invalid( argv[ optind - 1 ] );
				err++;
				continue;
			}
			
			error( -1, 0 );
		}
		
		for( int index = optind; index < argc; index++ )
		{
			process_non_option( argv[ index ] );
		}
		
		if( err > 0 )
		{
			error( -1, 0 );
		}
	}
	
	void add
	( const char arg_char
	, const char* arg_str
	, Kind kind
	, const char* description
	, function< void( const char* ) > action
	, const char* metatag = "arg"
	)
	{
		const char* key = (const char*)(size_t)arg_char;
		
		assert( ( ( arg_char == 0 ) ||
				  ( arg_char >= 'a' && arg_char <= 'z' ) ||
				  ( arg_char >= 'A' && arg_char <= 'Z' ) ||
				  ( arg_char >= '0' && arg_char <= '9' ) ) &&
				"invalid 'arg_char' value" );
		
		if( arg_char > 0 )
		{
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
			
			if( !key )
			{
				key = arg_str;		
			}
		}
	    
		options[ key ] = Option();
		
	    Option& opt = options[ key ];
		
		options[ key ] = opt;
		
		opt.field.name    = arg_str;
		opt.field.has_arg = kind;
		opt.field.flag    = 0;
		opt.field.val     = arg_char;
		
		opt.action        = action;
		opt.description   = description;
		opt.metatag       = metatag;
		
		if( arg_char > 0 )
		{
			char _arg_char[] = ".";
			_arg_char[0] = arg_char;
			format_str.append( _arg_char );
			
			if( kind == REQUIRED or kind == OPTIONAL )
			{
				format_str.append( ":" );
			}
		}
	}
	
	
    /**
	   @brief    TODO

	   TODO
	   
	   @param    arg0    TODO
	   @return   TODO
	   @retval   TODO
	*/
};

#endif /* _LIB_STDHL_CPP_ARGS_H_ */


/*
  Local variables:
  mode: c++
  indent-tabs-mode: t
  c-basic-offset: 4
  tab-width: 4
  End:
  vim:noexpandtab:sw=4:ts=4:
*/
