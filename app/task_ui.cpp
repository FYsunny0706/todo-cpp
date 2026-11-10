#include "task_ui.h"

#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>

#include "DateUtils.h"
#include "app_utils.h"

namespace {


// 添加任务
void addTaskFlow(TodoList& list) {
	std::cout << "标题（/ 取消）: ";
	std::string title;

	if (!readLineOrCancel(title)) {
		setStatus("已取消。\n");
		return;
	}

	if (title.empty()) {
		setStatus("添加失败：标题不能为空。\n");
		return;
	}

	std::cout << "描述（可留空，/ 取消）: ";
	std::string description;

	if (!readLineOrCancel(description)) {
		setStatus("已取消。\n");
		return;
	}

	std::cout << "截止日期 YYYY-MM-DD 或 YYYY-MM-DD HH:MM（可留空，/ 取消）: ";
	std::string deadline;

	if (!readLineOrCancel(deadline)) {
		setStatus("已取消。\n");
		return;
	}

	if (!isValidDate(deadline)) {
		setStatus("添加失败：截止日期必须是 YYYY-MM-DD 或 YYYY-MM-DD HH:MM，或留空。\n");
		return;
	}

	std::cout << "分类（1 学业 / 2 生活 / 3 工作 / 4 其他，回车 = 其他，/ 取消）: ";

	int categoryChoice = 4;
	bool canceled = false;

	if (!readIntOrCancel(categoryChoice, canceled)) {
		if (canceled) {
			setStatus("已取消。\n");
			return;
		}

		categoryChoice = 4;
	}

	std::cout << "是否重要？（1 是 / 0 否，回车 = 否，/ 取消）: ";

	int importance = 0;

	if (!readIntOrCancel(importance, canceled)) {
		if (canceled) {
			setStatus("已取消。\n");
			return;
		}

		importance = 0;
	}

	if (importance != 1) importance = 0;

	const std::string category = categoryName(categoryChoice);

	if (!list.addTask(title, description, deadline, category, importance)) {
		setStatus("添加失败。\n");
		return;
	}

	setStatus("已添加。\n");
}

void searchFlow(TodoList& list) {
	Query query;

	std::cout << "关键词（留空不限制，/ 取消）: ";

	if (!readLineOrCancel(query.keyword)) {
		setStatus("已取消。\n");
		return;
	}

	std::cout << "分类（1 学业 / 2 生活 / 3 工作 / 4 其他，0 不限，/ 取消）: ";

	int categoryChoice = 0;
	bool canceled = false;

	if (!readIntOrCancel(categoryChoice, canceled)) {
		if (canceled) {
			setStatus("已取消。\n");
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
			setStatus("已取消。\n");
			return;
		}

		stateChoice = 0;
	}

	if (stateChoice == 1) query.completed = 1;
	if (stateChoice == 2) query.completed = 0;

	const std::vector<int> found = list.find(query);

	if (found.empty()) {
		setStatus("没有匹配的任务。\n");
		return;
	}

	showFullScreen("找到 " + std::to_string(found.size()) + " 条：\n\n" + list.tasksBriefText(found));
}


// 修改标题
void editTitleFlow(TodoList& list, int id) {
	std::cout << "新标题（/ 取消）: ";
	std::string title;

	if (!readLineOrCancel(title)) {
		setStatus("已取消。\n");
		return;
	}

	if (title.empty()) {
		setStatus("修改失败：标题不能为空。\n");
		return;
	}

	if (!list.updateTitle(id, title)) {
		setStatus("没有这个编号。\n");
		return;
	}

	setStatus(list.taskText(id));
}

// 修改描述
void editDescriptionFlow(TodoList& list, int id) {
	std::cout << "新描述（可留空，/ 取消）: ";
	std::string description;

	if (!readLineOrCancel(description)) {
		setStatus("已取消。\n");
		return;
	}

	if (!list.updateDescription(id, description)) {
		setStatus("没有这个编号。\n");
		return;
	}

	setStatus(list.taskText(id));
}

// 修改截止日期
void updateDeadlineFlow(TodoList& list, int id) {
	std::cout << "新的截止日期 YYYY-MM-DD 或 YYYY-MM-DD HH:MM（留空表示无期限，/ 取消）: ";
	std::string deadline;

	if (!readLineOrCancel(deadline)) {
		setStatus("已取消。\n");
		return;
	}

	if (!isValidDate(deadline)) {
		setStatus("修改失败：截止日期必须是 YYYY-MM-DD 或 YYYY-MM-DD HH:MM，或留空。\n");
		return;
	}

	if (!list.updateDeadline(id, deadline)) {
		setStatus("没有这个编号。\n");
		return;
	}

	setStatus(list.taskText(id));
}

// 修改分类
void editCategoryFlow(TodoList& list, int id) {
	std::cout << "新分类（1 学业 / 2 生活 / 3 工作 / 4 其他，回车 = 其他，/ 取消）: ";

	int categoryChoice = 4;
	bool canceled = false;

	if (!readIntOrCancel(categoryChoice, canceled)) {
		if (canceled) {
			setStatus("已取消。\n");
			return;
		}

		categoryChoice = 4;
	}

	if (categoryChoice < 1 || categoryChoice > 4) categoryChoice = 4;

	const std::string category = categoryName(categoryChoice);

	if (!list.updateCategory(id, category)) {
		setStatus("没有这个编号。\n");
		return;
	}

	setStatus(list.taskText(id));
}

// 修改重要度
void editImportanceFlow(TodoList& list, int id) {
	std::cout << "是否重要？（1 是 / 0 否，回车 = 否，/ 取消）: ";

	int importance = 0;
	bool canceled = false;

	if (!readIntOrCancel(importance, canceled)) {
		if (canceled) {
			setStatus("已取消。\n");
			return;
		}

		importance = 0;
	}

	if (importance != 1) importance = 0;

	if (!list.updateImportance(id, importance)) {
		setStatus("没有这个编号。\n");
		return;
	}

	setStatus(list.taskText(id));
}

// 修改任务子菜单
void editMenu(TodoList& list, int& saved) {
	std::cout << "要修改的任务编号（/ 取消）: ";

	int id = 0;
	bool canceled = false;

	if (!readIntOrCancel(id, canceled)) {
		if (canceled) {
			setStatus("已取消。\n");
		} else {
			setStatus("请输入数字。\n");
		}

		return;
	}

	const std::string current = list.taskText(id);

	if (current.empty()) {
		setStatus("没有这个编号。\n");
		return;
	}

	setStatus(current);

	const std::vector<std::string> items = {
		"修改标题",
		"修改描述",
		"修改截止日期",
		"修改分类",
		"修改重要度",
	};

	while (true) {
		const int choice = askMenu("\n--- 修改任务 ---", items);

		if (choice <= 0) break;

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

			default:
				break;
		}

		// 数据有变动就自动存盘
		if (list.getRevision() != saved) {
			if (persist(list)) saved = list.getRevision();
		}
	}
}


