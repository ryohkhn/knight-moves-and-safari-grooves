#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H

#include "../controller/Controller.hpp"

class PlayerState: public State {
public:
    PlayerState(Controller *controller);
    virtual ~PlayerState();
    PlayerState(const PlayerState&) = delete;
    PlayerState& operator=(const PlayerState&) = delete;

    /**
     * Ces 4 méthodes sont expliqués dans State.hpp
     * */
	void init();
	void handleInput();
	void update();
	void draw();
private:
    /**
     * Des infos récupérées depuis controller -> game pour savoir combien de pseudos demander
     * */
    Controller *controller;
    const int minPlayers;
    const int maxPlayers;
    vector<string> playerNames;
    Sprite addPlayerButton;
    Sprite startButton;
    Text title;
    vector<Text> playerDisplayNames;
    void repositionNameDisplay();
    /**
     * une méthode pour lancer le "GameState" qui est un État qui est dédié à l'affichage d'un des jeux et
     * à son bon déroulement
     * */
    void goToGameState();
    void saveName();
};

#endif