//
//  Copyright (C) 2014-2018 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
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

#include <libstdhl/Test>

#include <libstdhl/net/lsp/LSP>

using namespace libstdhl;
using namespace Network;
using namespace LSP;

TEST( libstdhl_cpp_network_lsp_content, CompletionItem )
{
    CompletionItem obj( std::string( "label" ) );
    EXPECT_STREQ( obj.label().c_str(), "label" );

    EXPECT_FALSE( obj.hasKind() );
    obj.setKind( CompletionItemKind::Text );
    EXPECT_EQ( obj.kind(), CompletionItemKind::Text );
    EXPECT_TRUE( obj.hasKind() );

    EXPECT_FALSE( obj.hasDetail() );
    obj.setDetail( "detail" );
    EXPECT_STREQ( obj.detail().c_str(), "detail" );
    EXPECT_TRUE( obj.hasDetail() );

    EXPECT_FALSE( obj.hasDocumentation() );
    auto documentation = MarkupContent( std::string( "doc" ) );
    obj.setDocumentation( documentation );
    EXPECT_STREQ( obj.documentation().value().c_str(), "doc" );
    EXPECT_TRUE( obj.hasDocumentation() );

    EXPECT_FALSE( obj.hasDeprecated() );
    obj.setDeprecated( true );
    EXPECT_TRUE( obj.isDeprecated() );
    EXPECT_TRUE( obj.hasDeprecated() );

    EXPECT_FALSE( obj.hasPreselected() );
    obj.setPreselected( true );
    EXPECT_TRUE( obj.isPreselected() );
    EXPECT_TRUE( obj.hasPreselected() );

    EXPECT_FALSE( obj.hasSortText() );
    obj.setSortText( "SortText" );
    EXPECT_STREQ( obj.sortText().c_str(), "SortText" );
    EXPECT_TRUE( obj.hasSortText() );

    EXPECT_FALSE( obj.hasFilterText() );
    obj.setFilterText( "FilterText" );
    EXPECT_STREQ( obj.filterText().c_str(), "FilterText" );
    EXPECT_TRUE( obj.hasFilterText() );

    EXPECT_FALSE( obj.hasInsertText() );
    obj.setInsertText( "InsertText" );
    EXPECT_STREQ( obj.insertText().c_str(), "InsertText" );
    EXPECT_TRUE( obj.hasInsertText() );

    EXPECT_FALSE( obj.hasInsertTextFormat() );
    obj.setInsertTextFormat( InsertTextFormat::PlainText );
    EXPECT_EQ( obj.insertTextFormat(), InsertTextFormat::PlainText );
    EXPECT_TRUE( obj.hasInsertTextFormat() );

    EXPECT_FALSE( obj.hasTextEdit() );
    auto range = Range( Position( 1, 1 ), Position( 1, 10 ) );
    auto textEdit = TextEdit( range, std::string( "newText" ) );
    obj.setTextEdit( textEdit );
    EXPECT_STREQ( obj.textEdit().newText().c_str(), "newText" );
    EXPECT_TRUE( obj.hasTextEdit() );
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
