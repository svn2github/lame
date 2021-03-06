/*
 *	ID3 and Xing VBR tagging for LAME.
 *
 * Copyright (C) 1999 A.L. Faber
 *           (C) 2000 Don Melton.
 *           (C) 2004 Takehiro TOMINAGA
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef LAME_TAGS_H
#define LAME_TAGS_H

int InitVbrTag(lame_t gfc);
void AddVbrFrame(lame_t gfc);

/* write tag into "buf" */
int id3tag_write_v2(lame_t gfc, unsigned char *buf, size_t size);
int id3tag_write_v1(lame_t gfc, unsigned char *buf, size_t size);

#endif
