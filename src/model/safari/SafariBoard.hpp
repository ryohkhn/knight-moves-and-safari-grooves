#ifndef SAFARIBOARD_HPP
#define SAFARIBOARD_HPP

#include <cmath>
#include <memory>

#include "../Board.hpp"
#include "SafariPiece.hpp"
#include "SafariConfig.hpp"

class SafariBoard : public Board {
private:
    const int tilesToCapture;
    bool isCaptured(SafariPiece*) const;
    int getAccessibleTiles(const Vector2i &from, const vector<Vector2i> &patterns, vector<vector<bool>>* mark) const;
    vector<Vector2i> getPositionFromPatterns(const Vector2i &from, const vector<Vector2i> &patterns) const;
public:
    SafariBoard();
    virtual ~ SafariBoard();
    /**
     * Copies and assignments are not possible
     */
    SafariBoard(const SafariBoard &) = delete;
    SafariBoard& operator=(const SafariBoard&) = delete;

    /**
     * @see Board::isGameDone()
     */
    virtual bool isGameDone() const;

    /**
     * @see Board::initializeGame()
     */
    virtual void initializeGame(const GameConfig&);

    /**
     * @see Board::getBoardState()
     */
    virtual vector<vector<vector<string>>> getBoardState() const;

    /**
     * @see Board::makeMove()
     */
    virtual void makeMove(ActionKey action, int playerIndex, const Vector2i &from, const Vector2i &to);

    /**
     * @see Board::validMoves()
     */
    virtual vector<Vector2i> validMoves(ActionKey action, int playerIndex, const Vector2i &from) const;

    /**
     * @see Board::isWithinBounds()
     */
    bool isWithinBounds(Vector2i pos) const;

    int getCapturedPieces(int playerIndex) const;

    friend ostream &operator<<(ostream &, const SafariBoard &);
};

#endif
