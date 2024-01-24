#ifndef SAFARISTATE_HPP
#define SAFARISTATE_HPP

#include "../GameState.hpp"

class SafariState : public GameState {
public:
    SafariState(Controller *controller);
    virtual ~SafariState();
    SafariState(const SafariState&) = delete;
    SafariState& operator=(const SafariState&) = delete;

    /**
     * Ces 4 méthodes sont expliqués dans State.hpp
     * */
	void init();
	void handleInput();
	void update();
	void draw();
private:
    /**
     * Contrairement à Butin où c'est très simple, ici on doit créer un plateau spécial et
     * dissimuler les cases pour les barrières ainsi que de réellement faire la distinction entre click gauche
     * et droit
     */
    void incrementPlayer();
    Sprite *horizontalFenceSprite;
    Sprite *verticalFenceSprite;
    Vector2i getTileWithinBoard(Vector2f mousePos, ActionKey action);
    void positionPieceWithinBoard(Sprite *piece, Vector2i pos);
    void positionFenceWithinBoard(Sprite *fence, Vector2i pos);
    void drawPieces();
    bool moveAnimal;
    Vector2i *onTile = nullptr;
    bool placeFence;
    int fencePlaced;
    void boardFactory();
    Texture& backBoardFactory();
    Sprite endTurnButton;
    bool canEndTurn;
    void endTurnButtonEffect();

    /**
     * pour qu'une partie puisse commencer, chque joueur doit placer 3 animaux et 3 barrières
     * les méthodees en dessous servent à celà
     */
    SafariConfig config;
    void addCrocodile(Vector2i v);
    void addElephant(Vector2i v);
    void addLion(Vector2i v);
    void addFence(Vector2i v);
    Text initInstructions;

    float boardScale;
};


#endif
