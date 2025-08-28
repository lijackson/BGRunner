#include "../../interface/BGInterface.hpp"
#include <string>

int8_t get_pos_count(const GameState& game_state, int index);
bool is_white_column(const GameState& game_state, int index);

void print_board(const GameState& game_state, bool is_white_turn);