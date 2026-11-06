#include <iostream>
#include "DateUtils.h"

int main() {
    std::string dates[] = {
        "2026-11-06",
        "2024-02-29",
        "2026-02-29",
        "2026-13-01",
        "2026-04-31",
        "2026/11/06",
        "",
        "2000-02-29",
        "1900-02-29"
    };

    for (const std::string& date : dates) {
        std::cout << date << " : "
                  << (isValidDate(date) ? "有效" : "无效")
                  << std::endl;
    }

    return 0;
}