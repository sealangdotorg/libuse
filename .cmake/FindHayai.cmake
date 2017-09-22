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

# HAYAI_FOUND        - system has found the package
# HAYAI_INCLUDE_DIRS - the package include directories
# HAYAI_LIBRARY      - the package library

include( LibPackage )

libfind_pkg_check_modules( HAYAI_PKGCONF hayai )

find_path( HAYAI_INCLUDE_DIR
  NAMES hayai/hayai.hpp
  PATHS ${hayai_PKGCONF_INCLUDE_DIRS}
)

find_library( HAYAI_LIBRARY
  NAMES hayai_main
  PATHS ${hayai_PKGCONF_LIBRARY_DIRS}
)

set( HAYAI_PROCESS_INCLUDES HAYAI_INCLUDE_DIR )
set( HAYAI_PROCESS_LIBS     HAYAI_LIBRARY )

libfind_process( HAYAI )
