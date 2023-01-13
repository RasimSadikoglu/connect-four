#include "board.h"

#include <cstdio>
#include <stdexcept>

#define RED "\033[1;91m"
#define YELLOW "\033[1;93m"
#define CLEAR "\033[1;0m"

static const std::array<const std::string, 3> cell_values = {"   ", RED " o " CLEAR, YELLOW " o " CLEAR};

void Board::make_move(uint8_t column) {
    if (column >= BOARD_X) throw std::invalid_argument("Column value is out of bound!");

    auto merged_board = tokens[0] | tokens[1];

    std::bitset<BOARD_SIZE> mask{0x1};
    mask <<= (BOARD_X * (BOARD_Y - 1)) + column;

    int count; for (count = 0; count < BOARD_Y; count++) {
        if ((mask & merged_board).none()) break;

        mask >>= BOARD_X;
    }

    if (count == BOARD_Y) throw std::invalid_argument("Non-empty column!");

    count = BOARD_Y - count - 1;

    mask = 0x1;
    mask <<= (count * BOARD_X) + column;

    tokens[turn] |= mask;

    turn ^= true;
}

uint8_t Board::check_status() const {
    if ((tokens[0] | tokens[1]).count() == BOARD_SIZE) return TIE;
    std::bitset<BOARD_SIZE> mask;
    uint8_t loop_count = 0;

    auto player_tokens = tokens[turn ^ true];

    /* #region: horizontal */
    mask = 0xf;
    for (int i = 0; i < BOARD_Y; i++) {
        for (int j = 0; j < BOARD_X - 3; j++) {
            if ((player_tokens & mask) == mask) return FINISHED;

            mask <<= 1;
        }
        mask <<= 3;
    }
    /* #endregion */

    /* #region: vertical */
    mask = 0x01010101;
    loop_count = (BOARD_Y - 3) * BOARD_X;
    for (int i = 0; i < loop_count; i++) {
        if ((player_tokens & mask) == mask) return FINISHED;

        mask <<= 1;
    }
    /* #endregion */
        
    /* #region: positive diagonal */
    mask = 0x01020408;
    for (int i = 0; i < BOARD_Y - 3; i++) {
        for (int j = 0; j < BOARD_X - 3; j++) {
            if ((player_tokens & mask) == mask) return FINISHED;

            mask <<= 1;
        }
        mask <<= 3;
    }
    /* #endregion */

    /* #region: negative diagonal */
    mask = 0x08040201;
    for (int i = 0; i < BOARD_Y - 3; i++) {
        for (int j = 0; j < BOARD_X - 3; j++) {
            if ((player_tokens & mask) == mask) return FINISHED;

            mask <<= 1;
        }
        mask <<= 3;
    }
    /* #endregion */

    return NOT_FINISHED;
}

void Board::print_board() const {

    std::printf("\033[u");
    std::printf("\033[0J");

    auto odd_tokens = tokens[0];
    auto even_tokens = tokens[1];

    std::bitset<BOARD_SIZE> mask{0x1};

    for (int i = 0; i < BOARD_SIZE; i++) {
        std::printf("%s", i % BOARD_X == 0 ? "\n" : "|");

        int lookup_value = (odd_tokens & mask).any() + (even_tokens & mask).any() * 2;
        std::printf("%s", cell_values[lookup_value].c_str());

        mask <<= 1;
    }

    std::printf("\n\n");
}