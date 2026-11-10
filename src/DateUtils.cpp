#include "DateUtils.h"

#include <ctime>

namespace {

// 按给定格式格式化当前本地时间
std::string formatLocalTime(const char* format) {
	const std::time_t current = std::time(nullptr);
	const std::tm* local = std::localtime(&current);

	if (!local) return "";

	char buffer[32] = {0};
	std::strftime(buffer, sizeof(buffer), format, local);

	return std::string(buffer);
}

// 把年月日填进 tm，时分设为中午以避开夏令时边界
std::tm toTm(const Date& date) {
	std::tm tm = {};

	tm.tm_year = date.year - 1900;
	tm.tm_mon = date.month - 1;
	tm.tm_mday = date.day;
	tm.tm_hour = 12;

	return tm;
}

// 从 tm 取回年月日
Date fromTm(const std::tm& tm) {
	Date date;

	date.year = tm.tm_year + 1900;
	date.month = tm.tm_mon + 1;
	date.day = tm.tm_mday;

	return date;
}

}  // namespace

int daysInMonth(int year, int month) {
	static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if (month < 1 || month > 12) return 0;

	const bool leap = (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);

	if (month == 2 && leap) return 29;

	return days[month - 1];
}

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

	const int year = std::stoi(date.substr(0, 4));
	const int month = std::stoi(date.substr(5, 2));
	const int day = std::stoi(date.substr(8, 2));

	if (year < 1 || month < 1 || month > 12) return false;

	return day >= 1 && day <= daysInMonth(year, month);
}

std::string today() {
	return formatLocalTime("%Y-%m-%d");
}

std::string now() {
	return formatLocalTime("%Y-%m-%d %H:%M");
}

bool isDeadlineBefore(const std::string& deadline, const std::string& nowText) {
	if (deadline.empty()) return false;

	if (deadline.size() > 10) return deadline < nowText;

	return deadline < nowText.substr(0, 10);
}

bool parseDate(const std::string& text, Date& date) {
	if (text.size() < 10) return false;

	const std::string datePart = text.substr(0, 10);

	if (!isValidDate(datePart)) return false;

	date.year = std::stoi(datePart.substr(0, 4));
	date.month = std::stoi(datePart.substr(5, 2));
	date.day = std::stoi(datePart.substr(8, 2));

	return true;
}

std::string formatDate(const Date& date) {
	std::string out = std::to_string(date.year);

	out += '-';

	if (date.month < 10) out += '0';
	out += std::to_string(date.month);

	out += '-';

	if (date.day < 10) out += '0';
	out += std::to_string(date.day);

	return out;
}

int weekdayOf(const Date& date) {
	std::tm tm = toTm(date);

	std::mktime(&tm);

	return tm.tm_wday;
}

Date addDays(const Date& date, int days) {
	std::tm tm = toTm(date);

	tm.tm_mday += days;
	std::mktime(&tm);

	return fromTm(tm);
}

Date addMonths(const Date& date, int months) {
	std::tm tm = toTm(date);

	tm.tm_mon += months;
	tm.tm_mday = 1;          // 先归到 1 号，避免 31 号溢出到下个月
	std::mktime(&tm);

	Date first = fromTm(tm);

	const int last = daysInMonth(first.year, first.month);

	first.day = (date.day > last) ? last : date.day;

	return first;
}
