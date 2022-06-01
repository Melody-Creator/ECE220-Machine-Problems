#include <stdio.h>
#include <stdlib.h>

#include "mp4.h"

int
main (int argc, const char* const argv[])
{
    int32_t width; /* row width */
    int32_t r1;    /* region 1 size */
    int32_t r2;    /* region 2 size */
    int32_t r3;    /* region 3 size */
    int32_t r4;    /* region 4 size */
    int32_t r5;    /* region 5 size */

    if (7 != argc) {
        fprintf (stderr, "syntax: %s <width> <r1> <r2> <r3> <r4> <r5>\n", 
		 argv[0]);
	return 2;
    }

    width = atoi (argv[1]);
    r1 = atoi (argv[2]);
    r2 = atoi (argv[3]);
    r3 = atoi (argv[4]);
    r4 = atoi (argv[5]);
    r5 = atoi (argv[6]);

    if (1 > width || 50 < width || 1 > r1 || 50 < r1 || 0 > r2 || 50 < r2 || 
        0 > r3 || 50 < r3 || 0 > r4 || 50 < r4 || 0 > r5 || 50 < r5) {
        fputs ("Width must be between 1 and 50 pixels.\n", stderr);
        fputs ("Region 1 must be between 1 and 50 pixels.\n", stderr);
        fputs ("Regions 2-5 must be between 0 and 50 pixels.\n", stderr);
	return 2;
    }

    if (1 != print_row (width, r1, r2, r3, r4, r5)) {
        printf ("Impossible!\n");
	return 3;
    }

    return 0;
}

