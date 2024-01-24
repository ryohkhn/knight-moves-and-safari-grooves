#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class ResourceManager {
public:
    ResourceManager() = default;
	virtual ~ResourceManager() = default;
    /**
     * On ne veut pas pouvoir faire de copie ni d'affectation pour cette classe car on ne veut qu'une seule instance
     * à tout moment car devoir aller chercher 2 fois une même resource dans notre arborressance de fichier
     * va à l'encontre du concept de ResourceManager
     * */
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    /**
     * loadTexture creates a Texture from the .png found at "filepath" and associates it with "key" in a map
     * @param key a string that will be used to acces the Texture created from the image at "filePath"
     * @param filePath the filePath where can be found an image to build a Texture out of
     * */
	void loadTexture(string key, string filePath);
    /**
     * getTexture renvoie la Texture créée par l'appel à loadTexture() pour la même valeur de "key"
     * @param key la clef qui a servie à load la texture
     * @return la Texture stocké dans la map pour la valeur de "key"
     * */
	Texture &getTexture(string key);

    /**
     * Littéralement la même chose que pour les Textures mais avec les fonts
     * */
	void loadFont(string key, string filePath);
	Font &getFont(string key);
private:
	map<string, Texture> textures;
	map<string, Font> fonts;
};

#endif