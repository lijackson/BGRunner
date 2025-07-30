#include "../include/GameRunner.hpp"


GameRunner::GameRunner(const BGBot &bot1, const BGBot &bot2) {
    _bot1 = bot1;
    _bot2 = bot2;
    Reset();
}

void GameRunner::Reset() {
    _state = INITIAL_STATE;
    _is_player1_turn = true;
    _opponentAcceptsDraw = false;
    _p1_home_count = 0;
    _p2_home_count = 0;
    for (uint8_t i = 1; i < 7; i++)
        if (_state.board[i] > 0) _p1_home_count += _state.board[i];
    for (uint8_t i = 24; i > 18; i--)
        if (_state.board[i] < 0) _p2_home_count -= _state.board[i];
}

bool GameRunner::IsLegal(Move move) {
    uint8_t pos = ToMovePos(move);
    uint8_t dist = ToMoveDist(move);
    // need to escape the bar
    if (IsBarred() && !(pos == 0 || pos == 25))
        return false;

    int8_t dir = _is_player1_turn*2 - 1;
    uint8_t endPos = pos + dir*dist;

    // can't score unless all pieces are on your home board
    if (_is_player1_turn ? (endPos <= 0 && _p1_home_count != 15)
                         : (endPos >= 25 && _p2_home_count != 15))
            return false;

    // overshooting to score is only allowed for the furthest checker
    if (endPos < 0 || endPos > 25) {
        uint8_t outside_home = 18 - 11*_is_player1_turn;
        for (uint8_t i = outside_home + dir; i != pos; i += dir) {
            if (_state.board[endPos]*dir < 0)
                return false;
        }
    }

    // is this column blocked by opponent pillar
    if (_state.board[endPos]*dir > 1)
        return false;

    return true;
}

// assumes its a legal move
void GameRunner::MakeMove(Move move) {
    uint8_t pos = ToMovePos(move);
    uint8_t dist = ToMoveDist(move);
    int8_t dir = _is_player1_turn*2 - 1;
    uint8_t endPos = pos + dir*dist;

    // correct end location for overshoot scoring moves
    if (endPos < 0) endPos = 0;
    if (endPos > 25) endPos = 25;

    // handle captures
    if (_state.board[endPos] == dir && (endPos != 0 && endPos != 25)) {
        // move captured checker to bar
        _state.board[endPos] = 0;
        _state.bar += dir;

        // has a checker been captured off of the home board?
        if (_is_player1_turn && endPos <= 6)
            _p2_home_count--;
        if (!_is_player1_turn && endPos >= 19)
            _p1_home_count--;
    }

    // has a checked moved onto the home board?
    if (_is_player1_turn && endPos <= 6 && pos > 6)
        _p1_home_count++;
    if (!_is_player1_turn && endPos >= 19 && pos < 19)
        _p2_home_count++;

    // move piece
    _state.board[pos] += dir;
    _state.board[endPos] -= dir;
}

void GameRunner::PlayMoveset(MoveSet moveset) {
    while (moveset & 0xFF != 0) {
        MakeMove(moveset & 0xFF != 0);
        moveset = moveset >> 8;
    }
}

MoveSet GameRunner::RequestMove() {
    _opponentAcceptsDraw = false;
    MoveSet moves = 0;
    _is_player1_turn
        ? _bot1.MakeMove(_state, &moves, &_opponentAcceptsDraw, 100)
        : _bot2.MakeMove(_state, &moves, &_opponentAcceptsDraw, 100);

    return moves;
}

bool RunGame() {
    // todo
    return false;
}
bool RunMatch() {
    // todo
    return false;
}

uint8_t ToMovePos(Move move) {
    return move >> 3;
}

uint8_t ToMoveDist(Move move) {
    return move & 0b111;
}
