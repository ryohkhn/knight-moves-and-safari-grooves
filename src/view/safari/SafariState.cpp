#include "SafariState.hpp"
#include "../PauseState.hpp"

SafariState::SafariState(Controller *controller): GameState(controller, SAFARI_BOARD_SIZE), moveAnimal{false}, placeFence{false}, fencePlaced{0}, canEndTurn{false}, boardScale{1} {
    // Do stuff in Init
}

SafariState::~SafariState() {
    delete(pieceSprite);
    cout << "deleting SafariGameState" << endl;
}

void SafariState::init() {
    Vector2f center = controller -> machine -> getCenter();

    initWinner();

    initInstructions.setFont(controller -> resource -> getFont("pixel"));
    initInstructions.setFillColor(Color(172, 50, 50));
    initInstructions.setCharacterSize(TEXT_SIZE);
    initInstructions.setString("Each player must place their animals and fences");
    initInstructions.setPosition(
        center.x - initInstructions.getGlobalBounds().width / 2, TILE_SIZE * 0.25
    );

    pieceSprite = new Sprite();
    pieceSprite -> setScale(2, 2);
    horizontalFenceSprite = new Sprite();
    horizontalFenceSprite -> setTexture(controller -> resource -> getTexture("redHorizontalFence"));
    horizontalFenceSprite -> setScale(boardScale, boardScale);
    verticalFenceSprite = new Sprite();
    verticalFenceSprite -> setTexture(controller -> resource -> getTexture("redVerticalFence"));
    verticalFenceSprite -> setScale(boardScale, boardScale);

    boardFactory();
    board.setTexture(render.getTexture());
    board.setScale(boardScale, boardScale);

    backBoard.setTexture(backBoardFactory());

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
    redTileSprite.setScale(boardScale, boardScale);
    blueTileSprite.setTexture(controller -> resource -> getTexture("blueTile"));
    blueTileSprite.setScale(boardScale, boardScale);

    endTurnButton.setTexture(controller -> resource -> getTexture("disabledEndTurnButton"));
    endTurnButton.setPosition(
        SCREEN_WIDTH - TILE_SIZE * 3 - endTurnButton.getGlobalBounds().width,
        INFO_HEIGHT
    );
}
void SafariState::incrementPlayer() {
    currentPlayerIndex = (currentPlayerIndex + 1) % playerNamesDisplay.size();
    colorCurrentPlayer();
}

void SafariState::endTurnButtonEffect() {
    if (canEndTurn) {
        if (controller -> game -> hasGameStarted()) {
            moveReady = true;
        } else {
            canEndTurn = false;
            fencePlaced = 0;
            incrementPlayer();
            if (currentPlayerIndex == 0) {
                controller -> game -> initializeGame(config);
                pieces = controller -> game -> getBoardState();
                updateScoresDisplay();
            }
        }
    }
}

