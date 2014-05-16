/*
   OpenChange MAPI implementation.

   Copyright (C) Julien Kerihuel 2012.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "libmapirops.h"

#ifndef	MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

/**
 */
static void mapirops_print_asc(const uint8_t *buf, int len)
{
	int i;

	for (i = 0; i < len; i++)
		printf("%c", isprint(buf[i]) ? buf[i] : '.');
}

/**
   \details This function print an array of uint8_t with both their
   hexadecimal and ASCII representation or '.' if not printable

   \param buf Pointer to the uint8_t array to hexdump
   \param len Length of the uint8_t array
 */
void mapirops_hexdump(const uint8_t *buf, int len)
{
	int i = 0;

	for (i=0;i<len;) {
		if (i%16 == 0 && i < len) {
			printf("[%04X] ",i);
		}

		printf("%02X ",(int)buf[i]);
		i++;
		if (i % 8 == 0) printf("  ");
		if (i%16 == 0) {
			mapirops_print_asc(&buf[i-16],8); printf(" ");
			mapirops_print_asc(&buf[i-8],8); printf("\n");
		}
	}

	if (i%16) {
		int n;
		n = 16 - (i%16);
		printf(" ");
		if (n>8) printf(" ");
		while (n--) printf("   ");
		n = MIN(8,i%16);
		mapirops_print_asc(&buf[i-(i%16)],n); printf(" ");
		n = (i%16) - n;
		if (n>0) mapirops_print_asc(&buf[i-n], n);
		printf("\n");
	}
}
