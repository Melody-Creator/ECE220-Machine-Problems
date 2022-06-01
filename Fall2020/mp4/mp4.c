#include <stdio.h>
#include <stdbool.h>    // in order to use boolean type
#include "mp4.h"
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
int32_t print_row(int32_t width, int32_t r1, int32_t r2, int32_t r3, int32_t r4, int32_t r5){
    bool Pixel[60];     // declare a boolean array, true for '*', false for '-'
    int32_t i, j, sep, sum, P = 1;  // i, j are used in the loops, sep is the number of '-' between each '*' block
    for(i = 0; i < 55; i++)  Pixel[i] = false;      // initialize to make all the elements false
    sep = (r2 != 0) + (r3 != 0) + (r4 != 0) + (r5 != 0);    // calculate the '-' we need to separate blocks
    sum = r1 + r2 + r3 + r4 + r5 + sep;     // the total length we need to place pixels
    if(sum > width)  return 0;      // the width is too small
    // P is the position pointer
    for(i = 0; i < r1; i++)  Pixel[P+i] = true;     // fill the five regions if they are not zero
    P += r1 + (r2 != 0);                            // the white pixel exists in between if the next region is not zero
    for(i = 0; i < r2; i++)  Pixel[P+i] = true;
    P += r2 + (r3 != 0);
    for(i = 0; i < r3; i++)  Pixel[P+i] = true;
    P += r3 + (r4 != 0);
    for(i = 0; i < r4; i++)  Pixel[P+i] = true;
    P += r4 + (r5 != 0);
    for(i = 0; i < r5; i++)  Pixel[P+i] = true;
    /* the above finished filling in the first line which does not fail (and the pattern is decided), 
    we tranform the pattern to the next line one by one */
    for(i = sum+1; i <= width; i++)     // the lines left to fill in
        for(j = i; j > 0; j--)          // fill the current line from the end
            Pixel[j] = Pixel[j] && Pixel[j-1];  // only when this pixel and last pixel are both black in the last line, 
                                                // this pixel in this line can be considered as a certain black one
    for(i = 1; i <= width; i++){
        if(Pixel[i])  putchar('*');
        else  putchar('-');     // output '*' and '-'
    }
    puts("");       // output '\n'
    return 1;
}