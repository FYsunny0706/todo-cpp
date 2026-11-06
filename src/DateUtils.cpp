#include "DateUtils.h"

bool isValidDate(const std::string& date) {
	if (date.empty()) return true;

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

	bool isLeapYear = (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);

	if (isLeapYear) daysInMonth[1] = 29;

	return day >= 1 && day <= daysInMonth[month - 1];
}