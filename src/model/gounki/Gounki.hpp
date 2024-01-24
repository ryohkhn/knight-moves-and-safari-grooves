#ifndef GOUNKI_HPP
#define GOUNKI_HPP

#include "../Game.hpp"
#include "GounkiBoard.hpp"

class Gounki : public Game {
public:
    Gounki();
    virtual ~Gounki();
    /**
     * Copies and assignments are not possible
     */
    Gounki(const Gounki &) = delete;
    Gounki& operator=(const Gounki&) = delete;

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
     *
     */
    virtual void initializeGame(const GameConfig &);

    /**
     * @see Game::makeMove()
     */
    virtual void makeMove(ActionKey action, const Vector2i& from, const Vector2i& to);

    /**
     * @see Game::getMinMaxPlayers()
     */
    virtual pair<int, int> getMinMaxPlayers() const;

    friend ostream &operator<<(ostream &, const Gounki &);
};

#endif
