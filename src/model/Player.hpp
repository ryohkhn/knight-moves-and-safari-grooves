#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <iostream>
#include "../utilities/Utilities.hpp"
using namespace std;

class Player {
private:
    int score;
public:
    /**
     * Constructor of a player
     * @param name  the name of the player
     */
    Player(string name);
    virtual ~Player();
    /**
     * Copies and assignments are not possible
     */
    Player(const Player &) = delete;
    Player& operator=(const Player&) = delete;

    /**
     * Name of the player
     */
    const string name;

    /**
     * Increases the score of the player
     * @param amount    amount to increase
     */
    void increaseScore(int amount);

    /**
     * Sets the score of a player
     * @param newScore  new score of the player
     */
    void setScore(int newScore);

    /**
     * Returns the current score of the player
     * @return  the score
     */
    int getScore() const;

    friend ostream &operator<<(ostream &, const Player &);
};

#endif