#pragma once

#include <map>
#include <set>
#include <sstream>
#include <string>

class Config {
public:
	static std::map<std::string, std::string> getListParams(const std::string &path, std::set<std::string> required);

	template <typename T>
	static T getParam(std::string_view param);
};

template <typename T>
T Config::getParam(std::string_view param) {
	T ret;
	std::stringstream str;
	str << param;
	str >> ret;
	return ret;
}
