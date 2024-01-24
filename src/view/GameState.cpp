#include "GameState.hpp"

GameState::GameState(Controller *controller, int boardSize): controller{controller}, boardSize{boardSize}, moveReady{false}, playerPlayed{false}, printWinner{false}, currentPlayerIndex{controller -> game -> getCurrentPlayerIndex()} {
    // Do stuff in init()
}
GameState::~GameState() {
    if (fromTile != nullptr) delete(fromTile);
    if (toTile != nullptr) delete(toTile);
    controller -> removeGame();
    cout << "deleting gamestate" << endl;
}
Texture& GameState::backBoardFactory() {
    Image bgColor;
    bgColor.create(BUTIN_BOARD_SIZE * 120 + 50, BUTIN_BOARD_SIZE * 120 + 50, Color(24, 24, 24));
    Texture *bgTexture = new Texture();
    bgTexture -> loadFromImage(bgColor);
    return *bgTexture;
}

void GameState::drawBase() {
    controller -> window -> draw(background);
    controller -> window -> draw(backBoard);
    controller -> window -> draw(board);
}

Vector2i GameState::getTileWithinBoard(Vector2f mousePos) {
    return Vector2i{
            (int) ((mousePos.x - board.getGlobalBounds().left) / (board.getGlobalBounds().width / boardSize)),
            (int) ((mousePos.y  - board.getGlobalBounds().top) / (board.getGlobalBounds().height / boardSize))
    };
}
void GameState::positionPieceWithinBoard(Sprite *piece, Vector2i pos) {
    int tileWidth = board.getGlobalBounds().width / boardSize;
    int tileHeight = board.getGlobalBounds().height / boardSize;
    piece -> setPosition(
        pos.x * tileWidth + board.getGlobalBounds().left + (tileWidth / 2 - piece -> getGlobalBounds().width / 2),
        pos.y * tileHeight + board.getGlobalBounds().top + (tileHeight / 2 - piece -> getGlobalBounds().height / 2)
    );
}
void GameState::drawPieces() {
    for (int i = 0; i < (int) pieces.size(); i++) {
        for (int j = 0; j < (int) pieces[i].size(); j++) {
            if (pieces[i][j][0] != "") {
                pieceSprite -> setTexture(controller -> resource -> getTexture(pieces[i][j][0]));
                Vector2i v{i, j};
                positionPieceWithinBoard(pieceSprite, v);
                controller -> window -> draw(*pieceSprite);
            }
        }
    }
}
void GameState::updateScoresDisplay() {
    vector<string> newScores = ((Butin *) controller -> game) -> getPlayerScores();
    for (size_t i = 0; i < playerScoresDisplay.size(); i++) {
        playerScoresDisplay[i].setString(newScores[i]);
    }
}
void GameState::initializePlayerDisplay(int x) {
    for (size_t i = 0; i < controller -> game -> getPlayerNames().size(); i++) {
        playerNamesDisplay.push_back(Text());
        playerNamesDisplay[i].setFont(controller -> resource -> getFont("pixel"));
        playerNamesDisplay[i].setCharacterSize(TEXT_SIZE);
        playerNamesDisplay[i].setFillColor(Color::Black);
        playerNamesDisplay[i].setString(controller -> game -> getPlayerNames()[i]);
        playerNamesDisplay[i].setPosition(
            x * 2.5,
            INFO_HEIGHT + TILE_SIZE * i
        );

        playerScoresDisplay.push_back(Text());
        playerScoresDisplay[i].setFont(controller -> resource -> getFont("pixel"));
        playerScoresDisplay[i].setCharacterSize(TEXT_SIZE);
        playerScoresDisplay[i].setFillColor(Color::Black);
        playerScoresDisplay[i].setString(to_string(0));
        playerScoresDisplay[i].setPosition(
            x * 3 + playerNamesDisplay[i].getGlobalBounds().width,
            INFO_HEIGHT + TILE_SIZE * i
        );
    }
}
void GameState::drawPlayerDisplay() {
    for (Text t : playerNamesDisplay) {
        controller -> window -> draw(t);
    }
    for (Text t : playerScoresDisplay) {
        controller -> window -> draw(t);
    }
}
void GameState::colorCurrentPlayer() {
    for (size_t i = 0; i < playerNamesDisplay.size(); i++) {
        playerNamesDisplay[i].setFillColor(Color::Black);
    }
    playerNamesDisplay[currentPlayerIndex].setFillColor(Color(172, 50, 50));
}
void GameState::positionRedTile(Vector2i v) {
    positionPieceWithinBoard(&redTileSprite, v);
}
void GameState::positionBlueTile(Vector2i v) {
    positionPieceWithinBoard(&blueTileSprite, v);
}
void GameState::drawSelectedTile() {
    if (fromTile != nullptr) {
        positionBlueTile(*fromTile);
        controller -> window -> draw(blueTileSprite);
        if (toTile != nullptr) {
            positionBlueTile(*toTile);
            controller -> window -> draw(blueTileSprite);
        }
    }
}
void GameState::drawMovesPossible() {
    for (Vector2i v : movesPossible) {
        positionRedTile(v);
        controller -> window -> draw(redTileSprite);
    }
}

void GameState::initWinner() {
    winner.setFont(controller -> resource -> getFont("pixel"));
    winner.setFillColor(Color(9, 109, 8));
    winner.setCharacterSize(TITLE_SIZE * 3);
}
void GameState::gameOver() {
    winner.setString(controller -> game -> getWinner());
    winner.setPosition(controller -> machine -> getCenter().x - winner.getGlobalBounds().width / 2, controller -> machine -> getCenter().y - winner.getGlobalBounds().height / 2);
    printWinner = true;
}