// 全部标记完成
void markAllFlow(TodoList& list) {
	const int changed = list.completeAll();

	if (changed == 0) {
		setStatus("没有需要标记的任务。\n");
		return;
	}

	setStatus("已把 " + std::to_string(changed) + " 条任务标记为完成。\n");
}

// 按编号标记多个任务
void markMultipleFlow(TodoList& list) {
	std::cout << "要标记完成的编号（空格分隔，例如 1 3 5，/ 取消）: ";
	std::string line;

	if (!readLineOrCancel(line)) {
		setStatus("已取消。\n");
		return;
	}

	std::istringstream input(line);

	std::string report;
	int id = 0;
	int ok = 0;
	int skip = 0;
	int fail = 0;

	while (input >> id) {
		const int result = list.completeTask(id);

		if (result == 1) {
			report += "编号 " + std::to_string(id) + " 已标记完成。\n";
			++ok;
		} else if (result == 0) {
			report += "编号 " + std::to_string(id) + " 已经是完成状态。\n";
			++skip;
		} else {
			report += "编号 " + std::to_string(id) + " 不存在。\n";
			++fail;
		}
	}

	if (!input.eof()) {
		report += "注意：输入中含有无法识别的内容，后面的编号已被忽略。\n";
	}

	if (ok + skip + fail == 0) {
		setStatus("没有输入任何编号。\n");
		return;
	}

	report += "处理完成：成功 " + std::to_string(ok) + " 个，跳过 " + std::to_string(skip) +
			  " 个，失败 " + std::to_string(fail) + " 个。\n";

	setStatus(report);
}

// 全部取消标记
void unmarkAllFlow(TodoList& list) {
	const int changed = list.uncompleteAll();

	if (changed == 0) {
		setStatus("没有被标记完成的任务。\n");
		return;
	}

	setStatus("已取消 " + std::to_string(changed) + " 条任务的完成标记。\n");
}

// 按编号取消标记
void unmarkMultipleFlow(TodoList& list) {
	std::cout << "要取消标记的编号（空格分隔，例如 1 3 5，/ 取消）: ";
	std::string line;

	if (!readLineOrCancel(line)) {
		setStatus("已取消。\n");
		return;
	}

	std::istringstream input(line);

	std::string report;
	int id = 0;
	int ok = 0;
	int skip = 0;
	int fail = 0;

	while (input >> id) {
		const int result = list.uncompleteTask(id);

		if (result == 1) {
			report += "编号 " + std::to_string(id) + " 已取消标记。\n";
			++ok;
		} else if (result == 0) {
			report += "编号 " + std::to_string(id) + " 本来就是未完成。\n";
			++skip;
		} else {
			report += "编号 " + std::to_string(id) + " 不存在。\n";
			++fail;
		}
	}

	if (!input.eof()) {
		report += "注意：输入中含有无法识别的内容，后面的编号已被忽略。\n";
	}

	if (ok + skip + fail == 0) {
		setStatus("没有输入任何编号。\n");
		return;
	}

	report += "处理完成：成功 " + std::to_string(ok) + " 个，跳过 " + std::to_string(skip) +
			  " 个，失败 " + std::to_string(fail) + " 个。\n";

	setStatus(report);
}

