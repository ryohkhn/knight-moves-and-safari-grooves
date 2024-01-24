#include "SafariPiece.hpp"
#include "../../settings/SETTINGS.hpp"

SafariPiece::SafariPiece(SafariPieceType animal, Vector2i position): animal{animal} {
    this -> position = position;
}

void SafariPiece::movePiece(Vector2i position) {
    this -> position = position;
}

vector<Vector2i> SafariPiece::getMovementPatterns() const {
    vector<Vector2i> patterns;

    switch (animal) {
        case SafariPieceType::EmptySafari:
        case SafariPieceType::Fence:
            return patterns;
        default:
            // Move to the right
            for (int i = 0; i < SAFARI_BOARD_SIZE; i+=2) {
                patterns.push_back(Vector2i(i, 0));
            }
            // Move to the left
            for (int i = SAFARI_BOARD_SIZE - 1; i >= 0; i-=2) {
                patterns.push_back(Vector2i(-i, 0));
            }
            // Move to the top
            for (int i = 0; i < SAFARI_BOARD_SIZE; i+=2) {
                patterns.push_back(Vector2i(0, i));
            }
            // Move to the bottom
            for (int i = SAFARI_BOARD_SIZE - 1; i >= 0; i-=2) {
                patterns.push_back(Vector2i(0, -i));
            }
            break;
    }
    return patterns;
}

const Vector2i& SafariPiece::getPosition() const {
    return this -> position;
}

SafariPiece::~SafariPiece() {
    cout << "Destruction of " << *this;
}

ostream &operator<<(ostream &out, const SafariPiece &sp) {
    out << "piece: Safari" << endl << "\t Animal: " << UtilityFunctions::getSafariPieceString(sp.animal) << endl;
    return out;
}