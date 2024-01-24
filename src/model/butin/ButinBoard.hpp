#ifndef BUTINBOARD_HPP
#define BUTINBOARD_HPP

#include <vector>
#include <array>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <algorithm>

#include "ButinPiece.hpp"
#include "../Board.hpp"
#include "ButinConfig.hpp"
#include "../../settings/SETTINGS.hpp"

class ButinBoard : public Board {
private:
    /**
     * Generates the default board for the game "Butin"
     */
    void generateDefaultBoard();

    /**
     * Calcultes the position of the jumped case when a yellow piece moves from position `from` to position `to`
     * @param from  origin position
     * @param to    landing position
     * @return      the position of the jumped case
     */
    Vector2i calculateJumpedPos(const Vector2i &from, const Vector2i &to) const;
public:
    ButinBoard();
    virtual ~ButinBoard();
    /**
     * Copies and assignments are not possible
     */
    ButinBoard(const ButinBoard &) = delete;
    ButinBoard& operator=(const ButinBoard&) = delete;

    /**
     * @see Board::isGameDone()
     */
    virtual bool isGameDone() const;

    /**
     * @see Board::initializeGame()
     */
    virtual void initializeGame(const GameConfig&);

    /**
     * @see Board::getboardState()
     */
    virtual vector<vector<vector<string>>> getBoardState() const;

    /**
     * @see Board::makeMove()
     */
    virtual void makeMove(ActionKey action, int playerIndex, const Vector2i& from, const Vector2i& to);

    /**
     * @see Board::validMoves()
     */
    virtual vector<Vector2i> validMoves(ActionKey action, int playerIndex, const Vector2i& from) const;

    /**
     * @see Board::isWithinBounds()
     */
    virtual bool isWithinBounds(Vector2i pos) const;

    /**
     * Returns the enum corresponding to the jumped piece type
     * @param from  origin position
     * @param to    landing position
     * @return      the enum of the pieces' type
     */
    ButinPieceType getJumpedPieceType(const Vector2i &from, const Vector2i &to) const;

    /**
     * Returns the sum of the pieces' value currently on the board
     * @return  the total points
     */
    int getBoardTotalPoints() const;

    friend ostream &operator<<(ostream &, const ButinBoard &);
};

#endif
