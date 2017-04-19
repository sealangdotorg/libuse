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

import os
import sys
import fileinput

rootdir = os.path.join( "." )
if len( sys.argv ) == 2 :
    rootdir = sys.argv[1]

licensepath = os.path.join( rootdir, "LICENSE.txt" )
if not os.path.exists( licensepath ) :
    sys.stderr.write( "%s: license file '%s' does not exist!\n" % ( sys.argv[0], licensepath ) )
    sys.exit( -1 )

licensetext = []
with open( licensepath, "r" ) as fd :
    for line in fd :
        if line.startswith( "----------" ) :
            break
        licensetext.append( line.replace( "\n", "" ).replace( "\r", "" ) )

        
for txt in licensetext :
    print txt
        
def relicense( filepath, comment, licensetext = licensetext ) :
    print filepath, comment

    context = True
    cnt = 0
    for line in fileinput.FileInput( filepath, inplace = 1 ) :
        cnt = cnt + 1
        if cnt == 1 :
            sys.stderr.write( "'%s' '%s' '%s'\n" % ( line, comment, licensetext[0] ) )
            if comment == "//" and line.replace( "\n", "" ) != ( "%s" % ( comment ) ) :
                context = False
            else :
                for txt in licensetext :
                    if len( txt ) == 0 :
                        print "%s" % comment
                    else :
                        print "%-4s%s" % ( comment, txt )
        
        if context :
            if not line.startswith( comment ) :
                context = False
            else :
                continue
        
        print line.replace( "\n", "" )
# end def

def searcher( dirpath, rootdir = True ) :
    if  not rootdir \
    and os.path.exists( os.path.join( dirpath, ".git" ) ) :
        return

    
    for filename in os.listdir( dirpath ) :
        filepath = os.path.join( dirpath, filename )

        if os.path.isdir( filepath ) :
            searcher( filepath, False )
            continue
        
        _fn, fileext = os.path.splitext( filepath )
        
        if fileext in [ ".h", ".c", ".cc", ".cpp", ".js", ".casm" ] :
            relicense( filepath, "//" )
        if fileext in [ ".ll" ] :
            relicense( filepath, ";;" )
        if fileext in \
        [ ".py"
        , ".mk"
        , ".yml"
        , ".cfg"
        , ".sh"
        ] \
        or filename in \
        [ "Makefile"
        , "Doxyfile"
        , "CMakeLists.txt"
        , ".clang-format"
        ] :
            relicense( filepath, "#" )
# end def

searcher( rootdir )
