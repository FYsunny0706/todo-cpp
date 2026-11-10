#include "stats_ui.h"

#include <ostream>
#include <string>
#include <vector>

#include "DateUtils.h"
#include "Report.h"
#include "app_utils.h"
#include "report_menu.h"
#include "text_art.h"

namespace {

// 完成率
class CompletionReport : public Report {
   public:
    std::string title() const override { return "完成率"; }

    void render(const TodoList& list, std::ostream& out) const override {
        Query all;
        const int total = static_cast<int>(list.find(all).size());

        Query doneQuery;
        doneQuery.completed = 1;
        const int done = static_cast<int>(list.find(doneQuery).size());

        out << "总任务数: " << total << '\n';
        out << "已完成:   " << done << '\n';
        out << "未完成:   " << (total - done) << '\n';

        const double percent = (total == 0) ? 0.0 : (100.0 * done / total);

        out << "完成率:   " << progressBar(percent, 20) << ' ' << formatPercent(percent) << '\n';
    }
};

// 逾期任务
class OverdueReport : public Report {
   public:
    std::string title() const override { return "逾期任务"; }

    bool isLong() const override { return true; }

    void render(const TodoList& list, std::ostream& out) const override {
        Query query;
        query.before = now();
        query.completed = 0;

        const std::vector<int> overdue = list.find(query);

        if (overdue.empty()) {
            out << "没有逾期任务。\n";
            return;
        }

        out << "逾期 " << overdue.size() << " 条：\n\n";
        out << list.tasksBriefText(overdue);
    }
};

// 分类分布
class CategoryReport : public Report {
   public:
    std::string title() const override { return "分类分布"; }

    void render(const TodoList& list, std::ostream& out) const override {
        const char* categories[] = {"学业", "生活", "工作", "其他"};

        int counts[4] = {0, 0, 0, 0};
        int total = 0;

        for (int i = 0; i < 4; i++) {
            Query query;
            query.category = categories[i];

            counts[i] = static_cast<int>(list.find(query).size());
            total += counts[i];
        }

        out << "分类分布：\n";

        for (int i = 0; i < 4; i++) {
            out << "  " << categories[i] << "  " << counts[i] << "  ";

            const int width = (total == 0) ? 0 : counts[i] * 20 / total;

            for (int j = 0; j < width; j++) {
                out << "█";
            }

            out << '\n';
        }
    }
};

// 重要任务分布
class ImportanceReport : public Report {
   public:
    std::string title() const override { return "重要任务分布"; }

    void render(const TodoList& list, std::ostream& out) const override {
        Query important;
        important.importance = 1;

        Query all;

        const int yes = static_cast<int>(list.find(important).size());
        const int total = static_cast<int>(list.find(all).size());

        const double percent = (total == 0) ? 0.0 : (100.0 * yes / total);

        out << "重要任务: " << yes << " / " << total << '\n';
        out << "占比:     " << progressBar(percent, 20) << ' ' << formatPercent(percent) << '\n';
    }
};

const CompletionReport g_completion;
const OverdueReport g_overdue;
const CategoryReport g_category;
const ImportanceReport g_importance;

}  // namespace

// 统计子菜单
void statsMenu(TodoList& list) {
    const std::vector<const Report*> reports = {
        &g_completion,
        &g_overdue,
        &g_category,
        &g_importance,
    };

    runReportMenu("统计", reports, list);
}
