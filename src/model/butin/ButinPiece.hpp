#ifndef BUTINPIECE_HPP
#define BUTINPIECE_HPP

#include "../Piece.hpp"
#include "../../utilities/Utilities.hpp"

class ButinPiece : public Piece {
public:
    /**
     * Constructor of a Butin piece
     * @param color     the enum of the piece's color
     * @param position  the position of the piece on the board
     */
    ButinPiece(ButinPieceType color, Vector2i position);
    virtual ~ButinPiece();
    /**
     * Copies and assignments are not possible
     */
    ButinPiece(const ButinPiece &) = delete;
    ButinPiece& operator=(const ButinPiece&) = delete;

    /**
     * The value associated to the piece.
     * Yellow: 1, Red: 2, Black: 3
     */
    const int value;

    /**
     * The color of the piece
     */
    const ButinPieceType color;

    /**
     * @see Piece::movePiece()
     */
    virtual void movePiece(Vector2i position);

    /**
     * @see Piece::getMovementPatterns()
     */
    virtual vector<Vector2i> getMovementPatterns() const;

    /**
     * @see Piece::getPosition()
     */
    virtual const Vector2i& getPosition() const;

    friend ostream &operator<<(ostream &, const ButinPiece &);
};

#endif
