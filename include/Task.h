#ifndef TASK_H
#define TASK_H

#include <string>
using namespace std;

class Task {
private:
	int id;
	string title;
	string description;
	bool completed;
	string deadline;

public:
	Task(int id,const string& title,const string& description,
		bool completed,const string& deadline);

	int getId() const;
	string getTitle() const;
	string getDescription() const;
	bool isCompleted() const;
	string getDeadline() const;

	void setTitle(const string& title);
	void setDescrition(const string& description);
	void setDeadline(const string& deadline);

	void markCompleted();
};

#endif