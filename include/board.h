#pragma once

#include <bitset>
#include <array>
#include <stack>

#define BOARD_X 8
#define BOARD_Y 7
#define BOARD_SIZE BOARD_X * BOARD_Y

enum GameStatus {
    TIE,
    FINISHED,
    NOT_FINISHED
};

class Board {
private:
    std::array<std::bitset<BOARD_SIZE>, 2> tokens;
    std::array<uint8_t, BOARD_SIZE> move_stack;
    std::array<uint8_t, BOARD_X> column_counts;
    std::bitset<BOARD_SIZE> winning_mask;
    uint8_t turn_count;
public:
    Board();

    void make_move(uint8_t column);
    void undo_move();
    
    bool is_valid_move(uint8_t column) const;
    std::array<std::bitset<BOARD_SIZE>, 2> get_tokens() const;
    uint8_t check_status();
    void print_board() const;
};