#pragma once

#include "player.h"

class AIPlayer: public Player {
public:
    void make_move(Game &game) const override;
};