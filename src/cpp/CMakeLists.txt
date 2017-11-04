#
#   Copyright (c) 2014-2017 CASM Organization https://casm-lang.org
#   All rights reserved.
#
#   Developed by: Philipp Paulweber
#                 Emmanuel Pescosta
#                 https://github.com/casm-lang/libstdhl
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

add_library( ${PROJECT}-cpp OBJECT
  Ansi.cpp
  Args.cpp
  file/TextDocument.cpp
  log/Category.cpp
  log/Chronograph.cpp
  log/Data.cpp
  log/Filter.cpp
  log/Formatter.cpp
  log/Item.cpp
  log/Level.cpp
  log/Logger.cpp
  log/Router.cpp
  log/Sink.cpp
  log/Source.cpp
  log/Stream.cpp
  log/Switch.cpp
  log/Timestamp.cpp
  network/ethernet/Link.cpp
  network/ethernet/Packet.cpp
  network/ethernet/Protocol.cpp
  network/ethernet/Socket.cpp
  network/Interface.cpp
  network/lsp/Content.cpp
  network/lsp/Interface.cpp
  network/lsp/Message.cpp
  network/lsp/Packet.cpp
  network/lsp/Protocol.cpp
  network/Packet.cpp
  network/tcp/IPv4.cpp
  network/tcp/Socket.cpp
  network/udp/IPv4.cpp
  network/udp/Socket.cpp
  standard/rfc3986.cpp
  type/Boolean.cpp
  type/Data.cpp
  type/Decimal.cpp
  type/Integer.cpp
  type/Natural.cpp
  type/Rational.cpp
  type/String.cpp
  xml/pugixml.cpp
  )


configure_file(
  Version.in.h
  ${PROJECT}/Version
  )


ecm_generate_headers( ${PROJECT}_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    Allocator
    Ansi
    Args
    Array
    Binding
    Enum
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
    Random
    Standard
    String
    Test
    Type
    Variadic
    Xml
    libstdhl
    file/TextDocument
  PREFIX
    ${PROJECT}
  REQUIRED_HEADERS
    ${PROJECT}_HEADERS
  )
install(
  FILES
    ${${PROJECT}_HEADERS}
    ${${PROJECT}_HEADERS_CPP}
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}/Version
  DESTINATION
    "include/${PROJECT}"
  )


ecm_generate_headers( ${PROJECT}_FILE_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    TextDocument
  PREFIX
    ${PROJECT}/file
  RELATIVE
    file
  REQUIRED_HEADERS
    ${PROJECT}_FILE_HEADERS
  )
install(
  FILES
    ${${PROJECT}_FILE_HEADERS}
    ${${PROJECT}_FILE_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/file"
  )


ecm_generate_headers( ${PROJECT}_JSON_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    json
  PREFIX
    ${PROJECT}/json
  RELATIVE
    json
  REQUIRED_HEADERS
    ${PROJECT}_JSON_HEADERS
  )
install(
  FILES
    ${${PROJECT}_JSON_HEADERS}
    ${${PROJECT}_JSON_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/json"
  )


ecm_generate_headers( ${PROJECT}_LOG_HEADERS_CPP
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
    ${PROJECT}/log
  RELATIVE
    log
  REQUIRED_HEADERS
    ${PROJECT}_LOG_HEADERS
  )
install(
  FILES
    ${${PROJECT}_LOG_HEADERS}
    ${${PROJECT}_LOG_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/log"
  )


ecm_generate_headers( ${PROJECT}_NETWORK_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    Interface
    Lsp
    Packet
    Protocol
    Socket
  PREFIX
    ${PROJECT}/network
  RELATIVE
    network
  REQUIRED_HEADERS
    ${PROJECT}_NETWORK_HEADERS
  )
install(
  FILES
    ${${PROJECT}_NETWORK_HEADERS}
    ${${PROJECT}_NETWORK_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/network"
  )


ecm_generate_headers( ${PROJECT}_NETWORK_ETHERNET_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    Link
    Packet
    Protocol
    Socket
  PREFIX
    ${PROJECT}/network/ethernet
  RELATIVE
    network/ethernet
  REQUIRED_HEADERS
    ${PROJECT}_NETWORK_ETHERNET_HEADERS
  )
install(
  FILES
    ${${PROJECT}_NETWORK_ETHERNET_HEADERS}
    ${${PROJECT}_NETWORK_ETHERNET_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/network/ethernet"
  )


ecm_generate_headers( ${PROJECT}_NETWORK_IPV4_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    Protocol
  PREFIX
    ${PROJECT}/network/ipv4
  RELATIVE
    network/ipv4
  REQUIRED_HEADERS
    ${PROJECT}_NETWORK_IPV4_HEADERS
  )
install(
  FILES
    ${${PROJECT}_NETWORK_IPV4_HEADERS}
    ${${PROJECT}_NETWORK_IPV4_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/network/ipv4"
  )


ecm_generate_headers( ${PROJECT}_NETWORK_LSP_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    Content
    Exception
    Identifier
    Interface
    Message
    Packet
    Protocol
  PREFIX
    ${PROJECT}/network/lsp
  RELATIVE
    network/lsp
  REQUIRED_HEADERS
    ${PROJECT}_NETWORK_LSP_HEADERS
  )
install(
  FILES
    ${${PROJECT}_NETWORK_LSP_HEADERS}
    ${${PROJECT}_NETWORK_LSP_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/network/lsp"
  )
  

ecm_generate_headers( ${PROJECT}_NETWORK_TCP_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    IPv4
    Packet
    Protocol
    Session
    Socket
  PREFIX
    ${PROJECT}/network/tcp
  RELATIVE
    network/tcp
  REQUIRED_HEADERS
    ${PROJECT}_NETWORK_TCP_HEADERS
  )
install(
  FILES
    ${${PROJECT}_NETWORK_TCP_HEADERS}
    ${${PROJECT}_NETWORK_TCP_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/network/tcp"
  )


ecm_generate_headers( ${PROJECT}_NETWORK_UDP_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    IPv4
    Packet
    Protocol
    Socket
  PREFIX
    ${PROJECT}/network/udp
  RELATIVE
    network/udp
  REQUIRED_HEADERS
    ${PROJECT}_NETWORK_UDP_HEADERS
  )
install(
  FILES
    ${${PROJECT}_NETWORK_UDP_HEADERS}
    ${${PROJECT}_NETWORK_UDP_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/network/udp"
  )


ecm_generate_headers( ${PROJECT}_STANDARD_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    ieee802
    rfc3986
  PREFIX
    ${PROJECT}/standard
  RELATIVE
    standard
  REQUIRED_HEADERS
    ${PROJECT}_STANDARD_HEADERS
  )
install(
  FILES
    ${${PROJECT}_STANDARD_HEADERS}
    ${${PROJECT}_STANDARD_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/standard"
  )


ecm_generate_headers( ${PROJECT}_TYPE_HEADERS_CPP
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
    ${PROJECT}/type
  RELATIVE
    type
  REQUIRED_HEADERS
    ${PROJECT}_TYPE_HEADERS
  )
install(
  FILES
    ${${PROJECT}_TYPE_HEADERS}
    ${${PROJECT}_TYPE_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/type"
  )


ecm_generate_headers( ${PROJECT}_XML_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    pugiconfig
    pugixml
  PREFIX
    ${PROJECT}/xml
  RELATIVE
    xml
  REQUIRED_HEADERS
    ${PROJECT}_XML_HEADERS
  )
install(
  FILES
    ${${PROJECT}_XML_HEADERS}
    ${${PROJECT}_XML_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/xml"
  )
