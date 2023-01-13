#pragma once

#define BOARD_X 8
#define BOARD_Y 7

#include <bitset>
#include <array>

class Board {
private:
    std::array<std::bitset<BOARD_X * BOARD_Y>, 2> tokens;
    bool turn;
public:
    void make_move(uint8_t column);
    void print_board() const;
};