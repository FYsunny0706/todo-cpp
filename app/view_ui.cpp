#include "view_ui.h"

#include <ostream>
#include <string>
#include <vector>

#include "DateUtils.h"
#include "Report.h"
#include "app_utils.h"
#include "report_menu.h"

namespace {

// 把数字右对齐到指定宽度
std::string padLeft(int value, std::size_t width) {
	std::string text = std::to_string(value);

	while (text.size() < width) {
		text = ' ' + text;
	}

	return text;
}

// 未来 30 天到期柱状图
class BarChartReport : public Report {
   public:
	std::string title() const override { return "未来 30 天到期柱状图"; }

	bool isLong() const override { return true; }

	void render(const TodoList& list, std::ostream& out) const override {
		const int days = 30;
		const int maxRows = 10;

		Date todayDate;

		if (!parseDate(today(), todayDate)) {
			out << "无法取得今天的日期。\n";
			return;
		}

		int counts[30] = {0};
		Date dates[30];

		Date day = todayDate;

		for (int i = 0; i < days; i++) {
			dates[i] = day;

			Query query;
			query.completed = 0;
			query.onDate = formatDate(day);

			counts[i] = static_cast<int>(list.find(query).size());

			day = addDays(day, 1);
		}

		Query overdueQuery;
		overdueQuery.completed = 0;
		overdueQuery.before = now();

		const int overdue = static_cast<int>(list.find(overdueQuery).size());

		int peak = 0;
		int total = 0;

		for (int i = 0; i < days; i++) {
			total += counts[i];

			if (counts[i] > peak) peak = counts[i];
		}

		out << "未来 " << days << " 天到期任务：共 " << total << " 条";

		if (overdue > 0) {
			out << "，另有 " << overdue << " 条已逾期";
		}

		out << '\n';

		if (peak == 0) {
			out << "这段时间没有到期的任务。\n";
			return;
		}

		const int rows = (peak > maxRows) ? maxRows : peak;

		for (int level = rows; level >= 1; level--) {
			// 这一行代表多少个任务
			const int need = (level * peak + rows - 1) / rows;

			out << padLeft(need, 3) << " |";

			for (int i = 0; i < days; i++) {
				out << (counts[i] >= need ? "█ " : "  ");
			}

			out << '\n';
		}

		out << "    +";

		for (int i = 0; i < days * 2; i++) {
			out << '-';
		}

		out << '\n';

		// 每 5 天标一个日期
		std::string labels(4 + days * 2, ' ');

		for (int i = 0; i < days; i += 5) {
			const std::string text = formatDate(dates[i]).substr(5);

			for (std::size_t j = 0; j < text.size(); j++) {
				labels[4 + i * 2 + j] = text[j];
			}
		}

		while (!labels.empty() && labels.back() == ' ') labels.pop_back();

		out << labels << '\n';
	}
};

const BarChartReport g_barChart;

}  // namespace

// 视图子菜单
void viewMenu(const TodoList& list) {
	const std::vector<const Report*> reports = {&g_barChart};

	runReportMenu("视图", reports, list);
}
