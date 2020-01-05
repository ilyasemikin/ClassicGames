#include <fstream>
#include "config_parser.h"

std::map<std::string, std::string> Config::getListParams(const std::string &path, std::set<std::string> required) {
	std::map<std::string, std::string> ret;
	std::ifstream input;
	input.open(path);

	std::string line;
	size_t nline = 0;

	while (std::getline(input, line)) {
		++nline;
		if (line.length() == 0 || line[0] == '#')
			continue;

		auto findedChar { line.find('=') };
		if (findedChar != std::string::npos) {
			auto name { line.substr(0, findedChar) };
			auto value { line.substr(findedChar + 1, line.length()) };
			
			if (ret.find(name) != ret.end())
				throw;
			auto finded { required.find(name) };
			if (finded != required.end()) {
				required.erase(finded);
				ret.insert(std::pair(name, value));
			}
			else
				throw;
		}
		else
			throw;
	}

	if (!required.empty())
		throw;

	return ret;
}
