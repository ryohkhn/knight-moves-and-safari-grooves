#include "Game.hpp"

bool Game::hasGameStarted() const {
    return gameStarted;
}

void Game::initPlayers(vector<string> playerNames) {
    for (const string& s : playerNames){
        playerList.push_back(new Player(s));
    }
    currentPlayerIndex = 0;
}

vector<Vector2i> Game::validMoves(ActionKey action, const Vector2i &from) const {
    return board->validMoves(action, currentPlayerIndex, from);
}

vector<vector<vector<string>>> Game::getBoardState() const {
    return board->getBoardState();
}

int Game::getCurrentPlayerIndex() const {
    return currentPlayerIndex;
}

vector<string> Game::getPlayerNames() const {
    vector<string> res;
    for (Player* p: playerList) {
        res.push_back(p->name);
    }
    return res;
}

Game::~Game() {
    for (Player* player : playerList) {
        delete player;
    }
    delete board;
    cout << "Destruction of Game" << endl;
}