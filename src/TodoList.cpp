#include "TodoList.h"
#include "DateUtils.h"
#include <iostream>
#include <algorithm>

namespace {

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

void TodoList::addTask(const std::string& title,
					   const std::string& description,
					   const std::string& deadline) {
	Task task(nextId, title, description, false, deadline);

	tasks.push_back(task);

	nextId++;
}

void TodoList::showTasks() const {
	if (tasks.empty()) {
		std::cout << "当前没有待办事项。" << std::endl;
		return;
	}

	for (const Task& task : tasks) {
		std::cout << "------------------------" << std::endl;

		std::cout << "ID: " << task.getId() << std::endl;
		std::cout << "标题: " << task.getTitle() << std::endl;
		std::cout << "描述: " << task.getDescription() << std::endl;

		std::cout << "完成状态: "
				  << (task.isCompleted() ? "已完成" : "未完成")
				  << std::endl;

		std::cout << "截止时间: "
				  << (task.getDeadline().empty()
						  ? "无"
						  : task.getDeadline())
				  << std::endl;
	}

	std::cout << "------------------------" << std::endl;
}

bool TodoList::completeTask(int id) {
	for (Task& task : tasks) {
		if (task.getId() == id) {
			task.markCompleted();
			return true;
		}
	}

	return false;
}

bool TodoList::deleteTask(int id) {
	for (auto it = tasks.begin(); it != tasks.end(); it++) {
		if (it->getId() == id) {
			tasks.erase(it);
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

void TodoList::sortByDeadline() {
	std::stable_sort(tasks.begin(), tasks.end(), deadlineLess);
}

void TodoList::completeAll() {
	for (Task& task : tasks) {
		task.markCompleted();
	}
}

void TodoList::clearCompleted() {
	auto it = tasks.begin();
	while (it != tasks.end()) {
		if (it->isCompleted())
			it = tasks.erase(it);
		else
			it++;
	}
}