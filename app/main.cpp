// 待办事项主程序：入口与主菜单
//
// 运行：在项目根目录执行 build\todo.exe（数据文件用相对路径）

#include <filesystem>
#include <iostream>
#include <string>
#include <system_error>

#include <windows.h>

#include "TodoList.h"
#include "app_utils.h"
#include "task_ui.h"
#include "stats_ui.h"

namespace {

// 主菜单
void printMainMenu() {
	std::cout << "\n========== 待办事项 ==========\n";
	std::cout << "  1. 任务管理\n";
	std::cout << "  2. 统计\n";
	std::cout << "  3. 保存\n";
	std::cout << "  0. 退出\n";
	std::cout << "请选择: ";
}

}  // namespace

// 程序入口：载入数据，循环处理菜单
int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	TodoList list;

	if (list.loadFromFile(DATA_FILE)) {
		std::cout << "已载入 " << DATA_FILE << "。\n";
	} else if (std::filesystem::exists(DATA_FILE)) {
		const std::string backup = std::string(DATA_FILE) + ".bak";
		std::error_code error;
		std::filesystem::rename(DATA_FILE, backup, error);

		if (error) {
			std::cout << "数据文件损坏且无法备份，请手工检查 " << DATA_FILE << "。\n";
			return 1;
		}

		std::cout << "数据文件损坏，已备份为 " << backup << "，从空列表开始。\n";
	} else {
		std::cout << "未找到数据文件，从空列表开始。\n";
	}

	bool dirty = false;  // 本轮是否改动过数据
	bool running = true;

	while (running) {
		printMainMenu();

		int choice = -1;

		if (!readInt(choice)) {
			if (std::cin.eof()) {
				std::cout << "\n输入结束，程序退出。\n";
				break;
			}

			std::cout << "请输入 0-3 的数字。\n";
			continue;
		}

		switch (choice) {
			case 1:
				taskMenu(list, dirty);
				if (std::cin.eof()) running = false;
				break;

			case 2:
				statsMenu(list);
				if (std::cin.eof()) running = false;
				break;

			case 3:
				if (persist(list)) {
					std::cout << "已保存。\n";
					dirty = false;
				}
				break;

			case 0:
				running = false;
				break;
		}
	}

	if (dirty) persist(list);

	std::cout << "再见。\n";

	return 0;
}
