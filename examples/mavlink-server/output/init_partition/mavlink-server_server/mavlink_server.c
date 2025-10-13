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

#define BUFFER_LENGTH 2041 // MAVLink 最大缓冲区大小
#define UDP_PORT 14550     // 监听的 UDP 端口

// 接收并解析心跳包
void receive_heartbeat(int sock) {
    uint8_t buf[BUFFER_LENGTH];
 void receive_heartbeat_sense_1(uint8_t buf[2041], ssize_t recsize)
{
  int len = recsize / (sizeof(uint8_t));
  if (recsize > 0)
  {
    mavlink_message_t msg;
    mavlink_status_t status;
    for (int i = 0; i < recsize; i++)
    {
      if (mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status))
      {
        if (msg.msgid == 0)
        {
          mavlink_heartbeat_t heartbeat;
          mavlink_msg_heartbeat_decode(&msg, &heartbeat);
          printf("Received heartbeat: sysid=%d, compid=%d, type=%d, autopilot=%d, mode=%d, state=%d\n", msg.sysid, msg.compid, heartbeat.type, heartbeat.autopilot, heartbeat.base_mode, heartbeat.system_status);
        }
      }
    }

  }
  else
    if (((recsize < 0) && (errno != EAGAIN)) && (errno != EWOULDBLOCK))
  {
    perror("Error receiving data");
  }
}

if