#include "TodoList.h"
#include "DateUtils.h"
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <set>
#include <sstream>
#include <system_error>

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
bool splitLine(const std::string& line, std::vector<std::string>& fields, std::size_t expected) {
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
	return fields.size() == expected;
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
	std::cout << "------------------------" << std::endl;

	std::cout << "ID: " << task.getId() << std::endl;
	std::cout << "标题: " << task.getTitle() << std::endl;
	std::cout << "描述: " << task.getDescription() << std::endl;
	std::cout << "分类: " << task.getCategory();
	std::cout << "  重要: " << (task.getImportance() ? "是" : "否") << std::endl;

	std::cout << "完成状态: "
			  << (task.isCompleted() ? "已完成" : "未完成")
			  << std::endl;

	std::cout << "截止时间: "
			  << (task.getDeadline().empty()
					  ? "无"
					  : task.getDeadline())
			  << std::endl;
}

void TodoList::showTasks() const {
	if (tasks.empty()) {
		std::cout << "当前没有待办事项。" << std::endl;
		return;
	}

	for (const Task& task : tasks) {
		printTask(task);
	}

	std::cout << "------------------------" << std::endl;
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

	std::cout << "------------------------" << std::endl;
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
	const std::string tempName = filename + ".tmp";
	std::error_code ignored;

	bool ok = true;

	{
		std::ofstream file(tempName);

		if (!file.is_open()) return false;

		file << "v2" << '\n';

		for (const Task& task : tasks) {
			file << task.getId() << '|'
				 << escapeField(task.getTitle()) << '|'
				 << escapeField(task.getDescription()) << '|'
				 << (task.isCompleted() ? 1 : 0) << '|'
				 << escapeField(task.getDeadline()) << '|'
				 << escapeField(task.getCategory()) << '|'
				 << task.getImportance() << '\n';

			if (!file) {
				ok = false;
				break;
			}
		}

		if (ok) {
			file.flush();

			if (!file) ok = false;
		}
	}

	// 写入失败时删掉临时文件，避免留下半成品
	if (!ok) {
		std::filesystem::remove(tempName, ignored);
		return false;
	}

	// 用临时文件写入，成功后再替换原文件
	std::error_code error;
	std::filesystem::rename(tempName, filename, error);

	if (error) {
		std::filesystem::remove(tempName, ignored);
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

	bool v2 = false;
	bool firstLine = true;

	while (std::getline(file, line)) {
		while (!line.empty() && (line.back() == ' ' || line.back() == '\t' || line.back() == '\r'))
			line.pop_back();

		if (line.empty()) continue;

		if (firstLine) {
			firstLine = false;

			// 去掉可能的 UTF-8 BOM
			if (line.size() >= 3 &&
				static_cast<unsigned char>(line[0]) == 0xEF &&
				static_cast<unsigned char>(line[1]) == 0xBB &&
				static_cast<unsigned char>(line[2]) == 0xBF) {
				line.erase(0, 3);
			}

			if (line.empty()) continue;

			if (line == "v2") {
				v2 = true;
				continue;
			}
		}

		std::vector<std::string> fields;

		if (!splitLine(line, fields, v2 ? 7 : 5)) return false;

		std::string idStr;
		std::string title;
		std::string description;
		std::string completedStr;
		std::string deadline;
		std::string category = "其他";
		int importance = 0;

		if (!unescapeField(fields[0], idStr)) return false;
		if (!unescapeField(fields[1], title)) return false;
		if (!unescapeField(fields[2], description)) return false;
		if (!unescapeField(fields[3], completedStr)) return false;
		if (!unescapeField(fields[4], deadline)) return false;

		if (v2) {
			std::string importanceStr;

			if (!unescapeField(fields[5], category)) return false;
			if (!unescapeField(fields[6], importanceStr)) return false;

			if (importanceStr != "0" && importanceStr != "1") return false;

			importance = (importanceStr == "1") ? 1 : 0;
		}

		int id;
		try {
			std::size_t pos = 0;
			id = std::stoi(idStr, &pos);

			if (pos != idStr.size() || id <= 0) return false;
		} catch (...) {
			return false;
		}

		if (seenIds.count(id) > 0) return false;
		seenIds.insert(id);

		if (completedStr != "0" && completedStr != "1") return false;

		if (title.empty()) return false;

		if (!isValidDate(deadline)) return false;

		Task task(id, title, description, completedStr == "1", deadline, category, importance);

		loadedTasks.push_back(task);
	}

	// 检查读取过程中是否发生错误
	if (file.bad()) return false;

	tasks = loadedTasks;
	// 恢复下一个可用编号
	renumber();

	return true;
}
