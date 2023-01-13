#include "player.h"

Player::Player(const std::string name):
    player_name{name}
{
}

const std::string Player::get_player_name() const {
    return player_name;    
}