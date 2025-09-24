# MAVLink客户端/服务器Makefile

# 编译器设置
CC = gcc
CFLAGS = -g -O0 -Igenerated/include
FLOWCHECK_PATH = /home/raoxue/Desktop/IF-driver-partition/Flowcheckdocker/flowcheck-1.20/include
FLOWCHECK_PATH_DOCKER=/Desktop/IF-driver-partition/Flowcheckdocker/flowcheck-1.20/include

# 目标文件
CLIENT = mavlink_client
SERVER = mavlink_server
# 32位目标文件
CLIENT_32 = mavlink_client_32
SERVER_32 = mavlink_server_32

# 默认目标
all: $(CLIENT) $(SERVER)

# 客户端目标
$(CLIENT): mavlink_client.c
	$(CC) $(CFLAGS) -I$(FLOWCHECK_PATH) -o $@ $<

# 服务器目标
$(SERVER): mavlink_server.c
	$(CC) $(CFLAGS) -I$(FLOWCHECK_PATH) -o $@ $<

# 32位客户端目标(用于docker)
$(CLIENT_32): mavlink_client.c
	$(CC) $(CFLAGS) -m32 -I$(FLOWCHECK_PATH_DOCKER) -o $(CLIENT) $<

$(SERVER_32): mavlink_server.c
	$(CC) $(CFLAGS) -m32  -I$(FLOWCHECK_PATH_DOCKER) -o $(SERVER) $<

# 清理目标
clean:
	rm -f $(CLIENT) $(SERVER) *.o

# 帮助信息
help:
    @echo "可用的make目标:"
	@echo "  make all        - 构建客户端和服务器"
	@echo "  make $(CLIENT)  - 只构建客户端"
	@echo "  make $(SERVER)  - 只构建服务器" 
	@echo "  make $(CLIENT_32) - 构建32位客户端(用于docker)"
	@echo "  make $(SERVER_32) - 构建32位服务器(用于docker)"
	@echo "  make clean      - 删除所有构建的程序"

.PHONY: all clean help client-32
