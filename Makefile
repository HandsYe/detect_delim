# Makefile for detect_delim

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
TARGET = detect_delim
SOURCE = detect_delim.c

# 默认目标
all: $(TARGET)

# 编译主程序
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# 调试版本
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

# 静态链接版本（便于分发）
static: CFLAGS += -static
static: $(TARGET)

# 清理编译文件
clean:
	rm -f $(TARGET) $(TARGET).exe

# 安装到系统路径
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/
	sudo chmod +x /usr/local/bin/$(TARGET)

# 卸载
uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)

# 测试
test: $(TARGET)
	@echo "=== 测试基本功能 ==="
	@echo "测试分隔符检测..."
	@echo -e "name,age,city\nJohn,25,NYC\nJane,30,LA" > test_data.csv
	./$(TARGET) test_data.csv
	@echo ""
	@echo "测试列信息显示..."
	./$(TARGET) test_data.csv head
	@echo ""
	@echo "测试字符串拆分..."
	./$(TARGET) "apple,banana;orange|grape" split
	@echo ""
	@echo "清理测试文件..."
	@rm -f test_data.csv
	@echo "测试完成！"

# Windows版本（使用MinGW）
windows:
	x86_64-w64-mingw32-gcc $(CFLAGS) -o $(TARGET).exe $(SOURCE)

# 帮助信息
help:
	@echo "可用的make目标："
	@echo "  all      - 编译程序（默认）"
	@echo "  debug    - 编译调试版本"
	@echo "  static   - 编译静态链接版本"
	@echo "  windows  - 交叉编译Windows版本"
	@echo "  clean    - 清理编译文件"
	@echo "  install  - 安装到系统路径"
	@echo "  uninstall- 从系统路径卸载"
	@echo "  test     - 运行基本测试"
	@echo "  help     - 显示此帮助信息"

.PHONY: all debug static clean install uninstall test windows help