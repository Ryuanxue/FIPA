#include <valgrind/flowcheck.h>

void main(){

int a,b,c,d;

a=4; FC_TAINT_WORD(&a);

b=a+5;

c=b+ 7;
FC_UNTAINT_WORD(&a);
d=a+8;

}
