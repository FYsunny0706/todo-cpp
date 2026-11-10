#include "task_ui.h"

#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>

#include "DateUtils.h"
#include "app_utils.h"

namespace {

// 任务管理子菜单
void printTaskMenu() {
	std::cout << "\n--- 任务管理 ---\n";
	std::cout << "  1. 查看全部任务\n";
	std::cout << "  2. 添加任务\n";
	std::cout << "  3. 修改任务\n";
	std::cout << "  4. 按截止日期排序\n";
	std::cout << "  5. 标记任务与取消标记\n";
	std::cout << "  6. 删除任务\n";
	std::cout << "  7. 查找任务\n";
	std::cout << "  0. 返回\n";
	std::cout << "请选择: ";
}

// 添加任务
void addTaskFlow(TodoList& list) {
	std::cout << "标题（/ 取消）: ";
	std::string title;

	if (!readLineOrCancel(title)) {
		std::cout << "已取消。\n";
		return;
	}

	if (title.empty()) {
		std::cout << "添加失败：标题不能为空。\n";
		return;
	}

	std::cout << "描述（可留空，/ 取消）: ";
	std::string description;

	if (!readLineOrCancel(description)) {
		std::cout << "已取消。\n";
		return;
	}

	std::cout << "截止日期 YYYY-MM-DD 或 YYYY-MM-DD HH:MM（可留空，/ 取消）: ";
	std::string deadline;

	if (!readLineOrCancel(deadline)) {
		std::cout << "已取消。\n";
		return;
	}

	if (!isValidDate(deadline)) {
		std::cout << "添加失败：截止日期必须是 YYYY-MM-DD 或 YYYY-MM-DD HH:MM，或留空。\n";
		return;
	}

	std::cout << "分类（1 学业 / 2 生活 / 3 工作 / 4 其他，回车 = 其他，/ 取消）: ";

	int categoryChoice = 4;
	bool canceled = false;

	if (!readIntOrCancel(categoryChoice, canceled)) {
		if (canceled) {
			std::cout << "已取消。\n";
			return;
		}

		categoryChoice = 4;
	}

	std::cout << "是否重要？（1 是 / 0 否，回车 = 否，/ 取消）: ";

	int importance = 0;

	if (!readIntOrCancel(importance, canceled)) {
		if (canceled) {
			std::cout << "已取消。\n";
			return;
		}

		importance = 0;
	}

	if (importance != 1) importance = 0;

	const std::string category = categoryName(categoryChoice);

	if (!list.addTask(title, description, deadline, category, importance)) {
		std::cout << "添加失败。\n";
		return;
	}

	std::cout << "已添加。\n";
}

void searchFlow(TodoList& list) {
	Query query;

	std::cout << "关键词（留空不限制，/ 取消）: ";

	if (!readLineOrCancel(query.keyword)) {
		std::cout << "已取消。\n";
		return;
	}

	std::cout << "分类（1 学业 / 2 生活 / 3 工作 / 4 其他，0 不限，/ 取消）: ";

	int categoryChoice = 0;
	bool canceled = false;

	if (!readIntOrCancel(categoryChoice, canceled)) {
		if (canceled) {
			std::cout << "已取消。\n";
			return;
		}

		categoryChoice = 0;
	}

	if (categoryChoice >= 1 && categoryChoice <= 4) {
		query.category = categoryName(categoryChoice);
	}

	std::cout << "状态（1 已完成 / 2 未完成，0 不限，/ 取消）: ";

	int stateChoice = 0;

	if (!readIntOrCancel(stateChoice, canceled)) {
		if (canceled) {
			std::cout << "已取消。\n";
			return;
		}

		stateChoice = 0;
	}

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
void editTitleFlow(TodoList& list, int id) {
	std::cout << "新标题（/ 取消）: ";
	std::string title;

	if (!readLineOrCancel(title)) {
		std::cout << "已取消。\n";
		return;
	}

	if (title.empty()) {
		std::cout << "修改失败：标题不能为空。\n";
		return;
	}

	if (!list.updateTitle(id, title)) {
		std::cout << "修改失败：ID 不存在。\n";
		return;
	}

	list.showTask(id);
}

// 修改描述
void editDescriptionFlow(TodoList& list, int id) {
	std::cout << "新描述（可留空，/ 取消）: ";
	std::string description;

	if (!readLineOrCancel(description)) {
		std::cout << "已取消。\n";
		return;
	}

	if (!list.updateDescription(id, description)) {
		std::cout << "修改失败：ID 不存在。\n";
		return;
	}

	list.showTask(id);
}

// 修改截止日期
void updateDeadlineFlow(TodoList& list, int id) {
	std::cout << "新的截止日期 YYYY-MM-DD 或 YYYY-MM-DD HH:MM（留空表示无期限，/ 取消）: ";
	std::string deadline;

	if (!readLineOrCancel(deadline)) {
		std::cout << "已取消。\n";
		return;
	}

	if (!isValidDate(deadline)) {
		std::cout << "修改失败：截止日期必须是 YYYY-MM-DD 或 YYYY-MM-DD HH:MM，或留空。\n";
		return;
	}

	if (!list.updateDeadline(id, deadline)) {
		std::cout << "修改失败：ID 不存在。\n";
		return;
	}

	list.showTask(id);
}

// 修改分类
void editCategoryFlow(TodoList& list, int id) {
	std::cout << "新分类（1 学业 / 2 生活 / 3 工作 / 4 其他，回车 = 其他，/ 取消）: ";

	int categoryChoice = 4;
	bool canceled = false;

	if (!readIntOrCancel(categoryChoice, canceled)) {
		if (canceled) {
			std::cout << "已取消。\n";
			return;
		}

		categoryChoice = 4;
	}

	if (categoryChoice < 1 || categoryChoice > 4) categoryChoice = 4;

	const std::string category = categoryName(categoryChoice);

	if (!list.updateCategory(id, category)) {
		std::cout << "修改失败：ID 不存在。\n";
		return;
	}

	list.showTask(id);
}

// 修改重要度
void editImportanceFlow(TodoList& list, int id) {
	std::cout << "是否重要？（1 是 / 0 否，回车 = 否，/ 取消）: ";

	int importance = 0;
	bool canceled = false;

	if (!readIntOrCancel(importance, canceled)) {
		if (canceled) {
			std::cout << "已取消。\n";
			return;
		}

		importance = 0;
	}

	if (importance != 1) importance = 0;

	if (!list.updateImportance(id, importance)) {
		std::cout << "修改失败：ID 不存在。\n";
		return;
	}

	list.showTask(id);
}

// 修改任务子菜单
void editMenu(TodoList& list, int& saved) {
	std::cout << "要修改的任务编号（/ 取消）: ";

	int id = 0;
	bool canceled = false;

	if (!readIntOrCancel(id, canceled)) {
		if (canceled) {
			std::cout << "已取消。\n";
		} else {
			std::cout << "请输入数字。\n";
		}

		return;
	}

	if (!list.showTask(id)) {
		std::cout << "没有这个编号。\n";
		return;
	}

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
				editTitleFlow(list, id);
				break;

			case 2:
				editDescriptionFlow(list, id);
				break;

			case 3:
				updateDeadlineFlow(list, id);
				break;

			case 4:
				editCategoryFlow(list, id);
				break;

			case 5:
				editImportanceFlow(list, id);
				break;

			case 0:
				back = true;
				break;

			default:
				std::cout << "没有这个选项。\n";
				break;
		}

		// 数据有变动就自动存盘
		if (list.getRevision() != saved && !back) {
			if (persist(list)) saved = list.getRevision();
		}
	}
}

