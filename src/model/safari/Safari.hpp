#ifndef SAFARI_HPP
#define SAFARI_HPP

#include "../Game.hpp"
#include "SafariConfig.hpp"
#include "SafariBoard.hpp"

class Safari : public Game {
private:
    int currentStep;
    void updateScores();
public:
    Safari();
    virtual ~Safari();
    /**
     * Copies and assignments are not possible
     */
    Safari(const Safari &) = delete;
    Safari& operator=(const Safari&) = delete;

    /**
     * @see Game::isGameDone()
     */
    virtual bool isGameDone() const;

    /**
     * @see Game::getWinner()
     */
    virtual string getWinner() const;

    /**
     * Safari needs an overriden initilization since the players' scores are set to the number of animals at the beginning of the game
     * @param playerNames   names of the players
     */
    void initPlayers(vector<string> playerNames) override;

    /**
     * @see Game::initializeGame()
     */
    virtual void initializeGame(const GameConfig &);

    /**
     * @see Game::makeMove()
     */
    virtual void makeMove(ActionKey action, const Vector2i& from, const Vector2i& to = Vector2i(0, 0));

    /**
     * @see Game::getMinMaxPlayers()
     */
    virtual pair<int, int> getMinMaxPlayers() const;

    friend ostream &operator<<(ostream &, const Safari &);
};

#endif
