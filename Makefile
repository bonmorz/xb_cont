# Makefile

CC = g++
CFLAGS=-Wall -std=c++11 -g
LDFLAGS = -lusb-1.0

OBJECTS=main.o xboxcont.o

# 定义最终目标程序名
EXECUTABLE=xbtest

# 默认目标
all: $(EXECUTABLE)

# 如何生成最终执行文件
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

# 如何从.cpp文件生成.o文件
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# 清理目标
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# 其他依赖
main.o: main.cpp xboxcont.h
xboxcont.o: xboxcont.cpp xboxcont.h