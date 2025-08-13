#include <vector>

#include "../interface/BGInterface.hpp"
#include "./DefaultBots.hpp"

const uint16_t MS_PER_MOVE = 500;

class GameRunner {
 private:
    GameState _state = INITIAL_STATE;
    bool _is_player1_turn = true;
    const uint16_t _timeout = MS_PER_MOVE;
    BGBot* _bot1;
    BGBot* _bot2;
    uint8_t _p1_home_count;
    uint8_t _p2_home_count;
    bool _opponentAcceptsDraw;

    bool IsBarred();
    void Reset();
    bool IsLegal(Move move);
    void MakeMove(Move move);
    void PlayMoveset(MoveSet moveset);
    std::vector<MoveSet> AllLegalMoves();
    MoveSet RequestMove();

 public:
    GameRunner(BGBot* bot1, BGBot* bot2);

    // A game doesn't enforce anything, just for testing
    bool RunGame();

    // A match has enforced time constraints and checks to make sure you're making legal moves
    bool RunMatch();
};

uint8_t ToMovePos(Move move);
uint8_t ToMoveDist(Move move);
