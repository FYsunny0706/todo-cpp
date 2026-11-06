#include "TodoList.h"
#include "DateUtils.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

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

bool TodoList::saveToFile(const std::string& filename) const {
	std::ofstream file(filename);

	if (!file.is_open()) return false;

	for (const Task& task : tasks) {
		file << task.getId() << '|'
			 << task.getTitle() << '|'
			 << task.getDescription() << '|'
			 << (task.isCompleted() ? 1 : 0) << '|'
			 << task.getDeadline() << '\n';

		if (!file) return false;
	}

	return true;
}

bool TodoList::loadFromFile(const std::string& filename) {
	std::ifstream file(filename);

	if (!file.is_open()) return false;

	std::vector<Task> loadedTasks;
	std::string line;
	int maxId = 0;

	while (std::getline(file, line)) {
		std::stringstream ss(line);

		std::string idStr;
		std::string title;
		std::string description;
		std::string completedStr;
		std::string deadline;

		if (!std::getline(ss, idStr, '|') ||
			!std::getline(ss, title, '|') ||
			!std::getline(ss, description, '|') ||
			!std::getline(ss, completedStr, '|') ||
			!std::getline(ss, deadline)) {
			return false;
		}

		int id;
		try {
			std::size_t pos = 0;
			id = std::stoi(idStr, &pos);

			if (pos != idStr.size() || id <= 0) return false;
		} catch (...) {
			return false;
		}

		if (completedStr != "0" && completedStr != "1") return false;

		if (!isValidDate(deadline)) return false;

		Task task(
			id,
			title,
			description,
			completedStr == "1",
			deadline);

		loadedTasks.push_back(task);

		if (id > maxId) maxId = id;
	}

	if (file.bad()) return false;

	tasks = loadedTasks;
	nextId = maxId + 1;

	return true;
}