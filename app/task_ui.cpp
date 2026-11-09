#include "task_ui.h"

#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>

#include "DateUtils.h"
#include "app_utils.h"

// 任务管理子菜单
void printTaskMenu() {
	std::cout << "\n--- 任务管理 ---\n";
	std::cout << "  1. 查看全部任务\n";
	std::cout << "  2. 添加任务\n";
	std::cout << "  3. 标记完成\n";
	std::cout << "  4. 取消标记\n";
	std::cout << "  5. 删除任务\n";
	std::cout << "  6. 修改任务\n";
	std::cout << "  7. 按截止日期排序\n";
	std::cout << "  8. 批量标记\n";
	std::cout << "  9. 批量删除\n";
	std::cout << " 10. 查找任务\n";
	std::cout << "  0. 返回\n";
	std::cout << "请选择: ";
}

// 添加任务
void addTaskFlow(TodoList& list) {
	std::cout << "标题: ";
	const std::string title = readLine();

	if (title.empty()) {
		std::cout << "添加失败：标题不能为空。\n";
		return;
	}

	std::cout << "描述（可留空）: ";
	const std::string description = readLine();

	std::cout << "截止日期 YYYY-MM-DD 或 YYYY-MM-DD HH:MM（可留空）: ";
	const std::string deadline = readLine();

	if (!isValidDate(deadline)) {
		std::cout << "添加失败：截止日期必须是 YYYY-MM-DD 或 YYYY-MM-DD HH:MM，或留空。\n";
		return;
	}

	std::cout << "分类（1 学业 / 2 生活 / 3 工作 / 4 其他，回车 = 其他）: ";

	int categoryChoice = 4;

	if (!readInt(categoryChoice)) categoryChoice = 4;

	std::cout << "是否重要？（1 是 / 0 否，回车 = 否）: ";

	int importance = 0;

	if (!readInt(importance)) importance = 0;

	if (importance != 1) importance = 0;

	const std::string category = categoryName(categoryChoice);

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

	const int result = list.completeTask(id);

	if (result == 1) {
		std::cout << "已标记完成。\n";
	} else if (result == 0) {
		std::cout << "该任务已经是完成状态。\n";
	} else {
		std::cout << "没有这个 ID。\n";
	}
}

