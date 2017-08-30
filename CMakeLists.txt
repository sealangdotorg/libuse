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

cmake_minimum_required( VERSION 3.1 )

if( EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../gtest" )
  set( LIBS .. )
else()
  set( LIBS lib )
endif()

foreach( DEPENDENCY
    )
  if( NOT TARGET ${DEPENDENCY} )
    add_subdirectory( ${LIBS}/${DEPENDENCY} lib/${DEPENDENCY} )
  endif()  
endforeach()


file( GLOB_RECURSE C_SRC_FILES   "c/*.c"   )
file( GLOB_RECURSE C_UTS_FILES   "uts/*.c" )

file( GLOB_RECURSE CXX_SRC_FILES "cpp/*.cpp" )
file( GLOB_RECURSE CXX_UTS_FILES "uts/*.cpp" )


set( TARGET stdhl )
project( ${TARGET} )


include_directories(
  .
  ${LIBS}/gtest/googletest
  ${LIBS}/gtest/googletest/include
  ${LIBS}/hayai/src
  ${LIBS}/json
  ${LIBS}/xml
)


set( CMAKE_C_FLAGS            "${CMAKE_C_FLAGS}            -std=c11   -Wall" )
set( CMAKE_C_FLAGS_DEBUG      "${CMAKE_C_FLAGS_DEBUG}      -O0" )
set( CMAKE_C_FLAGS_RELEASE    "${CMAKE_C_FLAGS_RELEASE}" )
set( CMAKE_C_FLAGS_SANITIZE   "${CMAKE_C_FLAGS_SANITIZE}   -O1 -Wextra -g -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=undefined -fsanitize=address" )

set( CMAKE_CXX_FLAGS          "${CMAKE_CXX_FLAGS}          -std=c++11 -Wall" )
set( CMAKE_CXX_FLAGS_DEBUG    "${CMAKE_CXX_FLAGS_DEBUG}    -O0" )
set( CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE}" )
set( CMAKE_CXX_FLAGS_SANITIZE "${CMAKE_CXX_FLAGS_SANITIZE} -O1 -Wextra -g -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=undefined -fsanitize=address" )


add_library( ${TARGET} STATIC
  ${C_SRC_FILES}
  ${CXX_SRC_FILES}
)


add_library( ${TARGET}-test OBJECT
  ${C_UTS_FILES}
  ${CXX_UTS_FILES}
)

add_executable( ${TARGET}-check
  ${LIBS}/gtest/googletest/src/gtest_main.cc
  ${LIBS}/gtest/googletest/src/gtest-all.cc
  $<TARGET_OBJECTS:${TARGET}-test>
)

target_link_libraries( ${TARGET}-check
  ${TARGET}
  pthread
)


add_library( ${TARGET}-benchmark OBJECT
  uts/main.cpp
)
  
add_executable( ${TARGET}-run
  ${LIBS}/hayai/src/hayai_posix_main.cpp
  ${LIBS}/gtest/googletest/src/gtest-all.cc
  $<TARGET_OBJECTS:${TARGET}-benchmark>
)

target_link_libraries( ${TARGET}-run
  ${TARGET}
  pthread
)


if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Debug
    CACHE STRING "Choose the type of build : debug sanitize release."
    FORCE
  )
endif( NOT CMAKE_BUILD_TYPE )


message( "* ${CMAKE_BUILD_TYPE} : ${TARGET} : ${LIBS}" )
