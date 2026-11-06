#include "TodoList.h"
#include <iostream>

int main() {
    TodoList todoList;

    // 添加测试任务
    todoList.addTask("任务A", "测试A", "2026-11-10");
    todoList.addTask("任务B", "测试B", "");
    todoList.addTask("任务C", "测试C", "2026-11-06");
    todoList.addTask("任务D", "测试D", "2026-11-06");
    todoList.addTask("任务E", "测试E", "");

    std::cout << "========== 排序前 ==========" << std::endl;
    todoList.showTasks();

    // 测试排序
    todoList.sortByDeadline();

    std::cout << "\n========== 排序后 ==========" << std::endl;
    todoList.showTasks();

    // 测试排序后标记完成
    std::cout << "\n========== 测试排序后标记完成 ==========" << std::endl;

    if (todoList.completeTask(1)) {
        std::cout << "任务1标记成功。" << std::endl;
    }

    // 测试排序后删除任务
    std::cout << "\n========== 测试排序后删除 ==========" << std::endl;

    if (todoList.deleteTask(3)) {
        std::cout << "任务3删除成功。" << std::endl;
    }

    // 测试排序后继续添加
    std::cout << "\n========== 测试排序后继续添加 ==========" << std::endl;

    todoList.addTask("任务F", "测试F", "2026-11-08");

    todoList.showTasks();

    return 0;
}