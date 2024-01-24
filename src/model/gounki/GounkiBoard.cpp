#include "GounkiBoard.hpp"

GounkiBoard::GounkiBoard(): gameDone {false} {
    board.resize(GOUNKI_BOARD_SIZE);
    for (vector<vector<Piece *>>& column : board) {
        column.resize(GOUNKI_BOARD_SIZE);
    }
    generateDefaultBoard();
    cout << "Construction of " << *this;
}

void GounkiBoard::generateDefaultBoard() {
    for (int i = 0; i < GOUNKI_BOARD_SIZE; ++i) {
        board[i][0].push_back(new GounkiPiece(
                i % 2 == 0 ? GounkiPieceType::BlackCircle : GounkiPieceType::BlackSquare,
                Vector2i{0, i}));
        board[i][1].push_back(new GounkiPiece(
                i % 2 == 0 ? GounkiPieceType::BlackSquare : GounkiPieceType::BlackCircle,
                Vector2i{1, i}));
        board[i][GOUNKI_BOARD_SIZE - 2].push_back(new GounkiPiece(
                i % 2 == 0 ? GounkiPieceType::WhiteCircle : GounkiPieceType::WhiteSquare,
                Vector2i{GOUNKI_BOARD_SIZE - 1, i}));
        board[i][GOUNKI_BOARD_SIZE - 1].push_back(new GounkiPiece(
                i % 2 == 0 ? GounkiPieceType::WhiteSquare : GounkiPieceType::WhiteCircle,
                Vector2i{GOUNKI_BOARD_SIZE - 2, i}));
    }
}

bool GounkiBoard::isGameDone() const {
    return gameDone;
}

void GounkiBoard::initializeGame(const GameConfig &) {}

vector<vector<vector<string>>> GounkiBoard::getBoardState() const {
    vector<vector<vector<string>>> boardState(GOUNKI_BOARD_SIZE, vector<vector<string>>(GOUNKI_BOARD_SIZE));
    for (int i = 0; i < GOUNKI_BOARD_SIZE; i++) {
        for (int j = 0; j < GOUNKI_BOARD_SIZE; j++) {
            string s;
            if (board[i][j].size() == 0) {
                s = UtilityFunctions::getGounkiPieceString(GounkiPieceType::EmptyGounki);
                boardState[i][j].push_back(s);
            }
            else {
                for (Piece* p : board[i][j]) {
                    s = UtilityFunctions::getGounkiPieceString(((GounkiPiece*) p)->type);
                    boardState[i][j].push_back(s);
                }
            }
        }
    }
    return boardState;
}

bool GounkiBoard::isLandedCaseEnnemy(const Vector2i& from, const Vector2i& to) const{
    const vector<Piece*>& currentCase = board[from.x][from.y];
    const vector<Piece*>& nextCase = board[to.x][to.y];
    if (nextCase.empty())
        return false;

    GounkiPiece* currentPiece = (GounkiPiece*)currentCase[0];
    GounkiPiece* nextPiece = (GounkiPiece*)nextCase[0];
    switch (currentPiece->type) {
        case GounkiPieceType::BlackCircle:
        case GounkiPieceType::BlackSquare:
            return (nextPiece->type == GounkiPieceType::WhiteSquare
            || nextPiece->type == GounkiPieceType::WhiteCircle);
        case GounkiPieceType::WhiteCircle:
        case GounkiPieceType::WhiteSquare:
             return (nextPiece->type == GounkiPieceType::BlackSquare
             || nextPiece->type == GounkiPieceType::BlackCircle);
        default:
            return false;
    }
}

GounkiPieceType GounkiBoard::determinePieceType(int playerIndex, const Vector2i& from, const Vector2i& to) const {
    Vector2i diff = to - from;

    // iterate through each piece type and its movement patterns
    const map<GounkiPieceType, vector<Vector2<int>>>& map = GounkiPiece::getMovesForPlayer(playerIndex);
    for (const pair<const GounkiPieceType, vector<Vector2i>>& pair : map) {
        const vector<Vector2i>& movementPattern = pair.second;

        if (find(movementPattern.begin(), movementPattern.end(), diff)
        != movementPattern.end()) {
            return pair.first;
        }
    }

    return GounkiPieceType::EmptyGounki;
}

