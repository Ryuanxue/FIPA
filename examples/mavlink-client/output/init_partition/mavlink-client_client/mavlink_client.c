#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>

// 假设 MAVLink 头文件已生成并位于此路径
#include "mavlink/v2.0/common/mavlink.h"

#include "mavlink-client_rpc_wrapper.h"

#define BUFFER_LENGTH 2041 // MAVLink 最大缓冲区大小
#define UDP_PORT 14550     // 目标 UDP 端口
#define TARGET_IP "127.0.0.1" // 发送到本机

// 发送心跳包
void send_heartbeat(int sock, struct sockaddr_in *dest_addr) {
    mav O_NONBLOCK);

    // 设置目标地址
    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(UDP_PORT);
    inet_pton(AF_INET, TARGET_IP, &dest_addr.sin_addr);

    // 主循环：每秒发送一次心跳包
    while (1) {
        send_heartbeat(sock, &dest_addr);
        sleep(1); // 等待 1 秒
    }

    close(sock);
    return 0;
}
