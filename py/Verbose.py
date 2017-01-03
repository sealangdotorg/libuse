#
#   Copyright (c) 2014-2017 Philipp Paulweber
#   All rights reserved.
#
#   Developed by: Philipp Paulweber
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



