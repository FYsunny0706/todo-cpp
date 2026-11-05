#include "TodoList.h"

int main() {
    TodoList todoList;

    // 测试空列表
    todoList.showTasks();

    // 添加任务
    todoList.addTask("学习C++", "完成Todo-Cpp项目", "2026-11-06");
    todoList.addTask("学习高数", "复习微分学", "2026-11-07");

    // 查看任务
    todoList.showTasks();

    return 0;
}