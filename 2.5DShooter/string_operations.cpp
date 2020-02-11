#include <algorithm>
#include <cctype>
#include "string_operations.h"

std::vector<std::string> StringOperations::split(const std::string &input, const std::string &separators) {
	std::vector<std::string> ret;
	auto isSeparator { [&](char c) { return separators.find(c) != std::string::npos; } };
	auto isNotSeparator { [&](char c) { return !isSeparator(c); } };
	auto itStartWord { input.begin() };
	auto itEnd { input.end() };
	while (itStartWord != itEnd) {
		itStartWord = std::find_if(itStartWord, itEnd, isNotSeparator);
		auto itEndWord = std::find_if(itStartWord, itEnd, isSeparator);
		if (itStartWord != itEnd) {
			ret.push_back(std::string(itStartWord, itEndWord));
			itStartWord = itEndWord;
		}
	}
	return ret;
}

std::string StringOperations::join(std::list<std::string>::iterator start, std::list<std::string>::iterator end, std::string separator) {
	if (start == end)
		return "";

	auto ret { *(start++) };
	while (start != end)
		ret += separator + *start++;
	return ret;
}

bool StringOperations::isNumber(const std::string &str) {
	if (!str.size())
		return false;
	for (auto c : str)
		if (!std::isdigit(c))
			return false;
	return true;
}
