#include "ResourceManager.hpp"

void ResourceManager::loadTexture(string key, string filePath) {
	sf::Texture texture;

	if (texture.loadFromFile(filePath)) {
		this -> textures[key] = texture;
	}
}
sf::Texture &ResourceManager::getTexture(string key) {
	return this -> textures.at(key);
}

void ResourceManager::loadFont(string key, string filePath) {
	sf::Font font;

	if (font.loadFromFile(filePath)) {
		this -> fonts[key] = font;
	}
}
sf::Font &ResourceManager::getFont(string key) {
	return this -> fonts.at(key);
}
