#ifndef GOUNKIPIECE_HPP
#define GOUNKIPIECE_HPP

#include <map>

#include "../Piece.hpp"
#include "../../utilities/Utilities.hpp"

class GounkiPiece : public Piece {
public:
    /**
     * Constructor of a Gounki piece
     * @param type      the enum of the piece's type
     * @param position  the position of the piece on the board
     */
    GounkiPiece(GounkiPieceType type, Vector2i position);
    virtual ~GounkiPiece();
    /**
     * Copies and assignments are not possible
     */
    GounkiPiece(const GounkiPiece &) = delete;
    GounkiPiece& operator=(const GounkiPiece&) = delete;

    /**
     * The type of the piece
     */
    const GounkiPieceType type;

    /**
     * A static map with each Butin piece type and his possible movements
     */
    static const map<GounkiPieceType, vector<Vector2i>> gounkiMovements;

    /**
     * A static map with the white and black movements, and a function to get them by player index
     */

    static const map<GounkiPieceType, vector<Vector2i>> gounkiWhiteMovements;

    static const map<GounkiPieceType, vector<Vector2i>> gounkiBlackMovements;

    static const map<GounkiPieceType, vector<Vector2i>>& getMovesForPlayer(int playerIndex);


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

    friend ostream &operator<<(ostream &, const GounkiPiece &);
};

#endif
