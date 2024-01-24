#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "../controller/Controller.hpp"

using namespace sf;

class MenuState: public State {
public:
    MenuState(Controller *controller);
    virtual ~MenuState();
    MenuState(const MenuState &) = delete;
    MenuState &operator=(const MenuState &) = delete;

    /**
     * Ces 4 méthodes sont expliqués dans State.hpp
     * */
	void init();
	void handleInput();
	void update();
	void draw();

private:
    /**
     * Les différents éléments necessaires pour l'affichage du menu
     * */
    Controller *controller;
    Text gameTitle;
	Sprite butinButton;
	Sprite gounkiButton;
	Sprite safariButton;

    Sprite closeButton;
    Sprite quitButin;
    Sprite quitGounki;
    Sprite quitSafari;

    /**
     * Cette méthode est un outil pour créer des .png de boutton sans avoir à passer super
     * longtemps dans un logiciel paint
     * */
    RenderTexture rd;
    void buttonFactory();
};

#endif