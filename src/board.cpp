#include "board.h"

#include <cstdio>
#include <stdexcept>

#define RED "\033[91m"
#define BOLD_UNDERLINE "\033[1;4m"
#define YELLOW "\033[93m"
#define CLEAR "\033[0m"

static const std::array<const std::string, 3> cell_values = {"   ", RED " o " CLEAR, YELLOW " o " CLEAR};

Board::Board():
    tokens{{0x0, 0x0}},
    move_stack{{}},
    column_counts{{}},
    turn_count{0}
{
}

void Board::make_move(uint8_t column)
{
    if (column >= BOARD_X) throw std::invalid_argument("Column value is out of bound!");

    uint8_t depth = BOARD_Y - column_counts[column] - 1;
    if (depth == 0xff) throw std::invalid_argument("Non-empty column!");

    bool turn = turn_count % 2;
    tokens[turn].set((BOARD_X * depth) + column);
    
    column_counts[column]++;
    move_stack[turn_count] = column;
    turn_count++;
}

void Board::undo_move()
{
    bool turn = !(turn_count % 2);

    if (turn_count == 0) throw std::invalid_argument("No moves have been made!");

    turn_count--;
    uint8_t column = move_stack[turn_count];
    uint8_t depth = BOARD_Y - column_counts[column];
    column_counts[column]--;

    tokens[turn].reset((BOARD_X * depth) + column);
}

std::array<std::bitset<BOARD_SIZE>, 2> Board::get_tokens() const {
    return {tokens[0], tokens[1]};
}

bool Board::is_valid_move(uint8_t column) const {
    return column_counts[column] != BOARD_Y;
}

void debug_print(std::bitset<56> board) {

    for (int i = 0; i < BOARD_SIZE; i++) {
        std::printf("%s", i % BOARD_X == 0 ? "\n" : "|");

        int lookup_value = board[i];
        std::printf("%s", cell_values[lookup_value].c_str());
    }

    std::printf("\n\n");
}

uint8_t Board::check_status() {

    if (turn_count < 7) return NOT_FINISHED;
    if (turn_count == BOARD_SIZE) return TIE; 

    auto player_tokens = tokens[!(turn_count % 2)];

    __attribute_maybe_unused__ uint8_t column = move_stack[turn_count - 1];
    __attribute_maybe_unused__ uint8_t depth = BOARD_Y - column_counts[column];

    std::bitset<BOARD_SIZE> mask;
    uint8_t loop_count;

    /* #region: horizontal */
    mask = 0xf;
    for (int i = 0; i < BOARD_Y; i++) {
        for (int j = 0; j < BOARD_X - 3; j++) {
            if ((player_tokens & mask) == mask) {
                winning_mask = mask;
                return FINISHED;
            }

            mask <<= 1;
        }
        mask <<= 3;
    }
    /* #endregion */

    /* #region: vertical */
    mask = 0x01010101;
    loop_count = (BOARD_Y - 3) * BOARD_X;
    for (int i = 0; i < loop_count; i++) {
        if ((player_tokens & mask) == mask) {
            winning_mask = mask;
            return FINISHED;
        }

        mask <<= 1;
    }
    /* #endregion */
        
    /* #region: positive diagonal */
    mask = 0x01020408;
    for (int i = 0; i < BOARD_Y - 3; i++) {
        for (int j = 0; j < BOARD_X - 3; j++) {
            if ((player_tokens & mask) == mask) {
                winning_mask = mask;
                return FINISHED;
            }

            mask <<= 1;
        }
        mask <<= 3;
    }
    /* #endregion */

    /* #region: negative diagonal */
    mask = 0x08040201;
    for (int i = 0; i < BOARD_Y - 3; i++) {
        for (int j = 0; j < BOARD_X - 3; j++) {
            if ((player_tokens & mask) == mask) {
                winning_mask = mask;
                return FINISHED;
            }

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

    for (int i = 0; i < BOARD_SIZE; i++) {
        std::printf("%s", i % BOARD_X == 0 ? "\n" : "|");

        if (winning_mask[i]) std::printf(BOLD_UNDERLINE);
        int lookup_value = odd_tokens[i] + even_tokens[i] * 2;
        std::printf("%s", cell_values[lookup_value].c_str());
        if (winning_mask[i]) std::printf(CLEAR);
    }

    std::printf("\n\n");
}