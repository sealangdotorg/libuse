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

# LIBSTDHL_FOUND        - system has found the package
# LIBSTDHL_INCLUDE_DIRS - the package include directories
# LIBSTDHL_LIBRARY      - the package library

include( LibPackage )

libfind_pkg_check_modules( LIBSTDHL_PKGCONF libstdhl )

find_path( LIBSTDHL_INCLUDE_DIR
  NAMES libstdhl/libstdhl.h
  PATHS ${LIBSTDHL_PKGCONF_INCLUDE_DIRS}
)

find_library( LIBSTDHL_LIBRARY
  NAMES libstdhl stdhl
  PATHS ${LIBSTDHL_PKGCONF_LIBRARY_DIRS}
)

if( LIBSTDHL_LIBRARY-NOTFOUND )
  find_library( LIBSTDHL_LIBRARY
    NAMES libstdhl libstdhl
    PATHS ${LIBSTDHL_PKGCONF_LIBRARY_DIRS}
    )
endif()

set( LIBSTDHL_PROCESS_INCLUDES LIBSTDHL_INCLUDE_DIR )
set( LIBSTDHL_PROCESS_LIBS     LIBSTDHL_LIBRARY )

libfind_process( LIBSTDHL )
