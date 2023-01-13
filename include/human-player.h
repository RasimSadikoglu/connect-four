#pragma once

#include "player.h"

class HumanPlayer: public Player {
public:
    HumanPlayer();
    void make_move(Game &game) const override;
};