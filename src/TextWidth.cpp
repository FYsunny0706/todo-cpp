#include "TextWidth.h"

namespace {

// 取 UTF-8 码点，同时返回它占用的字节数
unsigned int nextCodePoint(const std::string& text, std::size_t index, std::size_t& length) {
	const unsigned char first = static_cast<unsigned char>(text[index]);

	if (first < 0x80) {
		length = 1;
		return first;
	}

	if ((first >> 5) == 0x06 && index + 1 < text.size()) {
		length = 2;

		return ((first & 0x1F) << 6) |
			   (static_cast<unsigned char>(text[index + 1]) & 0x3F);
	}

	if ((first >> 4) == 0x0E && index + 2 < text.size()) {
		length = 3;

		return ((first & 0x0F) << 12) |
			   ((static_cast<unsigned char>(text[index + 1]) & 0x3F) << 6) |
			   (static_cast<unsigned char>(text[index + 2]) & 0x3F);
	}

	if ((first >> 3) == 0x1E && index + 3 < text.size()) {
		length = 4;

		return ((first & 0x07) << 18) |
			   ((static_cast<unsigned char>(text[index + 1]) & 0x3F) << 12) |
			   ((static_cast<unsigned char>(text[index + 2]) & 0x3F) << 6) |
			   (static_cast<unsigned char>(text[index + 3]) & 0x3F);
	}

	length = 1;
	return first;
}

// 码点占几格：CJK 与全角标点算 2，其余算 1
std::size_t charWidth(unsigned int code) {
	if (code >= 0x1100 && code <= 0x115F) return 2;
	if (code >= 0x2E80 && code <= 0xA4CF) return 2;
	if (code >= 0xAC00 && code <= 0xD7A3) return 2;
	if (code >= 0xF900 && code <= 0xFAFF) return 2;
	if (code >= 0xFE30 && code <= 0xFE6F) return 2;
	if (code >= 0xFF00 && code <= 0xFF60) return 2;
	if (code >= 0xFFE0 && code <= 0xFFE6) return 2;

	return 1;
}

}  // namespace

std::size_t displayWidth(const std::string& text) {
	std::size_t width = 0;
	std::size_t index = 0;

	while (index < text.size()) {
		std::size_t length = 0;
		const unsigned int code = nextCodePoint(text, index, length);

		width += charWidth(code);
		index += length;
	}

	return width;
}

std::string padRight(const std::string& text, std::size_t width) {
	std::string out = text;
	const std::size_t current = displayWidth(text);

	for (std::size_t i = current; i < width; i++) {
		out += ' ';
	}

	return out;
}

std::string padLeftText(const std::string& text, std::size_t width) {
	std::string out;
	const std::size_t current = displayWidth(text);

	for (std::size_t i = current; i < width; i++) {
		out += ' ';
	}

	return out + text;
}

std::string truncateToWidth(const std::string& text, std::size_t width) {
	if (displayWidth(text) <= width) return text;

	std::string out;
	std::size_t used = 0;
	std::size_t index = 0;

	while (index < text.size()) {
		std::size_t length = 0;
		const unsigned int code = nextCodePoint(text, index, length);
		const std::size_t w = charWidth(code);

		if (used + w + 2 > width) break;

		out += text.substr(index, length);
		used += w;
		index += length;
	}

	return out + "..";
}
