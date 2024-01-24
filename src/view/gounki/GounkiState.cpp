#include "GounkiState.hpp"
#include "../PauseState.hpp"


GounkiState::GounkiState(Controller *controller): GameState(controller, GOUNKI_BOARD_SIZE), isDeployement{false} {
    //Do stuuf in init
    pieces = controller -> game -> getBoardState();
}
GounkiState::~GounkiState() {
    delete(pieceSprite);
}

void GounkiState::init() {
    Vector2f center = controller -> machine -> getCenter();

    initWinner();

    pieceSprite = new Sprite();
    pieceSprite -> setScale(1.25, 1.25);

    boardFactory();
    board.setTexture(render.getTexture());
    board.setScale(0.75, 0.75);

    rowFactory();
    topRow.setTexture(rowRender.getTexture());
    topRow.setScale(0.75, 0.75);
    bottomRow.setTexture(rowRender.getTexture());
    bottomRow.setScale(0.75, 0.75);

    backBoard.setTexture(backBoardFactory());
    backBoard.setScale(0.75, 0.75);

    background.setTexture(controller -> resource -> getTexture("background"));

    background.setPosition(0, 0);

    board.setPosition(
        center.x - board.getGlobalBounds().width / 2, center.y - board.getGlobalBounds().height / 2
    );
    topRow.setPosition(
        center.x - topRow.getGlobalBounds().width / 2, center.y - board.getGlobalBounds().height / 2 - topRow.getGlobalBounds().height
    );
    bottomRow.setPosition(
        center.x - bottomRow.getGlobalBounds().width / 2, center.y + board.getGlobalBounds().height / 2
    );
    backBoard.setPosition(
        center.x - backBoard.getGlobalBounds().width / 2, center.y - backBoard.getGlobalBounds().height / 2
    );
    GameState::initializePlayerDisplay(TILE_SIZE);
    colorCurrentPlayer();
    redTileSprite.setTexture(controller -> resource -> getTexture("redTile"));
    redTileSprite.setScale(0.75, 0.75);
    blueTileSprite.setTexture(controller -> resource -> getTexture("blueTile"));
    blueTileSprite.setScale(0.75, 0.75);
}

void GounkiState::handleInput() {
    Event event;
    Vector2i tileClicked = GameState::getTileWithinBoard(controller -> input -> getMousePosition(*controller -> window));

	while (controller -> window -> pollEvent(event)) {
        switch (event.type) {
            case Event::Closed:
                controller -> window -> close();
                break;
            case Event::KeyPressed:
                if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                    controller -> machine -> addState(new PauseState(controller, controller -> getWindowAsTexture()), false);
                }
                break;
            case Event::MouseButtonPressed:
                if (controller -> input -> isSpriteClicked(topRow, Mouse::Left, *controller -> window)
                    || controller -> input -> isSpriteClicked(topRow, Mouse::Right, *controller -> window)
                ) {
                    if (fromTile != nullptr) {
                        tileClicked.y--;
                        cout << "TILE CLICKED: [" << tileClicked.x << "; " << tileClicked.y << "]\n";
                        vector<Vector2i>::const_iterator it = find(movesPossible.begin(), movesPossible.end(),tileClicked);
                        if (it != movesPossible.end()) {
                            toTile = new Vector2i(tileClicked);
                            moveReady = true;
                        }
                    }
                } else if (controller -> input -> isSpriteClicked(bottomRow, Mouse::Left, *controller -> window)
                    || controller -> input -> isSpriteClicked(bottomRow, Mouse::Right, *controller -> window)) {
                    if (fromTile != nullptr) {
                        tileClicked.y = GOUNKI_BOARD_SIZE;
                        cout << "TILE CLICKED: [" << tileClicked.x << "; " << tileClicked.y << "]\n";
                        vector<Vector2i>::const_iterator it = find(movesPossible.begin(), movesPossible.end(),tileClicked);
                        if (it != movesPossible.end()) {
                            toTile = new Vector2i(tileClicked);
                            moveReady = true;
                        }
                    }
                } else if (controller -> input -> isSpriteClicked(board, Mouse::Left, *controller -> window)) {
                    bool friendlyPiece = currentPlayerIndex == UtilityFunctions::getPlayerFromName(pieces[tileClicked.x][tileClicked.y][0]);
                    if (fromTile == nullptr) {
                        if (friendlyPiece) {
                            isDeployement = false;
                            fromTile = new Vector2i(tileClicked);
                            movesPossible = controller -> game -> validMoves(ActionKey::LeftClick, *fromTile);
                        }
                    } else {
                        if (!isDeployement) {
                            if (tileClicked == *fromTile) {
                                delete (fromTile);
                                fromTile = nullptr;
                                movesPossible.clear();
                            } else {
                                vector<Vector2i>::const_iterator it = find(movesPossible.begin(), movesPossible.end(),tileClicked);
                                if (it != movesPossible.end()) {
                                    toTile = new Vector2i(tileClicked);
                                    moveReady = true;
                                }
                            }
                        }
                    }
                } else if (controller -> input -> isSpriteClicked(board, Mouse::Right, *controller -> window)) {
                    bool friendlyPiece = currentPlayerIndex == UtilityFunctions::getPlayerFromName(pieces[tileClicked.x][tileClicked.y][0]);
                    if (fromTile == nullptr) {
                        if (friendlyPiece) {
                            isDeployement = true;
                            fromTile = new Vector2i(tileClicked);
                            movesPossible = controller -> game -> validMoves(ActionKey::RightClick, *fromTile);
                        }
                    } else {
                        if (isDeployement) {
                            if (tileClicked == *fromTile) {
                                delete (fromTile);
                                fromTile = nullptr;
                                movesPossible.clear();
                            } else {
                                vector<Vector2i>::const_iterator it = find(movesPossible.begin(), movesPossible.end(),tileClicked);
                                if (it != movesPossible.end()) {
                                    toTile = new Vector2i(tileClicked);
                                    moveReady = true;
                                }
                            }
                        }
                    }
                }
                break;
            default:
                break;
        }
	}
}
void GounkiState::drawBase() {
    controller -> window -> draw(background);
    controller -> window -> draw(topRow);
    controller -> window -> draw(bottomRow);
    controller -> window -> draw(backBoard);
    controller -> window -> draw(board);
}
void GounkiState::positionPieceWithinBoard(Sprite *piece, Vector2i pos, int offset) {
    int tileWidth = board.getGlobalBounds().width / boardSize;
    int tileHeight = board.getGlobalBounds().height / boardSize;
    piece -> setPosition(
        pos.x * tileWidth + board.getGlobalBounds().left + (tileWidth / 2 - piece -> getGlobalBounds().width / 2) + (15 * offset),
        pos.y * tileHeight + board.getGlobalBounds().top + (tileHeight / 2 - piece -> getGlobalBounds().height / 2) - (15 * offset)
    );
}

