// simple_test.c
#include <stdio.h>

int main() {
    printf("Hello, Valgrind Flowcheck!\n");
    printf("This is a simple test program.\n");
    
    int x = 10;
    int y = 20;
    int sum = x + y;
    
    printf("Sum: %d + %d = %d\n", x, y, sum);
    return 0;
}
