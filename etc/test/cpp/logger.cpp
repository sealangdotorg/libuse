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

#include <libstdhl/Test>

using namespace libstdhl;
using namespace Log;
using namespace Memory;

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