void GounkiState::drawPieces() {
    for (int i = 0; i < (int) pieces.size(); i++) {
        for (int j = 0; j < (int) pieces[i].size(); j++) {
            for (int k = (int) pieces[i][j].size() - 1; k >= 0 ; k--) {
                if (pieces[i][j][k] != (string)"") {
                    pieceSprite -> setTexture(controller -> resource -> getTexture(pieces[i][j][k]));
                    Vector2i v{i, j};
                    positionPieceWithinBoard(pieceSprite, v, k);
                    controller -> window -> draw(*pieceSprite);
                }
            }

        }
    }
}

void GounkiState::draw() {
	controller -> window -> clear();

    drawBase();

    if (printWinner) {
        controller -> window -> draw(winner);
    } else {
        if (controller -> game -> hasGameStarted()) {
            if (fromTile != nullptr) {
                GameState::drawMovesPossible();
                GameState::drawSelectedTile();
            }
        }
        // Draws all the pieces on their Tiles
        drawPieces();
    }
    GameState::drawPlayerDisplay();
	controller -> window -> display();
}

void GounkiState::update() {
    if (moveReady) {
        int oldPlayerIndex = currentPlayerIndex;
        moveReady = false;
        if (isDeployement) {
            controller -> game -> makeMove(ActionKey::RightClick, *fromTile, *toTile);
        } else {
            controller -> game -> makeMove(ActionKey::LeftClick, *fromTile, *toTile);
        }
        currentPlayerIndex = controller -> game -> getCurrentPlayerIndex();
        movesPossible.clear();

        if (controller -> game -> isGameDone()) {
            gameOver();
        } else {
            if (currentPlayerIndex != oldPlayerIndex) {
                delete(fromTile);
                delete(toTile);
                fromTile = nullptr;
                toTile = nullptr;
                GameState::colorCurrentPlayer();
            } else {
                delete(fromTile);
                fromTile = new Vector2i(*toTile);
                delete(toTile);
                toTile = nullptr;
                movesPossible = controller -> game -> validMoves(ActionKey::RightClick, *fromTile);
            }
        }

        updateScoresDisplay();
        pieces = controller -> game -> getBoardState();
    }
}


void GounkiState::rowFactory() {
    Sprite whiteTileSprite= Sprite();
    Texture whiteTile = Texture();

    whiteTile.loadFromFile("./resources/board/WhiteTile.png");
    whiteTileSprite.setTexture(whiteTile);

    rowRender.create(120 * GOUNKI_BOARD_SIZE, 120);
    rowRender.clear();
    for (int i = 0; i < GOUNKI_BOARD_SIZE; i++) {
        whiteTileSprite.setPosition(i * 120, 0);
        rowRender.draw(whiteTileSprite);
    }
    rowRender.display();
}

void GounkiState::boardFactory() {
    Sprite whiteTileSprite, blackTileSprite = Sprite();
    Texture whiteTile, blackTile = Texture();

    whiteTile.loadFromFile("./resources/board/WhiteTile.png");
    blackTile.loadFromFile("./resources/board/BlackTile.png");
    whiteTileSprite.setTexture(whiteTile);
    blackTileSprite.setTexture(blackTile);


    render.create(120 * GOUNKI_BOARD_SIZE, 120 * GOUNKI_BOARD_SIZE);
    render.clear();
    for (int i = 0; i < GOUNKI_BOARD_SIZE; i++) {
        for (int j = 0; j < GOUNKI_BOARD_SIZE; j++) {
            if ((j + i) % 2 == 0) {
                whiteTileSprite.setPosition(i * 120, j * 120);
                render.draw(whiteTileSprite);
            } else {
                blackTileSprite.setPosition(i * 120, j * 120);
                render.draw(blackTileSprite);
            }
        }
    }
    render.display();
}