#include <iostream>
#include "Task.h"

using namespace std;

int main() {
    // 测试构造函数
    Task task(1, "学习C++", "完成Todo-Cpp项目", false, "2026-11-06");

    // 测试 Getter
    cout << "ID: " << task.getId() << endl;
    cout << "标题: " << task.getTitle() << endl;
    cout << "描述: " << task.getDescription() << endl;
    cout << "完成状态: " << task.isCompleted() << endl;
    cout << "截止时间: " << task.getDeadline() << endl;

    // 测试 Setter
    task.setTitle("学习C++面向对象");
    task.setDescription("完成Task类测试");
    task.setDeadline("2026-11-07");

    // 测试 markCompleted
    task.markCompleted();

    cout << "\n修改后的任务：" << endl;
    cout << "标题: " << task.getTitle() << endl;
    cout << "描述: " << task.getDescription() << endl;
    cout << "完成状态: " << task.isCompleted() << endl;
    cout << "截止时间: " << task.getDeadline() << endl;

    return 0;
}