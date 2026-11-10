#ifndef DATEUTILS_H
#define DATEUTILS_H

#include <string>

// 年月日
struct Date {
	int year = 1970;
	int month = 1;
	int day = 1;
};

// 校验日期是否为合法的 YYYY-MM-DD 或 YYYY-MM-DD HH:MM，空字符串视为有效
bool isValidDate(const std::string& date);

// 取今天的日期，格式 YYYY-MM-DD
std::string today();

// 取当前时间，格式 YYYY-MM-DD HH:MM
std::string now();

// deadline 是否已过期：带时分的按完整时间比较，只有日期的按天比较
bool isDeadlineBefore(const std::string& deadline, const std::string& nowText);

// 某月有多少天
int daysInMonth(int year, int month);

// 解析 YYYY-MM-DD 或 YYYY-MM-DD HH:MM，成功返回 true
bool parseDate(const std::string& text, Date& date);

// 格式化成 YYYY-MM-DD
std::string formatDate(const Date& date);

// 星期几：0 周日，1 周一，…，6 周六
int weekdayOf(const Date& date);

// 加减天数
Date addDays(const Date& date, int days);

// 加减月份，日期超出当月天数时截断到月末
Date addMonths(const Date& date, int months);

// 两个日期相差多少天（to 晚于 from 为正）
int daysBetween(const Date& from, const Date& to);

#endif
