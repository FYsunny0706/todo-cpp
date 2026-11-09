#include "TodoList.h"

#include <algorithm>
#include <iostream>

#include "DateUtils.h"
#include "TaskFile.h"

namespace {
//----------自定义排序函数-----------
// 比较两个任务的截止日期，无期限视为最后
bool deadlineLess(const Task& a, const Task& b) {
	const std::string& deadlineA = a.getDeadline();
	const std::string& deadlineB = b.getDeadline();

	if (deadlineA.empty()) return false;

	if (deadlineB.empty()) return true;

	return deadlineA < deadlineB;
}

}  // namespace

TodoList::TodoList()
	: nextId(1) {
}

void TodoList::renumber() {
	for (std::size_t i = 0; i < tasks.size(); i++) {
		tasks[i].setId(static_cast<int>(i + 1));
	}

	nextId = static_cast<int>(tasks.size()) + 1;
}

bool TodoList::addTask(const std::string& title,
					   const std::string& description,
					   const std::string& deadline,
					   const std::string& category,
					   int importance) {
	if (title.empty()) return false;

	if (category.empty()) return false;

	if (!isValidDate(deadline)) return false;

	if (importance != 0 && importance != 1) importance = 0;

	Task task(nextId, title, description, false, deadline, category, importance);

	tasks.push_back(task);

	nextId++;

	return true;
}

void TodoList::printTask(const Task& task) const {
	std::cout << "------------------------" << "\n";

	std::cout << "ID: " << task.getId() << "\n";
	std::cout << "标题: " << task.getTitle() << "\n";
	std::cout << "描述: " << task.getDescription() << "\n";
	std::cout << "分类: " << task.getCategory();
	std::cout << "  重要: " << (task.getImportance() ? "是" : "否") << "\n";

	std::cout << "完成状态: "
			  << (task.isCompleted() ? "已完成" : "未完成")
			  << "\n";

	std::cout << "截止时间: "
			  << (task.getDeadline().empty()
					  ? "无"
					  : task.getDeadline())
			  << "\n";
}

void TodoList::showTasks() const {
	if (tasks.empty()) {
		std::cout << "当前没有待办事项。" << "\n";
		return;
	}

	for (const Task& task : tasks) {
		printTask(task);
	}

	std::cout << "------------------------" << "\n";
}

void TodoList::showTasks(const std::vector<int>& ids) const {
	for (int id : ids) {
		for (const Task& task : tasks) {
			if (task.getId() == id) {
				printTask(task);
				break;
			}
		}
	}

	std::cout << "------------------------" << "\n";
}

std::vector<int> TodoList::find(const Query& query) const {
	std::vector<int> result;

	for (const Task& task : tasks) {
		if (!query.keyword.empty() &&
			task.getTitle().find(query.keyword) == std::string::npos &&
			task.getDescription().find(query.keyword) == std::string::npos) {
			continue;
		}

		if (!query.category.empty() && task.getCategory() != query.category) {
			continue;
		}

		if (query.completed != -1 && task.isCompleted() != (query.completed == 1)) {
			continue;
		}

		if (!query.before.empty()) {
			const std::string deadline = task.getDeadline();

			if (deadline.empty() || deadline.substr(0, 10) >= query.before) {
				continue;
			}
		}

		if (query.importance != -1 && task.getImportance() != query.importance) {
			continue;
		}

		result.push_back(task.getId());
	}

	return result;
}

int TodoList::completeTask(int id) {
	for (Task& task : tasks) {
		if (task.getId() == id) {
			if (task.isCompleted()) return 0;

			task.markCompleted();
			return 1;
		}
	}

	return -1;
}

int TodoList::uncompleteTask(int id) {
	for (Task& task : tasks) {
		if (task.getId() == id) {
			if (!task.isCompleted()) return 0;

			task.markUncompleted();
			return 1;
		}
	}

	return -1;
}

bool TodoList::deleteTask(int id) {
	for (auto it = tasks.begin(); it != tasks.end(); it++) {
		if (it->getId() == id) {
			tasks.erase(it);
			renumber();
			return true;
		}
	}

	return false;
}

bool TodoList::updateDeadline(int id, const std::string& deadline) {
	if (!isValidDate(deadline)) return false;

	for (Task& task : tasks) {
		if (task.getId() == id) {
			task.setDeadline(deadline);
			return true;
		}
	}

	return false;
}

bool TodoList::updateTitle(int id, const std::string& title) {
	if (title.empty()) return false;

	for (Task& task : tasks) {
		if (task.getId() == id) {
			task.setTitle(title);
			return true;
		}
	}

	return false;
}

bool TodoList::updateDescription(int id, const std::string& description) {
	for (Task& task : tasks) {
		if (task.getId() == id) {
			task.setDescription(description);
			return true;
		}
	}

	return false;
}

bool TodoList::updateCategory(int id, const std::string& category) {
	if (category.empty()) return false;

	for (Task& task : tasks) {
		if (task.getId() == id) {
			task.setCategory(category);
			return true;
		}
	}

	return false;
}

bool TodoList::updateImportance(int id, int importance) {
	if (importance != 0 && importance != 1) return false;

	for (Task& task : tasks) {
		if (task.getId() == id) {
			task.setImportance(importance);
			return true;
		}
	}

	return false;
}

void TodoList::sortByDeadline() {
	std::stable_sort(tasks.begin(), tasks.end(), deadlineLess);
	renumber();
}

int TodoList::completeAll() {
	int changed = 0;

	for (Task& task : tasks) {
		if (!task.isCompleted()) {
			task.markCompleted();
			++changed;
		}
	}

	return changed;
}

int TodoList::uncompleteAll() {
	int changed = 0;

	for (Task& task : tasks) {
		if (task.isCompleted()) {
			task.markUncompleted();
			++changed;
		}
	}

	return changed;
}

int TodoList::deleteAll() {
	const int count = static_cast<int>(tasks.size());

	tasks.clear();
	renumber();

	return count;
}

int TodoList::clearCompleted() {
	const std::size_t before = tasks.size();

	auto it = tasks.begin();

	while (it != tasks.end()) {
		if (it->isCompleted())
			it = tasks.erase(it);
		else
			it++;
	}

	renumber();

	return static_cast<int>(before - tasks.size());
}

bool TodoList::saveToFile(const std::string& filename) const {
	return saveTasks(tasks, filename);
}

bool TodoList::loadFromFile(const std::string& filename) {
	std::vector<Task> loaded;

	if (!loadTasks(loaded, filename)) return false;

	tasks = loaded;
	renumber();

	return true;
}
