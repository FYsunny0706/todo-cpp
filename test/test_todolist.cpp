#include "TodoList.h"
#include <iostream>

int main() {
    TodoList todoList;

    todoList.addTask("学习C++", "完成Todo-Cpp项目", "2026-11-06");
    todoList.addTask("学习高数", "复习微分学", "2026-11-07");

    // 测试标记任务完成
    if (todoList.completeTask(1)) {
        std::cout << "任务 1 已成功标记为完成。" << std::endl;
    } else {
        std::cout << "没有找到该任务。" << std::endl;
    }

    // 测试不存在的 ID
    if (todoList.completeTask(100)) {
        std::cout << "任务已成功标记为完成。" << std::endl;
    } else {
        std::cout << "没有找到 ID 为 100 的任务。" << std::endl;
    }

    // 查看最终状态
    todoList.showTasks();

    return 0;
}