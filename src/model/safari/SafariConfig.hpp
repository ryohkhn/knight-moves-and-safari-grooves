#ifndef SAFARICONFIG_HPP
#define SAFARICONFIG_HPP

#include "../GameConfig.hpp"

class SafariConfig : public GameConfig {
public:
    SafariConfig();
    virtual ~SafariConfig();
    /**
     * Constructor of a copy of SafariConfig
     */
    SafariConfig(const SafariConfig&);

    /**
     * Vectors of the placed pieces during the initialization of "Safari Round Up"
     */
    vector<Vector2i> crocodiles;
    vector<Vector2i> elephants;
    vector<Vector2i> lions;
    vector<Vector2i> fences;

    friend ostream &operator<<(ostream &, const SafariConfig &);
};

#endif
