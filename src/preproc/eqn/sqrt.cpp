/* Copyright (C) 1989-2023 Free Software Foundation, Inc.
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

#include "eqn.h"
#include "pbox.h"


class sqrt_box : public pointer_box {
public:
  sqrt_box(box *);
  int compute_metrics(int style);
  void output();
  void debug_print();
  void diagnose_tab_stop_usage(int);
};

box *make_sqrt_box(box *pp)
{
  return new sqrt_box(pp);
}

sqrt_box::sqrt_box(box *pp) : pointer_box(pp)
{
}

#define SQRT_CHAR "\\[sqrt]"
#define RADICAL_EXTENSION_CHAR "\\[sqrtex]"

#define SQRT_CHAIN "\\[sqrt\\\\n[" INDEX_REG "]]"
#define BAR_CHAIN "\\[sqrtex\\\\n[" INDEX_REG "]]"

int sqrt_box::compute_metrics(int style)
{
  // 11
  int r = p->compute_metrics(cramped_style(style));
  printf(".nr " TEMP_REG " \\n[" HEIGHT_FORMAT "]+\\n[" DEPTH_FORMAT
	 "]+%dM+(%dM/4)\n",
	 p->uid, p->uid, get_param("default_rule_thickness"),
	 (style > SCRIPT_STYLE ? get_param("x_height")
			       : get_param("default_rule_thickness")));
  printf(".nr " SIZE_FORMAT " \\n[.ps]\n", uid);
  printf(".ds " SQRT_STRING_FORMAT " " SQRT_CHAR "\n", uid);
  printf(".ds " BAR_STRING " " RADICAL_EXTENSION_CHAR "\n");
  printf(".nr " SQRT_WIDTH_FORMAT
	 " 0\\w" DELIMITER_CHAR SQRT_CHAR DELIMITER_CHAR "\n",
	 uid);
  printf(".if \\n[rst]-\\n[rsb]-%dM<\\n[" TEMP_REG "] \\{",
	 get_param("default_rule_thickness"));

  printf(".nr " INDEX_REG " 0\n"
	 ".de " TEMP_MACRO "\n"
	 ".ie c" SQRT_CHAIN " \\{"
	 ".ds " SQRT_STRING_FORMAT " " SQRT_CHAIN "\n"
	 ".ie c" BAR_CHAIN " .ds " BAR_STRING " " BAR_CHAIN "\n"
	 ".el .ds " BAR_STRING " " RADICAL_EXTENSION_CHAR "\n"
	 ".nr " SQRT_WIDTH_FORMAT
	 " 0\\w" DELIMITER_CHAR SQRT_CHAIN DELIMITER_CHAR "\n"
	 ".if \\\\n[rst]-\\\\n[rsb]-%dM<\\n[" TEMP_REG "] \\{"
	 ".nr " INDEX_REG " +1\n"
	 "." TEMP_MACRO "\n"
	 ".\\}\\}\n"
	 ".el .nr " INDEX_REG " 0-1\n"
	 "..\n"
	 "." TEMP_MACRO "\n",
	 uid, uid, get_param("default_rule_thickness"));

  printf(".if \\n[" INDEX_REG "]<0 \\{");

  // Determine the maximum point size
  printf(".ps 1000\n");
  printf(".nr " MAX_SIZE_REG " \\n[.ps]\n");
  printf(".ps \\n[" SIZE_FORMAT "]u\n", uid);
  // We define a macro that will increase the current point size
  // until we get a radical sign that's tall enough or we reach
  // the maximum point size.
  printf(".de " TEMP_MACRO "\n"
	 ".nr " SQRT_WIDTH_FORMAT
	 " 0\\w" DELIMITER_CHAR "\\*[" SQRT_STRING_FORMAT "]" DELIMITER_CHAR "\n"
	 ".if \\\\n[rst]"
	 "&(\\\\n[rst]-\\\\n[rsb]-%dM<\\n[" TEMP_REG "])"
	 "&(\\\\n[.ps]<\\n[" MAX_SIZE_REG "]) \\{"
	 ".ps +1\n"
	 "." TEMP_MACRO "\n"
	 ".\\}\n"
	 "..\n"
	 "." TEMP_MACRO "\n",
	 uid, uid, get_param("default_rule_thickness"));

  printf(".\\}\\}\n");

  printf(".nr " SMALL_SIZE_FORMAT " \\n[.ps]\n", uid);
  // set TEMP_REG to the amount by which the radical sign is too big
  printf(".nr " TEMP_REG " \\n[rst]-\\n[rsb]-%dM-\\n[" TEMP_REG "]\n",
	 get_param("default_rule_thickness"));
  // If TEMP_REG is negative, the bottom of the radical sign should
  // be -TEMP_REG above the bottom of p. If it's positive, the bottom
  // of the radical sign should be TEMP_REG/2 below the bottom of p.
  // This calculates the amount by which the baseline of the radical
  // should be raised.
  printf(".nr " SUP_RAISE_FORMAT " (-\\n[" TEMP_REG "]>?(-\\n[" TEMP_REG "]/2))"
	 "-\\n[rsb]-\\n[" DEPTH_FORMAT "]\n", uid, p->uid);
  printf(".nr " HEIGHT_FORMAT " \\n[" HEIGHT_FORMAT "]"
	 ">?(\\n[" SUP_RAISE_FORMAT "]+\\n[rst])\n",
	 uid, p->uid, uid);
  printf(".nr " DEPTH_FORMAT " \\n[" DEPTH_FORMAT "]"
	 ">?(-\\n[" SUP_RAISE_FORMAT "]-\\n[rsb])\n",
	 uid, p->uid, uid);
  // Do this last, so we don't lose height and depth information on
  // the radical sign.
  // Remember that the width of the bar might be greater than the width of p.

  printf(".nr " TEMP_REG " "
	 "\\n[" WIDTH_FORMAT "]"
	 ">?\\w" DELIMITER_CHAR "\\*[" BAR_STRING "]" DELIMITER_CHAR "\n",
	 p->uid);
  printf(".as " SQRT_STRING_FORMAT " "
	 "\\l'\\n[" TEMP_REG "]u\\&\\*[" BAR_STRING "]'\n",
	 uid);
  printf(".nr " WIDTH_FORMAT " \\n[" TEMP_REG "]"
	 "+\\n[" SQRT_WIDTH_FORMAT "]\n",
	 uid, uid);

  if (r)
    printf(".nr " MARK_REG " +\\n[" SQRT_WIDTH_FORMAT "]\n", uid);
  // the top of the bar might be higher than the top of the radical sign
  printf(".nr " HEIGHT_FORMAT " \\n[" HEIGHT_FORMAT "]"
	 ">?(\\n[" SUP_RAISE_FORMAT "]+\\n[rst])\n",
	 uid, p->uid, uid);
  // put a bit of extra space above the bar
  printf(".nr " HEIGHT_FORMAT " +%dM\n", uid,
	 get_param("default_rule_thickness"));
  printf(".ps \\n[" SIZE_FORMAT "]u\n", uid);
  return r;
}

void sqrt_box::output()
{
  if (output_format == troff) {
    printf("\\Z" DELIMITER_CHAR);
    printf("\\s[\\n[" SMALL_SIZE_FORMAT "]u]", uid);
    printf("\\v'-\\n[" SUP_RAISE_FORMAT "]u'", uid);
    printf("\\*[" SQRT_STRING_FORMAT "]", uid);
    printf("\\s[\\n[" SIZE_FORMAT "]u]", uid);
    printf(DELIMITER_CHAR);

    printf("\\Z" DELIMITER_CHAR);
    printf("\\h'\\n[" WIDTH_FORMAT "]u-\\n[" WIDTH_FORMAT "]u"
	   "+\\n[" SQRT_WIDTH_FORMAT "]u/2u'",
	   uid, p->uid, uid);
    p->output();
    printf(DELIMITER_CHAR);

    printf("\\h'\\n[" WIDTH_FORMAT "]u'", uid);
  }
  else if (output_format == mathml) {
    printf("<msqrt>");
    p->output();
    printf("</msqrt>");
  }
}

void sqrt_box::debug_print()
{
  fprintf(stderr, "sqrt { ");
  p->debug_print();
  fprintf(stderr, " }");
}

void sqrt_box::diagnose_tab_stop_usage(int level)
{
  p->diagnose_tab_stop_usage(level + 1);
}

// Local Variables:
// fill-column: 72
// mode: C++
// End:
// vim: set cindent noexpandtab shiftwidth=2 textwidth=72:
