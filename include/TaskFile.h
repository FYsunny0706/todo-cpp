#ifndef TASKFILE_H
#define TASKFILE_H

#include <string>
#include <vector>

#include "Task.h"

// 把任务列表写入文件，成功返回 true
bool saveTasks(const std::vector<Task>& tasks, const std::string& filename);

// 从文件读入任务，成功返回 true
bool loadTasks(std::vector<Task>& out, const std::string& filename);

#endif
