#include "report_menu.h"

#include <sstream>
#include <string>
#include <vector>

#include "app_utils.h"

void runReportMenu(const char* menuTitle, const std::vector<const Report*>& reports,
				   const TodoList& list) {
	std::vector<std::string> items;

	for (const Report* report : reports) {
		items.push_back(report->title());
	}

	const std::string header = std::string("\n--- ") + menuTitle + " ---";

	while (true) {
		const int choice = askMenu(header, items);

		if (choice <= 0) return;

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
