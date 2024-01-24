#ifndef GOUNKIBOARD_HPP
#define GOUNKIBOARD_HPP

#include <iostream>
#include <map>
#include <algorithm>

#include "../Board.hpp"
#include "GounkiPiece.hpp"

class GounkiBoard : public Board {
private:
    /**
     * Direction of the last deployement stage
     */
    Vector2i lastDeploymentDirection;

    /**
     * Pointer to the last deployed piece
     */
    GounkiPiece* lastDeploymentPiece;

    /**
     * A vector storing all the pieces currently deploying
     */
    vector<GounkiPiece*> currentDeployment;

    bool gameDone;

    /**
     * Index of the player that won the game
     */
    int winningPlayer;

    /**
     * Generates the default board for the game "Gounki"
     */
    void generateDefaultBoard();

    /**
     * Verifies that the case `from` has at least a piece of the current player on it
     * @param playerIndex   current playerindex
     * @param from          origin vector
     * @return              true if the case is one of the player, else false
     */
    bool isOriginCaseMine(int playerIndex, const Vector2i& from) const;

    /**
     * Returns a vector of the valid moves associated with what direction the piece needs to take to land at the position
     * @param action    type of action
     * @param from      origin vector
     * @return          a vector of pair<landing position, direction pattern>
     */
    vector<pair<Vector2i, Vector2i>> validMovesPattern(ActionKey action, const Vector2i& from) const;

    /**
     * Returns the enum of the piece that matches the movement `from` -> `to`
     * @param from  origin vector
     * @param to    landing vector
     * @return      the matching piece type
     */
    GounkiPieceType determinePieceType(int playerIndex, const Vector2i& from, const Vector2i& to) const;

    /**
     * Calculates if the provided vector will encounter a wall rebound during the movement `from` -> `finalPos`
     * @param from      origin vector
     * @param finalPos  landing vector
     * @return          the right position if a wall is in the way, else the position won't change
     */
    Vector2i handleRebounds(const Vector2i& from, Vector2i finalPos) const;

    /**
     * Returns a map with the quantity of each piece type on the case.
     * If the action is a deployement we don't count pieces that are not part of the current deployement
     * @param action    normal movement or a deployement
     * @param from      origin vector
     * @return          a map with each piece type and the associated quantity
     */
    map<GounkiPieceType, int> calculatePieceDistribution(ActionKey action, const Vector2i &from) const;

    /**
     * Verifies that the provided is a winning position.
     * Above the top of the board, or below the bottom of the board
     * @param position  the position of the case
     * @return          true if it's a winning position, else false
     */
    bool isWinningPosition(const Vector2i& position) const;

    /**
     * Verifies depending on the action type if the case is a case the piece can land on.
     * For a classic movement, the case needs to be empty, not full of friend pieces or be an ennemy case.
     * For a deployement, the case needs to be empty or not full of friend pieces
     * @param action    normal movement or a deployement
     * @param from      origin vector
     * @param to        landing vector
     * @return          true if the case is takeable, else false
     */
    bool isNextCaseTakeable(ActionKey action, const Vector2i &from, const Vector2i &to) const;

    /**
     * Initiates the movement from the case `from` to the case `to`
     * @param from      origin vector
     * @param to        landing vector
     */
    void makeMovement(const Vector2i &from, const Vector2i &to);

    /**
     * Initiates or continues the deployment from the case `from` to the case `to`
     * @param from      origin vector
     * @param to        landing vector
     */
    void makeDeployment(int playerIndex, const Vector2i &from, const Vector2i &to);
public:
    GounkiBoard();
    virtual ~GounkiBoard();
    /**
     * Copies and assignments are not possible
     */
    GounkiBoard(const GounkiBoard &) = delete;
    GounkiBoard& operator=(const GounkiBoard&) = delete;

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
    virtual void makeMove(ActionKey action, int playerIndex, const Vector2i& from, const Vector2i& to);

    /**
     * @see Board::validMoves()
     */
    virtual vector<Vector2i> validMoves(ActionKey action, int playerIndex, const Vector2i& from) const;

    /**
     * @see Board::isWithinBounds()
     */
    bool isWithinBounds(Vector2i pos) const;

    /**
     * Returns weither the landing case is an enemy case
     * @param from  origin position
     * @param to    landing position
     * @return      true if the case has an ennemy piece on it, else false
     */
    bool isLandedCaseEnnemy(const Vector2i& from, const Vector2i& to) const;

    /**
     * Returns the number of elements that are still to be deployed
     * @return  the number of pieces not deployed
     */
    size_t currentDeploymentSize() const;

    /**
     * Returns index of the player who won the game
     * @return  the index of the list
     */
    int getWinnerIndex() const;

    /**
     * Returns the number of pieces on the case
     * @param pos   position of the case
     * @return      the quantity of pieces
     */
    int getCaseSize(const Vector2i& pos) const;

    /**
     * Deletes and clears the board at the provided position
     * @param pos   position to clear
     */
    void deleteRemainingDeploymentPieces(const Vector2i& pos);

    /**
     * Clears the data stored to keep track of a deployement
     */
    void clearDeploymentStatus();

    friend ostream &operator<<(ostream &, const GounkiBoard &);
};

#endif
