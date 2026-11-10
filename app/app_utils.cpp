#include "app_utils.h"

#include <iostream>

namespace {

// 解析一行整数，允许前导与尾随空白
bool parseIntLine(const std::string& line, int& value) {
	if (line.empty()) return false;

	try {
		std::size_t pos = 0;
		value = std::stoi(line, &pos);

		while (pos < line.size() && (line[pos] == ' ' || line[pos] == '\t')) {
			++pos;
		}

		return pos == line.size();
	} catch (...) {
		return false;
	}
}

}  // namespace

bool readInt(int& value) {
	const std::string line = readLine();

	// 输入流出错但不是结束时清掉状态，避免主循环空转
	if (std::cin.fail() && !std::cin.eof()) std::cin.clear();

	return parseIntLine(line, value);
}

bool readLineOrCancel(std::string& out) {
	out = readLine();

	return out != CANCEL_INPUT;
}

bool readIntOrCancel(int& value, bool& canceled) {
	const std::string line = readLine();

	if (std::cin.fail() && !std::cin.eof()) std::cin.clear();

	canceled = (line == CANCEL_INPUT);

	if (canceled) return false;

	return parseIntLine(line, value);
}

std::string readLine() {
	std::string text;

	std::getline(std::cin, text);

	return text;
}

std::string categoryName(int choice) {
	switch (choice) {
		case 1:
			return "学业";
		case 2:
			return "生活";
		case 3:
			return "工作";
		default:
			return "其他";
	}
}

bool persist(const TodoList& list) {
	if (!list.saveToFile(DATA_FILE)) {
		std::cout << "!! 保存失败，请检查 " << DATA_FILE << " 是否可写。\n";
		return false;
	}

	return true;
}
