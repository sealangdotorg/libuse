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

set( PROJECT libstdhl )
cmake_minimum_required( VERSION ${CMAKE_VERSION} )

set( CMAKE_VERSION 3.2 )
set( CMAKE_INCLUDE_CURRENT_DIR ON )

set( CMAKE_CXX_STANDARD 11 )
project( ${PROJECT} LANGUAGES CXX )

find_package( ECM REQUIRED CONFIG QUIET )
set( CMAKE_MODULE_PATH
  ${ECM_MODULE_PATH}
  ${${PROJECT}_SOURCE_DIR}/.cmake
  )

include( ECMGenerateHeaders )
include( FeatureSummary )
include( GenerateExportHeader )
include( CPack )
include( LibPackage )

package_git( ${PROJECT} )

package_print_path( CMAKE_MODULE_PATH )
package_print_path( CMAKE_INSTALL_PREFIX )

package_find_git( GTest 1.8   REQUIRED "../gtest;lib/gtest" build )
package_find_git( Hayai 1.0.1 REQUIRED "../hayai;lib/hayai" build )

add_subdirectory( src )
add_subdirectory( etc )


#
# library
#

add_library( ${PROJECT} STATIC
  $<TARGET_OBJECTS:${PROJECT}-cpp>
  )

set( ${PROJECT}_VERSION ${GIT_REVTAG} )
set_property( TARGET ${PROJECT} PROPERTY VERSION   ${PROJECT}_VERSION )
set_property( TARGET ${PROJECT} PROPERTY PREFIX "" )

install(
  TARGETS  ${PROJECT}
  EXPORT   ${PROJECT}Targets
  LIBRARY  DESTINATION lib
  ARCHIVE  DESTINATION lib
  RUNTIME  DESTINATION bin
  INCLUDES DESTINATION include
  )

generate_export_header( ${PROJECT} )

install(
  FILES
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}_export.h"
  DESTINATION
  "include/${PROJECT}"
  )

#
# test
# 

add_executable( ${PROJECT}-check
  $<TARGET_OBJECTS:${PROJECT}-test>
  )

target_link_libraries( ${PROJECT}-check
  ${PROJECT}
  GTest::GTest
  GTest::Main
  )


#
# benchmark
#

add_executable( ${PROJECT}-run
  $<TARGET_OBJECTS:${PROJECT}-benchmark>
  )

target_link_libraries( ${PROJECT}-run
  ${PROJECT}
  ${HAYAI_LIBRARY}
  )


#
# cmake
#

install(
  FILES
    .cmake/FindHayai.cmake
    .cmake/Find${PROJECT}.cmake
    .cmake/LibCompile.cmake
    .cmake/LibPackage.cmake
  DESTINATION
    "share/cmake/Module/${PROJECT}"
  )

#
# build features and checks
#

include( LibCompile )
compile_flags( CXX )
compile_check()
feature_summary( WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES )
