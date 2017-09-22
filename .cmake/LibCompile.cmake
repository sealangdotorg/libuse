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

message( "-- Loading: LibCompile" )


function( compile_flags KIND )
  add_definitions( -Wall )
  add_definitions( -Wno-deprecated )

  set( CMAKE_${KIND}_FLAGS          "${CMAKE_${KIND}_FLAGS}" )
  set( CMAKE_${KIND}_FLAGS_DEBUG    "${CMAKE_${KIND}_FLAGS_DEBUG} -O0" )
  set( CMAKE_${KIND}_FLAGS_RELEASE  "${CMAKE_${KIND}_FLAGS_RELEASE}" )
  set( CMAKE_${KIND}_FLAGS_SANITIZE "${CMAKE_${KIND}_FLAGS_SANITIZE} -O1 -Wextra -g -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=undefined -fsanitize=address" )
endfunction()


function( compile_check )
  if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Debug
      CACHE STRING "Choose the type of build : debug sanitize release."
      FORCE
      )
  endif( NOT CMAKE_BUILD_TYPE )
endfunction()

