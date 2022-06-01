#if !defined(MP4_H)
#define MP4_H

#include <stdint.h>

/*
 * The print_row function determines which pixels are known to be black
 * in one row of a nonogram, then prints a line indicating those pixels.
 * width is the number of pixels in the row.  Up to five region sizes 
 * are given as r1, r2, r3, r4, r5 (0 means a non-existent region, and 
 * r1 must exist).  The row is printed using * to denote known black
 * pixels, and hyphen ('-') to denote other pixels.  Finally, the 
 * function returns 1 for success, or 0 if the regions cannot fit within
 * the specified width.
 */
extern int32_t
print_row (int32_t width, int32_t r1, int32_t r2, int32_t r3, int32_t r4, 
           int32_t r5);

#endif /* MP4_H */
