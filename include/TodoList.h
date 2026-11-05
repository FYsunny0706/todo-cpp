#ifndef TODOLIST_H
#define TODOLIST_H

#include <vector>
#include <string>
#include "Task.h"

using namespace std;

class TodoList{
private:
	vector<Task> tasks;
	int nextId=1;

public:
	int addTask(const string& title,const string& description,
		const string& deadline);

	bool completeTask(int id);
	bool deleteTask(int id);

	void completeAll();
	void clearCompleted();

	void sortByDeadline();

	const vector<Task>& getTasks() const;

	bool saveToFile(const string& filename) const;
	bool loadFromFile(const string& filename);
};

#endif