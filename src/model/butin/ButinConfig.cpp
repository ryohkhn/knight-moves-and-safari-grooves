#include "ButinConfig.hpp"

ButinConfig::ButinConfig(const vector<Vector2i> deleted_pieces):deleted_pieces {deleted_pieces} {
    cout << "Construction of " << *this;
}

ButinConfig::ButinConfig(const ButinConfig &old) : deleted_pieces {old.deleted_pieces} {
    cout << "Copy of " << *this;
}

ButinConfig::~ButinConfig() {
    cout << "Destruction of " << *this;
}

ostream &operator<<(ostream &o, const ButinConfig &) {
    o << "Config: Butin" << endl;
    return o;
}
