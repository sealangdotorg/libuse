# 
# Copyright (c) 2014 Philipp Paulweber
# 
# This file is part of the 'libverbose' project which is released under a NCSA
# open source software license. For more information, see the LICENSE.txt 
# file in the project root directory.
#


def printf( message, *args ) :
    print( message % args )
# end def

def verbose( message, *args ) :
    
    printf( message, args )
    
# end def

