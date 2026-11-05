#include "TodoList.h"

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