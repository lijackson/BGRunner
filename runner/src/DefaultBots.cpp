#include "../include/DefaultBots.hpp"

void CLIBot::MakeMove(const GameState &state, MoveSet *legalMoves, bool *wouldAcceptDouble, int timeoutCutoff) {
    // enter move in form: pos, dist
    // no more moves enter: -
    std::string moveStr;
    while (moveStr != "-") {
        printf("Enter move: \n");
        std::getline(std::cin, moveStr);
    }
}

void CLIBot::OfferDouble(const GameState &state, bool *offerDouble, int timeoutCutoff) {

}

