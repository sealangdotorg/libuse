//
//  Copyright (C) 2014-2019 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
//                Christian Lascsak
//                <https://github.com/casm-lang/libstdhl>
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

#include "File.h"

#if defined( __WIN32__ ) or defined( __WIN32 ) or defined( _WIN32 )
#include <direct.h>
#include <fileapi.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

using namespace libstdhl;
using namespace File;

std::fstream File::open( const std::string& filename, const std::ios_base::openmode mode )
{
    std::fstream file;
    file.open( filename, mode );

    if( not exists( file ) )
    {
        throw std::invalid_argument( "filename '" + filename + "' does not exist" );
    }

    return file;
}

u1 File::exists( const std::string& filename )
{
    if( File::Path::exists( filename ) )
    {
        // filename is a directory
        return false;
    }

    try
    {
        open( filename );
    }
    catch( const std::invalid_argument& e )
    {
        return false;
    }

    return true;
}

u1 File::exists( const std::fstream& file )
{
    return file.is_open();
}

void File::remove( const std::string& filename )
{
    if( not exists( filename ) )
    {
        throw std::invalid_argument(
            "removing file '" + filename + "' failed, because it does not exist" );
    }

    auto result = std::remove( filename.c_str() );

    if( result != 0 )
    {
        throw std::invalid_argument(
            "removing file '" + filename + "' failed (error " + std::to_string( result ) + ")" );
    }

    assert( not exists( filename ) );
}

u8 File::readLines(
    const std::string& filename, std::function< void( u32, const std::string& ) > process_line )
{
    u32 cnt = 0;
    std::string line;
    std::fstream fd( filename );

    if( not exists( fd ) )
    {
        return -1;
    }

    while( std::getline( fd, line ) )
    {
        process_line( cnt, line );
        cnt++;
    }

    fd.close();
    return 0;
}

std::string File::readLine( const std::string& filename, const u32 num )
{
    std::string line;

    auto file = open( filename );

    try
    {
        gotoLine( file, num );
    }
    catch( const FileNumberOutOfRangeException& e )
    {
        throw FileNumberOutOfRangeException(
            "unable to read a line from file '" + filename + "', because the " + e.what() );
    }

    std::getline( file, line );

    return line;
}

std::fstream& File::gotoLine( std::fstream& file, const std::size_t lineNumber )
{
    file.seekg( std::ios::beg );

    std::size_t lineCounter = lineNumber;
    while( lineCounter != 1 )
    {
        file.ignore( std::numeric_limits< std::streamsize >::max(), '\n' );
        lineCounter--;
        if( lineCounter < 0 or file.eof() or file.bad() )
        {
            throw FileNumberOutOfRangeException(
                "file does not contain a line at '" + std::to_string( lineNumber ) + "'" );
        }
    }

    return file;
}

void File::Path::create( const std::string& path )
{
#if defined( __WIN32__ ) or defined( __WIN32 ) or defined( _WIN32 )
    if( _mkdir( path.c_str() ) != 0 )
#else
    if( mkdir( path.c_str(), 0755 ) != 0 )
#endif
    {
        throw std::domain_error( "unable to create path '" + path + "'" );
    }
}

u1 File::Path::exists( const std::string& path )
{
#if defined( __WIN32__ ) or defined( __WIN32 ) or defined( _WIN32 )
    const auto pathStatus = GetFileAttributesA( path.c_str() );
    return ( pathStatus & FILE_ATTRIBUTE_DIRECTORY );
#else
    struct stat pathStatus;
    if( ::stat( path.c_str(), &pathStatus ) != 0 )
    {
        return false;
    }
    return ( pathStatus.st_mode & S_IFDIR );
#endif
}

void File::Path::remove( const std::string& path )
{
#if defined( __WIN32__ ) or defined( __WIN32 ) or defined( _WIN32 )
    if( _rmdir( path.c_str() ) != 0 )
#else
    if( rmdir( path.c_str() ) != 0 )
#endif
    {
        throw std::domain_error( "unable to remove path '" + path + "'" );
    }
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
