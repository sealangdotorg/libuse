#   
#   Copyright (c) 2015 Philipp Paulweber
#   All rights reserved.
#   
#   Developed by: Philipp Paulweber
#                 https://github.com/ppaulweber/libstdhl
#   
#   Permission is hereby granted, free of charge, to any person obtaining a 
#   copy of this software and associated documentation files (the "Software"), 
#   to deal with the Software without restriction, including without limitation 
#   the rights to use, copy, modify, merge, publish, distribute, sublicense, 
#   and/or sell copies of the Software, and to permit persons to whom the 
#   Software is furnished to do so, subject to the following conditions:
#   
#   * Redistributions of source code must retain the above copyright 
#     notice, this list of conditions and the following disclaimers.
#   
#   * Redistributions in binary form must reproduce the above copyright 
#     notice, this list of conditions and the following disclaimers in the 
#     documentation and/or other materials provided with the distribution.
#   
#   * Neither the names of the copyright holders, nor the names of its 
#     contributors may be used to endorse or promote products derived from 
#     this Software without specific prior written permission.
#   
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
#   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
#   CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
#   WITH THE SOFTWARE.
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



