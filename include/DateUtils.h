#ifndef DATEUTILS_H
#define DATEUTILS_H

#include <string>

// 校验日期是否为合法的 YYYY-MM-DD 或 YYYY-MM-DD HH:MM，空字符串视为有效
bool isValidDate(const std::string& date);

// 取今天的日期，格式 YYYY-MM-DD
std::string today();

#endif