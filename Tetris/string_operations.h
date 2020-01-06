#include <string>
#include <vector>
#include <list>

class StringOperations {
public:
	static std::vector<std::string> Split(const std::string &input, const std::string &separators);
	static std::string Join(std::list<std::string>::iterator start, std::list<std::string>::iterator end, std::string separator);
	static bool IsNumber(const std::string &str);
};
