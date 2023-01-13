#pragma once

#include "player.h"

class AIPlayer: public Player {
public:
    AIPlayer();
    void make_move(Game &game) const override;
};