#include "../include/Utils.hpp"
#include <iostream>

// Convert visual character index to actual byte index in UTF-8 string
// Returns the byte position where the character at visual_index starts
// Returns string::npos if visual_index is out of bounds
size_t utf8_visual_to_byte_index(const std::string& str, size_t visual_index) {
    size_t current_visual = 0;
    size_t byte_pos = 0;
    
    while (byte_pos < str.size()) {
        if (current_visual == visual_index) {
            return byte_pos;
        }
        
        // Determine how many bytes this UTF-8 character uses
        unsigned char c = str[byte_pos];
        size_t char_bytes = 1;
        
        if (c < 0x80) {
            char_bytes = 1;  // ASCII (0xxxxxxx)
        } else if ((c >> 5) == 0x06) {  // 110xxxxx
            char_bytes = 2;
        } else if ((c >> 4) == 0x0E) {  // 1110xxxx
            char_bytes = 3;
        } else if ((c >> 3) == 0x1E) {  // 11110xxx
            char_bytes = 4;
        } else {
            // Invalid UTF-8, treat as single byte
            char_bytes = 1;
        }
        
        byte_pos += char_bytes;
        current_visual++;
    }
    
    // Check if we're asking for the position right after the last character
    if (current_visual == visual_index) {
        return byte_pos;  // This would be the end of string
    }
    
    return std::string::npos;  // Index out of bounds
}

// Replace character at visual index with new UTF-8 character
std::string utf8_replace_char_at_visual_index(const std::string& str, size_t visual_index, const std::string& new_char) {
    size_t byte_start = utf8_visual_to_byte_index(str, visual_index);
    if (byte_start == std::string::npos) {
        return str;  // Index out of bounds, return original
    }
    
    size_t byte_end = utf8_visual_to_byte_index(str, visual_index + 1);
    if (byte_end == std::string::npos) {
        byte_end = str.size();  // Last character
    }
    
    return str.substr(0, byte_start) + new_char + str.substr(byte_end);
}

int8_t get_pos_count(const GameState& game_state, int index) {
    int8_t count = game_state[index];
    uint8_t mask = count >> 8;
    count ^= mask;
    count += mask & 1;
    return count;
}

bool is_white_column(const GameState& game_state, int index) {
    int8_t count = game_state[index];
    uint8_t mask = count >> 8;
    return mask;
}

std::string count_top =    "                                 ";
std::string board_top =    "┏━━━┳━━━━━━━━━━━━━┳━━━━━━━━━━━━━┓";
std::string board_middle = "┣━━━┫             ┃             ┃";
std::string board_bottom = "┗━━━┻━━━━━━━━━━━━━┻━━━━━━━━━━━━━┛";
std::string count_bottom = "                                 ";
std::string board_row =    "┃   ┃             ┃             ┃";
const char* hex_arr = "0123456789ABCDEF";

int index_to_x(int index) {
    if(index >= 13)
        index = 25 - index;

    int x = 2 + index * 2;
    if(index > 0) x += 2;
    if(index > 6) x += 2;

    return x;
}

void print_board(const GameState& game_state, bool is_white_turn) {

    // print numbers above column
    for(int i = 0; i < 13; i++) {
        int count = get_pos_count(game_state, i);
        count_top[index_to_x(i)] = hex_arr[count];
    }

    // print numbers below column
    for(int i = 13; i < 26; i++) {
        int count = get_pos_count(game_state, i);
        count_bottom[index_to_x(i)] = hex_arr[count];
    }

    std::cout << count_top << std::endl;
    std::cout << board_top << std::endl;

    // Top half of board
    for(int row = 1; row <= 6; row++) {
        for(int i = 0; i < 13; i++) {
            std::string symbol = get_pos_count(game_state, i) < row ? 
                row == 1 && i != 0 && i != 25  ? "▼" : " " :
                is_white_column(game_state, i) ? "O" : "X";
            board_row = utf8_replace_char_at_visual_index(board_row, index_to_x(i), symbol);
        }
        std::cout << board_row << std::endl;
    }

    std::cout << board_middle << std::endl;

    // Bottom half of board
    for(int row = 6; row > 0; row--) {
        for(int i = 13; i < 26; i++) {
            std::string symbol = get_pos_count(game_state, i) < row ? 
                row == 1 && i != 0 && i != 25  ? "▲" : " " :
                is_white_column(game_state, i) ? "O" : "X";
            board_row = utf8_replace_char_at_visual_index(board_row, index_to_x(i), symbol);
        }
        std::cout << board_row << std::endl;
    }

    std::cout << board_bottom << std::endl;
    std::cout << count_bottom << std::endl;

}