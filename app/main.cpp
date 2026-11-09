// 待办事项主程序：菜单与交互
//
// 运行：在项目根目录执行 build\todo.exe（数据文件用相对路径）

#include <filesystem>
#include <iostream>
#include <limits>
#include <string>
#include <system_error>

#include <windows.h>

#include "TodoList.h"
#include "DateUtils.h"

namespace {

const char* const DATA_FILE = "data/tasks.txt";

// 主菜单
void printMainMenu() {
	std::cout << "\n========== 待办事项 ==========\n";
	std::cout << "  1. 任务管理\n";
	std::cout << "  2. 保存\n";
	std::cout << "  0. 退出\n";
	std::cout << "请选择: ";
}

// 任务管理子菜单
void printTaskMenu() {
	std::cout << "\n--- 任务管理 ---\n";
	std::cout << "  1. 查看全部任务\n";
	std::cout << "  2. 添加任务\n";
	std::cout << "  3. 标记完成\n";
	std::cout << "  4. 取消标记\n";
	std::cout << "  5. 删除任务\n";
	std::cout << "  6. 修改截止日期\n";
	std::cout << "  7. 按截止日期排序\n";
	std::cout << "  8. 全部标记完成\n";
	std::cout << "  9. 清空已完成\n";
	std::cout << "  0. 返回\n";
	std::cout << "请选择: ";
}

// 读一个整数，输入非法时返回 false
bool readInt(int& value) {
	if (std::cin >> value) {
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return true;
	}

	if (std::cin.eof()) return false;

	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return false;
}

// 读一整行，允许空行
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

// 把任务保存到数据文件，成功返回 true
bool persist(const TodoList& list) {
	if (!list.saveToFile(DATA_FILE)) {
		std::cout << "!! 保存失败，请检查 " << DATA_FILE << " 是否可写。\n";
		return false;
	}

	return true;
}

// 添加任务
void addTaskFlow(TodoList& list) {
	std::cout << "标题: ";
	const std::string title = readLine();

	std::cout << "描述（可留空）: ";
	const std::string description = readLine();

	std::cout << "截止日期 YYYY-MM-DD 或 YYYY-MM-DD HH:MM（可留空）: ";
	const std::string deadline = readLine();

	std::cout << "分类（1 学业 / 2 生活 / 3 工作 / 4 其他）: ";

	int categoryChoice = 4;

	if (!readInt(categoryChoice)) categoryChoice = 4;

	std::cout << "是否重要？（1 是 / 0 否）: ";

	int importance = 0;

	if (!readInt(importance)) importance = 0;

	if (importance != 1) importance = 0;

	const std::string category = categoryName(categoryChoice);

	if (title.empty()) {
		std::cout << "添加失败：标题不能为空。\n";
		return;
	}

	if (!isValidDate(deadline)) {
		std::cout << "添加失败：截止日期必须是 YYYY-MM-DD 或 YYYY-MM-DD HH:MM，或留空。\n";
		return;
	}

	if (!list.addTask(title, description, deadline, category, importance)) {
		std::cout << "添加失败。\n";
		return;
	}

	std::cout << "已添加。\n";
}

// 标记完成
void completeTaskFlow(TodoList& list) {
	std::cout << "要标记完成的 ID: ";

	int id = 0;

	if (!readInt(id)) {
		std::cout << "请输入数字。\n";
		return;
	}

	std::cout << (list.completeTask(id) ? "已标记完成。\n" : "没有这个 ID。\n");
}

// 取消标记
void uncompleteTaskFlow(TodoList& list) {
	std::cout << "要取消完成标记的 ID: ";

	int id = 0;

	if (!readInt(id)) {
		std::cout << "请输入数字。\n";
		return;
	}

	std::cout << (list.uncompleteTask(id) ? "已取消完成标记。\n" : "没有这个 ID。\n");
}

// 删除任务
void deleteTaskFlow(TodoList& list) {
	std::cout << "要删除的 ID: ";

	int id = 0;

	if (!readInt(id)) {
		std::cout << "请输入数字。\n";
		return;
	}

	std::cout << (list.deleteTask(id) ? "已删除。\n" : "没有这个 ID。\n");
}

// 修改截止日期
void updateDeadlineFlow(TodoList& list) {
	std::cout << "任务 ID: ";

	int id = 0;

	if (!readInt(id)) {
		std::cout << "请输入数字。\n";
		return;
	}

	std::cout << "新的截止日期 YYYY-MM-DD 或 YYYY-MM-DD HH:MM（留空表示无期限）: ";
	const std::string deadline = readLine();

	if (!list.updateDeadline(id, deadline)) {
		std::cout << "修改失败：ID 不存在，或日期不合法。\n";
		return;
	}

	std::cout << "已修改。\n";
}

void taskMenu(TodoList& list, bool& dirty) {
	bool back = false;

	while (!back) {
		printTaskMenu();

		int choice = -1;

		if (!readInt(choice)) {
			if (std::cin.eof()) return;

			std::cout << "请输入 0-9 的数字。\n";
			continue;
		}

		switch (choice) {
			case 1:
				list.showTasks();
				break;

			case 2:
				addTaskFlow(list);
				dirty = true;
				break;

			case 3:
				completeTaskFlow(list);
				dirty = true;
				break;

			case 4:
				uncompleteTaskFlow(list);
				dirty = true;
				break;

			case 5:
				deleteTaskFlow(list);
				dirty = true;
				break;

			case 6:
				updateDeadlineFlow(list);
				dirty = true;
				break;

			case 7:
				list.sortByDeadline();
				std::cout << "已按截止日期排序（无期限的排在最后）。\n";
				list.showTasks();
				dirty = true;
				break;

			case 8:
				list.completeAll();
				std::cout << "已把全部任务标记为完成。\n";
				dirty = true;
				break;

			case 9:
				list.clearCompleted();
				std::cout << "已清空完成的任务。\n";
				dirty = true;
				break;

			case 0:
				back = true;
				break;

			default:
				std::cout << "没有这个选项。\n";
				break;
		}

		//有改动则自动存盘
		if (dirty && !back) {
			if (persist(list)) dirty = false;
		}
	}
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

			std::cout << "请输入 0-2 的数字。\n";
			continue;
		}

		switch (choice) {
			case 1:
				taskMenu(list, dirty);
				if (std::cin.eof()) running = false;
				break;

			case 2:
				if (persist(list)) {
					std::cout << "已保存。\n";
					dirty = false;
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

	if (dirty) persist(list);

	std::cout << "再见。\n";

	return 0;
}
