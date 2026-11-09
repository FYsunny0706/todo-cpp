#ifndef TASK_H
#define TASK_H

#include <string>

// 待办事项：纯数据模型
class Task {
   private:
	int id;
	std::string title;
	std::string description;
	bool completed;
	std::string deadline;
	std::string category;
	int importance;

   public:
	// 构造函数：初始化全部成员
	Task(int id, const std::string& title,
		 const std::string& description,
		 bool completed, const std::string& deadline,
		 const std::string& category = "其他",
		 int importance = 0);

	int getId() const;

	std::string getTitle() const;

	std::string getDescription() const;

	bool isCompleted() const;

	std::string getDeadline() const;

	const std::string& getCategory() const;
	
	int getImportance() const;

	void setTitle(const std::string& title);

	void setDescription(const std::string& description);

	void setDeadline(const std::string& deadline);

	void setCategory(const std::string& category);

	void setImportance(int importance);

	// 标记为已完成
	void markCompleted();

	// 标记为未完成
	void markUncompleted();

	//仅在重排编号时使用
	void setId(int id);
};

#endif