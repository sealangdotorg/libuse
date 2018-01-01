#
#   Copyright (C) 2014-2018 CASM Organization <https://casm-lang.org>
#   All rights reserved.
#
#   Developed by: Philipp Paulweber
#                 Emmanuel Pescosta
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

# LIBHAYAI_FOUND        - system has found the package
# LIBHAYAI_INCLUDE_DIRS - the package include directories
# LIBHAYAI_LIBRARY      - the package library

include( LibPackage )

libfind_pkg_check_modules( LIBHAYAI_PKGCONF libhayai )

find_path( LIBHAYAI_INCLUDE_DIR
  NAMES hayai/hayai.hpp
  PATHS ${LIBHAYAI_PKGCONF_INCLUDE_DIRS}
  )

find_library( LIBHAYAI_LIBRARY
  NAMES hayai_main
  PATHS ${LIBHAYAI_PKGCONF_LIBRARY_DIRS}
  )

set( LIBHAYAI_PROCESS_INCLUDES LIBHAYAI_INCLUDE_DIR )
set( LIBHAYAI_PROCESS_LIBS     LIBHAYAI_LIBRARY )

libfind_process( LIBHAYAI )

if( EXISTS "${LIBHAYAI_INCLUDE_DIR}" AND
    EXISTS "${LIBHAYAI_LIBRARY}" AND
    ${LIBHAYAI_INCLUDE_DIR} AND
    ${LIBHAYAI_LIBRARY}
    )
  set( LIBHAYAI_FOUND TRUE PARENT_SCOPE )
else()
  set( LIBHAYAI_FOUND FALSE PARENT_SCOPE )
endif()