// 批量标记子菜单入口
void markMenu(TodoList& list, int& saved) {
	const std::vector<std::string> items = {
		"按编号标记完成",
		"按编号取消标记",
		"全部标记完成",
		"全部取消标记",
	};

	while (true) {
		const int choice = askMenu("\n--- 批量标记 ---", items);

		if (choice <= 0) break;

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

			default:
				break;
		}

		// 数据有变动就自动存盘
		if (list.getRevision() != saved) {
			if (persist(list)) saved = list.getRevision();
		}
	}
}


// 按编号删除多个任务
void deleteMultipleFlow(TodoList& list) {
	std::cout << "要删除的编号（空格分隔，例如 1 3 5，/ 取消）: ";
	std::string line;

	if (!readLineOrCancel(line)) {
		setStatus("已取消。\n");
		return;
	}

	std::istringstream input(line);

	std::vector<int> ids;
	int id = 0;

	while (input >> id) {
		ids.push_back(id);
	}

	if (!input.eof()) {
		setStatus("注意：输入中含有无法识别的内容，请重新输入。\n");
		return;
	}

	if (ids.empty()) {
		setStatus("没有输入任何编号。\n");
		return;
	}

	// 先排序去重，再从大到小删，避免重复编号删掉别的任务
	std::sort(ids.begin(), ids.end());
	ids.erase(std::unique(ids.begin(), ids.end()), ids.end());
	std::reverse(ids.begin(), ids.end());

	std::string report;
	int ok = 0;
	int fail = 0;

	for (int target : ids) {
		if (list.deleteTask(target)) {
			report += "编号 " + std::to_string(target) + " 已删除。\n";
			++ok;
		} else {
			report += "编号 " + std::to_string(target) + " 不存在。\n";
			++fail;
		}
	}

	report += "处理完成：成功 " + std::to_string(ok) + " 个，失败 " + std::to_string(fail) + " 个。\n";

	setStatus(report);
}

// 清空已完成
void clearCompletedFlow(TodoList& list) {
	const int changed = list.clearCompleted();

	if (changed == 0) {
		setStatus("没有已完成的任务。\n");
		return;
	}

	setStatus("已删除 " + std::to_string(changed) + " 条已完成的任务。\n");
}

// 删除所有任务
void deleteAllFlow(TodoList& list) {
	Query all;
	const int total = static_cast<int>(list.find(all).size());

	if (total == 0) {
		setStatus("当前没有任务。\n");
		return;
	}

	std::cout << "将删除全部 " << total << " 条任务，确定吗？（1 确认 / 0 取消）: ";

	int confirm = 0;

	if (!readInt(confirm) || confirm != 1) {
		setStatus("已取消。\n");
		return;
	}

	const int changed = list.deleteAll();

	setStatus("已删除 " + std::to_string(changed) + " 条任务。\n");
}

// 批量删除子菜单入口
void deleteMenu(TodoList& list, int& saved) {
	const std::vector<std::string> items = {
		"按编号删除",
		"清空已完成",
		"删除所有任务",
	};

	while (true) {
		const int choice = askMenu("\n--- 批量删除 ---", items);

		if (choice <= 0) break;

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

			default:
				break;
		}

		// 数据有变动就自动存盘
		if (list.getRevision() != saved) {
			if (persist(list)) saved = list.getRevision();
		}
	}
}
}  // namespace

void taskMenu(TodoList& list, int& saved) {
	const std::vector<std::string> items = {
		"查看全部任务",
		"添加任务",
		"修改任务",
		"按截止日期排序",
		"标记任务与取消标记",
		"删除任务",
		"查找任务",
	};

	while (true) {
		const int choice = askMenu("\n--- 任务管理 ---", items);

		if (choice <= 0) break;

		switch (choice) {
			case 1:
				showFullScreen(list.tasksBriefText());
				break;

			case 2:
				addTaskFlow(list);
				break;

			case 3:
				editMenu(list, saved);
				break;

			case 4:
				if (list.sortByDeadline()) {
					showFullScreen("已按截止日期排序（无期限的排在最后）。\n\n" + list.tasksBriefText());
				} else {
					showFullScreen("顺序没有变化。\n\n" + list.tasksBriefText());
				}
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

			default:
				break;
		}

		// 数据有变动就自动存盘
		if (list.getRevision() != saved) {
			if (persist(list)) saved = list.getRevision();
		}
	}
}
