#pragma once

#include <bitset>
#include <array>

#define BOARD_X 8
#define BOARD_Y 7
#define BOARD_SIZE BOARD_X * BOARD_Y

enum GameStatus {
    NOT_FINISHED,
    FINISHED,
    TIE
};

class Board {
private:
    std::array<std::bitset<BOARD_SIZE>, 2> tokens;
    bool turn;
public:
    void make_move(uint8_t column);
    uint8_t check_status() const;
    void print_board() const;
};