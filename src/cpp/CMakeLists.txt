#
#   Copyright (C) 2014-2021 CASM Organization <https://casm-lang.org>
#   All rights reserved.
#
#   Developed by: Philipp Paulweber
#                 Emmanuel Pescosta
#                 Christian Lascsak
#                 <https://github.com/casm-lang/libstdhl>
#
#   This file is part of libstdhl.
#
#   libstdhl is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   libstdhl is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with libstdhl. If not, see <http://www.gnu.org/licenses/>.
#
#   Additional permission under GNU GPL version 3 section 7
#
#   libstdhl is distributed under the terms of the GNU General Public License
#   with the following clarification and special exception: Linking libstdhl
#   statically or dynamically with other modules is making a combined work
#   based on libstdhl. Thus, the terms and conditions of the GNU General
#   Public License cover the whole combination. As a special exception,
#   the copyright holders of libstdhl give you permission to link libstdhl
#   with independent modules to produce an executable, regardless of the
#   license terms of these independent modules, and to copy and distribute
#   the resulting executable under terms of your choice, provided that you
#   also meet, for each linked independent module, the terms and conditions
#   of the license of that module. An independent module is a module which
#   is not derived from or based on libstdhl. If you modify libstdhl, you
#   may extend this exception to your version of the library, but you are
#   not obliged to do so. If you do not wish to do so, delete this exception
#   statement from your version.
#

include_directories(
  ${PROJECT_BINARY_DIR}/src/cpp
  )

add_library( ${PROJECT}-cpp OBJECT
  Ansi.cpp
  Args.cpp
  Environment.cpp
  Exception.cpp
  File.cpp
  Json.cpp
  SourceLocation.cpp
  String.cpp
  Unicode.cpp
  Yaml.cpp
  data/file/TextDocument.cpp
  data/log/Category.cpp
  data/log/Chronograph.cpp
  data/log/Data.cpp
  data/log/Filter.cpp
  data/log/Formatter.cpp
  data/log/Item.cpp
  data/log/Level.cpp
  data/log/Logger.cpp
  data/log/Router.cpp
  data/log/Sink.cpp
  data/log/Source.cpp
  data/log/Stream.cpp
  data/log/Switch.cpp
  data/log/Timestamp.cpp
  data/type/Boolean.cpp
  data/type/Data.cpp
  data/type/Decimal.cpp
  data/type/Integer.cpp
  data/type/Natural.cpp
  data/type/Rational.cpp
  data/type/String.cpp
  net/Packet.cpp
  net/eth/Frame.cpp
  net/eth/Link.cpp
  net/eth/Protocol.cpp
  net/eth/Socket.cpp
  net/lsp/Content.cpp
  net/lsp/Interface.cpp
  net/lsp/Message.cpp
  net/lsp/Packet.cpp
  net/lsp/Protocol.cpp
  net/lsp/Server.cpp
  net/tcp/IPv4.cpp
  net/tcp/Socket.cpp
  net/udp/IPv4.cpp
  net/udp/Socket.cpp
  std/rfc3629.cpp
  std/rfc3986.cpp
  vendor/pugixml/pugixml.cpp
  vendor/getopt/getopt.cpp
  )


configure_file(
  Version.in.h
  ${PROJECT}/Version.h
  )
configure_file(
  Version.in
  ${PROJECT}/Version
  )
install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}/Version.h
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}/Version
  DESTINATION
    "include/${PROJECT}"
  )


ecm_generate_headers( ${PROJECT}_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    libstdhl
    Allocator
    Ansi
    Args
    Array
    Binding
    Enum
    Environment
    Exception
    File
    Hash
    Json
    Labeling
    Limits
    List
    Log
    Math
    Memory
    Network
    Optional
    Random
    RestoreOnScopeExit
    SourceLocation
    Stack
    Standard
    String
    Test
    Type
    Unicode
    Variadic
    Xml
    Yaml
  PREFIX
    ${PROJECT}
  REQUIRED_HEADERS
    ${PROJECT}_HEADERS
  )
install(
  FILES
    ${${PROJECT}_HEADERS}
    ${${PROJECT}_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}"
  )


ecm_generate_headers( ${PROJECT}_DATA_FILE_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    TextDocument
  PREFIX
    ${PROJECT}/data/file
  RELATIVE
    data/file
  REQUIRED_HEADERS
    ${PROJECT}_DATA_FILE_HEADERS
  )
install(
  FILES
    ${${PROJECT}_DATA_FILE_HEADERS}
    ${${PROJECT}_DATA_FILE_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/data/file"
  )


ecm_generate_headers( ${PROJECT}_DATA_LOG_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    Category
    Channel
    Chronograph
    Data
    Filter
    Formatter
    Item
    Level
    Logger
    Router
    Sink
    Source
    Stream
    Switch
    Timestamp
  PREFIX
    ${PROJECT}/data/log
  RELATIVE
    data/log
  REQUIRED_HEADERS
    ${PROJECT}_DATA_LOG_HEADERS
  )
install(
  FILES
    ${${PROJECT}_DATA_LOG_HEADERS}
    ${${PROJECT}_DATA_LOG_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/data/log"
  )


ecm_generate_headers( ${PROJECT}_DATA_TYPE_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    Boolean
    Data
    Decimal
    Integer
    Layout
    Natural
    Rational
    String
  PREFIX
    ${PROJECT}/data/type
  RELATIVE
    data/type
  REQUIRED_HEADERS
    ${PROJECT}_DATA_TYPE_HEADERS
  )
install(
  FILES
    ${${PROJECT}_DATA_TYPE_HEADERS}
    ${${PROJECT}_DATA_TYPE_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/data/type"
  )


ecm_generate_headers( ${PROJECT}_NET_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    Interface
    Packet
    Protocol
    Socket
  PREFIX
    ${PROJECT}/net
  RELATIVE
    net
  REQUIRED_HEADERS
    ${PROJECT}_NET_HEADERS
  )
install(
  FILES
    ${${PROJECT}_NET_HEADERS}
    ${${PROJECT}_NET_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/net"
  )


ecm_generate_headers( ${PROJECT}_NET_ETH_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    Frame
    Link
    Protocol
    Socket
  PREFIX
    ${PROJECT}/net/eth
  RELATIVE
    net/eth
  REQUIRED_HEADERS
    ${PROJECT}_NET_ETH_HEADERS
  )
install(
  FILES
    ${${PROJECT}_NET_ETH_HEADERS}
    ${${PROJECT}_NET_ETH_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/net/eth"
  )


ecm_generate_headers( ${PROJECT}_NET_IP4_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    Protocol
  PREFIX
    ${PROJECT}/net/ip4
  RELATIVE
    net/ip4
  REQUIRED_HEADERS
    ${PROJECT}_NET_IP4_HEADERS
  )
install(
  FILES
    ${${PROJECT}_NET_IP4_HEADERS}
    ${${PROJECT}_NET_IP4_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/net/ip4"
  )


ecm_generate_headers( ${PROJECT}_NET_LSP_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    LSP
    Content
    Exception
    Identifier
    Interface
    Message
    Packet
    Protocol
    Server
  PREFIX
    ${PROJECT}/net/lsp
  RELATIVE
    net/lsp
  REQUIRED_HEADERS
    ${PROJECT}_NET_LSP_HEADERS
  )
install(
  FILES
    ${${PROJECT}_NET_LSP_HEADERS}
    ${${PROJECT}_NET_LSP_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/net/lsp"
  )


ecm_generate_headers( ${PROJECT}_NET_TCP_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    IPv4
    Packet
    Protocol
    Session
    Socket
  PREFIX
    ${PROJECT}/net/tcp
  RELATIVE
    net/tcp
  REQUIRED_HEADERS
    ${PROJECT}_NET_TCP_HEADERS
  )
install(
  FILES
    ${${PROJECT}_NET_TCP_HEADERS}
    ${${PROJECT}_NET_TCP_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/net/tcp"
  )


ecm_generate_headers( ${PROJECT}_NET_UDP_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    IPv4
    Packet
    Protocol
    Socket
  PREFIX
    ${PROJECT}/net/udp
  RELATIVE
    net/udp
  REQUIRED_HEADERS
    ${PROJECT}_NET_UDP_HEADERS
  )
install(
  FILES
    ${${PROJECT}_NET_UDP_HEADERS}
    ${${PROJECT}_NET_UDP_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/net/udp"
  )


ecm_generate_headers( ${PROJECT}_STD_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    ieee802
    rfc3629
    rfc3986
  PREFIX
    ${PROJECT}/std
  RELATIVE
    std
  REQUIRED_HEADERS
    ${PROJECT}_STD_HEADERS
  )
install(
  FILES
    ${${PROJECT}_STD_HEADERS}
    ${${PROJECT}_STD_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/std"
  )


ecm_generate_headers( ${PROJECT}_VENDOR_FLEX_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    FlexLexer
  PREFIX
    ${PROJECT}/vendor/flex
  RELATIVE
    vendor/flex
  REQUIRED_HEADERS
    ${PROJECT}_VENDOR_FLEX_HEADERS
  )
install(
  FILES
    ${${PROJECT}_VENDOR_FLEX_HEADERS}
    ${${PROJECT}_VENDOR_FLEX_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/vendor/flex"
  )


ecm_generate_headers( ${PROJECT}_VENDOR_GETOPT_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    getopt
  PREFIX
    ${PROJECT}/vendor/getopt
  RELATIVE
    vendor/getopt
  REQUIRED_HEADERS
    ${PROJECT}_VENDOR_GETOPT_HEADERS
  )
install(
  FILES
    ${${PROJECT}_VENDOR_GETOPT_HEADERS}
    ${${PROJECT}_VENDOR_GETOPT_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/vendor/getopt"
  )


ecm_generate_headers( ${PROJECT}_VENDOR_JSON_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    json
  PREFIX
    ${PROJECT}/vendor/json
  RELATIVE
    vendor/json
  REQUIRED_HEADERS
    ${PROJECT}_VENDOR_JSON_HEADERS
  )
install(
  FILES
    ${${PROJECT}_VENDOR_JSON_HEADERS}
    ${${PROJECT}_VENDOR_JSON_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/vendor/json"
  )


ecm_generate_headers( ${PROJECT}_VENDOR_PUGIXML_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    pugiconfig
    pugixml
  PREFIX
    ${PROJECT}/vendor/pugixml
  RELATIVE
    vendor/pugixml
  REQUIRED_HEADERS
    ${PROJECT}_VENDOR_PUGIXML_HEADERS
  )
install(
  FILES
    ${${PROJECT}_VENDOR_PUGIXML_HEADERS}
    ${${PROJECT}_VENDOR_PUGIXML_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/vendor/pugixml"
  )

ecm_generate_headers( ${PROJECT}_VENDOR_MINI_YAML_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    Yaml
  PREFIX
    ${PROJECT}/vendor/mini-yaml
  RELATIVE
    vendor/mini-yaml
  REQUIRED_HEADERS
    ${PROJECT}_VENDOR_MINI_YAML_HEADERS
  )
install(
  FILES
    ${${PROJECT}_VENDOR_MINI_YAML_HEADERS}
    ${${PROJECT}_VENDOR_MINI_YAML_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/vendor/mini-yaml"
  )
