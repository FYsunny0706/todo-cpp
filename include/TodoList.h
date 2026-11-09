#ifndef TODOLIST_H
#define TODOLIST_H

#include <vector>
#include <string>
#include "Task.h"

// 查询条件：空字符串或 -1 表示该项不限制
struct Query {
	// 标题或描述包含这个词
	std::string keyword;
	// 分类精确匹配
	std::string category;
	// -1 不限，0 未完成，1 已完成
	int completed = -1;
	std::string before;
	// 截止日期早于这一天才匹配，空表示不限
	int importance = -1;
	// -1 不限，0 不重要，1 重要
};

// 待办事项集合：增删改查、排序与持久化
class TodoList {
   private:
	std::vector<Task> tasks;
	// 下一个可用的任务编号
	int nextId;

	//按当前顺序重新编号
	void renumber();

	// 打印单条任务
	void printTask(const Task& task) const;

   public:
	// 构造一个空列表
	TodoList();

	// 添加任务，成功返回 true
	bool addTask(const std::string& title,
				 const std::string& description,
				 const std::string& deadline,
				 const std::string& category = "其他",
				 int importance = 0);

	// 打印全部任务，空列表时输出提示
	void showTasks() const;

	// 按条件查找，返回匹配任务的编号
	std::vector<int> find(const Query& query) const;

	// 只打印指定编号的任务
	void showTasks(const std::vector<int>& ids) const;

	// 按 ID 标记完成，找不到返回 false
	bool completeTask(int id);

	// 按 ID 取消完成，找不到返回 false
	bool uncompleteTask(int id);

	// 按 ID 删除任务，找不到返回 false
	bool deleteTask(int id);

	// 修改截止日期，日期非法或 ID 不存在返回 false
	bool updateDeadline(int id, const std::string& deadline);

	// 修改标题，ID 不存在或标题为空返回 false
	bool updateTitle(int id, const std::string& title);

	// 修改描述，ID 不存在返回 false
	bool updateDescription(int id, const std::string& description);

	// 修改分类，ID 不存在或分类为空返回 false
	bool updateCategory(int id, const std::string& category);

	// 修改重要度，ID 不存在或取值非法返回 false
	bool updateImportance(int id, int importance);

	// 按截止日期升序排序，无期限的排在最后
	void sortByDeadline();

	// 把所有任务标记为已完成，返回处理的数量
	int completeAll();

	// 删除所有已完成的任务，返回删除的数量
	int clearCompleted();

	// 把所有任务写入文件，成功返回 true
	bool saveToFile(const std::string& filename) const;

	// 从文件读入并替换当前列表，成功返回 true
	bool loadFromFile(const std::string& filename);
};

#endif