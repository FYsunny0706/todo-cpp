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

   public:
	Task(int id, const std::string& title,
		 const std::string& description,
		 bool completed, const std::string& deadline);

	int getId() const;

	std::string getTitle() const;

	std::string getDescription() const;

	bool isCompleted() const;

	std::string getDeadline() const;

	void setTitle(const std::string& title);

	void setDescription(const std::string& description);

	void setDeadline(const std::string& deadline);

	// 标记为已完成
	void markCompleted();
};

#endif