#include "text_art.h"

#include <sstream>

namespace {

// 实心方块与三档阴影，实测在控制台中均为单宽
const char* const FULL = "█";
const char* const LIGHT = "░";
const char* const MEDIUM = "▒";
const char* const DARK = "▓";

}  // namespace

std::string progressBar(double percent, int width) {
	if (percent < 0.0) percent = 0.0;
	if (percent > 100.0) percent = 100.0;

	const int filled = static_cast<int>(percent * width / 100.0 + 0.5);

	std::string bar = "[";

	for (int i = 0; i < width; i++) {
		bar += (i < filled) ? FULL : LIGHT;
	}

	bar += "]";

	return bar;
}

std::string densitySymbol(int count) {
	if (count <= 0) return " ";
	if (count == 1) return LIGHT;
	if (count == 2) return MEDIUM;
	if (count == 3) return DARK;

	return FULL;
}

std::string formatPercent(double percent) {
	std::ostringstream out;

	out << std::fixed;
	out.precision(1);
	out << percent << '%';

	return out.str();
}
