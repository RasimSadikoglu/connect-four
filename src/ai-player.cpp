#include "ai-player.h"

#include "board.h"
#include "game.h"

#define DEPTH_LIMIT 6

AIPlayer::AIPlayer():
    Player("AI")
{
}

int8_t max_utility(std::shared_ptr<const Board> board, const uint8_t limit, const uint8_t depth, const int8_t min_value);
int8_t min_utility(std::shared_ptr<const Board> board, const uint8_t limit, const uint8_t depth, const int8_t max_value);

int8_t min_utility(std::shared_ptr<const Board> board, const uint8_t limit, const uint8_t depth, const int8_t max_value) {
    int8_t board_status = board->check_status();

    if (board_status != NOT_FINISHED) {
        return board_status;
    }

    if (depth == limit) return 0;

    int8_t min_value = INT8_MAX;
    for (uint8_t c = 0; c < BOARD_X; c++) {
        if (!board->is_valid_move(c)) continue;

        auto new_board = std::make_shared<Board>(Board(*board));
        new_board->make_move(c);

        int8_t score = max_utility(new_board, limit, depth + 1, min_value);
        min_value = std::min(min_value, score);

        if (score <= max_value) return min_value;
    }

    return min_value;
}

int8_t max_utility(std::shared_ptr<const Board> board, const uint8_t limit, const uint8_t depth, const int8_t min_value) {
    int8_t board_status = board->check_status();

    if (board_status != NOT_FINISHED) {
        return board_status * -1;
    }

    if (depth == limit) return 0;

    int8_t max_value = INT8_MIN;
    for (uint8_t c = 0; c < BOARD_X; c++) {
        if (!board->is_valid_move(c)) continue;

        auto new_board = std::make_shared<Board>(Board(*board));
        new_board->make_move(c);

        int8_t score = min_utility(new_board, limit, depth + 1, max_value);
        max_value = std::max(max_value, score);

        if (score >= min_value) return max_value;
    }

    return max_value;
}

uint8_t find_next_move(std::shared_ptr<const Board> board, const uint8_t limit, const uint8_t depth) {
    int8_t max_score = INT8_MIN;
    uint8_t column = 0xff;

    for (uint8_t c = 0; c < BOARD_X; c++) {
        if (!board->is_valid_move(c)) continue;

        auto new_board = std::make_shared<Board>(Board(*board));
        new_board->make_move(c);

        int8_t score = min_utility(new_board, limit, depth, max_score);

        if (score > max_score) {
            max_score = score;
            column = c;
        }

        if (score == 1) {
            return column;
        }
    }

    return column;
}

void AIPlayer::make_move(Game &game) const {
    auto current_board = std::make_shared<Board>(Board(*game.get_board()));
    game.make_move(find_next_move(current_board, DEPTH_LIMIT, 0));
}

double AIPlayer::heuristic_1(__attribute_maybe_unused__ std::shared_ptr<Board> board) const
{
    return 0;
}

double AIPlayer::heuristic_2(__attribute_maybe_unused__ std::shared_ptr<Board> board) const
{
    return 0;
}

double AIPlayer::heuristic_3(__attribute_maybe_unused__ std::shared_ptr<Board> board) const
{
    return 0;
}