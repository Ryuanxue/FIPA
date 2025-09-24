### 编译
客户端
gcc -g -Igenerated/include -I/home/raoxue/Desktop/IF-driver-partition/Flowcheckdocker/flowcheck-1.20/include -o mavlink_client mavlink_client.c

服务器
gcc -Igenerated/include -o mavlink_server mavlink_server.c

### docker运行mavlink

在IF-driver-partition目录下运行
docker run --network host -it -v .:/Desktop/IF-driver-partition/ flowchecker
cd到mavlink_client或mavlink_server所在目录下

在docker中运行
./mavlink_server
在本地运行
./mavlink_client

在本地运行
./mavlink_server
在docker中运行
./mavlink_client


### 编译在docker中运行的程序为32程序
gcc -g -O0 -m32 -Igenerated/include -I/Desktop/IF-driver-partition/Flowcheckdocker/flowcheck-1.20/include -o mavlink_client mavlink_client.c
