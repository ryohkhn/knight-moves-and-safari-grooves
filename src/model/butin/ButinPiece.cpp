#include "ButinPiece.hpp"

ButinPiece::ButinPiece(ButinPieceType color, Vector2i position):value {UtilityFunctions::getValueFromColor(color)}, color {color} {
    this->position = position;
    cout << "Construction of " << *this;
}

void ButinPiece::movePiece(Vector2i position) {
    this->position = position;
}

vector<Vector2i> ButinPiece::getMovementPatterns() const {
    vector<Vector2i> patterns;

    // diagonal jumps
    patterns.push_back(Vector2i(2, 2));
    patterns.push_back(Vector2i(2, -2));
    patterns.push_back(Vector2i(-2, 2));
    patterns.push_back(Vector2i(-2, -2));

    // straight jumps
    patterns.push_back(Vector2i(0, 2));
    patterns.push_back(Vector2i(0, -2));
    patterns.push_back(Vector2i(2, 0));
    patterns.push_back(Vector2i(-2, 0));
    return patterns;
}

const Vector2i& ButinPiece::getPosition() const {
    return this->position;
}

ButinPiece::~ButinPiece() {
    cout << "Destruction of " << *this;
}

ostream &operator<<(ostream &o, const ButinPiece &bp) {
    o << "piece: Butin" << endl << "\t Value: " << bp.value << endl;
    return o;
}