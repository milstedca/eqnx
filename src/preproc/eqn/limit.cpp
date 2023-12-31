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

class limit_box : public box {
private:
  box *p;
  box *from;
  box *to;
public:
  limit_box(box *, box *, box *);
  ~limit_box();
  int compute_metrics(int);
  void output();
  void debug_print();
  void diagnose_tab_stop_usage(int);
};

box *make_limit_box(box *pp, box *qq, box *rr)
{
  return new limit_box(pp, qq, rr);
}

limit_box::limit_box(box *pp, box *qq, box *rr)
: p(pp), from(qq), to(rr)
{
  spacing_type = p->spacing_type;
}

limit_box::~limit_box()
{
  delete p;
  delete from;
  delete to;
}

int limit_box::compute_metrics(int style)
{
  printf(".nr " SIZE_FORMAT " \\n[.ps]\n", uid);
  if (!(style <= SCRIPT_STYLE && one_size_reduction_flag))
    set_script_size();
  printf(".nr " SMALL_SIZE_FORMAT " \\n[.ps]\n", uid);
  int res = 0;
  int mark_uid = -1;
  if (from != 0) {
    res = from->compute_metrics(cramped_style(script_style(style)));
    if (res)
      mark_uid = from->uid;
  }
  if (to != 0) {
    int r = to->compute_metrics(script_style(style));
    if (res && r)
      error("multiple marks and lineups");
    else  {
      mark_uid = to->uid;
      res = r;
    }
  }
  printf(".ps \\n[" SIZE_FORMAT "]u\n", uid);
  int r = p->compute_metrics(style);
  p->compute_subscript_kern();
  if (res && r)
    error("multiple marks and lineups");
  else {
    mark_uid = p->uid;
    res = r;
  }
  printf(".nr " LEFT_WIDTH_FORMAT " "
	 "0\\n[" WIDTH_FORMAT "]",
	 uid, p->uid);
  if (from != 0)
    printf(">?(\\n[" SUB_KERN_FORMAT "]+\\n[" WIDTH_FORMAT "])",
	   p->uid, from->uid);
  if (to != 0)
    printf(">?(-\\n[" SUB_KERN_FORMAT "]+\\n[" WIDTH_FORMAT "])",
	   p->uid, to->uid);
  printf("/2\n");
  printf(".nr " WIDTH_FORMAT " "
	 "0\\n[" WIDTH_FORMAT "]",
	 uid, p->uid);
  if (from != 0)
    printf(">?(-\\n[" SUB_KERN_FORMAT "]+\\n[" WIDTH_FORMAT "])",
	   p->uid, from->uid);
  if (to != 0)
    printf(">?(\\n[" SUB_KERN_FORMAT "]+\\n[" WIDTH_FORMAT "])",
	   p->uid, to->uid);
  printf("/2+\\n[" LEFT_WIDTH_FORMAT "]\n", uid);
  printf(".nr " WIDTH_FORMAT " 0\\n[" WIDTH_FORMAT "]", uid, p->uid);
  if (to != 0)
    printf(">?\\n[" WIDTH_FORMAT "]", to->uid);
  if (from != 0)
    printf(">?\\n[" WIDTH_FORMAT "]", from->uid);
  printf("\n");
  if (res)
    printf(".nr " MARK_REG " +(\\n[" LEFT_WIDTH_FORMAT "]"
	   "-(\\n[" WIDTH_FORMAT "]/2))\n",
	   uid, mark_uid);
  if (to != 0) {
    printf(".nr " SUP_RAISE_FORMAT " %dM+\\n[" DEPTH_FORMAT
	   "]>?%dM+\\n[" HEIGHT_FORMAT "]\n",
	   uid, get_param("big_op_spacing1"), to->uid,
	   get_param("big_op_spacing3"), p->uid);
    printf(".nr " HEIGHT_FORMAT " \\n[" SUP_RAISE_FORMAT "]+\\n["
	   HEIGHT_FORMAT "]+%dM\n",
	   uid, uid, to->uid, get_param("big_op_spacing5"));
  }
  else
    printf(".nr " HEIGHT_FORMAT " \\n[" HEIGHT_FORMAT "]\n", uid, p->uid);
  if (from != 0) {
    printf(".nr " SUB_LOWER_FORMAT " %dM+\\n[" HEIGHT_FORMAT
	   "]>?%dM+\\n[" DEPTH_FORMAT "]\n",
	   uid, get_param("big_op_spacing2"), from->uid,
	   get_param("big_op_spacing4"), p->uid);
    printf(".nr " DEPTH_FORMAT " \\n[" SUB_LOWER_FORMAT "]+\\n["
	   DEPTH_FORMAT "]+%dM\n",
	   uid, uid, from->uid, get_param("big_op_spacing5"));
  }
  else
    printf(".nr " DEPTH_FORMAT " \\n[" DEPTH_FORMAT "]\n", uid, p->uid);
  return res;
}

void limit_box::output()
{
  if (output_format == troff) {
    printf("\\s[\\n[" SMALL_SIZE_FORMAT "]u]", uid);
    if (to != 0) {
      printf("\\Z" DELIMITER_CHAR);
      printf("\\v'-\\n[" SUP_RAISE_FORMAT "]u'", uid);
      printf("\\h'\\n[" LEFT_WIDTH_FORMAT "]u"
	     "+(-\\n[" WIDTH_FORMAT "]u+\\n[" SUB_KERN_FORMAT "]u/2u)'",
	     uid, to->uid, p->uid);
      to->output();
      printf(DELIMITER_CHAR);
    }
    if (from != 0) {
      printf("\\Z" DELIMITER_CHAR);
      printf("\\v'\\n[" SUB_LOWER_FORMAT "]u'", uid);
      printf("\\h'\\n[" LEFT_WIDTH_FORMAT "]u"
	     "+(-\\n[" SUB_KERN_FORMAT "]u-\\n[" WIDTH_FORMAT "]u/2u)'",
	     uid, p->uid, from->uid);
      from->output();
      printf(DELIMITER_CHAR);
    }
    printf("\\s[\\n[" SIZE_FORMAT "]u]", uid);
    printf("\\Z" DELIMITER_CHAR);
    printf("\\h'\\n[" LEFT_WIDTH_FORMAT "]u"
	   "-(\\n[" WIDTH_FORMAT "]u/2u)'",
	   uid, p->uid);
    p->output();
    printf(DELIMITER_CHAR);
    printf("\\h'\\n[" WIDTH_FORMAT "]u'", uid);
  }
  else if (output_format == mathml) {
    if (from != 0 && to != 0) {
      printf("<munderover>");
      p->output();
      from->output();
      to->output();
      printf("</munderover>");
    }
    else if (from != 0) {
      printf("<munder>");
      p->output();
      from->output();
      printf("</munder>");
    }
    else if (to != 0) {
      printf("<mover>");
      p->output();
      to->output();
      printf("</mover>");
    }
  }
}

void limit_box::debug_print()
{
  fprintf(stderr, "{ ");
  p->debug_print();
  fprintf(stderr, " }");
  if (from) {
    fprintf(stderr, " from { ");
    from->debug_print();
    fprintf(stderr, " }");
  }
  if (to) {
    fprintf(stderr, " to { ");
    to->debug_print();
    fprintf(stderr, " }");
  }
}

void limit_box::diagnose_tab_stop_usage(int level)
{
  if (to)
    to->diagnose_tab_stop_usage(level + 1);
  if (from)
    from->diagnose_tab_stop_usage(level + 1);
  p->diagnose_tab_stop_usage(level + 1);
}

// Local Variables:
// fill-column: 72
// mode: C++
// End:
// vim: set cindent noexpandtab shiftwidth=2 textwidth=72:
