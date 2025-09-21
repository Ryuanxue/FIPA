//#include <valgrind/flowcheck.h>
int gval=3;
int buf[4];
int fun(int *a){
    return *a;
    };
int fun1(int a, int b){
    a=b+1;
return a;
};

int fun2(int *buf,int len)
{
    int a=0;
for (int i=0;i<len;i++)
{
a=a+buf[i];
}
return 1;
}

void fun4()
{
    int a;
    a=2;
}

int main(){
    int a;
  int  b=3;
  int lbuf[4];
//FC_TAINT_WORD(&b);//假设将b设为敏感的

// a=add(b);//qin,b, qout,b,a,
a=b+1;//qin,b,a, qout,b,a,
a=a-1;//qin,b,a, qout,b,a,
a=a*a;//qin,b,a, qout,b,a,
a=3;
int *ptr = (int *)malloc(2*sizeof(int));
ptr[0]=a;
ptr[1]=b;
if (a) //qin,b,a, qout,b,a,
{
a=a+2;
//a=0;
gval=b+3;
buf[0]=a;
buf[1]=b;
buf[2]=0;
buf[3]=gval;
lbuf[0]=a;
lbuf[1]=b;

a=a+3;
int c=a; //c被污染，但是c在if之外不可见
c=gval+c;
}
fun4();
a=fun(&b); 
a=fun1(a,b); 
a=fun2(buf,4);
a=fun2(ptr,2);
b=fun(&a);
}
