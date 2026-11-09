#ifndef APP_UTILS_H
#define APP_UTILS_H

#include <string>

#include "TodoList.h"

// 数据文件路径
constexpr const char* DATA_FILE = "data/tasks.txt";

// 读一个整数，输入非法时返回 false
bool readInt(int& value);

// 读一整行，允许空行
std::string readLine();

// 把分类编号转成分类名称
std::string categoryName(int choice);

// 把任务保存到数据文件，成功返回 true
bool persist(const TodoList& list);


#endif
