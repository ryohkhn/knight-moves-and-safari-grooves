#ifndef BUTIN_HPP
#define BUTIN_HPP

#include "../Game.hpp"
#include "ButinBoard.hpp"
#include "ButinConfig.hpp"

class Butin : public Game {
public:
    Butin();
    virtual ~Butin();
    /**
     * Copies and assignments are not possible
     */
    Butin(const Butin &) = delete;
    Butin& operator=(const Butin&) = delete;

    /**
     * @see Game::isGameDone()
     */
    virtual bool isGameDone() const;

    /**
     * @see Game::getWinner()
     */
    virtual string getWinner() const;

    /**
     * @see Game::initializeGame()
     */
    virtual void initializeGame(const GameConfig &);

    /**
     * @see Game::makeMove()
     */
    virtual void makeMove(ActionKey action, const Vector2i& from, const Vector2i& to);

    /**
     * @see Game::getMinxMaxPlayers()
     */
    virtual pair<int, int> getMinMaxPlayers() const;

    /**
     * Returns the score of each player
     * @return  a vector of the scores, in the same order as the players' list
     */
    vector<string> getPlayerScores() const;

    friend ostream &operator<<(ostream &, const Butin &);
};

#endif
