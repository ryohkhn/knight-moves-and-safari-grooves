#ifndef BUTINSTATE_H
#define BUTINSTATE_H

#include "../GameState.hpp"

using namespace sf;

class ButinState: public GameState {
public:
    ButinState(Controller *controller);
    virtual ~ButinState();
    ButinState(const ButinState&) = delete;
    ButinState& operator=(const ButinState&) = delete;

    /**
     * Ces 4 méthodes sont expliqués dans State.hpp
     * */
    void init();
	void handleInput();
	void update();
	void draw();
private:
    /**
     * Permet de générer un plateau spécifique à ce jeu
     */
    void boardFactory();
    /**
     * Pour pouvoir débuter, ce jeu à besoin de retirer un pion jaune donc on a besoin de ça
     */
    vector<Vector2i> piecesToRemove;
    void addPieceToRemove(Vector2i v);
    Text initInstructions;
};

#endif