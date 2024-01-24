#include "Board.hpp"

Board::~Board() {
    for (vector<vector<Piece*>>& fstVec: board) {
        for (vector<Piece*>& sndVec: fstVec) {
            for (Piece *piece: sndVec) {
                delete piece;
            }
            sndVec.clear();
        }
        fstVec.clear();
    }
    board.clear();
    cout << "Destruction of Board" << endl;
}