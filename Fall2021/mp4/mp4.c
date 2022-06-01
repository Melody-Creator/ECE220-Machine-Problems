/*
    This code is used to solve an optimization problem and to print the solution. The problem involves
    selecting the correct combination of “postage stamps” to affix to a package in order to exceed a required
    amount. Specifically, given a required postage cost and four different stamp values, the C function print_stamps
    will select the best number of each type of stamp. “Best” is defined as a combination of stamps with total 
    value of at least the postage cost, but otherwise minimal. If ties in total value are possible with
    different combinations of stamps, the function will select a combination with the smallest number of
    stamps. If ties are still possible, the function will select the combination with the largest number of
    stamps with the highest value (then maximize the number of stamps with second-highest value, and so
    forth).
*/

#include <stdio.h>
#include "mp4.h"

int32_t print_stamps (int32_t amount, int32_t s1, int32_t s2, int32_t s3, int32_t s4){
    
    int32_t num1, num2, num3, num4, total, count, fir = 1;      // use variables to record the numbers of each stamps in the currently best combination
                                                                // total is the total amount reached and count is the total numbers of stamps
                                                                // fir means whether we have got a possible solution already (1 means no)
    for(int32_t n1 = 0; (n1 - 1) * s1 < amount; n1++){
        for(int32_t n2 = 0; (n2 - 1) * s2 < amount - n1 * s1; n2++){
            for(int32_t n3 = 0; (n3 - 1) * s3 < amount - n1 * s1 - n2 * s2; n3++)
                for(int32_t n4 = 0; (n4 - 1) * s4 < amount - n1 * s1 - n2 * s2 - n3 * s3; n4++){        // consider only reasonable combinations

                    int32_t val1 = n1 * s1 + n2 * s2 + n3 * s3 + n4 * s4;
                    int32_t val2 = n1 + n2 + n3 + n4;       // calculate total value and total number of stamps with current combination
                    if(val1 < amount)  continue;            // less than amount -> invalid
                    if(fir || val1 < total || (val1 == total && val2 <= count)){    // find a better solution and break ties
                        fir = 0;        // have encountered a possible solution
                        num1 = n1;  num2 = n2;
                        num3 = n3;  num4 = n4;
                        total = val1;
                        count = val2;       // update the best combination with the current one
                    }
                }

        }
    }

    printf("%d %d %d %d -> %d\n", num1, num2, num3, num4, total);
    if(total == amount)  return 1;      // print the combination and return accordingly
    return 0;
}