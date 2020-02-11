#include <string>
#include <sstream>
#include <vector>
#include <list>

class StringOperations {
public:
	static std::vector<std::string> split(const std::string &input, const std::string &separators);
	static std::string join(std::list<std::string>::iterator start, std::list<std::string>::iterator end, std::string separator);
	
	static bool isNumber(const std::string &str);

	template <typename T>
	static T convertTo(std::string_view str);
};

template <typename T>
T StringOperations::convertTo(std::string_view str) {
	std::stringstream stream;
	stream << str;
	
	T ret;
	stream >> ret;
	return ret;
}
