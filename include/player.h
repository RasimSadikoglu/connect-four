#pragma once

#include <string>

class Game;

class Player {
private:
    const std::string player_name;
public:
    Player(std::string name);
    virtual void make_move(Game &game) = 0;
    const std::string get_player_name() const;
};