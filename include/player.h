#pragma once

class Game;

class Player {
public:
    virtual void make_move(Game &game) const = 0;
};