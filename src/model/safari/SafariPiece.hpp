#ifndef SAFARIPIECE_HPP
#define SAFARIPIECE_HPP

#include "../Piece.hpp"
#include "../../utilities/Utilities.hpp"

class SafariPiece : public Piece {
public:
    /**
     * Constructor of a Safari piece
     * @param animal    the enum of the piece's animal
     * @param position  the position of the piece on the board
     */
    SafariPiece(SafariPieceType animal, Vector2i position);
    virtual ~SafariPiece();
    /**
     * Copies and assignments are not possible
     */
    SafariPiece(const SafariPiece &) = delete;
    SafariPiece& operator=(const SafariPiece&) = delete;

    /**
     * The type of the piece
     */
    const SafariPieceType animal;

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

    friend ostream &operator<<(ostream &, const SafariPiece &);
};

#endif
