#pragma once

#include <map>
#include <string>
#include <SFML/Graphics.hpp>

class AssetManager {
private:
	std::map<std::string, sf::Texture> _textures;
	std::map<std::string, sf::Font> _fonts;
public:
	AssetManager() {}
	virtual ~AssetManager() {}

	void loadTexture(const std::string &name, const std::string &path);
	void unloadTexture(const std::string &name);
	sf::Texture &getTexture(const std::string &name);

	void loadFont(const std::string &name, const std::string &path);
	void unloadFont(const std::string &name);
	sf::Font &getFont(const std::string &name);
};
