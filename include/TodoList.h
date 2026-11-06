#ifndef TODOLIST_H
#define TODOLIST_H

#include <vector>
#include <string>
#include "Task.h"

class TodoList {
   private:
	std::vector<Task> tasks;
	int nextId;

   public:
	TodoList();

	void addTask(const std::string& title,
				 const std::string& description,
				 const std::string& deadline);

	void showTasks() const;

	bool completeTask(int id);

	bool deleteTask(int id);

	bool updateDeadline(int id,const std::string& deadline);

	void sortByDeadline();

	void completeAll();

	void clearCompleted();
};

#endif