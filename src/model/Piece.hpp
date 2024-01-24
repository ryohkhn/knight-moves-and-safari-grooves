#ifndef PIECE_HPP
#define PIECE_HPP

#include <utility>
#include <iostream>
#include <vector>
using namespace std;

#include <SFML/System/Vector2.hpp>
using namespace sf;

class Piece {
protected:
    /**
     * Position of the piece on the board
     */
    Vector2i position;
public:
    /**
     * Destructor of the interface
     */
    virtual ~Piece() {}

    /**
     * Pure virtual function changing the position of a piece
     * @param position  the new position
     */
    virtual void movePiece(Vector2i position) = 0;

    /**
     * Pure virtual function returning all the possible moving patterns of the piece
     * @return  a vector of position
     */
    virtual vector<Vector2i> getMovementPatterns() const = 0;

    /**
     * Pure virtual function returning the current position of a piece on the board
     * @return  a vector of position
     */
    virtual const Vector2i& getPosition() const = 0;
};

#endif