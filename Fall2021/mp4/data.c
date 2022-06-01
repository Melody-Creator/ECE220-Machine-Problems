#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){

    srand(time(NULL));

    int32_t X = rand() % 5000 + 5;

    int32_t a1 = rand() % 5000 + 5;
    while(a1 < 4)  a1 = rand() % 5000 + 5;

    if(a1 == 4){
        printf("%d 4 3 2 1\n", X);
        return 0;
    }

    int32_t a2 = rand() % a1;
    while(a2 < 3)  a2 = rand() % a1;

    int32_t a3 = rand() % a2 ;
    while(a3 < 2)  a3 = rand() % a2;

    int32_t a4 = rand() % a3;
    while(a4 < 1)  a4 = rand() % a3;

    printf("%d %d %d %d %d\n", X, a1, a2, a3, a4);

    return 0;
}
