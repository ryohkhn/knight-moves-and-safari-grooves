#ifndef PAUSESTATE_H
#define PAUSESTATE_H

#include "../controller/Controller.hpp"

class PauseState : public State {
public:
    /**
     * Cet État est accessible depuis n'importe quel "Gamestate" en appuyant sur "Échap"
     * */
    PauseState(Controller *controller, Texture *texture);
    virtual  ~PauseState();
    PauseState(const PauseState&) = delete;
    PauseState& operator=(const PauseState&) = delete;

    /**
     * Ces 4 méthodes sont expliqués dans State.hpp
     * */
    void init();
    void handleInput();
    void update();
    void draw();
private:
    /**
     * Différents Sprite pour afficher des boutons et rediriger l'utilisateur sur le logiciel
     * */

    Controller *controller;
    Texture backgroundTexture;
    Sprite resumeButton;
    Sprite returnMenuButton;
    Sprite exitButton;
};

#endif