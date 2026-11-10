#include "report_menu.h"

#include <iostream>
#include <sstream>
#include <string>

#include "app_utils.h"

void runReportMenu(const char* menuTitle, const std::vector<const Report*>& reports,
				   const TodoList& list) {
	bool back = false;

	while (!back) {
		beginFrame();

		std::cout << "\n--- " << menuTitle << " ---\n";

		for (std::size_t i = 0; i < reports.size(); i++) {
			std::cout << "  " << (i + 1) << ". " << reports[i]->title() << '\n';
		}

		std::cout << "  0. 返回\n请选择: ";

		int choice = -1;

		if (!readInt(choice)) {
			if (std::cin.eof()) return;

			setStatus("请输入 0-" + std::to_string(reports.size()) + " 的数字。\n");
			continue;
		}

		if (choice == 0) {
			back = true;
			continue;
		}

		if (choice < 1 || choice > static_cast<int>(reports.size())) {
			setStatus("没有这个选项。\n");
			continue;
		}

		const Report* report = reports[choice - 1];

		std::ostringstream buffer;
		report->render(list, buffer);

		if (report->isLong()) {
			showFullScreen(buffer.str());
		} else {
			setStatus(buffer.str());
		}
	}
}