// 批量标记子菜单
void printMarkMenu() {
	std::cout << "\n--- 批量标记 ---\n";
	std::cout << "  1. 按编号标记完成\n";
	std::cout << "  2. 按编号取消标记\n";
	std::cout << "  3. 全部标记完成\n";
	std::cout << "  4. 全部取消标记\n";
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
	std::cout << "要标记完成的编号（空格分隔，例如 1 3 5，/ 取消）: ";
	std::string line;

	if (!readLineOrCancel(line)) {
		std::cout << "已取消。\n";
		return;
	}

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

	if (!input.eof()) {
		std::cout << "注意：输入中含有无法识别的内容，后面的编号已被忽略。\n";
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
	std::cout << "要取消标记的编号（空格分隔，例如 1 3 5，/ 取消）: ";
	std::string line;

	if (!readLineOrCancel(line)) {
		std::cout << "已取消。\n";
		return;
	}

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

	if (!input.eof()) {
		std::cout << "注意：输入中含有无法识别的内容，后面的编号已被忽略。\n";
	}

	if (ok + skip + fail == 0) {
		std::cout << "没有输入任何编号。\n";
		return;
	}

	std::cout << "处理完成：成功 " << ok << " 个，跳过 " << skip << " 个，失败 " << fail << " 个。\n";
}

// 批量标记子菜单入口
void markMenu(TodoList& list, int& saved) {
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
				markMultipleFlow(list);
				break;

			case 2:
				unmarkMultipleFlow(list);
				break;

			case 3:
				markAllFlow(list);
				break;

			case 4:
				unmarkAllFlow(list);
				break;

			case 0:
				back = true;
				break;

			default:
				std::cout << "没有这个选项。\n";
				break;
		}

		// 数据有变动就自动存盘
		if (list.getRevision() != saved && !back) {
			if (persist(list)) saved = list.getRevision();
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
	std::cout << "要删除的编号（空格分隔，例如 1 3 5，/ 取消）: ";
	std::string line;

	if (!readLineOrCancel(line)) {
		std::cout << "已取消。\n";
		return;
	}

	std::istringstream input(line);

	std::vector<int> ids;
	int id = 0;

	while (input >> id) {
		ids.push_back(id);
	}

	if (!input.eof()) {
		std::cout << "注意：输入中含有无法识别的内容，后面的编号已被忽略。\n";
	}

	if (ids.empty()) {
		std::cout << "没有输入任何编号。\n";
		return;
	}

	// 先排序去重，再从大到小删，避免重复编号删掉别的任务
	std::sort(ids.begin(), ids.end());
	ids.erase(std::unique(ids.begin(), ids.end()), ids.end());
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
void deleteMenu(TodoList& list, int& saved) {
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
				break;

			case 2:
				clearCompletedFlow(list);
				break;

			case 3:
				deleteAllFlow(list);
				break;

			case 0:
				back = true;
				break;

			default:
				std::cout << "没有这个选项。\n";
				break;
		}

		// 数据有变动就自动存盘
		if (list.getRevision() != saved && !back) {
			if (persist(list)) saved = list.getRevision();
		}
	}
}
}  // namespace

void taskMenu(TodoList& list, int& saved) {
	bool back = false;

	while (!back) {
		printTaskMenu();

		int choice = -1;

		if (!readInt(choice)) {
			if (std::cin.eof()) return;

			std::cout << "请输入 0-7 的数字。\n";
			continue;
		}

		switch (choice) {
			case 1:
				list.showTasks();
				break;

			case 2:
				addTaskFlow(list);
				break;

			case 3:
				editMenu(list, saved);
				break;

			case 4:
				if (list.sortByDeadline()) {
					std::cout << "已按截止日期排序（无期限的排在最后）。\n";
				} else {
					std::cout << "顺序没有变化。\n";
				}

				list.showTasks();
				break;

			case 5:
				markMenu(list, saved);
				break;

			case 6:
				deleteMenu(list, saved);
				break;

			case 7:
				searchFlow(list);
				break;

			case 0:
				back = true;
				break;

			default:
				std::cout << "没有这个选项。\n";
				break;
		}

		// 数据有变动就自动存盘
		if (list.getRevision() != saved && !back) {
			if (persist(list)) saved = list.getRevision();
		}
	}
}
