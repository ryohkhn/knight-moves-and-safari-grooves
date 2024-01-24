#include "Utilities.hpp"

string UtilityFunctions::getButinPieceString(ButinPieceType type) {
    switch (type) {
        case ButinPieceType::Red: return "redPiece";
        case ButinPieceType::Yellow: return "yellowPiece";
        case ButinPieceType::Black: return "blackPiece";
        case ButinPieceType::EmptyButin: return "";
        default: return "";
    }
}

string UtilityFunctions::getSafariPieceString(SafariPieceType type) {
    switch (type) {
        case SafariPieceType::Crocodile: return "crocodilePiece";
        case SafariPieceType::Elephant: return "elephantPiece";
        case SafariPieceType::Lion: return "lionPiece";
        case SafariPieceType::Fence: return "fence";
        case SafariPieceType::EmptySafari: return "";
        default: return "";
    }
}

string UtilityFunctions::getGounkiPieceString(GounkiPieceType type) {
    switch (type) {
        case GounkiPieceType::BlackSquare: return "blackSquare";
        case GounkiPieceType::BlackCircle: return "blackCircle";
        case GounkiPieceType::WhiteSquare: return "whiteSquare";
        case GounkiPieceType::WhiteCircle: return "whiteCircle";
        case GounkiPieceType::EmptyGounki: return "";
        default: return "";
    }
}

int UtilityFunctions::getValueFromColor(ButinPieceType color){
    switch (color) {
        case ButinPieceType::Black:
            return 3;
        case ButinPieceType::Red:
            return 2;
        case ButinPieceType::Yellow:
            return 1;
        case ButinPieceType::EmptyButin:
            return -1;
    }
    return 0;
}

SafariPieceType UtilityFunctions::getSafariPieceType(string type) {
    if (type == "crocodilePiece")   return SafariPieceType::Crocodile;
    if (type == "elephantPiece")    return SafariPieceType::Elephant;
    if (type == "lionPiece")    return SafariPieceType::Lion;
    if (type == "fence")    return SafariPieceType::Fence;
    return SafariPieceType::EmptySafari;
}

int UtilityFunctions::getPlayerFromAnimal(SafariPieceType animal) {
    return (int) animal;
}

int UtilityFunctions::getPlayerFromName(string color) {
    if (color == "blackSquare" || color == "blackCircle") {
        return 0;
    } else if (color == "whiteSquare" || color == "whiteCircle"){
        return 1;
    } else {
        return -1;
    }
}

void UtilityFunctions::printBoard(const std::vector<std::vector<std::vector<std::string>>> &board) {
    if (board.empty()) return; // Handle empty board

    // Find the maximum length in each row
    std::vector<size_t> rowWidths;
    for (const auto& col : board) {
        rowWidths.push_back(0); // Initialize widths for each column
        for (const auto& cell : col) {
            if (!cell.empty()) {
                // Extract filename from path
                std::string filename = cell.front().substr(cell.front().rfind('/') + 1);
                // Simplify string from path to filename
                rowWidths.back() = std::max(rowWidths.back(), filename.length());
            }
        }
    }

    // Print the board
    for (size_t j = 0; j < board[0].size(); ++j) { // Loop over rows
        std::string separator = "+";
        for (size_t i = 0; i < board.size(); ++i) { // Loop over columns
            std::cout << "|";
            if (!board[i][j].empty()) {
                std::string filename = board[i][j].front().substr(board[i][j].front().rfind('/') + 1);
                std::cout << " " << filename;
                std::cout << std::string(rowWidths[i] - filename.length(), ' '); // Adjust spacing
            } else {
                std::cout << std::string(rowWidths[i] + 1, ' '); // Adjust for empty cell
            }
            separator += std::string(rowWidths[i] + 2, '-') + "+";
        }
        std::cout << "|\n" << separator << "\n";
    }
}