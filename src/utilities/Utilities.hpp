#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <unordered_map>
#include <iostream>
#include <vector>
#include <SFML/System/Vector2.hpp>
using namespace std;
using namespace sf;

enum class ButinPieceType {
    Red,
    Yellow,
    Black,
    EmptyButin
};

// Crocodile => Player 0
// Elephant  => Player 1
// Lion      => Player 2
enum class SafariPieceType {
    Crocodile,
    Elephant,
    Lion,
    Fence,
    EmptySafari
};

// Black => Player 0
// White => Player 1
enum class GounkiPieceType {
    BlackSquare,
    BlackCircle,
    WhiteSquare,
    WhiteCircle,
    EmptyGounki
};

enum class ActionKey {
    LeftClick,
    RightClick
};

namespace UtilityFunctions {
    string getButinPieceString(ButinPieceType type);
    string getSafariPieceString(SafariPieceType type);
    string getGounkiPieceString(GounkiPieceType type);
    int getValueFromColor(ButinPieceType color);
    SafariPieceType getSafariPieceType(string type);
    int getPlayerFromAnimal(SafariPieceType animal);
    int getPlayerFromName(string color);
    void printBoard(const vector<vector<vector<string>>>& board);
}

#endif