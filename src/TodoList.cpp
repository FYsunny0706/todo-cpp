#include "TodoList.h"
#include "DateUtils.h"
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <set>
#include <sstream>

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

// ---------- 文件字段转义 ----------
// 存盘格式：id|title|description|completed|deadline
// 转义单个字段中的 \、| 和换行
std::string escapeField(const std::string& s) {
	std::string out;
	out.reserve(s.size() + 8);
	for (char c : s) {
		switch (c) {
			case '\\':
				out += "\\\\";
				break;
			case '|':
				out += "\\|";
				break;
			case '\n':
				out += "\\n";
				break;
			case '\r':
				out += "\\r";
				break;
			default:
				out += c;
				break;
		}
	}
	return out;
}

// 还原字段中的转义，遇到非法转义返回 false
bool unescapeField(const std::string& s, std::string& out) {
	out.clear();
	out.reserve(s.size());
	for (std::size_t i = 0; i < s.size(); ++i) {
		if (s[i] != '\\') {
			out += s[i];
			continue;
		}
		if (i + 1 >= s.size()) return false;
		switch (s[++i]) {
			case '\\':
				out += '\\';
				break;
			case '|':
				out += '|';
				break;
			case 'n':
				out += '\n';
				break;
			case 'r':
				out += '\r';
				break;
			default:
				return false;
		}
	}
	return true;
}

// 按未被转义的 | 切分一行，保留转义序列
bool splitLine(const std::string& line, std::vector<std::string>& fields) {
	fields.clear();
	std::string current;
	for (std::size_t i = 0; i < line.size(); ++i) {
		char c = line[i];
		if (c == '\\') {
			if (i + 1 >= line.size()) return false;
			current += c;
			current += line[++i];
			continue;
		}
		if (c == '|') {
			fields.push_back(current);
			current.clear();
			continue;
		}
		current += c;
	}
	fields.push_back(current);
	return fields.size() == 5;
}

}  // namespace

TodoList::TodoList()
	: nextId(1) {
}

// 添加任务，成功返回 true
bool TodoList::addTask(const std::string& title,
					   const std::string& description,
					   const std::string& deadline) {
	if (title.empty()) return false;

	if (!isValidDate(deadline)) return false;

	Task task(nextId, title, description, false, deadline);

	tasks.push_back(task);

	nextId++;

	return true;
}

// 打印全部任务，空列表时输出提示
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

// 按 ID 标记完成，找不到返回 false
bool TodoList::completeTask(int id) {
	for (Task& task : tasks) {
		if (task.getId() == id) {
			task.markCompleted();
			return true;
		}
	}

	return false;
}

// 按 ID 删除任务，找不到返回 false
bool TodoList::deleteTask(int id) {
	for (auto it = tasks.begin(); it != tasks.end(); it++) {
		if (it->getId() == id) {
			tasks.erase(it);
			return true;
		}
	}

	return false;
}

// 修改截止日期，日期非法或 ID 不存在返回 false
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

// 按截止日期升序排序，无期限的排在最后
void TodoList::sortByDeadline() {
	std::stable_sort(tasks.begin(), tasks.end(), deadlineLess);
}

// 把所有任务标记为已完成
void TodoList::completeAll() {
	for (Task& task : tasks) {
		task.markCompleted();
	}
}

// 删除所有已完成的任务
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
	const std::string tempName = filename + ".tmp";

	{
		std::ofstream file(tempName);

		if (!file.is_open()) return false;

		for (const Task& task : tasks) {
			file << task.getId() << '|'
				 << escapeField(task.getTitle()) << '|'
				 << escapeField(task.getDescription()) << '|'
				 << (task.isCompleted() ? 1 : 0) << '|'
				 << escapeField(task.getDeadline()) << '\n';

			if (!file) return false;
		}

		file.flush();

		if (!file) return false;
	}

	// 用临时文件写入，成功后再替换原文件
	std::remove(filename.c_str());

	if (std::rename(tempName.c_str(), filename.c_str()) != 0) {
		std::remove(tempName.c_str());
		return false;
	}

	return true;
}

bool TodoList::loadFromFile(const std::string& filename) {
	std::ifstream file(filename);

	if (!file.is_open()) return false;

	std::vector<Task> loadedTasks;
	std::string line;
	std::set<int> seenIds;
	int maxId = 0;

	while (std::getline(file, line)) {
		if (!line.empty() && line.back() == '\r') line.pop_back();

		if (line.empty()) continue;

		std::vector<std::string> fields;

		if (!splitLine(line, fields)) return false;

		std::string idStr;
		std::string title;
		std::string description;
		std::string completedStr;
		std::string deadline;

		if (!unescapeField(fields[0], idStr)) return false;
		if (!unescapeField(fields[1], title)) return false;
		if (!unescapeField(fields[2], description)) return false;
		if (!unescapeField(fields[3], completedStr)) return false;
		if (!unescapeField(fields[4], deadline)) return false;

		int id;
		try {
			std::size_t pos = 0;
			id = std::stoi(idStr, &pos);

			if (pos != idStr.size() || id <= 0) return false;
		} catch (...) {
			return false;
		}

		if (completedStr != "0" && completedStr != "1") return false;

		if (title.empty()) return false;

		if (!seenIds.insert(id).second) return false;

		if (!isValidDate(deadline)) return false;

		Task task(id, title, description, completedStr == "1", deadline);

		loadedTasks.push_back(task);

		if (id > maxId) maxId = id;
	}

	// 检查读取过程中是否发生错误
	if (file.bad()) return false;

	tasks = loadedTasks;
	// 恢复下一个可用编号
	nextId = maxId + 1;

	return true;
}
