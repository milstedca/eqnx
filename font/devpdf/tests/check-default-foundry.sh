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

# Ensure that groff's PDF device has font description files for the
# default (unnamed) foundry's PostScript Level 2 base 35 fonts, plus
# groff's EURO.
#
# These font descriptions should be available in any gropdf
# configuration because they can be copied from the devps font
# descriptions even if the URW fonts are not available.
#
# Another test script checks the "U" (URW) foundry.

devpdf_fontbuilddir="${abs_top_builddir:-.}"/font/devpdf

base_fonts='CB
CBI
CI
CR
EURO
HB
HBI
HI
HR
S
TB
TBI
TI
TR
ZD'

additional_fonts='AB
ABI
AI
AR
BMB
BMBI
BMI
BMR
HNB
HNBI
HNI
HNR
NB
NBI
NI
NR
PB
PBI
PI
PR
ZCMI'

fail=

for f in $base_fonts
do
    printf "checking for font description %s...\n" "$f" >&2
    if ! [ -f "$devpdf_fontbuilddir"/"$f" ]
    then
        echo FAILED >&2
        fail=yes
    fi
done

test -n "$fail" && exit 1 # fail

# The "Base 35" fonts (not including the PDF base 14) are not guaranteed
# to be present even if the Ghostscript executable is installed.  Skip
# the test rather than failing if they're not present.
#
# TODO: Come up with a more sophisicated Autoconf test to detect this.

for f in $additional_fonts
do
    printf "checking for font description %s...\n" "$f" >&2
    if ! [ -f "$devpdf_fontbuilddir"/"$f" ]
    then
        echo "non-base fonts not available; skipping test" >&2
        exit 77 # skip
    fi
done

# vim:set ai et sw=4 ts=4 tw=72:
