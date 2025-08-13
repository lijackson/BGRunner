#include <iostream>
#include <string>

#include "../interface/BGInterface.hpp"

class CLIBot : public BGBot {
    void MakeMove(const GameState &state, MoveSet *legalMoves, bool *wouldAcceptDouble, int timeoutCutoff);
    void OfferDouble(const GameState &state, bool *offerDouble, int timeoutCutoff);
};

class RandomBot : public BGBot {
    void MakeMove(const GameState &state, MoveSet *legalMoves, bool *wouldAcceptDouble, int timeoutCutoff);
    void OfferDouble(const GameState &state, bool *offerDouble, int timeoutCutoff);
};
