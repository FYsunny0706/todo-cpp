#include "app_utils.h"

#include <iostream>

bool readInt(int& value) {
	const std::string line = readLine();

	if (line.empty()) return false;

	try {
		std::size_t pos = 0;
		value = std::stoi(line, &pos);

		// 允许数字后面带空格或制表符
		while (pos < line.size() && (line[pos] == ' ' || line[pos] == '\t')) {
			++pos;
		}

		return pos == line.size();
	} catch (...) {
		return false;
	}
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
