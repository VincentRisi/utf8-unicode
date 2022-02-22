/*
 * Copyright (C) 1999-2001, 2016 Free Software Foundation, Inc.
 * This file is part of the GNU LIBICONV Library.
 *
 * The GNU LIBICONV Library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * The GNU LIBICONV Library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with the GNU LIBICONV Library; see the file COPYING.LIB.
 * If not, see <https://www.gnu.org/licenses/>.
 */

/*
 * CP855
 */

static const unsigned short cp855_2uni[128] = {
  /* 0x80 */
  0x0452, 0x0402, 0x0453, 0x0403, 0x0451, 0x0401, 0x0454, 0x0404,
  0x0455, 0x0405, 0x0456, 0x0406, 0x0457, 0x0407, 0x0458, 0x0408,
  /* 0x90 */
  0x0459, 0x0409, 0x045a, 0x040a, 0x045b, 0x040b, 0x045c, 0x040c,
  0x045e, 0x040e, 0x045f, 0x040f, 0x044e, 0x042e, 0x044a, 0x042a,
  /* 0xa0 */
  0x0430, 0x0410, 0x0431, 0x0411, 0x0446, 0x0426, 0x0434, 0x0414,
  0x0435, 0x0415, 0x0444, 0x0424, 0x0433, 0x0413, 0x00ab, 0x00bb,
  /* 0xb0 */
  0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x0445, 0x0425, 0x0438,
  0x0418, 0x2563, 0x2551, 0x2557, 0x255d, 0x0439, 0x0419, 0x2510,
  /* 0xc0 */
  0x2514, 0x2534, 0x252c, 0x251c, 0x2500, 0x253c, 0x043a, 0x041a,
  0x255a, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256c, 0x00a4,
  /* 0xd0 */
  0x043b, 0x041b, 0x043c, 0x041c, 0x043d, 0x041d, 0x043e, 0x041e,
  0x043f, 0x2518, 0x250c, 0x2588, 0x2584, 0x041f, 0x044f, 0x2580,
  /* 0xe0 */
  0x042f, 0x0440, 0x0420, 0x0441, 0x0421, 0x0442, 0x0422, 0x0443,
  0x0423, 0x0436, 0x0416, 0x0432, 0x0412, 0x044c, 0x042c, 0x2116,
  /* 0xf0 */
  0x00ad, 0x044b, 0x042b, 0x0437, 0x0417, 0x0448, 0x0428, 0x044d,
  0x042d, 0x0449, 0x0429, 0x0447, 0x0427, 0x00a7, 0x25a0, 0x00a0,
};

static int
cp855_mbtowc (conv_t conv, ucs4_t *pwc, const unsigned char *s, size_t n)
{
  unsigned char c = *s;
  if (c < 0x80)
    *pwc = (ucs4_t) c;
  else
    *pwc = (ucs4_t) cp855_2uni[c-0x80];
  return 1;
}

