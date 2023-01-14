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
    Board();
    Board(std::bitset<BOARD_SIZE> odd_tokens, std::bitset<BOARD_SIZE> even_tokens);
    Board(const Board &board);

    void make_move(uint8_t column);
    
    bool is_valid_move(uint8_t column) const;
    std::array<std::bitset<BOARD_SIZE>, 2> get_tokens() const;
    uint8_t check_status() const;
    void print_board() const;
};