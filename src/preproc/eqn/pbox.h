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

// ms.eqn relies on this!

#define LINE_STRING "10"
#define MARK_OR_LINEUP_FLAG_REG "MK"

#define WIDTH_FORMAT PREFIX "w%d"
#define HEIGHT_FORMAT PREFIX "h%d"
#define DEPTH_FORMAT PREFIX "d%d"
#define TOTAL_FORMAT PREFIX "t%d"
#define SIZE_FORMAT PREFIX "z%d"
#define SMALL_SIZE_FORMAT PREFIX "Z%d"
#define SUP_RAISE_FORMAT PREFIX "p%d"
#define SUB_LOWER_FORMAT PREFIX "b%d"
#define SUB_KERN_FORMAT PREFIX "k%d"
#define FONT_FORMAT PREFIX "f%d"
#define SKEW_FORMAT PREFIX "s%d"
#define LEFT_WIDTH_FORMAT PREFIX "lw%d"
#define LEFT_DELIM_STRING_FORMAT PREFIX "l%d"
#define RIGHT_DELIM_STRING_FORMAT PREFIX "r%d"
#define SQRT_STRING_FORMAT PREFIX "sqr%d"
#define SQRT_WIDTH_FORMAT PREFIX "sq%d"
#define BASELINE_SEP_FORMAT PREFIX "bs%d"
// this needs two parameters, the uid and the column index
#define COLUMN_WIDTH_FORMAT PREFIX "cw%d,%d"

#define BAR_STRING PREFIX "sqb"
#define TEMP_REG PREFIX "temp"
#define MARK_REG PREFIX "mark"
#define MARK_WIDTH_REG PREFIX "mwidth"
#define SAVED_MARK_REG PREFIX "smark"
#define MAX_SIZE_REG PREFIX "mxsz"
#define REPEAT_APPEND_STRING_MACRO PREFIX "ras"
#define TOP_HEIGHT_REG PREFIX "th"
#define TOP_DEPTH_REG PREFIX "td"
#define MID_HEIGHT_REG PREFIX "mh"
#define MID_DEPTH_REG PREFIX "md"
#define BOT_HEIGHT_REG PREFIX "bh"
#define BOT_DEPTH_REG PREFIX "bd"
#define EXT_HEIGHT_REG PREFIX "eh"
#define EXT_DEPTH_REG PREFIX "ed"
#define TOTAL_HEIGHT_REG PREFIX "tot"
#define DELTA_REG PREFIX "delta"
#define DELIM_STRING PREFIX "delim"
#define DELIM_WIDTH_REG PREFIX "dwidth"
#define SAVED_FONT_REG PREFIX "sfont"
#define SAVED_PREV_FONT_REG PREFIX "spfont"
#define SAVED_INLINE_FONT_REG PREFIX "sifont"
#define SAVED_INLINE_PREV_FONT_REG PREFIX "sipfont"
#define SAVED_SIZE_REG PREFIX "ssize"
#define SAVED_INLINE_SIZE_REG PREFIX "sisize"
#define SAVED_INLINE_PREV_SIZE_REG PREFIX "sipsize"
#define SAVE_FONT_STRING PREFIX "sfont"
#define RESTORE_FONT_STRING PREFIX "rfont"
#define INDEX_REG PREFIX "i"
#define TEMP_MACRO PREFIX "tempmac"

#define DELIMITER_CHAR "\\(EQ"

const int CRAMPED_SCRIPT_STYLE = 0;
const int SCRIPT_STYLE = 1;
const int CRAMPED_DISPLAY_STYLE = 2;
const int DISPLAY_STYLE = 3;

extern int script_style(int);
extern int cramped_style(int);

const int ORDINARY_TYPE = 0;
const int OPERATOR_TYPE = 1;
const int BINARY_TYPE = 2;
const int RELATION_TYPE = 3;
const int OPENING_TYPE = 4;
const int CLOSING_TYPE = 5;
const int PUNCTUATION_TYPE = 6;
const int INNER_TYPE = 7;
const int SUPPRESS_TYPE = 8;

void set_script_size();

enum { HINT_PREV_IS_ITALIC = 01, HINT_NEXT_IS_ITALIC = 02 };

extern const char *current_roman_font;

// Local Variables:
// fill-column: 72
// mode: C++
// End:
// vim: set cindent noexpandtab shiftwidth=2 textwidth=72:
