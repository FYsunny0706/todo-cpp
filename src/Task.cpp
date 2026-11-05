#include "Task.h"

Task::Task(int id, const std::string& title,
		   const std::string& description,
		   bool completed, const std::string& deadline)
	: id(id),
	  title(title),
	  description(description),
	  completed(completed),
	  deadline(deadline) {
}

int Task::getId() const {
	return id;
}

std::string Task::getTitle() const {
	return title;
}

std::string Task::getDescription() const {
	return description;
}

bool Task::isCompleted() const {
	return completed;
}

std::string Task::getDeadline() const {
	return deadline;
}