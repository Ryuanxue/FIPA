# 定义编译器
CC = gcc

# 定义编译选项
CFLAGS = -Wall -g -I../include

# 定义源文件和目标文件
SRC =  test.c
OBJ = $(SRC:.c=.o)
XML = $(SRC:.c=.xml)

# 定义目标
#all: $(OBJ) $(XML)
all: $(OBJ)

# 编译目标文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 运行get_function_range并生成XML文件
%.xml: %.c
	./get_function_range $<

# 清理生成的文件
clean:
	rm -f $(OBJ) $(XML)

.PHONY: all clean

