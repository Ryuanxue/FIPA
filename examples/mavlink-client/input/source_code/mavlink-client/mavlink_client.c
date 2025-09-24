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

#define BUFFER_LENGTH 2041 // MAVLink 最大缓冲区大小
#define UDP_PORT 14550     // 目标 UDP 端口
#define TARGET_IP "127.0.0.1" // 发送到本机

// 发送心跳包
void send_heartbeat(int sock, struct sockaddr_in *dest_addr) {
    mavlink_message_t msg;
    uint8_t buf[BUFFER_LENGTH];
    uint16_t len;

    uint8_t sysid = 1; // 系统 ID
    uint8_t compid = 200; // 组件 ID
    uint8_t type = MAV_TYPE_QUADROTOR; // 车辆类型
    uint8_t autopilot = MAV_AUTOPILOT_PX4; // 自动驾驶仪类型
    uint8_t base_mode = MAV_MODE_MANUAL_ARMED; // 模式
    uint32_t custom_mode = 0; // 自定义模式
    uint8_t system_status = MAV_STATE_ACTIVE; // 系统状态
    
    // 初始化心跳消息
    mavlink_msg_heartbeat_pack(
        sysid,              // 系统 ID (sysid)
        compid,            // 组件 ID (compid)
        &msg,           // 消息结构体
        // MAV_TYPE_QUADROTOR, // 车辆类型（四旋翼）
        // MAV_AUTOPILOT_PX4,  // 自动驾驶仪类型
        // MAV_MODE_MANUAL_ARMED, // 模式
        // 0,              // 自定义模式
        // MAV_STATE_ACTIVE // 系统状态
        type,             // 车辆类型
        autopilot,        // 自动驾驶仪类型
        base_mode,        // 模式
        custom_mode,      // 自定义模式
        system_status     // 系统状态
    );

    // 序列化消息
    len = mavlink_msg_to_send_buffer(buf, &msg);

    // 发送消息
    if (sendto(sock, buf, len, 0, (struct sockaddr *)dest_addr, sizeof(struct sockaddr_in)) < 0) {
        perror("Error sending heartbeat");
    } else {
        printf("Sent heartbeat: sysid=%d, compid=%d\n", 1, 200);
    }
}

int main(void) {
    // 创建 UDP 套接字
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        perror("Error creating socket");
        return 1;
    }

    // 设置套接字为非阻塞
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

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
