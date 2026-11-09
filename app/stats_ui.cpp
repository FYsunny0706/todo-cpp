#include "stats_ui.h"

#include <iostream>
#include <string>
#include <vector>

#include "DateUtils.h"
#include "app_utils.h"
#include "text_art.h"

namespace {

// 打印统计菜单
void printStatsMenu() {
	std::cout << "\n--- 统计 ---\n";
	std::cout << "  1. 完成率\n";
	std::cout << "  2. 逾期任务\n";
	std::cout << "  3. 分类分布\n";
	std::cout << "  4. 重要任务分布\n";
	std::cout << "  0. 返回\n";
	std::cout << "请选择: ";
}

// 完成率
void showCompletion(const TodoList& list) {
	Query all;
	const int total = static_cast<int>(list.find(all).size());

	Query doneQuery;
	doneQuery.completed = 1;
	const int done = static_cast<int>(list.find(doneQuery).size());

	std::cout << "总任务数: " << total << '\n';
	std::cout << "已完成:   " << done << '\n';
	std::cout << "未完成:   " << (total - done) << '\n';

	const double percent = (total == 0) ? 0.0 : (100.0 * done / total);

	std::cout << "完成率:   " << progressBar(percent, 20) << ' ' << formatPercent(percent) << '\n';
}

// 逾期任务
void showOverdue(const TodoList& list) {
	Query query;
	query.before = now();
	query.completed = 0;

	const std::vector<int> overdue = list.find(query);

	if (overdue.empty()) {
		std::cout << "没有逾期任务。\n";
		return;
	}

	std::cout << "逾期 " << overdue.size() << " 条：\n";
	list.showTasks(overdue);
}

// 分类分布
void showCategoryDistribution(const TodoList& list) {
	const char* categories[] = {"学业", "生活", "工作", "其他"};

	int counts[4] = {0, 0, 0, 0};
	int total = 0;

	for (int i = 0; i < 4; i++) {
		Query query;
		query.category = categories[i];

		counts[i] = static_cast<int>(list.find(query).size());
		total += counts[i];
	}

	std::cout << "分类分布：\n";

	for (int i = 0; i < 4; i++) {
		std::cout << "  " << categories[i] << "  " << counts[i] << "  ";

		const int width = (total == 0) ? 0 : counts[i] * 20 / total;

		for (int j = 0; j < width; j++) {
			std::cout << "█";
		}

		std::cout << '\n';
	}
}

// 重要任务分布
void showImportanceDistribution(const TodoList& list) {
	Query important;
	important.importance = 1;

	Query all;

	const int yes = static_cast<int>(list.find(important).size());
	const int total = static_cast<int>(list.find(all).size());

	const double percent = (total == 0) ? 0.0 : (100.0 * yes / total);

	std::cout << "重要任务: " << yes << " / " << total << '\n';
	std::cout << "占比:     " << progressBar(percent, 20) << ' ' << formatPercent(percent) << '\n';
}

}  // namespace

// 统计子菜单
void statsMenu(TodoList& list) {
	bool back = false;

	while (!back) {
		printStatsMenu();

		int choice = -1;

		if (!readInt(choice)) {
			if (std::cin.eof()) return;

			std::cout << "请输入 0-4 的数字。\n";
			continue;
		}

		switch (choice) {
			case 1:
				showCompletion(list);
				break;

			case 2:
				showOverdue(list);
				break;

			case 3:
				showCategoryDistribution(list);
				break;

			case 4:
				showImportanceDistribution(list);
				break;

			case 0:
				back = true;
				break;

			default:
				std::cout << "没有这个选项。\n";
				break;
		}
	}
}