#
#   Copyright (C) 2014-2022 CASM Organization <https://casm-lang.org>
#   All rights reserved.
#
#   Developed by: Philipp Paulweber et al.
#                 <https://github.com/casm-lang/libstdhl/graphs/contributors>
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

set( PROJECT libstdhl )

cmake_minimum_required( VERSION ${CMAKE_VERSION} )

project( ${PROJECT} LANGUAGES C CXX )
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_EXTENSIONS OFF )
set( CMAKE_VERSION 3.2 )
set( CMAKE_MODULE_PATH ${${PROJECT}_SOURCE_DIR}/.cmake )
include( ${CMAKE_CONFIG_PATH}/LibPackage.cmake )

package_git( ${PROJECT} )
package_git_submodule( libgtest master REQUIRED build ../gtest lib/gtest )
package_git_submodule( libhayai master REQUIRED build ../hayai lib/hayai )
package_git_deps()

find_package( Threads REQUIRED )

include( ECMGenerateHeaders )
include( FeatureSummary )
include( GenerateExportHeader )
include( CPack )

add_subdirectory( src )
add_subdirectory( etc
  EXCLUDE_FROM_ALL
  )

set_property( TARGET ${PROJECT}-cpp PROPERTY POSITION_INDEPENDENT_CODE 1 )

#
#
# static library
#

set( ${PROJECT}_VERSION ${GIT_REVTAG} )

add_library( ${PROJECT} STATIC
  $<TARGET_OBJECTS:${PROJECT}-cpp>
  )

set_property( TARGET ${PROJECT} PROPERTY OUTPUT_NAME ${PROJECT} )
set_property( TARGET ${PROJECT} PROPERTY VERSION ${${PROJECT}_VERSION} )
set_property( TARGET ${PROJECT} PROPERTY PREFIX  "" )

if( WIN32 )
  target_link_libraries( ${PROJECT}
    ws2_32
    )
endif()

#
#
# shared library
#

set( ${PROJECT}_SHARED "" )
if( TARGET_LIBRARY AND NOT CMAKE_CXX_EMSCRIPTEN  )
  set( ${PROJECT}_SHARED ${PROJECT}-so )

  add_library( ${${PROJECT}_SHARED} SHARED
    $<TARGET_OBJECTS:${PROJECT}-cpp>
    )

  set_property( TARGET ${${PROJECT}_SHARED} PROPERTY OUTPUT_NAME ${PROJECT} )
  set_property( TARGET ${${PROJECT}_SHARED} PROPERTY VERSION ${${PROJECT}_VERSION} )
  set_property( TARGET ${${PROJECT}_SHARED} PROPERTY PREFIX  "" )
  set_property( TARGET ${${PROJECT}_SHARED} PROPERTY IMPORT_PREFIX  "" )

  if( WIN32 )
    target_link_libraries( ${${PROJECT}_SHARED}
      ws2_32
      )
  endif()

  add_dependencies( ${PROJECT}
    ${${PROJECT}_SHARED}
    )
endif()

#
#
# test
# 

add_executable( ${PROJECT}-check
  EXCLUDE_FROM_ALL
  $<TARGET_OBJECTS:${PROJECT}-test>
  )

if( ${LIBGTEST_FOUND} )
  target_link_libraries( ${PROJECT}-check
    ${PROJECT}
    ${LIBGTEST_LIBRARY}
    ${LIBGTEST_MAIN}
    Threads::Threads
    )
endif()

if( CMAKE_CXX_EMSCRIPTEN )
  set( EMCC_TARGET "${PROJECT}-check" )
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/js/emcc.in.js
    ${EMCC_TARGET}.js
  )
  set( EMCC_TARGET "" )
endif()

#
#
# benchmark
#

add_executable( ${PROJECT}-run
  EXCLUDE_FROM_ALL
  $<TARGET_OBJECTS:${PROJECT}-benchmark>
  )

if( ${LIBHAYAI_FOUND} )
  target_link_libraries( ${PROJECT}-run
    ${PROJECT}
    ${LIBHAYAI_LIBRARY}
    )
endif()

if( CMAKE_CXX_EMSCRIPTEN )
  set( EMCC_TARGET "${PROJECT}-run" )
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/js/emcc.in.js
    ${EMCC_TARGET}.js
  )
  set( EMCC_TARGET "" )
endif()

#
#
# install
#

install(
  TARGETS  ${PROJECT} ${${PROJECT}_SHARED}
  EXPORT   ${PROJECT}Targets
  LIBRARY  DESTINATION lib
  ARCHIVE  DESTINATION lib
  RUNTIME  DESTINATION bin
  INCLUDES DESTINATION include
  )

generate_export_header(
  ${PROJECT}
  )

install(
  FILES
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}_export.h"
  DESTINATION
  "include/${PROJECT}"
  )

install(
  FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/js/emcc.in.js"
  DESTINATION
  "include/${PROJECT}"
  )

export(
  TARGETS ${PROJECT}
  FILE    ${PROJECT}Config.cmake
  )

#
#
# cmake
#

install(
  FILES
    .cmake/ECMGenerateHeaders.cmake
    .cmake/Find${PROJECT}.cmake
    .cmake/Findlibgtest.cmake
    .cmake/Findlibhayai.cmake
    .cmake/LibCompile.cmake
    .cmake/LibPackage.cmake
    .cmake/config.mk
  DESTINATION
    "share/cmake/Module/${PROJECT}"
  )

#
#
# build features and checks
#

include( LibCompile )
compile_flags( CXX )
compile_check()

feature_summary( WHAT
  PACKAGES_NOT_FOUND
  DISABLED_FEATURES
  )