static const unsigned char cp855_page00[32] = {
  0xff, 0x00, 0x00, 0x00, 0xcf, 0x00, 0x00, 0xfd, /* 0xa0-0xa7 */
  0x00, 0x00, 0x00, 0xae, 0x00, 0xf0, 0x00, 0x00, /* 0xa8-0xaf */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xb0-0xb7 */
  0x00, 0x00, 0x00, 0xaf, 0x00, 0x00, 0x00, 0x00, /* 0xb8-0xbf */
};
static const unsigned char cp855_page04[96] = {
  0x00, 0x85, 0x81, 0x83, 0x87, 0x89, 0x8b, 0x8d, /* 0x00-0x07 */
  0x8f, 0x91, 0x93, 0x95, 0x97, 0x00, 0x99, 0x9b, /* 0x08-0x0f */
  0xa1, 0xa3, 0xec, 0xad, 0xa7, 0xa9, 0xea, 0xf4, /* 0x10-0x17 */
  0xb8, 0xbe, 0xc7, 0xd1, 0xd3, 0xd5, 0xd7, 0xdd, /* 0x18-0x1f */
  0xe2, 0xe4, 0xe6, 0xe8, 0xab, 0xb6, 0xa5, 0xfc, /* 0x20-0x27 */
  0xf6, 0xfa, 0x9f, 0xf2, 0xee, 0xf8, 0x9d, 0xe0, /* 0x28-0x2f */
  0xa0, 0xa2, 0xeb, 0xac, 0xa6, 0xa8, 0xe9, 0xf3, /* 0x30-0x37 */
  0xb7, 0xbd, 0xc6, 0xd0, 0xd2, 0xd4, 0xd6, 0xd8, /* 0x38-0x3f */
  0xe1, 0xe3, 0xe5, 0xe7, 0xaa, 0xb5, 0xa4, 0xfb, /* 0x40-0x47 */
  0xf5, 0xf9, 0x9e, 0xf1, 0xed, 0xf7, 0x9c, 0xde, /* 0x48-0x4f */
  0x00, 0x84, 0x80, 0x82, 0x86, 0x88, 0x8a, 0x8c, /* 0x50-0x57 */
  0x8e, 0x90, 0x92, 0x94, 0x96, 0x00, 0x98, 0x9a, /* 0x58-0x5f */
};
static const unsigned char cp855_page25[168] = {
  0xc4, 0x00, 0xb3, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x00-0x07 */
  0x00, 0x00, 0x00, 0x00, 0xda, 0x00, 0x00, 0x00, /* 0x08-0x0f */
  0xbf, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, /* 0x10-0x17 */
  0xd9, 0x00, 0x00, 0x00, 0xc3, 0x00, 0x00, 0x00, /* 0x18-0x1f */
  0x00, 0x00, 0x00, 0x00, 0xb4, 0x00, 0x00, 0x00, /* 0x20-0x27 */
  0x00, 0x00, 0x00, 0x00, 0xc2, 0x00, 0x00, 0x00, /* 0x28-0x2f */
  0x00, 0x00, 0x00, 0x00, 0xc1, 0x00, 0x00, 0x00, /* 0x30-0x37 */
  0x00, 0x00, 0x00, 0x00, 0xc5, 0x00, 0x00, 0x00, /* 0x38-0x3f */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x40-0x47 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x48-0x4f */
  0xcd, 0xba, 0x00, 0x00, 0xc9, 0x00, 0x00, 0xbb, /* 0x50-0x57 */
  0x00, 0x00, 0xc8, 0x00, 0x00, 0xbc, 0x00, 0x00, /* 0x58-0x5f */
  0xcc, 0x00, 0x00, 0xb9, 0x00, 0x00, 0xcb, 0x00, /* 0x60-0x67 */
  0x00, 0xca, 0x00, 0x00, 0xce, 0x00, 0x00, 0x00, /* 0x68-0x6f */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x70-0x77 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x78-0x7f */
  0xdf, 0x00, 0x00, 0x00, 0xdc, 0x00, 0x00, 0x00, /* 0x80-0x87 */
  0xdb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x88-0x8f */
  0x00, 0xb0, 0xb1, 0xb2, 0x00, 0x00, 0x00, 0x00, /* 0x90-0x97 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x98-0x9f */
  0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xa0-0xa7 */
};

static int
cp855_wctomb (conv_t conv, unsigned char *r, ucs4_t wc, size_t n)
{
  unsigned char c = 0;
  if (wc < 0x0080) {
    *r = wc;
    return 1;
  }
  else if (wc >= 0x00a0 && wc < 0x00c0)
    c = cp855_page00[wc-0x00a0];
  else if (wc >= 0x0400 && wc < 0x0460)
    c = cp855_page04[wc-0x0400];
  else if (wc == 0x2116)
    c = 0xef;
  else if (wc >= 0x2500 && wc < 0x25a8)
    c = cp855_page25[wc-0x2500];
  if (c != 0) {
    *r = c;
    return 1;
  }
  return RET_ILUNI;
}
