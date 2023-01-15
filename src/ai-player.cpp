#include "ai-player.h"

#include "board.h"
#include "game.h"

#include <vector>
#include <random>
#include <functional>
#include <iostream>

AIPlayer::AIPlayer():
    Player("AI")
{
    std::cout << "Choose a depth (4-9 is suggested): ";
    int d;
    std::cin >> d;
    depth_limit = static_cast<uint8_t>(8);
    std::cout << "\033[u";
    std::cout << "\033[0J";

    std::cout << "Choose a heuristic:\n";
    std::cout << "1) Make closest best move\n";
    std::cout << "2) ???\n";
    std::cout << "3) ???\n";
    int h;
    std::cin >> h;
    std::cout << "\033[u";
    std::cout << "\033[0J";

    std::array<std::pair<bool, double> (AIPlayer::*)() const, 3> heuristic_table = {
        &AIPlayer::heuristic_1,
        &AIPlayer::heuristic_2,
        &AIPlayer::heuristic_3
    };

    heuristic_func = heuristic_table[h - 1];
}

void AIPlayer::make_move(Game &game) {
    game.make_move(find_next_move(game.get_board()));
}

uint8_t AIPlayer::find_next_move(std::shared_ptr<Board> b) {

    this->board = b;
    this->depth = 0;
    this->turn = true;

    std::vector<uint8_t> possible_moves;
    double max_score = -1;

    for (uint8_t c = 0; c < BOARD_X; c++) {
        if (!board->is_valid_move(c)) continue;

        board->make_move(c);
        depth++;
        this->turn ^= true;
        double score = min_utility(max_score);
        this->turn ^= true;
        depth--;
        board->undo_move();

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

double AIPlayer::min_utility(const double max_value) {
    auto heuristic = (this->*heuristic_func)();

    if (heuristic.first) return heuristic.second;

    double min_value = 1;
    for (uint8_t c = 0; c < BOARD_X; c++) {
        if (!board->is_valid_move(c)) continue;

        board->make_move(c);
        depth++;
        this->turn ^= true;
        double score = max_utility(min_value);
        this->turn ^= true;
        depth--;
        board->undo_move();

        min_value = std::min(min_value, score);

        if (score < max_value) return min_value;
    }

    return min_value;
}

double AIPlayer::max_utility(const double min_value) {
    auto heuristic = (this->*heuristic_func)();

    if (heuristic.first) return heuristic.second;

    double max_value = -1;
    for (uint8_t c = 0; c < BOARD_X; c++) {
        if (!board->is_valid_move(c)) continue;

        board->make_move(c);
        depth++;
        this->turn ^= true;
        double score = min_utility(max_value);
        this->turn ^= true;
        depth--;
        board->undo_move();

        max_value = std::max(max_value, score);

        if (score > min_value) return max_value;
    }

    return max_value;
}

std::pair<bool, double> AIPlayer::heuristic_1() const {
    int8_t board_status = board->check_status();

    if (board_status != NOT_FINISHED) {
        return {true, (-1 * turn) * static_cast<double>(board_status) / depth};
    }

    if (depth == depth_limit) return {true, 0};

    return {false, 0};
}

std::pair<bool, double> AIPlayer::heuristic_2() const {
    return {};
}

std::pair<bool, double> AIPlayer::heuristic_3() const {
    return {};
}