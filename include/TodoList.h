#ifndef TODOLIST_H
#define TODOLIST_H

#include <vector>
#include <string>
#include "Task.h"

// 待办事项集合：增删改查、排序与持久化
class TodoList {
   private:
	std::vector<Task> tasks;
	// 下一个可用的任务编号
	int nextId;

   public:
	TodoList();

	bool addTask(const std::string& title,
				 const std::string& description,
				 const std::string& deadline);

	void showTasks() const;

	bool completeTask(int id);

	bool uncompleteTask(int id);

	bool deleteTask(int id);

	bool updateDeadline(int id, const std::string& deadline);

	void sortByDeadline();

	void completeAll();

	void clearCompleted();

	// 把所有任务写入文件，成功返回 true
	bool saveToFile(const std::string& filename) const;

	// 从文件读入并替换当前列表，成功返回 true
	bool loadFromFile(const std::string& filename);
};

#endif