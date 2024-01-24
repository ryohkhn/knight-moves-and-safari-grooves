#ifndef STATE_H
#define STATE_H

#include <SFML/Graphics.hpp>

/**
 * La classe mère de tout les différents états que peut prendre notre logiciel
 * dans notre cas, un état est l'équivalent d'un JPanel en swing
 * */
class State {
public:
    virtual ~State();

    /**
     * la méthode qui se fait appelé par StateMachine dans processStateChanges lorsque l'État
     * se fait mettre sur le dessus de la pile
     * */
    virtual void init() = 0;
    /**
     * la méthode qui va gérer les différents entrées de l'utilisateur
     * */
    virtual void handleInput() = 0;
    /**
     * la méthode qui se fait appeler par le controleur à utiliser pour faire des appels de méthodes
     * qui ont pour incidence de modifier le model
     * */
    virtual void update() = 0;
    /**
     * la méthode qui se fait appeler par le controleur pour rafraichir ce qui se trouve sur l'écran en fonction
     * de ce qui se trouve dans le modèle
     * */
    virtual void draw() = 0;

    /**
     * Comme nos jeux sont en tour par tour et qu'aucun ne dépend d'un temps qui sécoule, ces deux méthodes ne sont
     * pas très utilies mais je souhaite les garder pour des futures versions de ce projet
     * */
    virtual void pause() {}
    virtual void resume() {}

    bool isPaused = false;
protected:
    sf::Sprite background;
};

#endif