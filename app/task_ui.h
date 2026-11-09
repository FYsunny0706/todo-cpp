#ifndef TASK_UI_H
#define TASK_UI_H

#include "TodoList.h"

// 任务管理子菜单：循环处理用户选择，数据有变动时自动存盘
// saved 是各层共享的已存盘版本号，只有存盘成功才更新
void taskMenu(TodoList& list, int& saved);

#endif
