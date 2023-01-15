#pragma once

#include "player.h"

#include <memory>

class Board;

class AIPlayer: public Player {
private:
    std::shared_ptr<Board> board;
    std::pair<bool, double> (AIPlayer::*heuristic_func)() const;
    uint8_t depth_limit;
    uint8_t depth;
    bool turn;
public:
    AIPlayer();
    void make_move(Game &game) override;
private:
    uint8_t find_next_move(std::shared_ptr<Board> b);
    double min_utility(const double max_value);
    double max_utility(const double min_value);

    std::pair<bool, double> heuristic_1() const;
    std::pair<bool, double> heuristic_2() const;
    std::pair<bool, double> heuristic_3() const;
};