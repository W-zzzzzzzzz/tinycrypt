

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char *argv[]) {
    
    int x = atoi(argv[1]);
    double sine = sin((double)x);
    double cosine = asin(sine);
    
    printf("%08lx %08lx %08lx\n", x, (int)sine, (int)cosine);
    
    return 0;
}
