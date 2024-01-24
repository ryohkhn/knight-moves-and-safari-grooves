#include "ButinState.hpp"
#include "../PauseState.hpp"

using namespace std;

ButinState::ButinState(Controller *controller): GameState(controller, BUTIN_BOARD_SIZE) {
	// do stuff in init rather then here
}
ButinState::~ButinState() {
    delete(pieceSprite);
    cout << "deleting butinGameState" << endl;
}

void ButinState::init() {
    Vector2f center = controller -> machine -> getCenter();

    initWinner();

    initInstructions.setFont(controller -> resource -> getFont("pixel"));
    initInstructions.setFillColor(Color(172, 50, 50));
    initInstructions.setCharacterSize(TEXT_SIZE);
    initInstructions.setString("Each player must select a yellow tile to remove from the game");
    initInstructions.setPosition(
        center.x - initInstructions.getGlobalBounds().width / 2, TILE_SIZE * 0.25
    );

    pieceSprite = new Sprite();
    pieceSprite -> setScale(1.25, 1.25);

    boardFactory();
    board.setTexture(render.getTexture());
    board.setScale(0.75, 0.75);

    backBoard.setTexture(backBoardFactory());
    backBoard.setScale(0.75, 0.75);

    background.setTexture(controller -> resource -> getTexture("background"));

    background.setPosition(0, 0);

    board.setPosition(
        center.x - board.getGlobalBounds().width / 2, center.y - board.getGlobalBounds().height / 2
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

void ButinState::handleInput() {
	Event event;

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
                if (controller -> input -> isSpriteClicked(board, Mouse::Left, *controller -> window)) {
                    Vector2i tileClicked = GameState::getTileWithinBoard(
                        controller -> input -> getMousePosition(*controller -> window)
                    );
                    if (pieces[tileClicked.x][tileClicked.y][0] == "yellowPiece") {
                        if (controller -> game -> hasGameStarted()) {

                            if (fromTile == nullptr) {
                                fromTile = new Vector2i(tileClicked);
                                movesPossible = controller -> game -> validMoves(ActionKey::LeftClick, *fromTile);
                            } else {
                                if (!playerPlayed) {
                                    if (*fromTile == tileClicked) {
                                        delete(fromTile);
                                        fromTile = nullptr;
                                        movesPossible.clear();
                                    } else {
                                        delete(fromTile);
                                        fromTile = new Vector2i(tileClicked);
                                        movesPossible = controller -> game -> validMoves(ActionKey::LeftClick, *fromTile);
                                    }
                                }
                            }
                        } else {
                            addPieceToRemove(tileClicked);
                            if (piecesToRemove.size() >= playerNamesDisplay.size()) {
                                controller -> game -> initializeGame(ButinConfig(piecesToRemove));
                                if (controller -> game -> hasGameStarted() && controller -> game -> isGameDone()) {
                                    gameOver();
                                }
                                GameState::colorCurrentPlayer();
                            }
                        }
                    } else if (pieces[tileClicked.x][tileClicked.y][0] == "") {
                        if (toTile == nullptr && fromTile != nullptr) {
                            for (Vector2i v : movesPossible) {
                                if (tileClicked == v) {
                                    playerPlayed = true;
                                    toTile = new Vector2i(tileClicked);
                                    moveReady = true;
                                    break;
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

void ButinState::draw() {
	controller -> window -> clear();

    GameState::drawBase();

    if (printWinner) {
        controller -> window -> draw(winner);
    } else {
        if (controller -> game -> hasGameStarted()) {
            if (fromTile != nullptr) {
                GameState::drawMovesPossible();
                GameState::drawSelectedTile();
            }
        } else {
            controller -> window -> draw(initInstructions);
            // Highlights the Tiles that will be removed by the players
            for (Vector2i v : piecesToRemove) {
                positionRedTile(v);
                controller -> window -> draw(redTileSprite);
            }
        }

        // Draws all the pieces on their Tiles
        GameState::drawPieces();
    }
    GameState::drawPlayerDisplay();
	controller -> window -> display();
}

void ButinState::update() {
	pieces = controller -> game -> getBoardState();
    if (moveReady) {
        int oldPlayerIndex = currentPlayerIndex;
        moveReady = false;
        controller -> game -> makeMove(ActionKey::LeftClick, *fromTile, *toTile);
        currentPlayerIndex = controller -> game -> getCurrentPlayerIndex();
        movesPossible.clear();

        if (currentPlayerIndex != oldPlayerIndex) {
            delete(fromTile);
            delete(toTile);
            fromTile = nullptr;
            toTile = nullptr;
            GameState::colorCurrentPlayer();
            playerPlayed = false;
        } else {
            delete(fromTile);
            fromTile = new Vector2i(*toTile);
            delete(toTile);
            toTile = nullptr;
            movesPossible = controller -> game -> validMoves(ActionKey::LeftClick, *fromTile);
        }

        if (controller -> game -> hasGameStarted() && controller -> game -> isGameDone()) {
            GameState::gameOver();
        }
        GameState::updateScoresDisplay();
    }
}

void ButinState::boardFactory() {
    Sprite whiteTileSprite = Sprite();
    Texture whiteTile = Texture();
    whiteTile.loadFromFile("./resources/board/WhiteTile.png");
    whiteTileSprite.setTexture(whiteTile);


    render.create(120 * BUTIN_BOARD_SIZE, 120 * BUTIN_BOARD_SIZE);
    render.clear();
    for (int i = 0; i < BUTIN_BOARD_SIZE; i++) {
        for (int j = 0; j < BUTIN_BOARD_SIZE; j++) {
            whiteTileSprite.setPosition(i * 120, j * 120);
            render.draw(whiteTileSprite);
        }
    }
    render.display();
}

void ButinState::addPieceToRemove(sf::Vector2i v) {
    auto it = find(piecesToRemove.begin(), piecesToRemove.end(),v);
    if (it != piecesToRemove.end()) {
        piecesToRemove.erase(it);
    } else {
        piecesToRemove.push_back(v);
    }
}