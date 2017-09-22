#
#   Copyright (c) 2014-2017 Philipp Paulweber
#   All rights reserved.
#
#   Developed by: Philipp Paulweber
#                 Emmanuel Pescosta
#                 https://github.com/ppaulweber/libstdhl
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

include_directories(
  ${PROJECT_BINARY_DIR}/src/cpp
  ${GTEST_INCLUDE_DIR}
  )

add_library( ${PROJECT}-test OBJECT
  main.cpp
  cpp/hash.cpp
  cpp/variadic.cpp
  cpp/type.cpp
  cpp/args.cpp
  cpp/list.cpp
  cpp/random.cpp
  cpp/standard/rfc3986.cpp
  cpp/file.cpp
  cpp/log.cpp
  cpp/string.cpp
  cpp/logger.cpp
  cpp/json.cpp
  cpp/math.cpp
  cpp/memory.cpp
  cpp/type/integer.cpp
  cpp/type/TODO.cpp
  cpp/type/data.cpp
  cpp/type/rational.cpp
  cpp/enum.cpp
  cpp/xml.cpp
  cpp/network/lsp.cpp
  )


ecm_generate_headers( ${PROJECT}_TEST_HEADERS_CPP
  ORIGINAL
    CAMELCASE
  HEADER_NAMES
    main
  PREFIX
    ${PROJECT}/test
  REQUIRED_HEADERS
    ${PROJECT}_TEST_HEADERS
  )
install(
  FILES
    ${${PROJECT}_TEST_HEADERS}
    ${${PROJECT}_TEST_HEADERS_CPP}
  DESTINATION
    "include/${PROJECT}/test"
  )
