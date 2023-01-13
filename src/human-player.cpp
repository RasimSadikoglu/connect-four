#include "human-player.h"

#include "game.h"

void HumanPlayer::make_move(__attribute__((unused)) Game &game) const {
    game.make_move(0);
}