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
    std::cout << players[0]->get_player_name() << " vs " << players[1]->get_player_name() << "\n";
    std::cout << "\033[s";

    board->print_board();

    while (board->check_status() == NOT_FINISHED) {
        players[turn]->make_move(*this);
        board->print_board();
    }

    uint8_t status = board->check_status();

    if (status == TIE) {
        std::cout << "Tie\n";
    } else if (status == FINISHED) {
        std::cout << "Player " << static_cast<int>((turn ^ true) + 1) << " won!\n";
    }
}

void Game::make_move(uint8_t column) {
    board->make_move(column);
    turn ^= true;
}