void SafariState::handleInput() {
	Event event;

	while (controller -> window -> pollEvent(event)) {
        switch (event.type) {
            case Event::Closed:
                controller -> window -> close();
                break;
            case sf::Event::KeyPressed:
                if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                    endTurnButtonEffect();
                } else if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                    controller -> machine -> addState(new PauseState(controller, controller -> getWindowAsTexture()), false);
                }
                break;
            case sf::Event::MouseButtonPressed:
                if (controller -> input -> isSpriteClicked(endTurnButton, Mouse::Left, *controller -> window)) {
                    endTurnButtonEffect();
                } else if (controller -> input -> isSpriteClicked(board, Mouse::Left, *controller -> window)) {
                    Vector2i pieceClicked = SafariState::getTileWithinBoard(
                    controller -> input -> getMousePosition(*controller -> window),
                    ActionKey::LeftClick
                    );
                    if (pieceClicked.x % 2 == 0 && pieceClicked.y % 2 == 0) {
                        if (controller -> game -> hasGameStarted()) {
                            int pieceValue = UtilityFunctions::getPlayerFromAnimal(UtilityFunctions::getSafariPieceType(pieces[pieceClicked.x][pieceClicked.y][0]));
                            if (pieceValue == currentPlayerIndex) {
                                if (fromTile == nullptr) {
                                    fromTile = new Vector2i(pieceClicked);
                                        movesPossible = controller -> game -> validMoves(ActionKey::LeftClick, *fromTile);
                                    } else {
                                    if (!moveAnimal) {
                                        if (*fromTile == pieceClicked) {
                                            delete(fromTile);
                                            fromTile = nullptr;
                                            movesPossible.clear();
                                        } else {
                                            delete(fromTile);
                                            fromTile = new Vector2i(pieceClicked);
                                            movesPossible = controller -> game -> validMoves(ActionKey::LeftClick, *fromTile);
                                        }
                                    }
                                }
                            } else if (pieceValue == 4) {// 4 is the value of the EmptySafari enum
                                if (toTile == nullptr && fromTile != nullptr) {
                                    for (Vector2i v : movesPossible) {
                                        if (pieceClicked == v) {
                                            playerPlayed = true;
                                            toTile = new Vector2i(pieceClicked);
                                            moveAnimal = true;
                                            if (placeFence) canEndTurn = true;
                                            break;
                                        }
                                    }
                                }
                            }
                        } else {
                            switch (currentPlayerIndex) {
                                case 0:
                                    addCrocodile(pieceClicked);
                                    if (config.crocodiles.size() >= 3 && fencePlaced == 3) {
                                        canEndTurn = true;
                                    } else {
                                        canEndTurn = false;
                                    }
                                    break;
                                case 1:
                                    addElephant(pieceClicked);
                                    if (config.elephants.size() >= 3 && fencePlaced == 3) {
                                        canEndTurn = true;
                                    } else {
                                        canEndTurn = false;
                                    }
                                    break;
                                case 2:
                                    addLion(pieceClicked);
                                    if (config.lions.size() >= 3 && fencePlaced == 3) {
                                        canEndTurn = true;
                                    } else {
                                        canEndTurn = false;
                                    }
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                } else if (controller -> input -> isSpriteClicked(board, Mouse::Right, *controller -> window)) {
                    Vector2i fenceClicked = SafariState::getTileWithinBoard(
                    controller -> input -> getMousePosition(*controller -> window),
                    ActionKey::RightClick
                    );
                    if ((fenceClicked.x % 2 != 0 && fenceClicked.y % 2 == 0) || (fenceClicked.x % 2 == 0 && fenceClicked.y % 2 != 0)) {
                        if (controller -> game -> hasGameStarted()) {
                            int pieceValue = UtilityFunctions::getPlayerFromAnimal(UtilityFunctions::getSafariPieceType(pieces[fenceClicked.x][fenceClicked.y][0]));
                            if (onTile == nullptr) {
                                if (pieceValue == 4) {// 4 is the value of the EmptySafari enum
                                    onTile = new Vector2i(fenceClicked);
                                    placeFence = true;
                                    if (moveAnimal) canEndTurn = true;
                                }
                            } else {
                                if (*onTile == fenceClicked) {
                                    delete(onTile);
                                    onTile = nullptr;
                                    placeFence = false;
                                    canEndTurn = false;
                                } else if (pieceValue == 4) {// 4 is the value of the EmptySafari enum
                                    delete(onTile);
                                    onTile = new Vector2i(fenceClicked);
                                }
                            }
                        } else {
                            size_t oldSize = config.fences.size();
                            addFence(fenceClicked);
                            fencePlaced = config.fences.size() - 3 * currentPlayerIndex;
                            if (config.fences.size() >= oldSize) {
                                if (fencePlaced == 3) {
                                    switch (currentPlayerIndex) {
                                        case 0:
                                            if (config.crocodiles.size() == 3) {
                                                canEndTurn = true;
                                            }
                                            break;
                                        case 1:
                                            if (config.elephants.size() == 3) {
                                                canEndTurn = true;
                                            }
                                            break;
                                        case 2:
                                            if (config.lions.size() == 3) {
                                                canEndTurn = true;
                                            }
                                            break;
                                        default:
                                            break;
                                    }
                                }
                            } else {
                                canEndTurn = false;
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

void SafariState::draw() {
	controller -> window -> clear();

    GameState::drawBase();
    controller -> window -> draw(endTurnButton);

    if (printWinner) {
        drawPieces();
        controller -> window -> draw(winner);
    } else {
        if (controller -> game -> hasGameStarted()) {
            if (fromTile != nullptr) {
                GameState::drawMovesPossible();
                GameState::drawSelectedTile();
            }
            if (onTile != nullptr) {
                if (onTile -> x % 2 == 0) {
                    horizontalFenceSprite -> setTexture(controller -> resource -> getTexture("redHorizontalFence"));
                    positionFenceWithinBoard(horizontalFenceSprite, *onTile);
                    controller -> window -> draw(*horizontalFenceSprite);
                } else {
                    verticalFenceSprite -> setTexture(controller -> resource -> getTexture("redVerticalFence"));
                    positionFenceWithinBoard(verticalFenceSprite, *onTile);
                    controller -> window -> draw(*verticalFenceSprite);
                }
            }
        } else {
            controller -> window -> draw(initInstructions);
            for (Vector2i croc : config.crocodiles) {
                if (currentPlayerIndex == 0) {
                    positionBlueTile(croc);
                    controller -> window -> draw(blueTileSprite);
                }

                pieceSprite -> setTexture(controller -> resource -> getTexture("crocodilePiece"));
                positionPieceWithinBoard(pieceSprite, croc);
                controller -> window -> draw(*pieceSprite);
            }
            for (Vector2i ele : config.elephants) {
                if (currentPlayerIndex == 1) {
                    positionBlueTile(ele);
                    controller -> window -> draw(blueTileSprite);
                }

                pieceSprite -> setTexture(controller -> resource -> getTexture("elephantPiece"));
                positionPieceWithinBoard(pieceSprite, ele);
                controller -> window -> draw(*pieceSprite);
            }
            for (Vector2i lio : config.lions) {
                if (currentPlayerIndex == 2) {
                    positionBlueTile(lio);
                    controller -> window -> draw(blueTileSprite);
                }

                pieceSprite -> setTexture(controller -> resource -> getTexture("lionPiece"));
                positionPieceWithinBoard(pieceSprite, lio);
                controller -> window -> draw(*pieceSprite);
            }
            for (int i = 0; i < (int) config.fences.size(); i++) {
                if (i >= currentPlayerIndex * 3) {
                    horizontalFenceSprite -> setTexture(controller -> resource -> getTexture("redHorizontalFence"));
                    verticalFenceSprite -> setTexture(controller -> resource -> getTexture("redVerticalFence"));
                } else {
                    horizontalFenceSprite -> setTexture(controller -> resource -> getTexture("horizontalFence"));
                    verticalFenceSprite -> setTexture(controller -> resource -> getTexture("verticalFence"));
                }
                if (config.fences[i].x % 2 == 0) {
                    positionFenceWithinBoard(horizontalFenceSprite, config.fences[i]);
                    controller -> window -> draw(*horizontalFenceSprite);
                } else {
                    positionFenceWithinBoard(verticalFenceSprite, config.fences[i]);
                    controller -> window -> draw(*verticalFenceSprite);
                }
            }
        }
        drawPieces();
    }
    GameState::drawPlayerDisplay();
	controller -> window -> display();
}

void SafariState::update() {
    if (canEndTurn) {
        endTurnButton.setTexture(controller -> resource -> getTexture("enabledEndTurnButton"));
    } else {
        endTurnButton.setTexture(controller -> resource -> getTexture("disabledEndTurnButton"));
    }
    if (moveReady) {
        moveReady = false;
        canEndTurn = false;
        placeFence = false;
        moveAnimal = false;
        controller -> game -> makeMove(ActionKey::LeftClick, *fromTile, *toTile);
        ((Safari *) controller -> game) -> makeMove(ActionKey::RightClick, *onTile);
        pieces = controller -> game -> getBoardState();
        currentPlayerIndex = controller -> game -> getCurrentPlayerIndex();
        movesPossible.clear();

        delete(fromTile);
        delete(toTile);
        delete(onTile);
        fromTile = nullptr;
        toTile = nullptr;
        onTile = nullptr;
        GameState::colorCurrentPlayer();
        if (controller -> game -> hasGameStarted() && controller -> game -> isGameDone()) {
            GameState::gameOver();
        }
        GameState::updateScoresDisplay();
    }
}
Vector2i SafariState::getTileWithinBoard(Vector2f mousePos, ActionKey action) {
    mousePos.x -= board.getGlobalBounds().left;
    mousePos.y -= board.getGlobalBounds().top;
    int accX = 0;
    int accY = 0;
    int actualTileSize = board.getGlobalBounds().width / ((SAFARI_BOARD_SIZE + 1) / 2);
    int i, j;
    for (i = 0; accX < mousePos.x - actualTileSize; i += 2) {
        accX += actualTileSize;
    }
    for (j = 0; accY < mousePos.y - actualTileSize; j += 2) {
        accY += actualTileSize;
    }
    if (action == ActionKey::RightClick) {
        int closerX;
        bool addX;
        int closerY;
        bool addY;
        int posInTile = mousePos.x - accX;
        if (posInTile < actualTileSize - posInTile) {
            closerX = posInTile;
            addX = false;
        } else {
            closerX = actualTileSize - posInTile;
            addX = true;
        }
        posInTile = mousePos.y - accY;
        if (posInTile < actualTileSize - posInTile) {
            closerY = posInTile;
            addY = false;
        } else {
            closerY = actualTileSize - posInTile;
            addY = true;
        }
        if (closerX < closerY) {
            if (addX && i < SAFARI_BOARD_SIZE - 1) {
                i++;
            } else if (i > 0) {
                i--;
            }
        } else {
            if (addY && j < SAFARI_BOARD_SIZE - 1) {
                j++;
            } else if (j > 0) {
                j--;
            }
        }
    }
    return Vector2i(i, j);
}
void SafariState::positionPieceWithinBoard(Sprite *piece, Vector2i pos) {
    int actualTileSize = board.getGlobalBounds().width / ((boardSize + 1) / 2);
    piece -> setPosition(pos.x / 2 * actualTileSize + board.getGlobalBounds().left, pos.y / 2 * actualTileSize + board.getGlobalBounds().top);
}
void SafariState::positionFenceWithinBoard(Sprite *fence, Vector2i pos) {
    positionPieceWithinBoard(fence, pos);
    int actualTileSize = board.getGlobalBounds().width / ((boardSize + 1) / 2);
    if (pos.x % 2 == 0) {
        fence -> setPosition(
            fence -> getGlobalBounds().left,
            fence -> getGlobalBounds().top + actualTileSize - (fence -> getGlobalBounds().height / 2)
        );
    } else {
        fence -> setPosition(
            fence -> getGlobalBounds().left + actualTileSize - (fence -> getGlobalBounds().width / 2),
            fence -> getGlobalBounds().top
        );
    }

}
void SafariState::drawPieces() {
    for (int i = 0; i < (int) pieces.size(); i++) {
        for (int j = 0; j < (int) pieces.size(); j++) {
            Vector2i v{i, j};
            if (pieces[i][j][0] == "fence") {
                if (i % 2 == 0) { //Horizontal Fence
                    horizontalFenceSprite -> setTexture(controller -> resource -> getTexture("horizontalFence"));
                    positionFenceWithinBoard(horizontalFenceSprite, v);
                    controller -> window -> draw(*horizontalFenceSprite);
                } else { // Vertical
                    verticalFenceSprite -> setTexture(controller -> resource -> getTexture("verticalFence"));
                    positionFenceWithinBoard(verticalFenceSprite, v);
                    controller -> window -> draw(*verticalFenceSprite);
                }
            } else if (pieces[i][j][0] != "") { //Animal
                pieceSprite -> setTexture(controller -> resource -> getTexture(pieces[i][j][0]));
                positionPieceWithinBoard(pieceSprite, v);
                controller -> window -> draw(*pieceSprite);
            }
        }
    }
}
void SafariState::boardFactory() {
    Texture tile = Texture();
    tile.loadFromFile("./resources/board/BrownTile.png");
    Sprite *tileSprite = new Sprite();
    tileSprite -> setTexture(tile);


    render.create(120 * (SAFARI_BOARD_SIZE / 2 + 1), 120 * (SAFARI_BOARD_SIZE / 2 + 1));
    render.clear(Color(24, 24, 24));
    for (int i = 0; i < (SAFARI_BOARD_SIZE + 1) / 2; i++) {
        for (int j = 0; j < (SAFARI_BOARD_SIZE + 1) / 2; j++) {
            tileSprite -> setPosition(
                i * tileSprite -> getGlobalBounds().width,
                j * tileSprite -> getGlobalBounds().height
            );
            render.draw(*tileSprite);
        }
    }
    render.display();
    delete(tileSprite);
}
Texture& SafariState::backBoardFactory() {
    Image bgColor;
    bgColor.create(board.getGlobalBounds().width + 50, board.getGlobalBounds().height + 50, Color(24, 24, 24));
    Texture *bgTexture = new Texture();
    bgTexture -> loadFromImage(bgColor);
    return *bgTexture;
}

void SafariState::addCrocodile(Vector2i v) {
    vector<Vector2i>::const_iterator itCroc = find(config.crocodiles.begin(), config.crocodiles.end(),v);
    vector<Vector2i>::const_iterator itEle = find(config.elephants.begin(), config.elephants.end(),v);
    vector<Vector2i>::const_iterator itLio = find(config.lions.begin(), config.lions.end(),v);
    if (itEle == config.elephants.end() && itLio == config.lions.end()) {
        if (itCroc != config.crocodiles.end()) {
            config.crocodiles.erase(itCroc);
        } else {
            if (config.crocodiles.size() < 3) {config.crocodiles.push_back(v);}
        }
    }
}
void SafariState::addElephant(sf::Vector2i v) {
    vector<Vector2i>::const_iterator itEle = find(config.elephants.begin(), config.elephants.end(),v);
    vector<Vector2i>::const_iterator itCroc = find(config.crocodiles.begin(), config.crocodiles.end(),v);
    vector<Vector2i>::const_iterator itLio = find(config.lions.begin(), config.lions.end(),v);
    if (itCroc == config.crocodiles.end() && itLio == config.lions.end()) {
        if (itEle != config.elephants.end()) {
            config.elephants.erase(itEle);
        } else {
            if (config.elephants.size() < 3) {config.elephants.push_back(v);}
        }
    }
}
void SafariState::addLion(sf::Vector2i v) {
    vector<Vector2i>::const_iterator itLio = find(config.lions.begin(), config.lions.end(),v);
    vector<Vector2i>::const_iterator itCroc = find(config.crocodiles.begin(), config.crocodiles.end(),v);
    vector<Vector2i>::const_iterator itEle = find(config.elephants.begin(), config.elephants.end(),v);
    if (itCroc == config.crocodiles.end() && itEle == config.elephants.end()) {
        if (itLio != config.lions.end()) {
            config.lions.erase(itLio);
        } else {
            if (config.lions.size() < 3) {config.lions.push_back(v);}
        }
    }
}
void SafariState::addFence(Vector2i v) {
    vector<Vector2i>::const_iterator it = find(config.fences.begin(), config.fences.end(),v);
    if (it != config.fences.end()) {
        if (it - config.fences.begin() >= currentPlayerIndex * 3) {config.fences.erase(it);}
    } else {
        if (fencePlaced < 3) {config.fences.push_back(v);}
    }
}