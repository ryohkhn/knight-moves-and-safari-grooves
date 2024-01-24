#include "Player.hpp"

Player::Player(string name):score {0}, name{name} {
    cout << "Construction of " << *this;
}

void Player::increaseScore(int amount) {
    score += amount;
}

void Player::setScore(int newScore) {
    score = newScore;
}

int Player::getScore() const {
    return score;
}

Player::~Player() {
    cout << "Destruction of " << *this;
}

ostream &operator<<(ostream &o, const Player &p) {
    o << "Player: " << p.name << endl;
    return o;
}