// 取消标记
void uncompleteTaskFlow(TodoList& list) {
	std::cout << "要取消完成标记的 ID: ";

	int id = 0;

	if (!readInt(id)) {
		std::cout << "请输入数字。\n";
		return;
	}

	const int result = list.uncompleteTask(id);

	if (result == 1) {
		std::cout << "已取消完成标记。\n";
	} else if (result == 0) {
		std::cout << "该任务本来就是未完成。\n";
	} else {
		std::cout << "没有这个 ID。\n";
	}
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

void searchFlow(TodoList& list) {
	Query query;

	std::cout << "关键词（留空不限制）: ";
	query.keyword = readLine();

	std::cout << "分类（1 学业 / 2 生活 / 3 工作 / 4 其他，0 不限）: ";

	int categoryChoice = 0;

	if (!readInt(categoryChoice)) categoryChoice = 0;

	if (categoryChoice >= 1 && categoryChoice <= 4) {
		query.category = categoryName(categoryChoice);
	}

	std::cout << "状态（1 已完成 / 2 未完成，0 不限）: ";

	int stateChoice = 0;

	if (!readInt(stateChoice)) stateChoice = 0;

	if (stateChoice == 1) query.completed = 1;
	if (stateChoice == 2) query.completed = 0;

	const std::vector<int> found = list.find(query);

	if (found.empty()) {
		std::cout << "没有匹配的任务。\n";
		return;
	}

	std::cout << "找到 " << found.size() << " 条：\n";
	list.showTasks(found);
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

	if (!isValidDate(deadline)) {
		std::cout << "修改失败：截止日期必须是 YYYY-MM-DD 或 YYYY-MM-DD HH:MM，或留空。\n";
		return;
	}

	if (!list.updateDeadline(id, deadline)) {
		std::cout << "修改失败：ID 不存在。\n";
		return;
	}

	std::cout << "已修改。\n";
}

// 修改任务子菜单
void printEditMenu() {
	std::cout << "\n--- 修改任务 ---\n";
	std::cout << "  1. 修改标题\n";
	std::cout << "  2. 修改描述\n";
	std::cout << "  3. 修改截止日期\n";
	std::cout << "  4. 修改分类\n";
	std::cout << "  5. 修改重要度\n";
	std::cout << "  0. 返回\n";
	std::cout << "请选择: ";
}

// 修改标题
void editTitleFlow(TodoList& list) {
	std::cout << "任务 ID: ";

	int id = 0;

	if (!readInt(id)) {
		std::cout << "请输入数字。\n";
		return;
	}

	std::cout << "新标题: ";
	const std::string title = readLine();

	if (title.empty()) {
		std::cout << "修改失败：标题不能为空。\n";
		return;
	}

	std::cout << (list.updateTitle(id, title) ? "已修改。\n" : "修改失败：ID 不存在。\n");
}

// 修改描述
void editDescriptionFlow(TodoList& list) {
	std::cout << "任务 ID: ";

	int id = 0;

	if (!readInt(id)) {
		std::cout << "请输入数字。\n";
		return;
	}

	std::cout << "新描述（可留空）: ";
	const std::string description = readLine();

	std::cout << (list.updateDescription(id, description) ? "已修改。\n" : "修改失败：ID 不存在。\n");
}

// 修改分类
void editCategoryFlow(TodoList& list) {
	std::cout << "任务 ID: ";

	int id = 0;

	if (!readInt(id)) {
		std::cout << "请输入数字。\n";
		return;
	}

	std::cout << "新分类（1 学业 / 2 生活 / 3 工作 / 4 其他，回车 = 其他）: ";

	int categoryChoice = 4;

	if (!readInt(categoryChoice)) {
		std::cout << "请输入数字。\n";
		return;
	}

	const std::string category = categoryName(categoryChoice);

	std::cout << (list.updateCategory(id, category) ? "已修改。\n" : "修改失败：ID 不存在。\n");
}

// 修改重要度
void editImportanceFlow(TodoList& list) {
	std::cout << "任务 ID: ";

	int id = 0;

	if (!readInt(id)) {
		std::cout << "请输入数字。\n";
		return;
	}

	std::cout << "是否重要？（1 是 / 0 否，回车 = 否）: ";

	int importance = 0;

	if (!readInt(importance)) {
		std::cout << "请输入数字。\n";
		return;
	}

	if (importance != 1) importance = 0;

	std::cout << (list.updateImportance(id, importance) ? "已修改。\n" : "修改失败：ID 不存在。\n");
}

// 修改任务子菜单
void editMenu(TodoList& list, bool& dirty) {
	bool back = false;

	while (!back) {
		printEditMenu();

		int choice = -1;

		if (!readInt(choice)) {
			if (std::cin.eof()) return;

			std::cout << "请输入 0-5 的数字。\n";
			continue;
		}

		switch (choice) {
			case 1:
				editTitleFlow(list);
				dirty = true;
				break;

			case 2:
				editDescriptionFlow(list);
				dirty = true;
				break;

			case 3:
				updateDeadlineFlow(list);
				dirty = true;
				break;

			case 4:
				editCategoryFlow(list);
				dirty = true;
				break;

			case 5:
				editImportanceFlow(list);
				dirty = true;
				break;

			case 0:
				back = true;
				break;

			default:
				std::cout << "没有这个选项。\n";
				break;
		}

		// 有改动则自动存盘
		if (dirty && !back) {
			if (persist(list)) dirty = false;
		}
	}
}

// 批量标记子菜单
void printMarkMenu() {
	std::cout << "\n--- 批量标记 ---\n";
	std::cout << "  1. 全部标记完成\n";
	std::cout << "  2. 按编号标记完成\n";
	std::cout << "  3. 全部取消标记\n";
	std::cout << "  4. 按编号取消标记\n";
	std::cout << "  0. 返回\n";
	std::cout << "请选择: ";
}

// 全部标记完成
void markAllFlow(TodoList& list) {
	const int changed = list.completeAll();

	if (changed == 0) {
		std::cout << "没有需要标记的任务。\n";
		return;
	}

	std::cout << "已把 " << changed << " 条任务标记为完成。\n";
}

// 按编号标记多个任务
void markMultipleFlow(TodoList& list) {
	std::cout << "要标记完成的编号（空格分隔，例如 1 3 5）: ";
	const std::string line = readLine();

	std::istringstream input(line);

	int id = 0;
	int ok = 0;
	int skip = 0;
	int fail = 0;

	while (input >> id) {
		const int result = list.completeTask(id);

		if (result == 1) {
			std::cout << "编号 " << id << " 已标记完成。\n";
			++ok;
		} else if (result == 0) {
			std::cout << "编号 " << id << " 已经是完成状态。\n";
			++skip;
		} else {
			std::cout << "编号 " << id << " 不存在。\n";
			++fail;
		}
	}

	if (ok + skip + fail == 0) {
		std::cout << "没有输入任何编号。\n";
		return;
	}

	std::cout << "处理完成：成功 " << ok << " 个，跳过 " << skip << " 个，失败 " << fail << " 个。\n";
}

// 全部取消标记
void unmarkAllFlow(TodoList& list) {
	const int changed = list.uncompleteAll();

	if (changed == 0) {
		std::cout << "没有被标记完成的任务。\n";
		return;
	}

	std::cout << "已取消 " << changed << " 条任务的完成标记。\n";
}

// 按编号取消标记
void unmarkMultipleFlow(TodoList& list) {
	std::cout << "要取消标记的编号（空格分隔，例如 1 3 5）: ";
	const std::string line = readLine();

	std::istringstream input(line);

	int id = 0;
	int ok = 0;
	int skip = 0;
	int fail = 0;

	while (input >> id) {
		const int result = list.uncompleteTask(id);

		if (result == 1) {
			std::cout << "编号 " << id << " 已取消标记。\n";
			++ok;
		} else if (result == 0) {
			std::cout << "编号 " << id << " 本来就是未完成。\n";
			++skip;
		} else {
			std::cout << "编号 " << id << " 不存在。\n";
			++fail;
		}
	}

	if (ok + skip + fail == 0) {
		std::cout << "没有输入任何编号。\n";
		return;
	}

	std::cout << "处理完成：成功 " << ok << " 个，跳过 " << skip << " 个，失败 " << fail << " 个。\n";
}

// 批量标记子菜单入口
void markMenu(TodoList& list, bool& dirty) {
	bool back = false;

	while (!back) {
		printMarkMenu();

		int choice = -1;

		if (!readInt(choice)) {
			if (std::cin.eof()) return;

			std::cout << "请输入 0-4 的数字。\n";
			continue;
		}

		switch (choice) {
			case 1:
				markAllFlow(list);
				dirty = true;
				break;

			case 2:
				markMultipleFlow(list);
				dirty = true;
				break;

			case 3:
				unmarkAllFlow(list);
				dirty = true;
				break;

			case 4:
				unmarkMultipleFlow(list);
				dirty = true;
				break;

			case 0:
				back = true;
				break;

			default:
				std::cout << "没有这个选项。\n";
				break;
		}

		// 有改动则自动存盘
		if (dirty && !back) {
			if (persist(list)) dirty = false;
		}
	}
}

// 批量删除子菜单
void printDeleteMenu() {
	std::cout << "\n--- 批量删除 ---\n";
	std::cout << "  1. 按编号删除\n";
	std::cout << "  2. 清空已完成\n";
	std::cout << "  3. 删除所有任务\n";
	std::cout << "  0. 返回\n";
	std::cout << "请选择: ";
}

// 按编号删除多个任务
void deleteMultipleFlow(TodoList& list) {
	std::cout << "要删除的编号（空格分隔，例如 1 3 5）: ";
	const std::string line = readLine();

	std::istringstream input(line);

	std::vector<int> ids;
	int id = 0;

	while (input >> id) {
		ids.push_back(id);
	}

	if (ids.empty()) {
		std::cout << "没有输入任何编号。\n";
		return;
	}

	// 从大到小删，避免删掉后面的编号后前面的编号前移
	std::sort(ids.begin(), ids.end());
	std::reverse(ids.begin(), ids.end());

	int ok = 0;
	int fail = 0;

	for (int target : ids) {
		if (list.deleteTask(target)) {
			std::cout << "编号 " << target << " 已删除。\n";
			++ok;
		} else {
			std::cout << "编号 " << target << " 不存在。\n";
			++fail;
		}
	}

	std::cout << "处理完成：成功 " << ok << " 个，失败 " << fail << " 个。\n";
}

// 清空已完成
void clearCompletedFlow(TodoList& list) {
	const int changed = list.clearCompleted();

	if (changed == 0) {
		std::cout << "没有已完成的任务。\n";
		return;
	}

	std::cout << "已删除 " << changed << " 条已完成的任务。\n";
}

// 删除所有任务
void deleteAllFlow(TodoList& list) {
	Query all;
	const int total = static_cast<int>(list.find(all).size());

	if (total == 0) {
		std::cout << "当前没有任务。\n";
		return;
	}

	std::cout << "将删除全部 " << total << " 条任务，确定吗？（1 确认 / 0 取消）: ";

	int confirm = 0;

	if (!readInt(confirm) || confirm != 1) {
		std::cout << "已取消。\n";
		return;
	}

	const int changed = list.deleteAll();

	std::cout << "已删除 " << changed << " 条任务。\n";
}

// 批量删除子菜单入口
void deleteMenu(TodoList& list, bool& dirty) {
	bool back = false;

	while (!back) {
		printDeleteMenu();

		int choice = -1;

		if (!readInt(choice)) {
			if (std::cin.eof()) return;

			std::cout << "请输入 0-3 的数字。\n";
			continue;
		}

		switch (choice) {
			case 1:
				deleteMultipleFlow(list);
				dirty = true;
				break;

			case 2:
				clearCompletedFlow(list);
				dirty = true;
				break;

			case 3:
				deleteAllFlow(list);
				dirty = true;
				break;

			case 0:
				back = true;
				break;

			default:
				std::cout << "没有这个选项。\n";
				break;
		}

		// 有改动则自动存盘
		if (dirty && !back) {
			if (persist(list)) dirty = false;
		}
	}
}

void taskMenu(TodoList& list, bool& dirty) {
	bool back = false;

	while (!back) {
		printTaskMenu();

		int choice = -1;

		if (!readInt(choice)) {
			if (std::cin.eof()) return;

			std::cout << "请输入 0-10 的数字。\n";
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
				editMenu(list, dirty);
				break;

			case 7:
				list.sortByDeadline();
				std::cout << "已按截止日期排序（无期限的排在最后）。\n";
				list.showTasks();
				dirty = true;
				break;

			case 8:
				markMenu(list, dirty);
				break;

			case 9:
				deleteMenu(list, dirty);
				break;

			case 10:
				searchFlow(list);
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
