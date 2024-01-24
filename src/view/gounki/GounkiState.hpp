#ifndef GOUNKISTATE_HPP
#define GOUNKISTATE_HPP

#include "../GameState.hpp"

class GounkiState: public GameState {
    public :
        GounkiState(Controller *controller);
        ~GounkiState();
        /**
         * Ces 4 méthodes sont expliqués dans State.hpp
         */
        void init();
        void handleInput();
        void update();
        void draw();
    private:
        /**
         * De quoi gérer les lignes en plus que les joueurs doivent atteindre pour gagner la partie
         */
        bool isDeployement;
        Sprite topRow;
        Sprite bottomRow;
        RenderTexture rowRender;
        void rowFactory();
        void boardFactory();
        void drawPieces();
        void positionPieceWithinBoard(Sprite *piece, Vector2i pos, int offset);
        void drawBase();
};


#endif
