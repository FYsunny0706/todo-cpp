#ifndef TEXT_ART_H
#define TEXT_ART_H

#include <string>

// 生成进度条，例如 [████░░░░]，percent 取 0-100
std::string progressBar(double percent, int width);

// 按数量返回密度符号：0 空、1 浅、2 中、3 深、4 以上实心
std::string densitySymbol(int count);

// 把百分比格式化成一位小数，例如 "41.7%"
std::string formatPercent(double percent);

#endif
