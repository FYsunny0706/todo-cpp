#ifndef APP_UTILS_H
#define APP_UTILS_H

#include <string>

#include "TodoList.h"

// 数据文件路径
constexpr const char* DATA_FILE = "data/tasks.txt";

// 输入这个字符串表示放弃当前操作
constexpr const char* CANCEL_INPUT = "/";

// 读一个整数，输入非法时返回 false
bool readInt(int& value);

// 读一整行，允许空行
std::string readLine();

// 读一行；输入 / 表示取消，返回 false
bool readLineOrCancel(std::string& out);

// 读整数；输入 / 时 canceled 置 true，格式错误时 canceled 为 false
bool readIntOrCancel(int& value, bool& canceled);

// 把分类编号转成分类名称
std::string categoryName(int choice);

// 把任务保存到数据文件，成功返回 true
bool persist(const TodoList& list);

// 清屏并把光标移到左上角
void clearScreen();

// 记录一条状态消息，下次画界面前会重新显示
void setStatus(const std::string& text);

// 画界面前调用：清屏并重显上一次的操作结果
void beginFrame();

#endif
