#include "ai-player.h"

#include "board.h"
#include "game.h"

#include <vector>
#include <random>

#define DEPTH_LIMIT 9

AIPlayer::AIPlayer():
    Player("AI")
{
}

double max_utility(std::shared_ptr<const Board> board, const uint8_t limit, const uint8_t depth, const double min_value);
double min_utility(std::shared_ptr<const Board> board, const uint8_t limit, const uint8_t depth, const double max_value);

double min_utility(std::shared_ptr<const Board> board, const uint8_t limit, const uint8_t depth, const double max_value) {
    int8_t board_status = board->check_status();

    if (board_status != NOT_FINISHED) {
        return board_status * (1.0 / depth);
    }

    if (depth == limit) return 0;

    double min_value = 1;
    for (uint8_t c = 0; c < BOARD_X; c++) {
        if (!board->is_valid_move(c)) continue;

        auto new_board = std::make_shared<Board>(Board(*board));
        new_board->make_move(c);

        double score = max_utility(new_board, limit, depth + 1, min_value);
        min_value = std::min(min_value, score);

        if (score < max_value) return min_value;
    }

    return min_value;
}

double max_utility(std::shared_ptr<const Board> board, const uint8_t limit, const uint8_t depth, const double min_value) {
    int8_t board_status = board->check_status();

    if (board_status != NOT_FINISHED) {
        return board_status * -1 * (1.0 / depth);
    }

    if (depth == limit) return 0;

    double max_value = -1;
    for (uint8_t c = 0; c < BOARD_X; c++) {
        if (!board->is_valid_move(c)) continue;

        auto new_board = std::make_shared<Board>(Board(*board));
        new_board->make_move(c);

        double score = min_utility(new_board, limit, depth + 1, max_value);
        max_value = std::max(max_value, score);

        if (score > min_value) return max_value;
    }

    return max_value;
}

uint8_t find_next_move(std::shared_ptr<const Board> board, const uint8_t limit, const uint8_t depth) {
    std::vector<uint8_t> possible_moves;
    double max_score = -1;

    for (uint8_t c = 0; c < BOARD_X; c++) {
        if (!board->is_valid_move(c)) continue;

        auto new_board = std::make_shared<Board>(Board(*board));
        new_board->make_move(c);

        double score = min_utility(new_board, limit, depth + 1, max_score);
        printf("score: %.4lf, c: %d\n", score, static_cast<int>(c));
        
        if (score > max_score) {
            max_score = score;
            possible_moves.clear();
            possible_moves.push_back(c);
        } else if (score == max_score) {
            possible_moves.push_back(c);
        }

        if (score == 1) {
            return c;
        }
    }

    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<std::size_t> distribution(0, possible_moves.size() - 1);

    return possible_moves[distribution(generator)];
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