void GounkiBoard::makeMovement(const Vector2i &from, const Vector2i &to) {
    vector<Piece *>& currentCase = board[from.x][from.y];
    vector<Piece *>& nextCase = board[to.x][to.y];
    lastDeploymentPiece = nullptr;

    // if the move eats enemy pieces we delete each piece on the case
    if (isLandedCaseEnnemy(from, to)) {
        for (Piece *p : nextCase) {
            delete p;
            p = nullptr;
        }
        nextCase.clear();
    }

    // each piece from the original case are moved to the new case
    for (int i = currentCase.size() - 1; i >= 0; --i) {
        nextCase.push_back(currentCase.at(i));
        currentCase.at(i)->movePiece(to);
    }
    currentCase.clear();
}

void GounkiBoard::makeDeployment(int playerIndex, const Vector2i &from, const Vector2i &to) {
    vector<Piece *>& currentCase = board[from.x][from.y];
    vector<Piece *>& nextCase = board[to.x][to.y];

    GounkiPieceType movingPieceType = determinePieceType(playerIndex, from, to);
    // we search for a piece with the same type as the last deployment piece
    vector<Piece *>::iterator it = find_if(currentCase.begin(),currentCase.end(),[this, movingPieceType](Piece *piece) {
        return ((GounkiPiece *) piece)->type == movingPieceType && piece != lastDeploymentPiece;
    });
    GounkiPiece *newDeploymentPiece = (GounkiPiece *) *it;
    // first step of a deployment: every piece is moved to the next case
    // and every piece is pushed to the current deployment vector
    if (lastDeploymentPiece == nullptr) {
        for (int i = currentCase.size() - 1; i >= 0; --i) {
            nextCase.push_back(currentCase.at(i));
            currentDeployment.push_back((GounkiPiece *) currentCase.at(i));
            currentCase.at(i)->movePiece(to);
        }
        currentCase.clear();
    }
    else {
        // in this case we move all the pieces from the current case to the next case
        // except for the piece that we moved during the previous state of deployment
        vector<Piece*> toMove;
        for (GounkiPiece *gp: currentDeployment) {
            if (gp != lastDeploymentPiece) {
                nextCase.push_back(gp);
                toMove.push_back(gp);
            }
        }
        // we first push to the next case and then erase from the current case to not mess with iterators
        currentCase.erase(
                remove_if(currentCase.begin(), currentCase.end(),[&toMove](const Piece *p) {
                    return find(toMove.begin(), toMove.end(),p) != toMove.end();
                }), currentCase.end());
        for (Piece* p : toMove) {
            p->movePiece(to);
        }
    }
    // remove from the current deployment vector the piece we just moved
    if (currentDeployment.size() == 1)
        currentDeployment.clear();
    else
        currentDeployment.erase(find(currentDeployment.begin(), currentDeployment.end(),newDeploymentPiece));
    lastDeploymentPiece = newDeploymentPiece;
    lastDeploymentDirection = to - from;
}

void GounkiBoard::makeMove(ActionKey action, int playerIndex, const Vector2i &from, const Vector2i &to) {
    if (isWinningPosition(to)) {
        gameDone = true;
        winningPlayer = playerIndex;
        return;
    }

    switch (action) {
        case ActionKey::LeftClick:
            makeMovement(from, to);
            break;
        case ActionKey::RightClick:
            makeDeployment(playerIndex, from, to);
            break;
    }
}

bool isWithinBoundsX(int x) {
    return (x >= 0 && x < GOUNKI_BOARD_SIZE);
}

bool GounkiBoard::isWithinBounds(Vector2i pos) const {
    return (isWithinBoundsX(pos.x) && isWithinBoundsX(pos.y));
}

Vector2i GounkiBoard::handleRebounds(const Vector2i& from, Vector2i finalPos) const {
    Vector2i newDir = from;
    // to calculate rebounds, we check if the landing position is not within bounds
    // in this case we transfrom the coordinate to be the symmetry in x and/or y
    if (!isWithinBounds(finalPos)) {
        if (!isWithinBoundsX(finalPos.x)) {
            newDir.x = -newDir.x;
        }
        int j = 0;
        while (!isWithinBounds(finalPos)) {
            finalPos -= from;
            j++;
        }
        while (j != 0) {
            finalPos += newDir;
            j--;
        }
    }
    return finalPos;
}

map<GounkiPieceType, int> GounkiBoard::calculatePieceDistribution(ActionKey action, const Vector2i &from) const {
    map<GounkiPieceType, int> stackCount;
    const vector<Piece *>& pieces = board[from.x][from.y];

    for (Piece* piece : pieces) {
        if (action == ActionKey::LeftClick) {
            stackCount[((GounkiPiece *) (piece))->type]++;
        }
        // we only generate moves for pieces that are currently deploying
        // this prevents generating moves for pieces stacking during deployment
        else if (action == ActionKey::RightClick) {
            if (currentDeployment.size() == 0
            || find(currentDeployment.begin(), currentDeployment.end(), piece) != currentDeployment.end()) {
                stackCount[((GounkiPiece *) (piece))->type]++;
            }
        }
    }
    return stackCount;
}

bool GounkiBoard::isWinningPosition(const Vector2i& position) const {
    return (position.y == GOUNKI_BOARD_SIZE || position.y == -1) && isWithinBoundsX(position.x);
}

bool GounkiBoard::isNextCaseTakeable(ActionKey action, const Vector2i &from, const Vector2i &to) const {
    bool isCaseEnnemy = isLandedCaseEnnemy(from, to);
    size_t currentCaseSize = board[from.x][from.y].size();
    size_t nextCaseSize = board[to.x][to.y].size();

    // if classic movement or deployement, the case can't be full
    bool isEnnemyCaseFull = (!isCaseEnnemy &&
            ((action == ActionKey::LeftClick && (currentCaseSize + nextCaseSize) <= 3)
            || (action == ActionKey::RightClick && nextCaseSize <= 3)));
    // if classic movement and the case is an ennemy case
    bool isNextCaseTakeable = isCaseEnnemy && action == ActionKey::LeftClick;

    return isEnnemyCaseFull || isNextCaseTakeable;
}

int setMaximumRange(ActionKey action, int defaultRange) {
    // we don't generate positions of distance more than 1 case away for deployments
    if (action == ActionKey::RightClick) {
        return 1;
    }
    return defaultRange;
}

vector<pair<Vector2i, Vector2i>> GounkiBoard::validMovesPattern(ActionKey action, const Vector2i &from) const {
    vector<pair<Vector2i, Vector2i>> moves;
    if (board[from.x][from.y].empty())
        return moves;

    int range;
    vector<size_t> blockingPatterns;
    // this maps holds each piece type and it's number of occurences of the original case
    map<GounkiPieceType, int> stackCount = calculatePieceDistribution(action, from);
    // for each piece that can be moved, we generate the positions based on the quantity of the piece type on the case
    for (const pair<const GounkiPieceType, int>& pairs: stackCount) {
        // possible moves from the piece type
        vector<Vector2i> stackPieceMoves = GounkiPiece::gounkiMovements.at(pairs.first);
        // if the action is a deployment, we limit the distance of movement to 1
        range = setMaximumRange(action, pairs.second);
        for (int i = 1; i <= range; i++) {
            for (size_t j = 0; j < stackPieceMoves.size(); j++) {
                Vector2i finalPos = from + (stackPieceMoves[j] * i);
                // when the calculated position is a winning case
                // we stop generating values in this direction
                if (finalPos.y >= GOUNKI_BOARD_SIZE || finalPos.y <= -1) {
                    if (isWinningPosition(finalPos))
                        moves.emplace_back(finalPos, stackPieceMoves[j]);
                    blockingPatterns.push_back(j);
                }
                else {
                    finalPos = handleRebounds(stackPieceMoves[j], finalPos);
                    if (isNextCaseTakeable(action, from, finalPos)) {
                        moves.emplace_back(finalPos, stackPieceMoves[j]);
                    }
                    // if the move calculated position contains a piece on the board
                    // we stop generating in this direction
                    // we don't add blocking patterns if this is the maximum attainable case
                    if (i != range && !board[finalPos.x][finalPos.y].empty()) {
                        blockingPatterns.push_back(j);
                    }
                }
            }
            sort(blockingPatterns.begin(), blockingPatterns.end(), std::greater<size_t>());

            for (const size_t& index: blockingPatterns) {
                if (index < stackPieceMoves.size()) {
                    stackPieceMoves.erase(stackPieceMoves.begin() + index);
                }
            }
            blockingPatterns.clear();
        }
    }
    return moves;
}

auto extractFirstElement = [](const pair<Vector2i, Vector2i>& pair) {
    return pair.first;
};

/**
 * Stores inside moves the transformed vector of pair to a simple vector with only the first element of each pair
 * @param moves             the vector to store values in
 * @param movesWithPattern  the vector of positions and patterns
 */
void extractMoves(vector<Vector2i>& moves, const vector<pair<Vector2i, Vector2i>>& movesWithPattern) {
    moves.clear();
    moves.resize(movesWithPattern.size());

    transform(movesWithPattern.begin(), movesWithPattern.end(), moves.begin(), extractFirstElement);
}

/**
 * Filters the vector of pair<movement, pattern direction> to only keep a vector of unique movements
 * @param movesWithPattern
 */
void filterUniqueFirst(vector<pair<Vector2i, Vector2i>>& movesWithPattern) {
    sort(movesWithPattern.begin(), movesWithPattern.end(),
         [](const pair<Vector2i, Vector2i> &a, const pair<Vector2i, Vector2i> &b) {
        return a.first.x < b.first.x || (a.first.x == b.first.x && a.first.y < b.first.y);
    });

    movesWithPattern.erase(
            unique(movesWithPattern.begin(), movesWithPattern.end(),[](const pair<Vector2i, Vector2i> &a, const pair<Vector2i, Vector2i> &b) {
                return a.first.x == b.first.x && a.first.y == b.first.y;
            }),movesWithPattern.end()
            );
}

bool GounkiBoard::isOriginCaseMine(int playerIndex, const Vector2i& from) const {
    const vector<Piece*>& vec = board[from.x][from.y];
    if (vec.empty()) {
        return false;
    }
    switch (((GounkiPiece*)vec[0])->type) {
        case GounkiPieceType::BlackSquare:
        case GounkiPieceType::BlackCircle:
            if (playerIndex != 0)
                return false;
            return true;
        case GounkiPieceType::WhiteSquare:
        case GounkiPieceType::WhiteCircle:
            if (playerIndex != 1)
                return false;
            return true;
        default:
            return false;
    }
}

vector<Vector2i> GounkiBoard::validMoves(ActionKey action, int playerIndex, const Vector2i& from) const {
    vector<Vector2i> moves;
    if(!isOriginCaseMine(playerIndex, from))
        return moves;
    vector<pair<Vector2i, Vector2i>> movesWithPattern = validMovesPattern(action, from);
    switch (action) {
        // handles the calculation of a piece's move
        case ActionKey::LeftClick:
            filterUniqueFirst(movesWithPattern);
            extractMoves(moves, movesWithPattern);
            return moves;
        // handles the calculation of a piece's deployment
        case ActionKey::RightClick:
            // deployment is impossible if there's less than 2 pieces on the case
            if (board[from.x][from.y].size() < 2)
                return moves;

            // we search for every calculated moves that correspond to the last deployment's pattern
            // remove_if will rearrange the vector and put newEnd to the last element to delete if he finds at least on element
            vector<pair<Vector2i, Vector2i>>::iterator newEnd = remove_if(
                    movesWithPattern.begin(), movesWithPattern.end(),[this](const pair<Vector2i, Vector2i> &move) {
                return move.second != lastDeploymentDirection;
            });
            // if no moves correspond to the last deployment direction
            // we return the list of all the moves possible
            // this handles the case of a first deployment and
            // the case were no more pieces of the same type needed are available
            if (newEnd != movesWithPattern.begin())
                movesWithPattern.erase(newEnd, movesWithPattern.end());
            filterUniqueFirst(movesWithPattern);
            extractMoves(moves, movesWithPattern);
            return moves;
    }
    return moves;
}

void GounkiBoard::deleteRemainingDeploymentPieces(const Vector2i &pos) {
    vector<Piece*>& pieces = board[pos.x][pos.y];

    for (GounkiPiece* p : currentDeployment) {
        vector<Piece*>::iterator it = find(pieces.begin(), pieces.end(), p);
        pieces.erase(it);
        delete p;
    }
}

void GounkiBoard::clearDeploymentStatus() {
    lastDeploymentDirection = Vector2i {0, 0};
    lastDeploymentPiece = nullptr;
    currentDeployment.clear();
}

size_t GounkiBoard::currentDeploymentSize() const {
    return currentDeployment.size();
}

int GounkiBoard::getWinnerIndex() const {
    return winningPlayer;
}

int GounkiBoard::getCaseSize(const Vector2i &pos) const {
    return board[pos.x][pos.y].size();
}

GounkiBoard::~GounkiBoard() {
    delete lastDeploymentPiece;
    for (GounkiPiece* gp : currentDeployment) {
        delete gp;
    }
    cout << "Destruction of " << *this;
}

ostream &operator<<(ostream &out, const GounkiBoard &) {
    out << "Board: Gounki" << endl;
    return out;
}