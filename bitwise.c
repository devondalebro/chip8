#include <stdio.h>

int main(void) {
    unsigned short n = 170;

    if ((n >> 15) == 1) {
        printf("ye\n");
    } else {
        printf("na\n");
    }
}
