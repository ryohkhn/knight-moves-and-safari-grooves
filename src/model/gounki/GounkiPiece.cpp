#include "GounkiPiece.hpp"

GounkiPiece::GounkiPiece(GounkiPieceType type, Vector2i position): type {type} {
    this->position = position;
    cout << "Construction of " << *this;
}

const map<GounkiPieceType, std::vector<Vector2i>> GounkiPiece::gounkiMovements = {
        { GounkiPieceType::BlackCircle, {Vector2i(-1, 1), Vector2i(1, 1)}},
        { GounkiPieceType::WhiteCircle, {Vector2i(-1, -1), Vector2i(1, -1)}},
        { GounkiPieceType::BlackSquare, {Vector2i(-1, 0), Vector2i(0, 1), Vector2i(1, 0)}},
        { GounkiPieceType::WhiteSquare, {Vector2i(-1, 0), Vector2i(0, -1), Vector2i(1, 0)}},
        { GounkiPieceType::EmptyGounki, {}}
};

const map<GounkiPieceType, std::vector<Vector2i>> GounkiPiece::gounkiWhiteMovements= {
        { GounkiPieceType::WhiteCircle, {Vector2i(-1, -1), Vector2i(1, -1)}},
        { GounkiPieceType::WhiteSquare, {Vector2i(-1, 0), Vector2i(0, -1), Vector2i(1, 0)}},
};

const map<GounkiPieceType, std::vector<Vector2i>> GounkiPiece::gounkiBlackMovements= {
        { GounkiPieceType::BlackCircle, {Vector2i(-1, 1), Vector2i(1, 1)}},
        { GounkiPieceType::BlackSquare, {Vector2i(-1, 0), Vector2i(0, 1), Vector2i(1, 0)}},
};

void GounkiPiece::movePiece(Vector2i position) {
    this->position = position;
}

const map<GounkiPieceType, vector<Vector2i>> & GounkiPiece::getMovesForPlayer(int playerIndex) {
    switch (playerIndex) {
        case 0:
            return gounkiBlackMovements;
        case 1:
            return gounkiWhiteMovements;
        default:
            return gounkiMovements;
    }
}

vector<Vector2i> GounkiPiece::getMovementPatterns() const {
    return gounkiMovements.at(type);
}

const Vector2i& GounkiPiece::getPosition() const {
    return this->position;
}

GounkiPiece::~GounkiPiece() {
    cout << "Destruction of " << *this;
}

ostream &operator<<(ostream &out, const GounkiPiece &g) {
    out << "Piece: Gounki" << endl << "\t Type: " << UtilityFunctions::getGounkiPieceString(g.type) << endl;
    return out;
}
