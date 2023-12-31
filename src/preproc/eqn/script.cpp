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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>

#include "eqn.h"
#include "pbox.h"

class script_box : public pointer_box {
private:
  box *sub;
  box *sup;
public:
  script_box(box *, box *, box *);
  ~script_box();
  int compute_metrics(int);
  void output();
  void debug_print();
  int left_is_italic();
  void hint(unsigned);
  void diagnose_tab_stop_usage(int);
};

/* The idea is that the script should attach to the rightmost box
of a list. For example, given '2x sup 3', the superscript should
attach to 'x' rather than '2x'. */

box *make_script_box(box *nuc, box *sub, box *sup)
{
  list_box *b = nuc->to_list_box();
  if (b != 0) {
    b->list.p[b->list.len-1] = make_script_box(b->list.p[b->list.len - 1],
					       sub,
					       sup);
    return b;
  }
  else
    return new script_box(nuc, sub, sup);
}

script_box::script_box(box *pp, box *qq, box *rr)
: pointer_box(pp), sub(qq), sup(rr)
{
}

script_box::~script_box()
{
  delete sub;
  delete sup;
}

int script_box::left_is_italic()
{
  return p->left_is_italic();
}

int script_box::compute_metrics(int style)
{
  int res = p->compute_metrics(style);
  p->compute_subscript_kern();
  printf(".nr " SIZE_FORMAT " \\n[.ps]\n", uid);
  if (!(style <= SCRIPT_STYLE && one_size_reduction_flag))
    set_script_size();
  printf(".nr " SMALL_SIZE_FORMAT " \\n[.ps]\n", uid);
  if (sub != 0)
    sub->compute_metrics(cramped_style(script_style(style)));
  if (sup != 0)
    sup->compute_metrics(script_style(style));
  // 18a
  if (p->is_char()) {
    printf(".nr " SUP_RAISE_FORMAT " 0\n", uid);
    printf(".nr " SUB_LOWER_FORMAT " 0\n", uid);
  }
  else {
    printf(".nr " SUP_RAISE_FORMAT " \\n[" HEIGHT_FORMAT "]-%dM>?0\n",
	   uid, p->uid, get_param("sup_drop"));
    printf(".nr " SUB_LOWER_FORMAT " \\n[" DEPTH_FORMAT "]+%dM\n",
	   uid, p->uid, get_param("sub_drop"));
  }
  printf(".ps \\n[" SIZE_FORMAT "]u\n", uid);
  if (sup == 0) {
    assert(sub != 0);
    // 18b
    printf(".nr " SUB_LOWER_FORMAT " \\n[" SUB_LOWER_FORMAT "]>?%dM>?(\\n["
	   HEIGHT_FORMAT "]-(%dM*4/5))\n",
	   uid, uid, get_param("sub1"), sub->uid,
	   get_param("x_height"));
  }
  else {
    // sup != 0
    // 18c
    int pos;
    if (style == DISPLAY_STYLE)
      pos = get_param("sup1");
    else if (style & 1)		// not cramped
      pos = get_param("sup2");
    else
      pos = get_param("sup3");
    printf(".nr " SUP_RAISE_FORMAT " \\n[" SUP_RAISE_FORMAT
	   "]>?%dM>?(\\n[" DEPTH_FORMAT "]+(%dM/4))\n",
	   uid, uid, pos, sup->uid, get_param("x_height"));
    // 18d
    if (sub != 0) {
      printf(".nr " SUB_LOWER_FORMAT " \\n[" SUB_LOWER_FORMAT "]>?%dM\n",
	     uid, uid, get_param("sub2"));
      // 18e
      printf(".nr " TEMP_REG " \\n[" DEPTH_FORMAT "]-\\n["
	     SUP_RAISE_FORMAT "]+\\n[" HEIGHT_FORMAT "]-\\n["
	     SUB_LOWER_FORMAT "]+(4*%dM)\n",
	     sup->uid, uid, sub->uid, uid,
	     get_param("default_rule_thickness"));
      printf(".if \\n[" TEMP_REG "] \\{");
      printf(".nr " SUB_LOWER_FORMAT " +\\n[" TEMP_REG "]\n", uid);
      printf(".nr " TEMP_REG " (%dM*4/5)-\\n[" SUP_RAISE_FORMAT
	     "]+\\n[" DEPTH_FORMAT "]>?0\n",
	     get_param("x_height"), uid, sup->uid);
      printf(".nr " SUP_RAISE_FORMAT " +\\n[" TEMP_REG "]\n", uid);
      printf(".nr " SUB_LOWER_FORMAT " -\\n[" TEMP_REG "]\n", uid);
      printf(".\\}\n");
    }
  }
  printf(".nr " WIDTH_FORMAT " 0\\n[" WIDTH_FORMAT "]", uid, p->uid);
  if (sub != 0 && sup != 0)
    printf("+((\\n[" WIDTH_FORMAT "]-\\n[" SUB_KERN_FORMAT "]>?\\n["
	   WIDTH_FORMAT "])+%dM)>?0\n",
	   sub->uid, p->uid, sup->uid, get_param("script_space"));
  else if (sub != 0)
    printf("+(\\n[" WIDTH_FORMAT "]-\\n[" SUB_KERN_FORMAT "]+%dM)>?0\n",
	   sub->uid, p->uid, get_param("script_space"));
  else if (sup != 0)
    printf("+(\\n[" WIDTH_FORMAT "]+%dM)>?0\n", sup->uid,
	   get_param("script_space"));
  else
    printf("\n");
  printf(".nr " HEIGHT_FORMAT " \\n[" HEIGHT_FORMAT "]",
	 uid, p->uid);
  if (sup != 0)
    printf(">?(\\n[" SUP_RAISE_FORMAT "]+\\n[" HEIGHT_FORMAT "])",
	   uid, sup->uid);
  if (sub != 0)
    printf(">?(-\\n[" SUB_LOWER_FORMAT "]+\\n[" HEIGHT_FORMAT "])",
	   uid, sub->uid);
  printf("\n");
  printf(".nr " DEPTH_FORMAT " \\n[" DEPTH_FORMAT "]",
	 uid, p->uid);
  if (sub != 0)
    printf(">?(\\n[" SUB_LOWER_FORMAT "]+\\n[" DEPTH_FORMAT "])",
	   uid, sub->uid);
  if (sup != 0)
    printf(">?(-\\n[" SUP_RAISE_FORMAT "]+\\n[" DEPTH_FORMAT "])",
	   uid, sup->uid);
  printf("\n");
  return res;
}

