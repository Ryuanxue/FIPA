#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

// 假设 MAVLink 头文件已生成并位于此路径
#include "mavlink/v2.0/common/mavlink.h"

#include "mavlink-server_rpc_wrapper.h"

#define BUFFER_LENGTH 2041 // MAVLink 最大缓冲区大小
#define UDP_PORT 14550     // 监听的 UDP 端口

// 接收并解析心跳包
void receive_heartbeat(int sock) {
    uint8_t buf[BUFFER_LENGTH];
 void receive_heartbeat(int sock)
{
  uint8_t buf[2041];
  struct sockaddr_in src_addr;
  socklen_t addrlen = sizeof(src_addr);
  ssize_t recsize;
  recsize = recvfrom(sock, buf, 2041, 0, (struct sockaddr *) (&src_addr), &addrlen);
  {
    receive_heartbeat_sense_1_wrapper(buf, recsize);
  }
}

if (sock < 0) {
        perror("Error creating socket");
        return 1;
    }

    // 设置套接字为非阻塞
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    // 绑定本地地址和端口
    struct sockaddr_in local_addr;
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(UDP_PORT);

    if (bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        perror("Error binding socket");
        close(sock);
        return 1;
    }

    printf("Server listening on port %d...\n", UDP_PORT);

    // 主循环：持续监听心跳包
    while (1) {
        receive_heartbeat(sock);
        usleep(100000); // 休眠 100ms，避免 CPU 占用过高
    }

    close(sock);
    return 0;
}
