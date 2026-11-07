#include "Task.h"

// 构造函数：初始化全部成员
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

void Task::setTitle(const std::string& title) {
	this->title = title;
}

void Task::setDescription(const std::string& description) {
	this->description = description;
}

void Task::setDeadline(const std::string& deadline) {
	this->deadline = deadline;
}

void Task::markCompleted() {
	completed = true;
}