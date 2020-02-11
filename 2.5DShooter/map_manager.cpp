#include <fstream>
#include <sstream>
#include "map_manager.h"
#include "string_operations.h"

MapManager::MapManager() {

}

void MapManager::loadMap(std::string_view name, std::string_view path) {
	std::ifstream mapFile((std::string(path)));

	std::string line;

	std::getline(mapFile, line);

	auto numbers { StringOperations::split(line, " ") };	

	if (numbers.size() < 1)
		throw MapManagerException("incorrect map file");

	size_t size { StringOperations::convertTo<size_t>(numbers[0]) };

	Array2d<GameMapObjects> map(size, size, GameMapObjects::WALL); 

	size_t curline = 0;

	std::map<char, GameMapObjects> letterMatch {
		{ 'w', GameMapObjects::WALL },
		{ 'e', GameMapObjects::EMPTY },
		{ ' ', GameMapObjects::EMPTY },
	};

	while (!mapFile.eof()) {
		std::getline(mapFile, line);
		
		size_t countSymbols = line.size() > size ? size : line.size();
		for (size_t i = 0; i < countSymbols; i++) {
			GameMapObjects obj;
			try {
				obj = letterMatch.at(std::tolower(line[i]));
			}
			catch (std::out_of_range e) {
				obj = GameMapObjects::WALL;
			}
			map(i, curline) = obj;
		}

		if (++curline > size)
			break;
	}

	_maps.insert(std::pair(name, map));
}

void MapManager::unloadMap(const std::string &map) {
	auto finded { _maps.find(map) };
	if (finded == _maps.end())
		throw;
	_maps.erase(finded);
}

Array2d<GameMapObjects> MapManager::getMap(const std::string &name) {
	return _maps.at(name);
}
