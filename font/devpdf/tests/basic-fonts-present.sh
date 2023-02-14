#!/bin/sh
#
# Copyright (C) 2022-2023 Free Software Foundation, Inc.
#
# This file is part of groff.
#
# groff is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# groff is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#

# Ensure that groff's PDF device has the copies it needs of PostScript
# device font descriptions.
#
# We need all of them except SS and ZDR.

# Our configure script warns that gropdf will not be fully functional if
# gs (Ghostscript) is not available.  Namely, we will have only
# descriptions for the PostScript Level 1 base 14 fonts, not the Level 2
# base 35 fonts.  We're using the presence of the command as a proxy for
# the availabilty of the fonts because locating the latter is
# unreliable (package names, directory locations, and file names all
# vary), but if the command is present, the fonts usually are too.
if ! command -v gs
then
    echo "$0: gs command not available; skipping test" >&2
    exit 77 # skip
fi

# Locate directory containing the font descriptions for the PostScript
# device.
for srcroot in . .. ../..
do
    # Look for a source file characteristic of the groff source tree.
    if ! [ -f "$srcroot"/ChangeLog.115 ]
    then
        continue
    fi

    d=$srcroot/font/devps
    if [ -d "$d" ]
    then
        devps_fontsrcdir=$d
        break
    fi
done

# If we can't find it, we can't test.
if [ -z "$devps_fontsrcdir" ]
then
    echo "$0: cannot locate font descriptions for 'ps' device;" \
        "skipping test" >&2
    exit 77 # skip
fi

devpdf_fontbuilddir="${abs_top_builddir:-.}"/font/devpdf

psfonts=$(cd "$devps_fontsrcdir" && ls [A-Z]* \
    | grep -Evx '(DESC\.in|SS|ZDR)')

fail=

for f in $psfonts
do
    printf "checking for font description %s...\n" "$f" >&2
    if ! [ -f "$devpdf_fontbuilddir"/"$f" ]
    then
        echo FAILED >&2
        fail=yes
    fi
done

test -z "$fail"

# vim:set ai et sw=4 ts=4 tw=72:
