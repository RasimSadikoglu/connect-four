#include "game.h"

#include "board.h"
#include "player.h"

#include <iostream>

Game::Game(std::shared_ptr<Player> odd_player, std::shared_ptr<Player> even_player):
    board{std::make_shared<Board>(Board())},
    players{{odd_player, even_player}},
    turn{false}
{
}

void Game::start_game() {
    std::cout << "Hello\n";
    players[0]->make_move(*this);
}

void Game::make_move(uint8_t column) {
    std::cout << "move test " << static_cast<int>(column) << "\n";
}