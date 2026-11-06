#include "TodoList.h"
#include <iostream>

int main() {
    TodoList todoList;

    todoList.addTask("学习C++", "完成Todo-Cpp项目", "2026-11-06");
    todoList.addTask("学习高数", "复习微分学", "2026-11-07");
    todoList.addTask("学习Python", "学习Pandas", "2026-11-08");

    // 测试删除任务
    if (todoList.deleteTask(2)) {
        std::cout << "任务 2 删除成功。" << std::endl;
    } else {
        std::cout << "任务不存在。" << std::endl;
    }

    // 测试删除不存在的任务
    if (todoList.deleteTask(100)) {
        std::cout << "任务删除成功。" << std::endl;
    } else {
        std::cout << "没有找到 ID 为 100 的任务。" << std::endl;
    }

    // 查看删除后的任务
    todoList.showTasks();

    return 0;
}