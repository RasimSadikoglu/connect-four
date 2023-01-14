#pragma once

#include "player.h"

#include <memory>

class Board;

class AIPlayer: public Player {
private:
    uint8_t depth;
public:
    AIPlayer(uint8_t d);
    void make_move(Game &game) const override;
private:
    // uint8_t find_next_move(std::shared_ptr<Board> board, uint8_t depth, double score) const;
    double heuristic_1(std::shared_ptr<Board> board) const;
    double heuristic_2(std::shared_ptr<Board> board) const;
    double heuristic_3(std::shared_ptr<Board> board) const;
};