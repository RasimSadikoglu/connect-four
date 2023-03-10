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
    depth_limit = static_cast<uint8_t>(d);
    std::cout << "\033[u";
    std::cout << "\033[0J";

    std::cout << "Choose a heuristic:\n";
    std::cout << "1) Just look ahead\n";
    std::cout << "2) Most neighbor is best\n";
    std::cout << "3) Forests are good\n";
    std::cout << "Option: ";
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

    // Initialize required variables for evolution.
    this->board = b;
    this->depth = 0;
    this->turn = true;

    // If there are multiple moves in the same score, put them into a list 
    // and randomly choose a move.
    std::vector<uint8_t> possible_moves;
    double max_score = -1;

    // Loop over every column and check the score.
    for (uint8_t c = 0; c < BOARD_X; c++) {
        if (!board->is_valid_move(c)) continue;

        // Call the min utility function also make some preperations.
        board->make_move(c);
        depth++;
        this->turn ^= true;
        double score = min_utility(max_score);

        // Reset back the states to the condition before utility function is called.
        this->turn ^= true;
        depth--;
        board->undo_move();

        printf("The score for column #%d: %.4lf\n", static_cast<int>(c + 1), score);
        
        // If new score is bigger than moximum reset the possible move list. Otherwise, 
        // if it is equal to the max score add it to the list. 
        if (score > max_score) {
            max_score = score;
            possible_moves.clear();
            possible_moves.push_back(c);
        } else if (score == max_score) {
            possible_moves.push_back(c);
        }

        // If there are a winning move return with that column.
        if (score == 1) {
            return c;
        }
    }

    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<std::size_t> distribution(0, possible_moves.size() - 1);

    return possible_moves[distribution(generator)];
}

/*
    Somewhat same with the function above.
*/
double AIPlayer::min_utility(const double max_value) {
    auto heuristic = (this->*heuristic_func)();

    // Call the heuristic function and return if depth limit is reached or the game is ended.
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

/*
Same function as above just the maximum one.
*/
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

/*
    A simple heuristic that only checks for if the game ended or not. Additional explanation is on the report.
*/
std::pair<bool, double> AIPlayer::heuristic_1() const {
    uint8_t board_status = board->check_status();

    if (board_status != NOT_FINISHED) {
        double score = static_cast<double>(board_status) / depth;
        if (turn) score *= -1;

        return {true, score};
    }

    if (depth == depth_limit) return {true, 0};

    return {false, 0};
}

/*
    If a index is out of bound return false. Otherwise check if there is a token in that location.
*/
static bool check_token_with_boundry_control(std::bitset<BOARD_SIZE> tokens, uint8_t i, uint8_t j) {
    if (i >= BOARD_Y || j >= BOARD_X) return false;

    uint8_t location = static_cast<uint8_t>(i * BOARD_X + j);

    return tokens[location];
}

/*
    Count the number of neighbors. Additional explanation is on the report.
*/
std::pair<bool, double> AIPlayer::heuristic_2() const {
    uint8_t board_status = board->check_status();

    if (board_status != NOT_FINISHED) {
        double score = static_cast<double>(board_status);
        if (turn) score *= -1;

        return {true, score};
    }

    if (depth != depth_limit) return {false, 0};

    auto tokens = board->get_tokens();
    std::array<double, 2> scores = {0, 0};

    for (uint8_t k = 0; k < 2; k++) {
        auto player_tokens = tokens[k];
        for (uint8_t i = 0; i < BOARD_Y; i++) {
            for (uint8_t j = 0; j < BOARD_X; j++) {   
                if (!player_tokens[i * BOARD_X + BOARD_Y]) continue;

                scores[k] += check_token_with_boundry_control(player_tokens, i - 1, j - 1);
                scores[k] += check_token_with_boundry_control(player_tokens, i - 1, j);
                scores[k] += check_token_with_boundry_control(player_tokens, i - 1, j + 1);
                scores[k] += check_token_with_boundry_control(player_tokens, i, j - 1);
                scores[k] += check_token_with_boundry_control(player_tokens, i, j + 1);
                scores[k] += check_token_with_boundry_control(player_tokens, i + 1, j - 1);
                scores[k] += check_token_with_boundry_control(player_tokens, i + 1, j);
                scores[k] += check_token_with_boundry_control(player_tokens, i + 1, j + 1);
            }
        }
    }

    bool player_turn = tokens[0].count() == tokens[1].count();

    double score = (scores[0] - scores[1]) / (BOARD_SIZE * 8);
    if (player_turn ^ turn) score *= -1;

    return {true, score};
}

/*
    A dfs function for the tokens.
*/
double dfs(std::bitset<BOARD_SIZE> tokens, uint8_t index, uint8_t depth, std::bitset<BOARD_SIZE> &visited) {

    if (visited[index] || !tokens[index]) return 0;

    visited.set(index);

    uint8_t i = index / BOARD_X, j = index % BOARD_X;

    double score = depth;

    score += check_token_with_boundry_control(tokens, i, j + 1)      ? dfs(tokens, index + 1,           depth * 2, visited) : 0;
    score += check_token_with_boundry_control(tokens, i + 1, j - 1)  ? dfs(tokens, index + BOARD_X - 1, depth * 2, visited) : 0;
    score += check_token_with_boundry_control(tokens, i + 1, j)      ? dfs(tokens, index + BOARD_X,     depth * 2, visited) : 0;
    score += check_token_with_boundry_control(tokens, i + 1, j + 1)  ? dfs(tokens, index + BOARD_X + 1, depth * 2, visited) : 0;

    return score;
}

/*
    A heuristic that checks trees on the board and score them accordingly. Additional explanation is on the report.
*/
std::pair<bool, double> AIPlayer::heuristic_3() const {
    uint8_t board_status = board->check_status();

    if (board_status != NOT_FINISHED) {
        double score = static_cast<double>(board_status);
        if (turn) score *= -1;

        return {true, score};
    }

    if (depth != depth_limit) return {false, 0};

    auto tokens = board->get_tokens();
    std::array<double, 2> scores = {0, 0};

    // A visited array for the graph search.
    std::bitset<BOARD_SIZE> visited{0x0};

    for (uint8_t i = 0; i < 2; i++) {
        auto player_tokens = tokens[i];
        for (uint8_t j = 0; j < BOARD_SIZE; j++) {
            scores[0] += dfs(player_tokens, j, 1, visited);
        }
    }

    bool player_turn = tokens[0].count() == tokens[1].count();

    double score = (scores[0] - scores[1]) / (BOARD_SIZE * 7);
    if (player_turn ^ turn) score *= -1;

    return {true, score};
}