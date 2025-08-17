#pragma once

#include <array>
#include <chrono>
#include <cstdint>

// board state:
//  0  1  2  3  4  5  6  7  8  9 10 11 12
// 25 24 23 22 21 20 19 18 17 16 15 14 13
// where index 25 is Player 1 bar and index 0 is player 2 bar
// player 1 is represented by a positive piece count, player 2 by negative
typedef std::array<int8_t, 26> GameState;

// Serialized single move: AAAAABBB -> A-bits describe initial piece index on board, B-bits describe distance moved
typedef int8_t Move;
// Serialized move list for a single turn: order of moves is right aligned, read right to left: 4321, --21, etc
typedef uint32_t MoveSet;

// Move order matters, if moving the same piece again, specify the new location
// Example of valid starting MoveSet for player 1 with dice: (3, 5)
// Moves:   (24, 3), (21, 5), -------- --------
// binary:  11000011 10101101 00000000 00000000

// Initial state of the board
const GameState INITIAL_STATE = {
    0, -2,  0,  0,  0,  0,  5,  0,  3,  0,  0,  0, -5,
    5,  0,  0,  0, -3,  0, -5,  0,  0,  0,  0,  2,  0  // representation is visually backwards here
};

// Here's how we're gonna get the ms since epoch
inline int get_time() {
    return std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
}

// Interface for a bot that plays backgammon to hook into the game engine
class BGBot {
 public:
    // Must set legalMoves and wouldAcceptDouble before hitting the timestamp defined by timeoutCutoff (ms since epoch)
    virtual void MakeMove(const GameState &state, MoveSet *legalMoves, bool *wouldAcceptDouble, int timeoutCutoff) = 0;
    // Must set offerDouble to true before timeoutCutoff is hit (default is false)
    virtual void OfferDouble(const GameState &state, bool *offerDouble, int timeoutCutoff) = 0;
};
