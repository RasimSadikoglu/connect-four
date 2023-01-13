#pragma once

#include "player.h"

class HumanPlayer: public Player {
public:
    void make_move(Game &game) const override;
};