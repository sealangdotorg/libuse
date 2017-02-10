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
  ${LIBS}/hayai/src/hayai_posix_main.cpp
)
  
add_executable( ${TARGET}-run
  $<TARGET_OBJECTS:${TARGET}-benchmark>
)

target_link_libraries( ${TARGET}-run
  pthread
)


if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Debug
    CACHE STRING "Choose the type of build : debug sanitize release."
    FORCE
  )
endif( NOT CMAKE_BUILD_TYPE )


message( "* ${CMAKE_BUILD_TYPE} : ${TARGET} : ${LIBS}" )
