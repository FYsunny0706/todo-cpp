#include "TaskFile.h"

#include <filesystem>
#include <fstream>
#include <set>
#include <system_error>

#include "DateUtils.h"

namespace {

// 存盘格式：v2 / id|title|description|completed|deadline|category|importance
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

// 分类只允许这四个值
bool isKnownCategory(const std::string& category) {
	return category == "学业" || category == "生活" ||
		   category == "工作" || category == "其他";
}

}  // namespace

bool saveTasks(const std::vector<Task>& tasks, const std::string& filename) {
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

bool loadTasks(std::vector<Task>& out, const std::string& filename) {
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

			if (!isKnownCategory(category)) category = "其他";
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

	if (file.bad()) return false;

	out = loadedTasks;

	return true;
}
