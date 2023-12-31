/* Copyright (C) 1989-2020 Free Software Foundation, Inc.
     Written by James Clark (jjc@jclark.com)

This file is part of groff.

groff is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or
(at your option) any later version.

groff is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include "lib.h"
#include "cset.h"
#include "stringclass.h"
#include "lf.h"

#include <ctype.h>

extern void change_filename(const char *);
extern void change_lineno(int);

bool interpret_lf_args(const char *p)
{
  while (*p == ' ')
    p++;
  if (!csdigit(*p))
    return false;
  int ln = 0;
  do {
    ln *= 10;
    ln += *p++ - '0';
  } while (csdigit(*p));
  if (*p != ' ' && *p != '\n' && *p != '\0')
    return false;
  while (*p == ' ')
    p++;
  if (*p == '\0' || *p == '\n')  {
    change_lineno(ln);
    return true;
  }
  const char *q;
  for (q = p;
       *q != '\0' && *q != ' ' && *q != '\n' && *q != '\\';
       q++)
    ;
  string tem(p, q - p);
  while (*q == ' ')
    q++;
  if (*q != '\n' && *q != '\0')
    return false;
  tem += '\0';
  change_filename(tem.contents());
  change_lineno(ln);
  return true;
}

#if defined(__MSDOS__) || (defined(_WIN32) && !defined(__CYGWIN__))
void normalize_for_lf (string &fn)
{
  int fnlen = fn.length();
  for (int i = 0; i < fnlen; i++) {
    if (fn[i] == '\\')
      fn[i] = '/';
  }
}
#else
void normalize_for_lf (string &)
{
}
#endif

// Local Variables:
// fill-column: 72
// mode: C++
// End:
// vim: set cindent noexpandtab shiftwidth=2 textwidth=72:
