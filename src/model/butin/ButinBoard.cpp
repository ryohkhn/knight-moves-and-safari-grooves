#include "ButinBoard.hpp"

ButinBoard::ButinBoard() {
    // Making the Board a BUTIN_BOARD_SIZE x BUTIN_BOARD_SIZE of nullptr
    board.resize(BUTIN_BOARD_SIZE);
    for (vector<vector<Piece *>> &column : board) {
        column.resize(BUTIN_BOARD_SIZE);
    }
    generateDefaultBoard();
    cout << "Construction of " << *this;
}

bool ButinBoard::isGameDone() const {
    // in this game the game is finished no matter wich player turn it is,
    // because none of the Pieces belong to a specific Player
    for (const vector<vector<Piece *>>& column : board) {
        for (const vector<Piece *>& line: column) {
            for (Piece *bp: line) {
                if (bp != nullptr) {
                    if (!validMoves(ActionKey::LeftClick, 0, bp -> getPosition()).empty()) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

vector<int> calculatePieceDistribution() {
    // default 8x8 ratio for the game Butin
    const int yellow_default_ratio = 34;
    const int red_default_ratio = 20;
    const int black_default_ratio = 10;
    double factor = (double) (BUTIN_BOARD_SIZE * BUTIN_BOARD_SIZE) / 64.0;

    int yellow_ratio = (int) (round(yellow_default_ratio * factor));
    int red_ratio = (int) (round(red_default_ratio * factor));
    int black_ratio = (int) (round(black_default_ratio * factor));
    return {yellow_ratio, red_ratio, black_ratio};
}

void ButinBoard::generateDefaultBoard() {
    vector<int> pieces_ratio = calculatePieceDistribution();
    vector<ButinPieceType> all_pieces;

    all_pieces.insert(all_pieces.end(), pieces_ratio[0], ButinPieceType::Yellow);
    all_pieces.insert(all_pieces.end(), pieces_ratio[1], ButinPieceType::Red);
    all_pieces.insert(all_pieces.end(), pieces_ratio[2], ButinPieceType::Black);

    srand(time(nullptr));
    random_shuffle(all_pieces.begin(), all_pieces.end());

    int count = 0;
    for (int i = 0; i < BUTIN_BOARD_SIZE; i++) {
        for (int j = 0; j < BUTIN_BOARD_SIZE; j++) {
            board[i][j].push_back(new ButinPiece(all_pieces[count++], Vector2i {i, j}));
        }
    }
}


void ButinBoard::initializeGame(const GameConfig& gc) {
    const ButinConfig& bc = (ButinConfig&) gc;
    // we remove and delete each piece removed during initialization
    for (const Vector2i& v: bc.deleted_pieces) {
        ButinPiece* tmp = (ButinPiece *) board[v.x][v.y][0];
        board[v.x][v.y][0] = nullptr;
        if (tmp != nullptr)
            delete tmp;
    }
}

vector<vector<vector<string>>> ButinBoard::getBoardState() const {
    // initialize a vector of BUTIN_BOARD_SIZE elements of vector<vector<string>> initalized at BUTIN_BOARD_SIZE
    vector<vector<vector<string>>> boardState(BUTIN_BOARD_SIZE, vector<vector<string>>(BUTIN_BOARD_SIZE));
    for (int i = 0; i < BUTIN_BOARD_SIZE; i++) {
        for (int j = 0; j < BUTIN_BOARD_SIZE; j++) {
            string s;
            if (board[i][j][0] == nullptr) {
                s = UtilityFunctions::getButinPieceString(ButinPieceType::EmptyButin);
            }
            else {
                s = UtilityFunctions::getButinPieceString(((ButinPiece*) board[i][j][0])->color);
            }
            boardState[i][j].push_back(s);
        }
    }
    return boardState;
}

Vector2i ButinBoard::calculateJumpedPos(const Vector2i &from, const Vector2i &to) const {
    return Vector2i((from.x + to.x) / 2, (from.y + to.y) / 2);
}

void ButinBoard::makeMove(ActionKey action, int playerIndex, const Vector2i &from, const Vector2i &to) {
    // no matter if it is a Left or Right-click the action stays the same for Butin
    const Vector2i jumpedPos = calculateJumpedPos(from, to);
    ButinPiece* jumpedPiece = (ButinPiece*) board[jumpedPos.x][jumpedPos.y][0];
    ButinPiece* toPiece = (ButinPiece*) board[to.x][to.y][0];

    board[to.x][to.y] = board[from.x][from.y];
    board[from.x][from.y][0] -> movePiece(to);
    board[from.x][from.y][0] = nullptr;
    board[jumpedPos.x][jumpedPos.y][0] = nullptr;
    delete jumpedPiece;
    delete toPiece;
}

ButinPieceType ButinBoard::getJumpedPieceType(const Vector2i &from, const Vector2i &to) const{
    const Vector2i jumpedPos = calculateJumpedPos(from, to);
    return ((ButinPiece*) board[jumpedPos.x][jumpedPos.y][0])->color;
}

bool ButinBoard::isWithinBounds(Vector2i pos) const {
    return (pos.x >= 0 && pos.x < BUTIN_BOARD_SIZE && pos.y >= 0 && pos.y < BUTIN_BOARD_SIZE);
}

vector<Vector2i> ButinBoard::validMoves(ActionKey action, int playerIndex, const Vector2i &from) const {
    vector<Vector2i> moves;
    ButinPiece* piece = (ButinPiece *)board[from.x][from.y][0];

    // piece that aren't yellow can't move
    if (piece == nullptr || piece->color != ButinPieceType::Yellow)
        return moves;

    vector<Vector2i> patterns = piece->getMovementPatterns();
    for (const Vector2i &pattern : patterns) {
        // position of the jumped piece
        Vector2i midPos = from + (pattern / 2);
        Vector2i endPos = from + pattern;

        if (isWithinBounds(midPos) && isWithinBounds(endPos)) {
            // checks that the jumped piece exists and that the end position is empty
            if (board[midPos.x][midPos.y][0] != nullptr && board[endPos.x][endPos.y][0] == nullptr)
                moves.push_back(endPos);
        }
    }
    return moves;
}

int ButinBoard::getBoardTotalPoints() const {
    int sum = 0;
    for (const vector<vector<Piece *>>& column : board) {
        for (const vector<Piece *>& row : column) {
            for (Piece* p : row) {
                if (p != nullptr) {
                    sum += ((ButinPiece *) p) -> value;
                }
            }
        }
    }
    return sum;
}

ButinBoard::~ButinBoard() {
    cout << "Destruction of " << *this;
}

ostream &operator<<(ostream &o, const ButinBoard &) {
    o << "Board: Butin" << endl;
    return o;
}



