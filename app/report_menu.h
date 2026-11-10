#ifndef REPORT_MENU_H
#define REPORT_MENU_H

#include <vector>

#include "Report.h"
#include "TodoList.h"

// 用一个通用菜单列出并执行若干报告
void runReportMenu(const char* menuTitle, const std::vector<const Report*>& reports,
				   const TodoList& list);

#endif
