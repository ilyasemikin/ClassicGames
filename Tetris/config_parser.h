#pragma once

#include <map>
#include <set>
#include <string>

class Config {
public:
	static std::map<std::string, std::string> getListParams(const std::string &path, std::set<std::string> required);
};
