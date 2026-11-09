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

	// 任务编号
	int getId() const;

	// 标题
	std::string getTitle() const;

	// 描述
	std::string getDescription() const;

	// 是否已完成
	bool isCompleted() const;

	// 截止时间，可能含时分
	std::string getDeadline() const;

	// 分类
	const std::string& getCategory() const;

	// 重要度：0 不重要，1 重要
	int getImportance() const;

	// 设置标题
	void setTitle(const std::string& title);

	// 设置描述
	void setDescription(const std::string& description);

	// 设置截止时间
	void setDeadline(const std::string& deadline);

	// 设置分类
	void setCategory(const std::string& category);

	// 设置重要度
	void setImportance(int importance);

	// 标记为已完成
	void markCompleted();

	// 标记为未完成
	void markUncompleted();

	//仅在重排编号时使用
	void setId(int id);
};

#endif