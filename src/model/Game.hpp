#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <array>
#include <iostream>
using namespace std;

#include "Player.hpp"
#include "Board.hpp"
#include "GameConfig.hpp"

/**
 * This abstract class is the main class and stores all the logic of a game
 */
class Game {
protected:
    Board* board;
    vector<Player*> playerList;
    bool gameStarted = false;
    int currentPlayerIndex = 0;
public:
    /**
     * The game destructor will delete the board and the vector of players
     */
    virtual ~Game();

     /**
     * Pure virtual function verifying that the game is done
     * @return  true if done, else false
     */
    virtual bool isGameDone() const = 0;

    /**
     * Pure virtual function returning the winner of the game
     * @return  the name of the winner
     */
    virtual string getWinner() const = 0;

    /**
     * Pure virtual function initializing the Game object with the provided abstact GameConfig
     * @param gameConfig    abstract game configuration object
     */
    virtual void initializeGame(const GameConfig&) = 0;

    /**
     * Pure virtual function moving a piece from the vector `from` to the vector `to`.
     * No verification will be made, the vectors `from` and `to` should match what the `validMoves` function generates
     * @param action        type of action
     * @param from          origin vector
     * @param to            landing vector
     */
    virtual void makeMove(ActionKey action, const Vector2i& from, const Vector2i& to) = 0;

    /**
     * Pure virtual function returning the minimun and maximum possible players for the game
     * @return  a pair of <minimum, maximum>
     */
    virtual pair<int, int> getMinMaxPlayers() const = 0;

    /**
     * Returns weither the game already started
     * @return  true if the game started, else false
     */
    virtual bool hasGameStarted() const;

    /**
     * Initializes the vector of players
     * @param playerNames   a vector of the players' names
     */
    virtual void initPlayers(vector<string> playerNames);

    /**
     * Generates all the valid moves of the piece on case `from`
     * @param action    type of action
     * @param from      origin vector
     * @return          a vector of the possible positions
     */
    virtual vector<Vector2i> validMoves(ActionKey action, const Vector2i& from) const;

    /**
     * Generates a vector of the board and each piece's name
     * @return  a vector of string
     */
    virtual vector<vector<vector<string>>> getBoardState() const;

    /**
     * Returns the current player index
     * @return  the index
     */
    virtual int getCurrentPlayerIndex() const;

    /**
     * Returns the names of the players
     * @return  a vector of string
     */
    virtual vector<string> getPlayerNames() const;
};

#endif
