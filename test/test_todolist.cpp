#include "TodoList.h"
#include <iostream>

int main() {
    TodoList todoList;

    std::cout << "========== 测试一：空列表 ==========" << std::endl;
    todoList.showTasks();

    std::cout << "\n========== 测试二：添加任务 ==========" << std::endl;

    todoList.addTask("学习C++", "完成Todo-Cpp项目", "2026-11-06");
    todoList.addTask("学习高数", "复习微分学", "2026-11-07");
    todoList.addTask("学习Python", "学习Pandas", "2026-11-08");

    todoList.showTasks();

    std::cout << "\n========== 测试三：标记完成 ==========" << std::endl;

    if (todoList.completeTask(1)) {
        std::cout << "任务1标记成功。" << std::endl;
    } else {
        std::cout << "任务1标记失败。" << std::endl;
    }

    if (!todoList.completeTask(100)) {
        std::cout << "不存在的任务处理正确。" << std::endl;
    }

    todoList.showTasks();

    std::cout << "\n========== 测试四：删除任务 ==========" << std::endl;

    if (todoList.deleteTask(2)) {
        std::cout << "任务2删除成功。" << std::endl;
    }

    if (!todoList.deleteTask(100)) {
        std::cout << "不存在的任务删除处理正确。" << std::endl;
    }

    todoList.showTasks();

    std::cout << "\n========== 测试五：删除后继续添加 ==========" << std::endl;

    todoList.addTask("学习数据结构", "复习vector", "2026-11-09");

    todoList.showTasks();

    return 0;
}