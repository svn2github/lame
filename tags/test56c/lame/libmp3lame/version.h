/*
 *      Version numbering for LAME.
 *
 *      Copyright (c) 1999 A.L. Faber
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef LAME_VERSION_H
#define LAME_VERSION_H

# include <stdio.h>

# define LAME_URL              "http://www.mp3dev.org/"

# define LAME_MAJOR_VERSION      3      /* Major version number */
# define LAME_MINOR_VERSION     88      /* Minor version number */
# define LAME_ALPHA_VERSION      7      /* Patch level of alpha version, otherwise zero */
# define LAME_BETA_VERSION       0      /* Patch level of beta  version, otherwise zero */

# define PSY_MAJOR_VERSION       0      /* Major version number */
# define PSY_MINOR_VERSION      80      /* Minor version number */
# define PSY_ALPHA_VERSION       0      /* Set number if this is an alpha version, otherwise zero */
# define PSY_BETA_VERSION        0      /* Set number if this is a beta version, otherwise zero */

# define MP3X_MAJOR_VERSION      0      /* Major version number */
# define MP3X_MINOR_VERSION     82      /* Minor version number */
# define MP3X_ALPHA_VERSION      0      /* Set number if this is an alpha version, otherwise zero */
# define MP3X_BETA_VERSION       0      /* Set number if this is a beta version, otherwise zero */

void get_lame_version       ( char *strbuf, size_t buflen, const char *prefix ); /* returns lame long version number string */
void get_lame_short_version ( char *strbuf, size_t buflen, const char *prefix ); /* returns lame short version number string */
void get_psy_version        ( char *strbuf, size_t buflen, const char *prefix ); /* returns psy model version number string */
void get_mp3x_version       ( char *strbuf, size_t buflen, const char *prefix ); /* returns mp3x version number string */
void get_lame_url           ( char *strbuf, size_t buflen, const char *prefix ); /* returns the lame home page */

#endif  /* LAME_VERSION_H */

/* End of version.h */
