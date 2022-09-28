#!/bin/sh
#
# Copyright (C) 2022 Free Software Foundation, Inc.
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

groff="${abs_top_builddir:-.}/test-groff"

fail=

wail () {
  echo "...FAILED" >&2
  fail=yes
}

# Regression-test Savannah #63011.
#
# A handful of escape sequences bizarrely accept newlines as argument
# delimiters.  Don't throw diagnostics if they are used.

input=".sp
\b
+|+
That's my bracket.
.pl \n(nlu"

echo "checking that newline is accepted as delimiter to 'b' escape" >&2
output=$(printf "%s\n" "$input" | "$groff" -Tascii -ww -z 2>&1)
test -z "$output" || wail

input="caf\o
e\(aa
in Paris
.pl \n(nlu"

echo "checking that newline is accepted as delimiter to 'o' escape" >&2
output=$(printf "%s\n" "$input" | "$groff" -Tascii -ww -z 2>&1)
test -z "$output" || wail

input="The word _girth_ has a width of
.nr w \w
girth
\nw units.
.pl \n(nlu"

echo "checking that newline is accepted as delimiter to 'w' escape" >&2
output=$(printf "%s\n" "$input" | "$groff" -Tascii -ww -z 2>&1)
test -z "$output" || wail

input="I am issuing a device control command
\X
tty: bogus
now.
.pl \n(nlu"

echo "checking that newline is accepted as delimiter to 'X' escape" >&2
output=$(printf "%s\n" "$input" | "$groff" -Tascii -ww -z 2>&1)
test -z "$output" || wail

test -z "$fail"

# vim:set autoindent expandtab shiftwidth=2 tabstop=2 textwidth=72:
