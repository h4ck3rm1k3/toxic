/*  toxic_strings.h
 *
 *
 *  Copyright (C) 2014 Toxic All Rights Reserved.
 *
 *  This file is part of Toxic.
 *
 *  Toxic is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Toxic is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Toxic.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _toxic_strings_h
#define _toxic_strings_h

#include "windows.h"

/* Adds char to line at pos */
void add_char_to_buf(ChatContext *ctx, wint_t ch);

/* Deletes the character before pos */
void del_char_buf_bck(ChatContext *ctx);

/* Deletes the character at pos */
void del_char_buf_frnt(ChatContext *ctx);

/* Deletes the line from beginning to pos */
void discard_buf(ChatContext *ctx);

/* Deletes the line from pos to len */
void kill_buf(ChatContext *ctx);

/* nulls line and sets pos, len and start to 0 */
void reset_buf(ChatContext *ctx);

/* Removes trailing spaces from line. */
void rm_trailing_spaces_buf(ChatContext *ctx);

/* looks for the first instance in list that begins with the last entered word in line according to pos,
   then fills line with the complete word. e.g. "Hello jo" would complete the line
   with "Hello john".

   list is a pointer to the list of strings being compared, n_items is the number of items
   in the list, and size is the size of each item in the list.

   Returns the difference between the old len and new len of line on success, -1 if error */
int complete_line(ChatContext *ctx, const void *list, int n_items, int size);

/* adds a line to the ln_history buffer at hst_pos and sets hst_pos to last history item. */
void add_line_to_hist(ChatContext *ctx);

/* copies history item at hst_pos to line. Sets pos and len to the len of the history item.
   hst_pos is decremented or incremented depending on key_dir.

   resets line if at end of history */
void fetch_hist_item(ChatContext *ctx, int key_dir);

#endif /* #define _toxic_strings_h */
