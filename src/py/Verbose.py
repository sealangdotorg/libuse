#
#   Copyright (C) 2014-2021 CASM Organization <https://casm-lang.org>
#   All rights reserved.
#
#   Developed by: Philipp Paulweber
#                 Emmanuel Pescosta
#                 Christian Lascsak
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

import sys
import os
import re

_CSI = "\x1B["

_SRG = \
       { "c"        : 0 # clear
       , "clear"    : 0
       
       , "b"        : 1 # bold       
       , "bold"     : 1
       
       , "u"        : 2 # underline
       , "single"   : 2
       
       , "l"        : 5 # blink
       , "blink"    : 5
       
       , "black"    : 30 # foreground color sequences
       , "red"      : 31
       , "green"    : 32
       , "yellow"   : 33
       , "blue"     : 34
       , "magenta"  : 35
       , "cyan"     : 36
       , "white"    : 37
       
       , "Black"    : 40 # background color sequences
       , "Red"      : 41
       , "Green"    : 42
       , "Yellow"   : 43
       , "Blue"     : 44
       , "Magenta"  : 45
       , "Cyan"     : 46
       , "White"    : 47
       }

_matcher = re.compile( "(\%\{)|(:)|(\%\})" )
_ignore  = re.compile( "[ \t]" )

def transform( text ) :
    
    state = []
    stack = []
    
    def generate_sequence( ) :
        seq = _CSI + "0;"

        if len(state) > 0 :
            for s in state :
                seq = "%s%d;" % ( seq, s, )
            
        return seq[:-1] + "m"
    # end def
    
    i = _matcher.finditer( text )
    
    valid  = None
    result = ""
    last   = -1
    
    for e in i :
        if e.group(1) is not None :
            valid = e.span()
            
            if last < 0 :
                last = valid[0]
                result = text[ : valid[0] ]
        
        elif e.group(2) is not None \
         and valid      is not None :
            
            options = _ignore.sub( "", text[ valid[1] : e.span()[0] ] ).split( "," )
            
            for option in options :
                try :
                    state.append( _SRG[ option ] )
                except KeyError :
                    assert( 0 and "invalid option used" )
            
            stack.append( len( options ) )
            
            result = "%s%s%s" % ( result, text[ last : valid[0] ], generate_sequence(), )
            last   = e.span()[1]
            
            valid = None
            
        elif e.group(3) is not None :
            if valid is not None :
                assert( 0 and "LIBVERBOSE: need a seperator in color format" )
            
            for i in range( stack.pop() ) :
                state.pop()
            
            result = "%s%s%s" % ( result, text[ last : e.span()[0]  ], generate_sequence() )
            last   = e.span()[1]
    
    if last < 0 :
        return text
    else :
        return "%s%s" % ( result, text[ last : ], )
# end def


def printf( message, *args, **kwargs ) :
    stream = sys.stdout
    
    if "stream" in kwargs :
        stream = kwargs[ "stream" ]
    
    if stream is None :
        return
    
    message = transform( message )
    stream.write( message % args )
# end def


def verbose( message, *args ) :
    
    printf( message, args )
    
# end def

def error( message, *args ) :
    
    printf( message, args )
    
    sys.exit( -1 )
# end def



