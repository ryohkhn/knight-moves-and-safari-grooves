#include "SafariConfig.hpp"

SafariConfig::SafariConfig() {
    cout << "Construction of " << *this;
}

SafariConfig::SafariConfig(const SafariConfig &old): crocodiles {old.crocodiles}, elephants {old.elephants}, lions {old.lions}, fences {old.fences} {
    cout << "Copy of " << *this;
}

SafariConfig::~SafariConfig() {
    cout << "Destruction of " << *this;
}

ostream &operator<<(ostream &o, const SafariConfig &) {
    o << "Config: Safari" << endl;
    return o;
}