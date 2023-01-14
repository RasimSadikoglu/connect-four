#include <iostream>

#include "game.h"
#include "human-player.h"
#include "ai-player.h"

int main() {
    Game game = Game(std::make_shared<HumanPlayer>(HumanPlayer()), std::make_shared<AIPlayer>(AIPlayer()));
    game.start_game();
}