#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SFML/Graphics.hpp>

using namespace sf;

class InputManager {
public:
    InputManager() {};
    virtual ~InputManager() {};
    /**
     * on ne veut pas plusieurs manager pour les inputs donc on ne va pas en faire de copie non plus
     * */
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    /**
     * Permet de vérifier si n'importe quel Sprite a été clické avec un click précis dans la fenêtre window
     * @param sp le Sprite dont on va regarder les coordonnées pour vérifier si la souris à clické dessus
     * @param mButton le type de click avec lequel le Sprite est clické ou non
     * @param window la fenêtre sur laquelle se trouve le sp
     * */
    bool isSpriteClicked(Sprite sp, Mouse::Button mButton, RenderWindow &window) const;
    /**
     * permet d'obtenir facilement les coordonnées de la souris sur une fenêtre
     * @param window la fenêtre sur laquelle on veut récupérer les coordonnées
     * */
    Vector2f getMousePosition(RenderWindow &window) const;
};

#endif