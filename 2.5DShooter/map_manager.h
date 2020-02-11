#pragma once

#include <map>
#include <string>
#include "array2d.h"
#include "game_map_objects.h"

class MapManagerException : public std::exception {
private:
	std::string _whatStr;
public:
	MapManagerException(std::string_view what) : _whatStr(what) {}
	const char *what() const noexcept override { return _whatStr.c_str(); }
};

class MapManager {
private:
	std::map<std::string, Array2d<GameMapObjects>> _maps;
public:
	MapManager();

	void loadMap(std::string_view name, std::string_view path);
	void unloadMap(const std::string &map);
	Array2d<GameMapObjects> getMap(const std::string &map);
};
