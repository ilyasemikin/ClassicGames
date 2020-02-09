#include "asset_manager.h"

void AssetManager::loadTexture(const std::string &name, const std::string &path) {
	sf::Texture texture;
	
	if (!texture.loadFromFile(path))
		throw;

	_textures[name] = texture;
}

void AssetManager::unloadTexture(const std::string &name) {
	auto finded { _textures.find(name) };
	if (finded == _textures.end())
		throw;
	
	_textures.erase(finded);
}

sf::Texture &AssetManager::getTexture(const std::string &name) {
	return _textures.at(name);
}

void AssetManager::loadFont(const std::string &name, const std::string &path) {
	sf::Font font;

	if (!font.loadFromFile(path))
		throw;
	
	_fonts[name] = font;
}

void AssetManager::unloadFont(const std::string &name) {
	auto finded { _fonts.find(name) };
	if (finded == _fonts.end())
		throw;

	_fonts.erase(finded);
}

sf::Font &AssetManager::getFont(const std::string &name) {
	return _fonts.at(name);
}
