#ifndef TASK_H
#define TASK_H

#include <string>

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
};

#endif