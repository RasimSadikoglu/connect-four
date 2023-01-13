#include <iostream>

#include "game.h"
#include "human-player.h"

int main() {
    Game game = Game(std::make_shared<HumanPlayer>(HumanPlayer()), std::make_shared<HumanPlayer>(HumanPlayer()));
    game.start_game();
}