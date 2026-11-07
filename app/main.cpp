// 待办事项主程序：菜单与交互
//
// 运行：在项目根目录执行 build\todo.exe（数据文件用相对路径）

#include <filesystem>
#include <iostream>
#include <limits>
#include <string>

#include <windows.h>

#include "TodoList.h"

namespace {

const char* const DATA_FILE = "data/tasks.txt";

// 打印菜单
void printMenu() {
	std::cout << "\n========== 待办事项 ==========\n";
	std::cout << "  1. 查看全部任务\n";
	std::cout << "  2. 添加任务\n";
	std::cout << "  3. 标记完成\n";
	std::cout << "  4. 删除任务\n";
	std::cout << "  5. 修改截止日期\n";
	std::cout << "  6. 按截止日期排序\n";
	std::cout << "  7. 全部标记完成\n";
	std::cout << "  8. 清空已完成\n";
	std::cout << "  9. 立即保存\n";
	std::cout << "  0. 退出\n";
	std::cout << "请选择: ";
}

// 读一个整数，输入非法时返回 false
bool readInt(int& value) {
	if (std::cin >> value) return true;

	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return false;
}

// 读一整行，跳过前导空白
std::string readLine() {
	std::string text;

	std::getline(std::cin >> std::ws, text);

	return text;
}

// 把任务保存到数据文件，失败时输出提示
void persist(const TodoList& list) {
	std::filesystem::create_directories("data");

	if (!list.saveToFile(DATA_FILE)) {
		std::cout << "!! 保存失败，请检查 " << DATA_FILE << " 是否可写。\n";
	}
}

// 添加任务
void addTaskFlow(TodoList& list) {
	std::cout << "标题: ";
	const std::string title = readLine();

	std::cout << "描述（可留空）: ";
	const std::string description = readLine();

	std::cout << "截止日期 YYYY-MM-DD（可留空）: ";
	const std::string deadline = readLine();

	if (!list.addTask(title, description, deadline)) {
		std::cout << "添加失败：标题不能为空，日期必须是合法的 YYYY-MM-DD。\n";
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

	std::cout << "新的截止日期 YYYY-MM-DD（留空表示无期限）: ";
	const std::string deadline = readLine();

	if (!list.updateDeadline(id, deadline)) {
		std::cout << "修改失败：ID 不存在，或日期不合法。\n";
		return;
	}

	std::cout << "已修改。\n";
}

}  // namespace

// 程序入口：载入数据，循环处理菜单
int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	TodoList list;

	if (list.loadFromFile(DATA_FILE)) {
		std::cout << "已载入 " << DATA_FILE << "。\n";
	} else {
		std::cout << "未找到数据文件，从空列表开始。\n";
	}

	bool dirty = false;   // 本轮是否改动过数据
	bool running = true;

	while (running) {
		printMenu();

		int choice = -1;

		if (!readInt(choice)) {
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
				deleteTaskFlow(list);
				dirty = true;
				break;

			case 5:
				updateDeadlineFlow(list);
				dirty = true;
				break;

			case 6:
				list.sortByDeadline();
				std::cout << "已按截止日期排序（无期限的排在最后）。\n";
				list.showTasks();
				dirty = true;
				break;

			case 7:
				list.completeAll();
				std::cout << "已把全部任务标记为完成。\n";
				dirty = true;
				break;

			case 8:
				list.clearCompleted();
				std::cout << "已清空完成的任务。\n";
				dirty = true;
				break;

			case 9:
				persist(list);
				std::cout << "已保存。\n";
				dirty = false;
				break;

			case 0:
				running = false;
				break;

			default:
				std::cout << "没有这个选项。\n";
				break;
		}

		// 有改动则自动存盘
		if (dirty && running) {
			persist(list);
			dirty = false;
		}
	}

	if (dirty) persist(list);

	std::cout << "再见。\n";

	return 0;
}