void script_box::output()
{
  if (output_format == troff) {
    p->output();
    if (sup != 0) {
      printf("\\Z" DELIMITER_CHAR);
      printf("\\v'-\\n[" SUP_RAISE_FORMAT "]u'", uid);
      printf("\\s[\\n[" SMALL_SIZE_FORMAT "]u]", uid);
      sup->output();
      printf("\\s[\\n[" SIZE_FORMAT "]u]", uid);
      printf(DELIMITER_CHAR);
    }
    if (sub != 0) {
      printf("\\Z" DELIMITER_CHAR);
      printf("\\v'\\n[" SUB_LOWER_FORMAT "]u'", uid);
      printf("\\s[\\n[" SMALL_SIZE_FORMAT "]u]", uid);
      printf("\\h'-\\n[" SUB_KERN_FORMAT "]u'", p->uid);
      sub->output();
      printf("\\s[\\n[" SIZE_FORMAT "]u]", uid);
      printf(DELIMITER_CHAR);
    }
    printf("\\h'\\n[" WIDTH_FORMAT "]u-\\n[" WIDTH_FORMAT "]u'",
	   uid, p->uid);
  }
  else if (output_format == mathml) {
    if (sup != 0 && sub != 0) {
      printf("<msubsup>");
      p->output();
      sub->output();
      sup->output();
      printf("</msubsup>");
    }
    else if (sup != 0) {
      printf("<msup>");
      p->output();
      sup->output();
      printf("</msup>");
  }
  else if (sub != 0) {
      printf("<msub>");
      p->output();
      sub->output();
      printf("</msub>");
    }
  }
}

void script_box::hint(unsigned flags)
{
  p->hint(flags & ~HINT_NEXT_IS_ITALIC);
}

void script_box::debug_print()
{
  fprintf(stderr, "{ ");
  p->debug_print();
  fprintf(stderr, " }");
  if (sub) {
    fprintf(stderr, " sub { ");
    sub->debug_print();
    fprintf(stderr, " }");
  }
  if (sup) {
    fprintf(stderr, " sup { ");
    sup->debug_print();
    fprintf(stderr, " }");
  }
}

void script_box::diagnose_tab_stop_usage(int level)
{
  if (sup)
    sup->diagnose_tab_stop_usage(level + 1);
  if (sub)
    sub->diagnose_tab_stop_usage(level + 1);
  p->diagnose_tab_stop_usage(level);
}

// Local Variables:
// fill-column: 72
// mode: C++
// End:
// vim: set cindent noexpandtab shiftwidth=2 textwidth=72:
