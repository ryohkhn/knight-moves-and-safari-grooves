#ifndef BUTINCONFIG_HPP
#define BUTINCONFIG_HPP

#include "../GameConfig.hpp"

class ButinConfig : public GameConfig {
public:
    /**
     * Constructor of the Butin configuration
     * @param deleted_pieces    the vector of the selected pieces
     */
    ButinConfig(const vector<Vector2i> deleted_pieces);
    virtual ~ButinConfig();
    /**
     * Constructor of a copy of ButinConfig
     */
    ButinConfig(const ButinConfig&);

    /**
     * The vector of the deleted pieces selected during the initialization of "Butin"
     */
    const vector<Vector2i> deleted_pieces;

    friend ostream &operator<<(ostream &, const ButinConfig &);
};

#endif
