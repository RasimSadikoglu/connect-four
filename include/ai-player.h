#pragma once

#include "player.h"

#include <memory>

class Board;

class AIPlayer: public Player {
public:
    AIPlayer();
    void make_move(Game &game) const override;
private:
    uint8_t find_next_move(std::shared_ptr<Board> board) const;
};