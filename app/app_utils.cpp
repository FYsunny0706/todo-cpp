#include "app_utils.h"

#include <iostream>
#include <string>
#include <vector>

#include <windows.h>

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

namespace {

// 上一次操作的提示，画界面时会重新显示
std::string lastStatus;

}  // namespace

void clearScreen() {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;

	if (!GetConsoleScreenBufferInfo(out, &info)) return;

	DWORD written = 0;
	const DWORD cells = static_cast<DWORD>(info.dwSize.X) * static_cast<DWORD>(info.dwSize.Y);
	const COORD home = {0, 0};

	FillConsoleOutputCharacterA(out, ' ', cells, home, &written);
	FillConsoleOutputAttribute(out, info.wAttributes, cells, home, &written);
	SetConsoleCursorPosition(out, home);
}

void setStatus(const std::string& text) {
	lastStatus = text;
}

void beginFrame() {
	clearScreen();

	if (!lastStatus.empty()) {
		std::cout << lastStatus;
	}
}


namespace {

// 把文本按行拆开
std::vector<std::string> splitLines(const std::string& text) {
	std::vector<std::string> lines;
	std::string current;

	for (char c : text) {
		if (c == '\n') {
			lines.push_back(current);
			current.clear();
		} else {
			current += c;
		}
	}

	if (!current.empty()) lines.push_back(current);

	return lines;
}

}  // namespace

void showFullScreen(const std::string& text) {
	const std::vector<std::string> lines = splitLines(text);
	const std::size_t pageSize = 20;
	const std::size_t pageCount = (lines.size() + pageSize - 1) / pageSize;

	setStatus("");

	std::size_t page = 0;

	while (true) {
		clearScreen();

		const std::size_t start = page * pageSize;

		for (std::size_t i = start; i < lines.size() && i < start + pageSize; i++) {
			std::cout << lines[i] << '\n';
		}

		if (pageCount <= 1) {
			std::cout << "\n按回车返回…";
			std::cout.flush();

			readLine();

			return;
		}

		std::cout << "\n第 " << (page + 1) << "/" << pageCount
				  << " 页  （1 下一页 / 2 上一页 / 0 返回）: ";
		std::cout.flush();

		const std::string answer = readLine();

		if (std::cin.eof()) return;

		if (answer == "0") return;
		if (answer == "1" && page + 1 < pageCount) ++page;
		if (answer == "2" && page > 0) --page;
	}
}
