#include "DateUtils.h"

#include <ctime>

bool isValidDate(const std::string& date) {
	if (date.empty()) return true;

	if (date.length() == 16) {
		if (date[10] != ' ') return false;

		const std::string timePart = date.substr(11);

		if (timePart.length() != 5 || timePart[2] != ':') return false;

		for (int i = 0; i < 5; i++) {
			if (i == 2) continue;

			if (timePart[i] < '0' || timePart[i] > '9') return false;
		}

		const int hour = std::stoi(timePart.substr(0, 2));
		const int minute = std::stoi(timePart.substr(3, 2));

		if (hour > 23 || minute > 59) return false;

		return isValidDate(date.substr(0, 10));
	}

	if (date.length() != 10) return false;

	if (date[4] != '-' || date[7] != '-') return false;

	for (int i = 0; i < 10; i++) {
		if (i == 4 || i == 7) continue;

		if (date[i] < '0' || date[i] > '9') return false;
	}

	int year = std::stoi(date.substr(0, 4));
	int month = std::stoi(date.substr(5, 2));
	int day = std::stoi(date.substr(8, 2));

	if (year < 1 || month < 1 || month > 12) return false;

	int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	// 判断是否闰年
	bool isLeapYear = (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);

	if (isLeapYear) daysInMonth[1] = 29;

	return day >= 1 && day <= daysInMonth[month - 1];
}

// 取今天的日期，格式 YYYY-MM-DD
std::string today() {
	const std::time_t now = std::time(nullptr);
	const std::tm* local = std::localtime(&now);

	char buffer[11] = {0};
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", local);

	return std::string(buffer);
}

// 取当前时间，格式 YYYY-MM-DD HH:MM
std::string now() {
	const std::time_t current = std::time(nullptr);
	const std::tm* local = std::localtime(&current);

	char buffer[17] = {0};
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", local);

	return std::string(buffer);
}

// deadline 是否已过期：带时分的按完整时间比较，只有日期的按天比较
bool isOverdue(const std::string& deadline, const std::string& nowText) {
	if (deadline.empty()) return false;

	if (deadline.size() > 10) return deadline < nowText;

	return deadline < nowText.substr(0, 10);
}
