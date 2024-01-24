#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "../controller/Controller.hpp"

using namespace sf;

class GameState: public State {
    public:
        virtual ~GameState();
        GameState(const GameState &) = delete;
        GameState& operator=(const GameState &) = delete;

        virtual void init() = 0;
        virtual void handleInput() = 0;
        virtual void update() = 0;
        virtual void draw() = 0;
        virtual void pause() {}
        virtual void resume() {}

    protected:
        GameState(Controller *controller, int BoardSize);
        Controller *controller;
        int boardSize;

        /**
         * valeurs booléennes utilisées par update() et handleInput() pour communiquer
         * */
        bool moveReady;
        bool playerPlayed;
        bool printWinner;

        /**
         * colonne x lignes x nombre de pièces sur cette tuile
         * & la chaîne contient le nom de la pièce
         * */
        vector<vector<vector<string>>> pieces;
        Sprite board;
        Sprite backBoard;



        /**
         * ces deux-là seront utilisés dans init() pour construire un gros Sprite pour le tableau à partir de plusieurs images d'une tuile
         * */
        RenderTexture render;
        virtual void boardFactory() = 0;

        Texture& backBoardFactory();

        void drawBase();
        /**
         * un Sprite verra sa texture modifiée et sa position remplacée pour dessiner toutes les pièces
         * au lieu d'avoir un sprite pour chacune des pièces du plateau. Ces deux méthodes sont virtuelles pour que
         * si un enfant de gameState est besoin d'une version différente de ces méthodes que ce soit leurs version qui
         * soient appelés
         * @param mousePos la position de la souris à transformer en position de Tuile sur le plateau de jeu
         * @return la position calculé dans le plateau
         * */
        virtual Vector2i getTileWithinBoard(Vector2f mousePos);
        virtual void positionPieceWithinBoard(Sprite *piece, Vector2i pos);

        Sprite *pieceSprite = nullptr;
        virtual void drawPieces();

        int currentPlayerIndex;
        vector<Text> playerNamesDisplay;
        vector<Text> playerScoresDisplay;
        void updateScoresDisplay();
        /**
         * x est le décalage de l'endroit où le dessiner sur la fenêtre
         */
        void initializePlayerDisplay(int x);
        void drawPlayerDisplay();
        void colorCurrentPlayer();

        /**
         * sera affiché à l'écran une fois le jeu terminé (printWinner = true)
         */
        Text winner;
        void initWinner();
        void gameOver();

        /**
         * utilisé pour montrer les mouvements possibles qu'une certaine pièce peut faire
         */
        Sprite redTileSprite;
        void positionRedTile(Vector2i v);
        /**
         * utilisé pour montrer quelle pièce est sélectionnée
         */
        Sprite blueTileSprite;
        void positionBlueTile(Vector2i v);
        void drawSelectedTile();

        Vector2i *fromTile = nullptr;
        Vector2i *toTile = nullptr;
        vector<Vector2i> movesPossible;
        void drawMovesPossible();
};
#endif
