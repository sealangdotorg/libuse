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

#include "uts/main.h"

using namespace libstdhl;
using namespace Log;

TEST( libstdhl_cpp_logger, example )
{
    Stream stream;
    Logger log( stream );

    log.error( "example error message" );
    log.warning( "example warning message" );
    log.hint( "example hint message" );
    log.debug( "example debug message" );

    StringFormatter format;
    OutputStreamSink sink( std::cerr, format );
    stream.flush( sink );

    EXPECT_EQ( log.errors(), 1 );
    EXPECT_EQ( log.warnings(), 1 );
}

TEST( libstdhl_cpp_logger, location_with_text )
{
    Stream stream;
    Logger log( stream );

    // Data data( Level::ID::ERROR,  );

    log.log( Level::ID::ERROR, log.source(), log.category(),
        Items( { make< TextItem >( "ddxy" ) } ) );

    log.log< Level::ID::ERROR >( make< TextItem >( "ddxy" ) );

    log.log< Level::ID::ERROR >(
        make< LocationItem >( TextItem( "file.txt" ),
            RangeItem( PositionItem( 1, 2 ), PositionItem( 3, 4 ) ) ),
        make< TextItem >( "nice range in a file :-)" ) );

    log.log< Level::ID::WARNING >(
        make< LocationItem >( "file.txt", 1, 2, 3, 4 ),
        make< TextItem >( "nice range in a file :-)" ) );

    StringFormatter format;
    OutputStreamSink sink( std::cerr, format );
    stream.flush( sink );
    
    EXPECT_EQ( log.errors(), 3 );
    EXPECT_EQ( log.warnings(), 1 );
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
