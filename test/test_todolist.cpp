#include "TodoList.h"
#include <iostream>
#include <fstream>

int main() {
	std::cout << "\n========== 测试文件读取 ==========" << std::endl;

TodoList loadTest;

if (loadTest.loadFromFile("data/tasks.txt")) {
    std::cout << "读取成功！" << std::endl;
    loadTest.showTasks();

    std::cout << "\n========== 测试继续添加 ==========" << std::endl;

    loadTest.addTask("新任务", "测试ID是否连续", "");

    loadTest.showTasks();
} else {
    std::cout << "读取失败！" << std::endl;
}
}