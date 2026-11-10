#ifndef REPORT_H
#define REPORT_H

#include <ostream>
#include <string>

#include "TodoList.h"

// 一份可显示的报告：统计项与视图项都继承它
class Report {
   public:
    virtual ~Report() = default;

    // 菜单上显示的名字
    virtual std::string title() const = 0;

    // 生成内容，写进 out
    virtual void render(const TodoList& list, std::ostream& out) const = 0;

    // 内容是否较长、需要全屏分页显示
    virtual bool isLong() const { return false; }
};

#endif
