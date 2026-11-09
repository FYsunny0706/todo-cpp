// 待办事项主程序：入口与主菜单
//
// 运行：在项目根目录执行 build\todo.exe（数据文件用相对路径）

#include <ctime>
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

	// 数据目录不存在时先建出来，避免保存失败
	std::error_code dirError;
	std::filesystem::create_directories(std::filesystem::path(DATA_FILE).parent_path(), dirError);

	if (dirError) {
		std::cout << "警告：无法创建数据目录，保存可能会失败。\n";
	}


	TodoList list;

	if (list.loadFromFile(DATA_FILE)) {
		std::cout << "已载入 " << DATA_FILE << "。\n";
	} else if (std::filesystem::exists(DATA_FILE)) {
		// 备份名带时间戳，避免覆盖旧备份
		const std::string backup =
			std::string(DATA_FILE) + ".bak." + std::to_string(std::time(nullptr));
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

	// 已存盘的版本号，用于判断退出前是否需要再存一次
	int saved = list.getRevision();
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
				// 不在子菜单内部更新 saved，避免其保存失败后丢失重试机会
				taskMenu(list, saved);
				if (std::cin.eof()) running = false;
				break;

			case 2:
				statsMenu(list);
				if (std::cin.eof()) running = false;
				break;

			case 3:
				if (persist(list)) {
					std::cout << "已保存。\n";
					saved = list.getRevision();
				}
				break;

			case 0:
				running = false;
				break;

			default:
				std::cout << "没有这个选项。\n";
				break;
		}
	}

	// 退出时仍有未保存的改动才写盘
	if (list.getRevision() != saved) persist(list);

	std::cout << "再见。\n";

	return 0;
}
