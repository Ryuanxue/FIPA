#include <valgrind/flowcheck.h>
#include <stdio.h>
#include <malloc.h>

/* This doesn't do the right thing, because the compiler (and the
   static enclosure region annotation) can't tell how large the array
   is. */
void write_to_array_broken(int *a, int secret) {
    FC_ENTER_ENCLOSE_V(6, a);
    a[secret] = 10;
    FC_LEAVE_ENCLOSE(6);
}    

/* It needs to be written like this, instead, passing an explicit
   size. */
void write_to_array_correct(int *a, int secret) {
    FC_ENTER_ENCLOSE_R(12, a, 100 * sizeof(int));
    a[secret] = 10;
    FC_LEAVE_ENCLOSE(12);
}    

int main(int argc, char **argv) {
    int *a;
    int secret = argc + 10;
    int total = 0, i;
    FC_TAINT_WORD(&secret);
    a = calloc(100, sizeof(int));
    write_to_array_correct(a, secret);
    write_to_array_broken(a, secret + 7);
    for (i = 0; i < 100; i++) {
	printf("Index %d\n", i);
	total += a[i];
    }
    /* This will print 10, rather than the expected 20, under
       Flowcheck, because the unenclosed write from the _broken
       version will be reverted. */
    printf("%d\n", total);
    return 0;    
}
