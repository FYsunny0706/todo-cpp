#ifndef TASK_UI_H
#define TASK_UI_H

#include "TodoList.h"

// 任务管理子菜单：循环处理用户选择，dirty 用于标记是否有改动
void taskMenu(TodoList& list, bool& dirty);

#endif
