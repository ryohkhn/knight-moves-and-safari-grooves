#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <memory>
#include <stack>
#include <SFML/Graphics.hpp>

#include "State.hpp"


class StateMachine {
public:
    StateMachine() = default;
	virtual ~StateMachine();
    /**
     * Même commentaire que pour InputManager pour les copies et les affectations
     * */
    StateMachine(const StateMachine&) = delete;
    StateMachine& operator=(const StateMachine&) = delete;

    /**
     * pointe l'attribut newState vers le "newState" passé en paramètre
     * et met à jour ou non l'attribut isReplacing pour savoir si le nouvel état doit remplacer l'ancien ou venir
     * se mettre au-dessus pour que processStateChanges() s'occupe de prendre en compte tout les changements sur le stack
     * d'états
     * */
	void addState(State *newState, bool isReplacing = true);
    /**
     * met l'attribut isRemoving à true et processStateChanges() fera en sorte de faire sauter de la pile
     * l'état sur le dessus de la pile
     * */
	void removeState();
    /**
     * met l'attribut isRemovingBackend à true et processStateChanges() fera en sote de retirer de la pile toutes les
     * fenêtres sauf celle qui se trouve sur le haut de la pile au moment ou est appelé processStateChanges()
     * c'est utile dans le cas ou on serait retourner au menu sans avoir terminé un jeu pour quitter cette partie
     * qui est en cours et pouvoir en commencer une autre
     * */
    void removeBackendState();
    /**
     * est appelé à chaque itérations de run pour modifier la pile des fenêtres du logiciel en fonction des valeurs
     * booleans qui on pu être modifié
     * */
	void processStateChanges();

	sf::Vector2f getCenter();

	State *getActiveState();

private:
    /**
     * On utilise une Pile pour faire tourner notre logiciel et pouvoir facilement se balader dedans en ajoutant
     * des fenêtre et en en retirant
     * */
    std::stack<State *> states;
	State *newState;

    /**
     * Des booleans pour que processStateChanges qui est appelé à chaque itérations de la méthode run() de Controller
     * sache quoi faire pour déplacer l'utilisateur sur la fenêtre souhaité
     * */
	bool isRemoving = false;
	bool isAdding = false;
	bool isReplacing = false;
    bool isRemovingBackend = false;
};

#endif