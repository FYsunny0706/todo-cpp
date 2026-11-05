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
};

#endif