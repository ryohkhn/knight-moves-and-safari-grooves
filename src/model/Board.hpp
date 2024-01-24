#ifndef BOARD_HPP
#define BOARD_HPP

#include <iostream>
#include <SFML/System/Vector2.hpp>
using namespace sf;

#include "../utilities/Utilities.hpp"
#include "Piece.hpp"
#include "GameConfig.hpp"
#include "../settings/SETTINGS.hpp"

/**
 * This abstract class represents the board of a game
 */
class Board {
protected:
    /**
     * This vector of vector represents the board where each case can store an arbitrary number of pieces
     */
    vector<vector<vector<Piece*>>> board;

    /**
     * Function verifying that the provided Vector is within bounds of the board size
     * @param pos   vector to verify
     * @return      true if within bounds, else false
     */
    virtual bool isWithinBounds(Vector2i pos) const = 0;
public:
    /**
     * The board destructor will delete each piece of the vector
     */
    virtual ~Board();

    /**
     * Pure virtual function verifying that the game is done
     * @return  true if done, else false
     */
    virtual bool isGameDone() const = 0;

    /**
     * Pure virtual function initializing the Board object with the provided abstact GameConfig
     * @param gameConfig    abstract game configuration object
     */
    virtual void initializeGame(const GameConfig&) = 0;

    /**
     * Pure virtual function generating a vector of the board and each piece's name
     * @return  a vector of strings
     */
    virtual vector<vector<vector<string>>> getBoardState() const = 0;

    /**
     * Pure virtual function moving a piece from the vector `from` to the vector `to`.
     * No verification will be made, the vectors `from` and `to` should match what the `validMoves` function generates
     * @param action        type of action
     * @param playerIndex   index of the current player
     * @param from          origin vector
     * @param to            landing vector
     */
    virtual void makeMove(ActionKey action, int playerIndex, const Vector2i& from, const Vector2i& to) = 0;

    /**
     * Pure virtual function generating all the valid moves of the piece on case `from`
     * @param action        type of action
     * @param playerIndex   index of the current player
     * @param from          origin vector
     * @return              a vector of the possible positions
     */
    virtual vector<Vector2i> validMoves(ActionKey action, int playerIndex, const Vector2i& from) const = 0;
};

#endif
