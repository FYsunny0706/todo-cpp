#include "TodoList.h"
#include <iostream>

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

bool TodoList::completeTask(int id){
	for(Task& task:tasks){
		if(task.getId()==id){
			task.markCompleted();
			return true;
		}
	}

	return false;
}

bool TodoList::deleteTask(int id){
	for(auto it =tasks.begin();it!=tasks.end();it++){
		if(it -> getId() ==id){
			tasks.erase(it);
			return true;
		}
	}

	return false;
}