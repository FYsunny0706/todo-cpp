#ifndef TEXTWIDTH_H
#define TEXTWIDTH_H

#include <string>

// 字符串在终端里占多少格（中文按 2 格算）
std::size_t displayWidth(const std::string& text);

// 右补空格到指定显示宽度，超出返回原串
std::string padRight(const std::string& text, std::size_t width);

// 左补空格到指定显示宽度，超出返回原串
std::string padLeftText(const std::string& text, std::size_t width);

// 按显示宽度截断，超长时末尾换成 ..
std::string truncateToWidth(const std::string& text, std::size_t width);

#endif
