#
#   Copyright (c) 2014-2017 CASM Organization https://casm-lang.org
#   All rights reserved.
#
#   Developed by: Philipp Paulweber
#                 Emmanuel Pescosta
#                 https://github.com/casm-lang/libstdhl
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

    xml = False
    context = True
    cnt = 0
    for line in fileinput.FileInput( filepath, inplace = 1 ) :

        if line.startswith( "<!DOCTYPE" ) \
        or line.startswith( "<!doctype" ) \
        or line.startswith( "<?xml" ) \
        :
            xml = True
            print "%s" % line.replace( "\n", "" )
            continue
        # end if

        cnt = cnt + 1
        if cnt == 1 :
            sys.stderr.write( "'%s' '%s' '%s'\n" % ( line, comment, licensetext[0] ) )
            if comment == "//" and line.replace( "\n", "" ) != ( "%s" % ( comment ) ) :
                context = False
            elif comment == "<!--" and line.replace( "\n", "" ) != ( "%s" % ( comment ) ) :
                context = False
            else :
                if xml :
                    print "%s" % comment
                # end if
                for txt in licensetext :
                    if len( txt ) == 0 :
                        if xml :
                            print ""
                        else :
                            print "%s" % comment
                        # end if
                    else :
                        if xml :
                            print "%-4s%s" % ( "", txt )
                        else :
                            print "%-4s%s" % ( comment, txt )
                        # end if
                    # end if
                # end for
            # end if
        # end if
        if context :
            if xml :
                if "-->" in line:
                    context = False
                else :
                    continue
                # end if
            else :
                if not line.startswith( comment ) :
                    context = False
                else :
                    continue
                # end if
            # end if
        # end if
        
        print line.replace( "\n", "" )
    # end for
# end def

def searcher( dirpath, rootdir = True ) :
    if  not rootdir and \
    (  os.path.exists( os.path.join( dirpath, ".git" ) )
    or os.path.exists( os.path.join( dirpath, ".3rd-party" ) )
    or os.path.exists( os.path.join( dirpath, ".generated" ) )
    ):
        return

    for filename in os.listdir( dirpath ) :
        filepath = os.path.join( dirpath, filename )

        if  os.path.isdir( filepath ) \
        and filename in \
        [ "node_modules"
        , "obj"
        ] :
            continue

        if os.path.isdir( filepath ) :
            searcher( filepath, False )
            continue
        
        _fn, fileext = os.path.splitext( filepath )
        
        if fileext in \
        [ ".h"                # C and C++ Header
        , ".hpp"              # C/C++ Header
        , ".c"                # C Source
        , ".cc"               # C++ Source
        , ".cpp"              # C++ Source
        , ".js"               # Javascript
        , ".ts"               # Typescript
        , ".casm"             # CASM Specification
        ] :
            relicense( filepath, "//" )

        if fileext in \
        [ ".ll"               # LLVM Source
        ] :
            relicense( filepath, ";;" )

        if fileext in \
        [ ".el"   # Emacs-Lisp Source
        ] :
            relicense( filepath, ";" )

        if fileext in \
        [ ".org"   # Org-Mode Source
        ] :
            relicense( filepath, "# " )

        if fileext in \
        [ ".py"               # Python Script
        , ".mk"               # Makefile Script
        , ".cmake"            # CMake Script
        , ".yml"              # YAML Configuration
        , ".cfg"              # UNIX Configuration
        , ".sh"               # BASH Shell Script
        ] \
        or filename in \
        [ "Makefile"          # Makefile Script
        , "Doxyfile"          # Doxygen Configuration
        , "CMakeLists.txt"    # CMake Script
        , ".clang-format"     # Clang Format Configuration
        ] :
            relicense( filepath, "#" )

        if fileext in \
        [ ".html"             # HTML Source
        , ".xml"              # XML Source
        , ".xsl"              # XSL Source
        ] :
            relicense( filepath, "<!--" )
# end def

searcher( rootdir )
